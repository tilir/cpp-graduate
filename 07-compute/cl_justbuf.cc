//-----------------------------------------------------------------------------
//
// Reading and writing buffers, C++ way
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

// g++ --std=c++2a cl_justbuf.cc -lOpenCL

#include <iostream>
#include <stdexcept>
#include <vector>

#define MYVERSION 120
#include "cl_selector.hpp"

#include "cl_defs.h"
#include "CL/opencl.hpp"

class ocl_ctx_t : private ocl_selector_t {
  cl::Context context;
  cl::CommandQueue queue;

public:
  using ocl_selector_t::devices;

#ifdef USESELECTOR
  // try here:
  //   MYVERSION = 120 and NVIDIA
  //   MYVERSION = 200 and NVIDIA (error in CreateCommandQueue)
  //   MYVERSION = 120 and Intel
  //   MYVERSION = 200 and Intel
  ocl_ctx_t() : ocl_selector_t{"Intel"}, context{devices}, queue{context} {}
#else
  ocl_ctx_t() : context{CL_DEVICE_TYPE_GPU}, queue{context} {
    // which devices are in context?
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    std::cout << "Devices:" << std::endl;
    for (auto d : devices)
      std::cout << "\t* " << d.getInfo<CL_DEVICE_NAME>() << std::endl;
  }
#endif

  void process_buffers();
};

int main() {
  try {
    ocl_ctx_t ct;
    ct.process_buffers();
  } catch (cl::Error err) {
    std::cerr << "ERROR " << err.err() << ":" << err.what() << "\n";
    return -1;
  }
}

constexpr size_t BUFSZ = 128;

class buf_write_error : public std::runtime_error {
  const char *msg_;
  int Bi_, i_;

public:
  buf_write_error(const char *msg, int Bi, int i)
      : std::runtime_error(msg), Bi_(Bi), i_(i) {}
  const char *what() const noexcept override {
    return std::runtime_error::what();
  }
};

void ocl_ctx_t::process_buffers() {
  std::vector<int> A(BUFSZ), B(BUFSZ);
  for (int i = 0; i < BUFSZ; ++i)
    A[i] = i * i;

  cl::Buffer oclbuf(context, CL_MEM_READ_WRITE, BUFSZ * sizeof(int));

  // A --> oclbuf
  cl::copy(queue, A.begin(), A.end(), oclbuf);

  // oclbuf --> B
  cl::copy(queue, oclbuf, B.begin(), B.end());

  for (int i = 0; i < BUFSZ; ++i)
    if (B[i] != i * i)
      throw buf_write_error("Buf checking error", B[i], i);

  std::cout << "Everything is correct" << std::endl;
}
