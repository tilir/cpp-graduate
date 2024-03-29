//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Enumerating platforms and devices, OpenCL C way
//
// clang cl_platform.c -lOpenCL
//
//-----------------------------------------------------------------------------

#include <assert.h>
#include <stdio.h>

#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif

#include "CL/cl.h"

struct platforms_t {
  cl_uint n;
  cl_platform_id *ids;
};

void cl_process_error(cl_int ret, const char *file, int line) {
  const char *cause = "unknown";
  switch (ret) {
  case CL_SUCCESS:
    return;
  case CL_DEVICE_NOT_FOUND:
    fprintf(stderr, "devices for this platform not found\n");
    return;
  case CL_INVALID_DEVICE:
    cause = "invalid device";
    break;
  case CL_INVALID_DEVICE_TYPE:
    cause = "invalid device type";
    break;
  case CL_INVALID_PLATFORM:
    cause = "invalid platform";
    break;
  case CL_INVALID_VALUE:
    cause = "invalid value";
    break;
  case CL_OUT_OF_HOST_MEMORY:
    cause = "out of host memory";
    break;
  case CL_OUT_OF_RESOURCES:
    cause = "out of resources";
    break;
  }

  fprintf(stderr, "Error %s at %s:%d code %d\n", cause, file, line, ret);
  abort();
}

#define CHECK_ERR(ret) cl_process_error(ret, __FILE__, __LINE__)

struct platforms_t get_platforms() {
  cl_int ret;
  struct platforms_t p;

  ret = clGetPlatformIDs(0, NULL, &p.n);
  CHECK_ERR(ret);
  assert(p.n > 0);

  p.ids = malloc(p.n * sizeof(cl_platform_id));
  assert(p.ids);

  ret = clGetPlatformIDs(p.n, p.ids, NULL);
  CHECK_ERR(ret);
  return p;
}

enum { STRING_BUFSIZE = 4096 };

void print_platform_info(cl_platform_id pid) {
  cl_int ret;
  char buf[STRING_BUFSIZE];

  ret = clGetPlatformInfo(pid, CL_PLATFORM_NAME, sizeof(buf), buf, NULL);
  CHECK_ERR(ret);
  printf("Platform: %s\n", buf);
  ret = clGetPlatformInfo(pid, CL_PLATFORM_VERSION, sizeof(buf), buf, NULL);
  CHECK_ERR(ret);
  printf("Version: %s\n", buf);
  ret = clGetPlatformInfo(pid, CL_PLATFORM_PROFILE, sizeof(buf), buf, NULL);
  CHECK_ERR(ret);
  printf("Profile: %s\n", buf);
  ret = clGetPlatformInfo(pid, CL_PLATFORM_VENDOR, sizeof(buf), buf, NULL);
  CHECK_ERR(ret);
  printf("Vendor: %s\n", buf);
  ret = clGetPlatformInfo(pid, CL_PLATFORM_EXTENSIONS, sizeof(buf), buf, NULL);
  CHECK_ERR(ret);
  printf("Extensions: %s\n", buf);
  printf("\n");
}

void print_device_info(cl_device_id devid) {
  cl_int ret;
  cl_uint ubuf, i;
  size_t *psbuf, sbuf;
  cl_bool cavail, lavail;
  char buf[STRING_BUFSIZE];

  ret = clGetDeviceInfo(devid, CL_DEVICE_NAME, sizeof(buf), buf, NULL);
  CHECK_ERR(ret);
  printf("Device: %s\n", buf);
  ret = clGetDeviceInfo(devid, CL_DEVICE_VERSION, sizeof(buf), buf, NULL);
  CHECK_ERR(ret);
  printf("OpenCL version: %s\n", buf);

  ret = clGetDeviceInfo(devid, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(ubuf), &ubuf,
                        NULL);
  CHECK_ERR(ret);
  printf("Max units: %u\n", ubuf);
  ret = clGetDeviceInfo(devid, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(ubuf),
                        &ubuf, NULL);
  CHECK_ERR(ret);
  printf("Max dimensions: %u\n", ubuf);

  psbuf = malloc(sizeof(size_t) * ubuf);
  ret = clGetDeviceInfo(devid, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                        sizeof(size_t) * ubuf, psbuf, NULL);
  CHECK_ERR(ret);
  printf("Max work item sizes: ");
  for (i = 0; i != ubuf; ++i)
    printf("%u ", (unsigned)psbuf[i]);
  printf("\n");
  free(psbuf);

  ret = clGetDeviceInfo(devid, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(sbuf),
                        &sbuf, NULL);
  CHECK_ERR(ret);
  printf("Max work group size: %u\n", (unsigned)sbuf);

  ret = clGetDeviceInfo(devid, CL_DEVICE_COMPILER_AVAILABLE, sizeof(cavail),
                        &cavail, NULL);
  CHECK_ERR(ret);
  printf("Compiler %savailable\n", cavail ? "" : "not ");
  ret = clGetDeviceInfo(devid, CL_DEVICE_LINKER_AVAILABLE, sizeof(lavail),
                        &lavail, NULL);
  CHECK_ERR(ret);
  printf("Linker %savailable\n", lavail ? "" : "not ");
  printf("\n");
}

void enumerate_devices(cl_platform_id platfid) {
  cl_int ret;
  cl_uint i, numdevices;
  cl_device_id *devices;

  ret = clGetDeviceIDs(platfid, CL_DEVICE_TYPE_ALL, 0, NULL, &numdevices);
  CHECK_ERR(ret);
  if (numdevices == 0) // no devices found
    return;

  devices = malloc(numdevices * sizeof(cl_device_id));
  assert(devices);

  ret = clGetDeviceIDs(platfid, CL_DEVICE_TYPE_ALL, numdevices, devices, NULL);
  CHECK_ERR(ret);

  for (i = 0; i < numdevices; ++i)
    print_device_info(devices[i]);

  free(devices);
}

int main() {
  cl_uint i;
  struct platforms_t platforms;

  platforms = get_platforms();

  for (i = 0; i < platforms.n; ++i) {
    print_platform_info(platforms.ids[i]);
    enumerate_devices(platforms.ids[i]);
  }

  free(platforms.ids);
  return 0;
}