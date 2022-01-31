//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple vectoradd OpenCL application
//
// clang++ -o vectoradd.exe cl_vectoradd.cc -lOpenCL
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS

#include "CL/opencl.hpp"

#ifndef ANALYZE
#define ANALYZE 1
#endif

#define dbgs                                                                   \
  if (!ANALYZE) {                                                              \
  } else                                                                       \
    std::cout

constexpr size_t ARR_SIZE = 64;
constexpr size_t LOCAL_SIZE = 1;

#define STRINGIFY(...) #__VA_ARGS__

// This example have built-in kernel to easy modify, etc
// ---------------------------------- OpenCL ---------------------------------
const char *vakernel = STRINGIFY(__kernel void vector_add(
    __global int *A, __global int *B, __global int *C) {
  int i = get_global_id(0);
  C[i] = A[i] + B[i];
});
// ---------------------------------- OpenCL ---------------------------------

// OpenCL application encapsulates platform, context and queue
// We can offload vector addition through its public interface
class OclApp {
  cl::Platform P_;
  cl::Context C_;
  cl::CommandQueue Q_;

  static cl::Platform select_platform();
  static cl::Context get_gpu_context(cl_platform_id);

  using vadd_t = cl::KernelFunctor<cl::Buffer, cl::Buffer, cl::Buffer>;

public:
  OclApp() : P_(select_platform()), C_(get_gpu_context(P_())), Q_(C_) {
    cl::string name = P_.getInfo<CL_PLATFORM_NAME>();
    cl::string profile = P_.getInfo<CL_PLATFORM_PROFILE>();
    dbgs << "Selected: " << name << ": " << profile << std::endl;
  }

  // C[i] = A[i] + B[i]
  // Here we shall ask ourselfes: why not template?
  void vadd(cl_int const *A, cl_int const *B, cl_int *C, size_t Sz);
};

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

void OclApp::vadd(cl_int const *APtr, cl_int const *BPtr, cl_int *CPtr,
                  size_t Sz) {
  size_t BufSz = Sz * sizeof(cl_int);

  cl::Buffer A(C_, CL_MEM_READ_ONLY, BufSz);
  cl::Buffer B(C_, CL_MEM_READ_ONLY, BufSz);
  cl::Buffer C(C_, CL_MEM_WRITE_ONLY, BufSz);

  cl::copy(Q_, APtr, APtr + Sz, A);
  cl::copy(Q_, BPtr, BPtr + Sz, B);

  // try forget context here and happy debugging CL_INVALID_MEM_OBJECT:
  // cl::Program program(vakernel, true /* build immediately */);
  cl::Program program(C_, vakernel, true /* build immediately */);

  vadd_t add_vecs(program, "vector_add");

  cl::NDRange GlobalRange(Sz);
  cl::NDRange LocalRange(LOCAL_SIZE);
  cl::EnqueueArgs Args(Q_, GlobalRange, LocalRange);

  cl::Event evt = add_vecs(Args, A, B, C);
  evt.wait();

  cl::copy(Q_, C, CPtr, CPtr + Sz);
}

int main() try {
  OclApp app;
  cl::vector<cl_int> src1(ARR_SIZE), src2(ARR_SIZE), dst(ARR_SIZE);

  std::iota(src1.begin(), src1.end(), 0);
  std::iota(src2.rbegin(), src2.rend(), 0);

  app.vadd(src1.data(), src2.data(), dst.data(), dst.size());

  for (int i = 0; i < ARR_SIZE; ++i) {
    auto lhs = dst[i];
    auto rhs = src1[i] + src2[i];
    if (lhs != rhs) {
      std::cerr << "Error at index " << i << ": " << lhs << " != " << rhs
                << std::endl;
      return -1;
    }
  }
  std::cout << "All checks passed" << std::endl;
} catch (cl::Error &err) {
  std::cerr << "OCL ERROR " << err.err() << ":" << err.what() << std::endl;
  return -1;
} catch (std::runtime_error &err) {
  std::cerr << "RUNTIME ERROR " << err.what() << std::endl;
  return -1;
} catch (...) {
  std::cerr << "UNKNOWN ERROR\n";
  return -1;
}