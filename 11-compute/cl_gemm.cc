//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple SGEMM/DGEMM OpenCL application
// GEMM is operation like: C = alpha * A * B + beta * C
// Here we just doing special case with alpha = 1.0, beta = 0.0
//
// clang++ -std=c++20 -DTYPE=float -o sgemm.exe cl_gemm.cc -lOpenCL
// clang++ -std=c++20 -DTYPE=double -o dgemm.exe cl_gemm.cc -lOpenCL
//
// sgemm.exe -kernel=gemm_simple.cl
// sgemm.exe -kernel=gemm_simple.cl -ay=5120
// sgemm.exe -kernel=gemm_localmem.cl -ay=5120
// sgemm.exe -kernel=gemm_localmem.cl -lsz=4 -ay=5120
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <charconv>
#include <chrono>
#include <sstream>
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

#ifndef TYPE
#define TYPE float
#endif

#define STRINGIFY(X) #X
#define TSTRINGIFY(X) STRINGIFY(X)
#define STYPE TSTRINGIFY(TYPE)

// config for program: we can also read it from options
struct Config {
  int AX = 256 * 2;
  int AY = 256 * 2;
  int BY = 256 * 2;
  int LSZ = 1;
  const char *PATH = "gemm_simple.cl";
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
template <typename It> void rand_init(It start, It end, TYPE low, TYPE up);

// basic reference multiplication
template <typename T>
void matrix_mult_ref(const T *A, const T *B, T *C, int AX, int AY, int BY);

// cache-friendly reference multiplication
template <typename T>
void transpose_mult_ref(const T *A, const T *B, T *C, int AX, int AY, int BY);

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

  using mmult_t =
      cl::KernelFunctor<cl::Buffer, cl::Buffer, cl::Buffer, int, int, int>;

public:
  OclApp(Config Cfg)
      : P_(select_platform()), C_(get_gpu_context(P_())), Q_(C_, Cfg.QProps),
        K_(readFile(Cfg.PATH)), Cfg_(Cfg) {
    std::string Def;
    cl::string name = P_.getInfo<CL_PLATFORM_NAME>();
    cl::string profile = P_.getInfo<CL_PLATFORM_PROFILE>();
    dbgs << "Selected: " << name << ": " << profile << std::endl;

#if ENABLE_EXT
    // example of how to enable OpenCL extension
    Def = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n";
    K_ = Def + K_;
#endif
    Def = std::string("#define TYPE ") + STYPE + "\n";
    K_ = Def + K_;
    Def = std::string("#define TS ") + std::to_string(Cfg.LSZ) + "\n";
    K_ = Def + K_;
#if DUMPKERNEL
    dbgs << "-- Kernel --\n" << K_ << "\n-- End kernel --" << std::endl;
#endif
  }

  int localx() const { return Cfg_.LSZ; }
  int localy() const { return Cfg_.LSZ; }

  cl::Event mmult(const TYPE *A, const TYPE *B, TYPE *C, int AX, int AY,
                  int BY);
};

void outm(const TYPE *M, int MX, int MY) {
  for (int i = 0; i < MX; ++i) {
    for (int j = 0; j < MY; ++j)
      std::cout << M[i * MY + j] << " ";
    std::cout << std::endl;
  }
}

