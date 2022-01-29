//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Reading and writing buffers, C++ way
//
// clang++ cl_justbuf.cc -lOpenCL
//
//-----------------------------------------------------------------------------

#include <iostream>
#include <stdexcept>
#include <vector>

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#define CL_HPP_ENABLE_EXCEPTIONS

#include "CL/opencl.hpp"

namespace {

// first platform with some GPUs...
cl::Platform select_platform() {
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

enum { BUFSZ = 128 };

} // namespace

int main() try {
  cl::Platform P = select_platform();
  cl::string name = P.getInfo<CL_PLATFORM_NAME>();
  cl::string profile = P.getInfo<CL_PLATFORM_PROFILE>();
  std::cout << "Selected: " << name << ": " << profile << std::endl;

  cl_context_properties properties[] = {
      CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(P()),
      0 // signals end of property list
  };

  cl::Context C(CL_DEVICE_TYPE_GPU, properties);
  cl::CommandQueue Q(C);

  cl::vector<int> A(BUFSZ), B(BUFSZ);
  for (int i = 0; i < BUFSZ; ++i)
    A[i] = i * i;

  cl::Buffer D(C, CL_MEM_READ_WRITE, BUFSZ * sizeof(int));

  cl::copy(Q, A.begin(), A.end(), D);
  cl::copy(Q, D, B.begin(), B.end());

  for (int i = 0; i < BUFSZ; ++i) {
    if (B[i] != i * i) {
      std::cout << "Error at: " << i << ": " << B[i] << " != " << i * i
                << std::endl;
      return -1;
    }
  }

  std::cout << "Checks passed" << std::endl;
} catch (cl::Error err) {
  std::cerr << "ERROR " << err.err() << ":" << err.what() << std::endl;
  return -1;
}