//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple histogram OpenCL application
//
// clang++ -std=c++20 -o hist.exe cl_hist.cc -lOpenCL
//
// hist.exe -kernel=hist-onlyg.cl -dsz=1000000 -hsz=1024 -lsz=256 -gsz=100
// hist.exe -kernel=hist-onlyg-corr.cl -dsz=1000000 -hsz=1024 -lsz=256 -gsz=100
// hist.exe -kernel=hist.cl -dsz=1000000 -hsz=1024 -lsz=256 -gsz=100
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <charconv>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS

#include "CL/opencl.hpp"

#ifndef COMPARE_CPU
#define COMPARE_CPU 1
#endif

#ifndef ANALYZE
#define ANALYZE 1
#endif

#define dbgs                                                                   \
  if (!ANALYZE) {                                                              \
  } else                                                                       \
    std::cout

constexpr int DATABLOCK = 256;

// config for program: we can also read it from options
struct Config {
  int DataSize = 1;   // number of 256-blocks
  int GlobalSize = 1; // number of 256-datagroups
  int HistSize = 32;  // number of histogram bins
  int LSZ = 1;
  const char *PATH = "hist.cl";
  cl::QueueProperties QProps =
      cl::QueueProperties::Profiling | cl::QueueProperties::OutOfOrder;
  static Config readCfg(int argc, char **argv);
  void dump(std::ostream &Os);
};

static std::ostream &operator<<(std::ostream &Os, Config Cfg) {
  Cfg.dump(Os);
  return Os;
}

// used in readcfg
struct option_error : public std::runtime_error {
  option_error(const char *s) : std::runtime_error(s) {}
};

// generic random initialization
template <typename It> void rand_init(It start, It end, int low, int up);

// reference histogram
template <typename TD, typename TH>
void hist_ref(const TD *Data, int DataSize, TH *Hist, int HistSize);

// OpenCL application encapsulates platform, context and queue
// We can offload vector addition through its public interface
class OclApp {
  cl::Platform P_;
  cl::Context C_;
  cl::CommandQueue Q_;
  std::string K_;
  Config Cfg_;

  static cl::Platform select_platform();
  static cl::Context get_gpu_context(cl_platform_id);
  static std::string readFile(const char *);

  using hist_t = cl::KernelFunctor<cl::Buffer, cl_int, cl::Buffer,
                                   cl::LocalSpaceArg, cl_int>;

public:
  OclApp(Config Cfg)
      : P_(select_platform()), C_(get_gpu_context(P_())), Q_(C_, Cfg.QProps),
        K_(readFile(Cfg.PATH)), Cfg_(Cfg) {
    std::string Def;
    cl::string name = P_.getInfo<CL_PLATFORM_NAME>();
    cl::string profile = P_.getInfo<CL_PLATFORM_PROFILE>();
    dbgs << "Selected: " << name << ": " << profile << std::endl;
  }

  int lsz() const { return Cfg_.LSZ; }

  cl::Event hist(const unsigned char *Data, int DataSize, int *Hist,
                 int HistSize);
};

int main(int argc, char **argv) try {
  std::chrono::high_resolution_clock::time_point TimeStart, TimeFin;
  cl_ulong GPUTimeStart, GPUTimeFin;
  long Dur, GDur;
  Config Cfg = Config::readCfg(argc, argv);
  dbgs << "Hello from hist. Config:\n" << Cfg << std::endl;

  OclApp App(Cfg);
  cl::vector<unsigned char> Data(Cfg.DataSize * DATABLOCK);
  cl::vector<int> Hist(Cfg.HistSize);

  rand_init(Data.begin(), Data.end(), 0, Cfg.HistSize - 1);

  TimeStart = std::chrono::high_resolution_clock::now();
  cl::Event evt = App.hist(Data.data(), Data.size(), Hist.data(), Cfg.HistSize);
  TimeFin = std::chrono::high_resolution_clock::now();
  Dur =
      std::chrono::duration_cast<std::chrono::milliseconds>(TimeFin - TimeStart)
          .count();
  std::cout << "GPU wall time measured: " << Dur << " ms" << std::endl;
  GPUTimeStart = evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
  GPUTimeFin = evt.getProfilingInfo<CL_PROFILING_COMMAND_END>();
  GDur = (GPUTimeFin - GPUTimeStart) / 1000000; // ns -> ms
  std::cout << "GPU pure time measured: " << GDur << " ms" << std::endl;

#if COMPARE_CPU
  cl::vector<int> HistCPU(Cfg.HistSize);
  TimeStart = std::chrono::high_resolution_clock::now();
  hist_ref(Data.data(), Data.size(), HistCPU.data(), Cfg.HistSize);
  TimeFin = std::chrono::high_resolution_clock::now();
  Dur =
      std::chrono::duration_cast<std::chrono::milliseconds>(TimeFin - TimeStart)
          .count();
  std::cout << "CPU time measured: " << Dur << " ms" << std::endl;

  for (int i = 0; i < Cfg.HistSize; ++i) {
    auto lhs = Hist[i];
    auto rhs = HistCPU[i];
    if (lhs != rhs) {
      std::cerr << "Error at index " << i << ": " << lhs << " != " << rhs
                << std::endl;
      return -1;
    }
  }
#endif

  dbgs << "All checks passed" << std::endl;
} catch (cl::BuildError &err) {
  std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
            << std::endl;
  std::cerr << "-- Log --\n";
  for (auto e : err.getBuildLog())
    std::cerr << e.second;
  std::cerr << "-- End log --\n";
  return -1;
} catch (cl::Error &err) {
  std::cerr << "OCL ERROR: " << err.err() << ":" << err.what() << std::endl;
  return -1;
} catch (option_error &err) {
  std::cerr << "INVALID OPTION: " << err.what() << std::endl;
  return -1;
} catch (std::runtime_error &err) {
  std::cerr << "RUNTIME ERROR: " << err.what() << std::endl;
  return -1;
} catch (...) {
  std::cerr << "UNKNOWN ERROR\n";
  return -1;
}

