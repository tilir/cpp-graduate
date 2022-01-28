//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Reading and writing buffers, C way
//
// clang cl_justbuf.c -lOpenCL
//
//-----------------------------------------------------------------------------

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif

#include "CL/cl.h"

static void cl_notify_fn(const char *errinfo, const void *private_info,
                         size_t cb, void *user_data);

void cl_process_error(cl_int ret, const char *file, int line);

#define CHECK_ERR(ret) cl_process_error(ret, __FILE__, __LINE__)

struct ocl_ctx_t {
  cl_context ctx;
  cl_command_queue que;
};

void process_buffers(struct ocl_ctx_t *ct);

struct platforms_t {
  cl_uint n;
  cl_platform_id *ids;
};

enum { STRING_BUFSIZE = 4096 };

// get any full profile platform
cl_platform_id select_platform() {
  cl_uint i;
  cl_int ret;
  struct platforms_t p;
  cl_platform_id selected = 0;

  ret = clGetPlatformIDs(0, NULL, &p.n);
  CHECK_ERR(ret);
  assert(p.n > 0);

  p.ids = malloc(p.n * sizeof(cl_platform_id));
  assert(p.ids);

  ret = clGetPlatformIDs(p.n, p.ids, NULL);
  CHECK_ERR(ret);

  for (i = 0; i < p.n; ++i) {
    char buf[STRING_BUFSIZE];
    cl_platform_id pid = p.ids[i];
    ret = clGetPlatformInfo(pid, CL_PLATFORM_PROFILE, sizeof(buf), buf, NULL);
    CHECK_ERR(ret);
    if (!strcmp(buf, "FULL_PROFILE")) {
      cl_uint numdevices = 0;
      ret = clGetPlatformInfo(pid, CL_PLATFORM_NAME, sizeof(buf), buf, NULL);
      CHECK_ERR(ret);
      ret = clGetDeviceIDs(pid, CL_DEVICE_TYPE_GPU, 0, NULL, &numdevices);
      if (numdevices == 0)
        continue;
      CHECK_ERR(ret);
      printf("Selected: %s, # of GPU devices = %d\n", buf, numdevices);
      selected = pid;
      break;
    }
  }

  free(p.ids);

  if (selected == 0) {
    fprintf(stderr, "No platform matches FULL_PROFILE\n");
    abort();
  }

  return selected;
}

int main() {
  size_t ndevs;
  cl_int ret;
  cl_device_id *devs;
  struct ocl_ctx_t ct;
  cl_platform_id selected_platform_id;

  selected_platform_id = select_platform();

  cl_context_properties properties[] = {
      CL_CONTEXT_PLATFORM, (cl_context_properties)selected_platform_id,
      0 // signals end of property list
  };

  // try NULL here instead of properties
  ct.ctx = clCreateContextFromType(properties, CL_DEVICE_TYPE_GPU,
                                   &cl_notify_fn, NULL, &ret);
  CHECK_ERR(ret);

  ret = clGetContextInfo(ct.ctx, CL_CONTEXT_DEVICES, 0, NULL, &ndevs);
  CHECK_ERR(ret);

  assert(ndevs > 0);

  devs = malloc(ndevs * sizeof(cl_device_id));
  ret = clGetContextInfo(ct.ctx, CL_CONTEXT_DEVICES, ndevs, devs, NULL);
  CHECK_ERR(ret);

#if CL_TARGET_OPENCL_VERSION < 200
  ct.que = clCreateCommandQueue(ct.ctx, devs[0], 0, &ret);
#else
  ct.que = clCreateCommandQueueWithProperties(ct.ctx, devs[0], NULL, &ret);
#endif
  CHECK_ERR(ret);

  process_buffers(&ct);

  ret = clFlush(ct.que);
  CHECK_ERR(ret);

  ret = clFinish(ct.que);
  CHECK_ERR(ret);

  ret = clReleaseCommandQueue(ct.que);
  CHECK_ERR(ret);

  ret = clReleaseContext(ct.ctx);
  CHECK_ERR(ret);
  free(devs);
}

enum { BUFSZ = 128 };

// just create buffer, populate it, read back in second buffer and check
void process_buffers(struct ocl_ctx_t *pct) {
  int A[BUFSZ], i;
  int B[BUFSZ] = {0};
  cl_mem oclbuf;
  cl_int ret;

  for (i = 0; i < BUFSZ; ++i)
    A[i] = i * i;

  oclbuf = clCreateBuffer(pct->ctx, CL_MEM_READ_WRITE, BUFSZ * sizeof(int),
                          NULL, &ret);
  CHECK_ERR(ret);

  // A --> oclbuf
  ret = clEnqueueWriteBuffer(pct->que, oclbuf, CL_TRUE, 0, BUFSZ * sizeof(int),
                             A, 0, NULL, NULL);
  CHECK_ERR(ret);

  // oclbuf --> B
  ret = clEnqueueReadBuffer(pct->que, oclbuf, CL_TRUE, 0, BUFSZ * sizeof(int),
                            B, 0, NULL, NULL);
  CHECK_ERR(ret);

  for (i = 0; i < BUFSZ; ++i)
    if (B[i] != i * i) {
      fprintf(stderr, "Error: B[%d] != %d * %d\n", i, i, i);
      abort();
    }

  fprintf(stdout, "%s\n", "Everything is correct");
  clReleaseMemObject(oclbuf);
}

void cl_process_error(cl_int ret, const char *file, int line) {
  const char *cause = "unknown";
  switch (ret) {
  case CL_SUCCESS:
    return;
  case CL_DEVICE_NOT_AVAILABLE:
    cause = "devices for this platform not available\n";
    break;
  case CL_DEVICE_NOT_FOUND:
    cause = "devices for this platform not found\n";
    break;
  case CL_INVALID_CONTEXT:
    cause = "invalid context";
    break;
  case CL_INVALID_PLATFORM:
    cause = "invalid platform";
    break;
  case CL_INVALID_VALUE:
    cause = "invalid value";
    break;
  case CL_INVALID_DEVICE:
    cause = "invalid device";
    break;
  case CL_INVALID_DEVICE_TYPE:
    cause = "invalid device type";
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

static void cl_notify_fn(const char *errinfo, const void *private_info,
                         size_t cb, void *user_data) {
  fprintf(stderr, "Context error: %s\n", errinfo);
  abort();
}