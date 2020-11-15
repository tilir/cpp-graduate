//-----------------------------------------------------------------------------
//
// Selector class to reuse code
//
// Please include it after MYVERSION for cl_defs defined
// Compilation with C++20 only for basic_string::starts_with
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#pragma once

#ifndef MYVERSION
#error "Please include it after MYVERSION for cl_defs defined"
#endif

#if __cplusplus <= 201703L
#error "Please compile with C++2a/C++20 for basic_string::starts_with"
#endif

#include "cl_defs.h"
#include "CL/opencl.hpp"

// intended to be private base only => no virtual dtor
struct ocl_selector_t {
  cl::Platform platform;
  cl::vector<cl::Device> devices;

  ocl_selector_t() = default;

  ocl_selector_t(std::string prefix) {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    std::cout << "Platforms:" << std::endl;
    for (auto p : platforms) {
      std::string vendor = p.getInfo<CL_PLATFORM_VENDOR>();
      std::string version = p.getInfo<CL_PLATFORM_VERSION>();
      std::cout << "\t* " << vendor << "\t" << version << std::endl;
      if (version.starts_with(prefix) || vendor.starts_with(prefix))
        cl::Platform::setDefault(p);
    }

    platform = cl::Platform::getDefault();
    std::cout << "selected: "
              << platform.getInfo<CL_PLATFORM_VENDOR>() << std::endl;

    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    std::cout << "Devices:" << std::endl;
    for (auto d : devices)
      std::cout << "\t* " << d.getInfo<CL_DEVICE_NAME>() << std::endl;
  }
};