int main(int argc, char **argv) try {
  std::chrono::high_resolution_clock::time_point TimeStart, TimeFin;
  cl_ulong GPUTimeStart, GPUTimeFin;
  long Dur, GDur;
  Config Cfg = Config::readCfg(argc, argv);
  dbgs << "Hello from mmult. Config:\n" << Cfg << std::endl;

  OclApp App(Cfg);
  cl::vector<TYPE> A(Cfg.AX * Cfg.AY), B(Cfg.AY * Cfg.BY), C(Cfg.AX * Cfg.BY);

  // random initialize -- we just want to excersize and measure
  rand_init(A.begin(), A.end(), 0, 10);
  rand_init(B.begin(), B.end(), 0, 10);

  // do matrix multiply
  TimeStart = std::chrono::high_resolution_clock::now();
  cl::Event Evt =
      App.mmult(A.data(), B.data(), C.data(), Cfg.AX, Cfg.AY, Cfg.BY);
  TimeFin = std::chrono::high_resolution_clock::now();
  Dur =
      std::chrono::duration_cast<std::chrono::milliseconds>(TimeFin - TimeStart)
          .count();
  std::cout << "GPU wall time measured: " << Dur << " ms" << std::endl;
  GPUTimeStart = Evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
  GPUTimeFin = Evt.getProfilingInfo<CL_PROFILING_COMMAND_END>();
  GDur = (GPUTimeFin - GPUTimeStart) / 1000000; // ns -> ms
  std::cout << "GPU pure time measured: " << GDur << " ms" << std::endl;

#ifdef VISUALIZE
  std::cout << "--- Matrix ---\n";
  outm(C.data(), Cfg.AX, Cfg.BY);
  std::cout << "--- End Matrix ---\n";
#endif

#if COMPARE_CPU
  cl::vector<TYPE> CCPU(Cfg.AX * Cfg.BY);
  TimeStart = std::chrono::high_resolution_clock::now();
  transpose_mult_ref(A.data(), B.data(), CCPU.data(), Cfg.AX, Cfg.AY, Cfg.BY);
  TimeFin = std::chrono::high_resolution_clock::now();
  Dur =
      std::chrono::duration_cast<std::chrono::milliseconds>(TimeFin - TimeStart)
          .count();
  std::cout << "CPU time measured: " << Dur << " ms" << std::endl;

#ifdef VISUALIZE
  std::cout << "--- Matrix ---\n";
  outm(CCPU.data(), Cfg.AX, Cfg.BY);
  std::cout << "--- End Matrix ---\n";
#endif

  for (int i = 0; i < Cfg.AX * Cfg.BY; ++i) {
    auto lhs = C[i];
    auto rhs = CCPU[i];
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

cl::Event OclApp::mmult(const TYPE *APtr, const TYPE *BPtr, TYPE *CPtr, int AX,
                        int AY, int BY) {
  size_t ASz = AX * AY, ABufSz = ASz * sizeof(TYPE);
  size_t BSz = AY * BY, BBufSz = BSz * sizeof(TYPE);
  size_t CSz = AX * BY, CBufSz = CSz * sizeof(TYPE);

  cl::Buffer A(C_, CL_MEM_READ_ONLY, ABufSz);
  cl::Buffer B(C_, CL_MEM_READ_ONLY, BBufSz);
  cl::Buffer C(C_, CL_MEM_WRITE_ONLY, CBufSz);

  cl::copy(Q_, APtr, APtr + ASz, A);
  cl::copy(Q_, BPtr, BPtr + ASz, B);

  // try forget context here and happy debugging CL_INVALID_MEM_OBJECT:
  // cl::Program program(vakernel, true /* build immediately */);
  cl::Program program(C_, K_, true /* build immediately */);

  mmult_t gemm(program, "matrix_multiply");

  cl::NDRange GlobalRange(AX, BY); // do you understand why C size here?
  cl::NDRange LocalRange(localx(), localy());
  cl::EnqueueArgs Args(Q_, GlobalRange, LocalRange);

  cl::Event Evt = gemm(Args, A, B, C, AX, AY, BY);
  Evt.wait();

  cl::copy(Q_, C, CPtr, CPtr + CSz);
  return Evt;
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
// -ax=<int>
// -ay=<int>
// -by=<int>
// -lsz=<int>
// -kernel=<string>
// I don't want to make study example depend from boost::po, so keep it simple
Config Config::readCfg(int argc, char **argv) {
  Config Cfg;
  for (int i = 1; i < argc; ++i) {
    std::string_view Argvi = argv[i];
    auto ArgviEnd = Argvi.data() + Argvi.size();
    if (Argvi.starts_with("-ax=")) {
      auto Result = std::from_chars(Argvi.data() + 4, ArgviEnd, Cfg.AX);
      if (Result.ec == std::errc::invalid_argument)
        std::cerr << "Can not parse -ax option, using default\n";
    } else if (Argvi.starts_with("-ay=")) {
      auto Result = std::from_chars(Argvi.data() + 4, ArgviEnd, Cfg.AY);
      if (Result.ec == std::errc::invalid_argument)
        std::cerr << "Can not parse -ay option, using default\n";
    } else if (Argvi.starts_with("-by=")) {
      auto Result = std::from_chars(Argvi.data() + 4, ArgviEnd, Cfg.BY);
      if (Result.ec == std::errc::invalid_argument)
        std::cerr << "Can not parse -by option, using default\n";
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
  Os << "[" << AX << " x " << AY << "] * ";
  Os << "[" << AY << " x " << BY << "]\n";
  Os << "local size = [" << LSZ << " x " << LSZ << "]\n";
  Os << "kernel path = " << PATH;
}

//-----------------------------------------------------------------------------
//
// Helpers
//
//-----------------------------------------------------------------------------

// generic random initialization
template <typename It> void rand_init(It start, It end, TYPE low, TYPE up) {
  static std::mt19937_64 mt_source;
  std::uniform_int_distribution<int> dist(low, up);
  for (It cur = start; cur != end; ++cur)
    *cur = dist(mt_source);
}

// multiply on CPU, basic version
template <typename T>
void matrix_mult_ref(const T *A, const T *B, T *C, int AX, int AY, int BY) {
  assert(A != NULL && B != NULL && C != NULL);
  assert(AX > 0 && AY > 0 && BY > 0);
  int i, j, k;
  for (i = 0; i < AX; i++) {
    for (j = 0; j < BY; j++) {
      T acc = 0;
      for (k = 0; k < AY; k++)
        acc += A[i * AY + k] * B[k * BY + j];
      C[i * BY + j] = acc;
    }
  }
}

// multiply on CPU, cache-friendly version
template <typename T>
void transpose_mult_ref(const T *A, const T *B, T *C, int AX, int AY, int BY) {
  assert(A != NULL && B != NULL && C != NULL);
  assert(AX > 0 && AY > 0 && BY > 0);
  std::vector<T> tmp(BY * AY);
  int i, j, k;

  for (i = 0; i < AY; i++)
    for (j = 0; j < BY; j++)
      tmp[j * AY + i] = B[i * BY + j];

  for (i = 0; i < AX; i++) {
    for (j = 0; j < BY; j++) {
      T acc = 0;
      for (k = 0; k < AY; k++)
        acc += A[i * AY + k] * tmp[j * AY + k];
      C[i * BY + j] = acc;
    }
  }
}