//-----------------------------------------------------------------------------
//
// OclApp methods
//
//-----------------------------------------------------------------------------

cl::Event OclApp::hist(const unsigned char *Data, int DataSize, int *Hist,
                       int HistSize) {
  size_t DataBufSize = DataSize * sizeof(unsigned char);
  size_t HistBufSize = HistSize * sizeof(int);

  cl::Buffer D(C_, CL_MEM_READ_ONLY, DataBufSize);
  cl::Buffer H(C_, CL_MEM_WRITE_ONLY, HistBufSize);

  cl::copy(Q_, Data, Data + DataSize, D);

  cl::Program program(C_, K_, true /* build immediately */);

  hist_t hist(program, "histogram");

  cl::NDRange GlobalRange(Cfg_.GlobalSize * DATABLOCK);
  cl::NDRange LocalRange(lsz());
  cl::EnqueueArgs Args(Q_, GlobalRange, LocalRange);

  cl::Event Evt =
      hist(Args, D, DataSize, H, cl::Local(HistSize * sizeof(int)), HistSize);
  Evt.wait();

  cl::copy(Q_, H, Hist, Hist + HistSize);
  return Evt; // to collect profiling info
}

// read program code from file
std::string OclApp::readFile(const char *Path) {
  std::string Code;
  std::ifstream ShaderFile;
  ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  ShaderFile.open(Path);
  std::stringstream ShaderStream;
  ShaderStream << ShaderFile.rdbuf();
  ShaderFile.close();
  Code = ShaderStream.str();
  return Code;
}

// select first platform with some GPUs
cl::Platform OclApp::select_platform() {
  cl::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  for (auto p : platforms) {
    // note: usage of p() for plain id
    cl_uint numdevices = 0;
    ::clGetDeviceIDs(p(), CL_DEVICE_TYPE_GPU, 0, NULL, &numdevices);
    if (numdevices > 0)
      return cl::Platform(p); // retain?
  }
  throw std::runtime_error("No platform selected");
}

// get context for selected platform
cl::Context OclApp::get_gpu_context(cl_platform_id PId) {
  cl_context_properties properties[] = {
      CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(PId),
      0 // signals end of property list
  };

  return cl::Context(CL_DEVICE_TYPE_GPU, properties);
}

//-----------------------------------------------------------------------------
//
// Config methods
//
//-----------------------------------------------------------------------------

// options are:
// -dsz=<int>
// -hsz=<int>
// -lsz=<int>
// -kernel=<string>
// I don't want to make study example depend from boost::po, so keep it simple
Config Config::readCfg(int argc, char **argv) {
  Config Cfg;
  for (int i = 1; i < argc; ++i) {
    std::string_view Argvi = argv[i];
    auto ArgviEnd = Argvi.data() + Argvi.size();
    if (Argvi.starts_with("-dsz=")) {
      auto Result = std::from_chars(Argvi.data() + 5, ArgviEnd, Cfg.DataSize);
      if (Result.ec == std::errc::invalid_argument)
        std::cerr << "Can not parse -dsz option, using default\n";
    } else if (Argvi.starts_with("-gsz=")) {
      auto Result = std::from_chars(Argvi.data() + 5, ArgviEnd, Cfg.GlobalSize);
      if (Result.ec == std::errc::invalid_argument)
        std::cerr << "Can not parse -gsz option, using default\n";
    } else if (Argvi.starts_with("-hsz=")) {
      auto Result = std::from_chars(Argvi.data() + 5, ArgviEnd, Cfg.HistSize);
      if (Result.ec == std::errc::invalid_argument)
        std::cerr << "Can not parse -hsz option, using default\n";
    } else if (Argvi.starts_with("-lsz=")) {
      auto Result = std::from_chars(Argvi.data() + 5, ArgviEnd, Cfg.LSZ);
      if (Result.ec == std::errc::invalid_argument)
        std::cerr << "Can not parse -lsz option, using default\n";
    } else if (Argvi.starts_with("-kernel=")) {
      Cfg.PATH = argv[i] + 8;
    } else {
      throw option_error(argv[i]);
    }
  }
  return Cfg;
}

// dump config to stream
void Config::dump(std::ostream &Os) {
  Os << "data size = " << DataSize << "\n";
  Os << "hist size = " << HistSize << "\n";
  Os << "global size = " << GlobalSize << "\n";
  Os << "local size = " << LSZ << "\n";
  Os << "kernel path = " << PATH;
}

//-----------------------------------------------------------------------------
//
// Helpers
//
//-----------------------------------------------------------------------------

// generic random initialization
template <typename It> void rand_init(It start, It end, int low, int up) {
  static std::mt19937_64 mt_source;
  std::uniform_int_distribution<int> dist(low, up);
  for (It cur = start; cur != end; ++cur)
    *cur = dist(mt_source);
}

// reference histogram
template <typename TD, typename TH>
void hist_ref(const TD *Data, int DataSize, TH *Hist, int HistSize) {
  for (int i = 0; i < DataSize; ++i) {
    assert(Data[i] < HistSize && Data[i] >= 0);
    Hist[Data[i]] += 1;
  }
}
