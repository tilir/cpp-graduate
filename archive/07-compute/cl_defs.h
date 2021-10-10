//-----------------------------------------------------------------------------
//
// Some defines before opencl.hpp inclusion
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#pragma once

#ifndef MYVERSION
#define MYVERSION 120
#endif

#ifndef CL_HPP_MINIMUM_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION MYVERSION
#endif

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_TARGET_OPENCL_VERSION MYVERSION
#endif

#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION MYVERSION
#endif

#define CL_HPP_ENABLE_EXCEPTIONS
