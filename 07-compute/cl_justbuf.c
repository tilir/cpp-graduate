//-----------------------------------------------------------------------------
//
// Reading and writing buffers, C way
//
//-----------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif

#include "CL/cl.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void cl_notify_fn(const char *errinfo, const void *private_info,
                         size_t cb, void *user_data);

void cl_process_error(cl_int ret, const char *file, int line);

#define CHECK_ERR(ret) cl_process_error(ret, __FILE__, __LINE__)

struct ocl_ctx_t {
  cl_context ctx;
  cl_command_queue que;
};

void process_buffers(struct ocl_ctx_t *ct);

int main() {
  size_t ndevs;
  cl_int ret;
  cl_device_id *devs;
  struct ocl_ctx_t ct;

  // minimalistic setup
  ct.ctx = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU, &cl_notify_fn,
                                   NULL, &ret);
  CHECK_ERR(ret);

  ret = clGetContextInfo(ct.ctx, CL_CONTEXT_DEVICES, 0, NULL, &ndevs);
  CHECK_ERR(ret);

  assert(ndevs > 0);

  devs = (cl_device_id *)malloc(ndevs * sizeof(cl_device_id));
  ret = clGetContextInfo(ct.ctx, CL_CONTEXT_DEVICES, ndevs, devs, NULL);
  CHECK_ERR(ret);

#if CL_TARGET_OPENCL_VERSION < 200
  ct.que = clCreateCommandQueue(ct.ctx, devs[0], 0, &ret);
#else
  ct.que = clCreateCommandQueueWithProperties(ct.ctx, devs[0], NULL, &ret);
#endif

  CHECK_ERR(ret);

  // do some stuff

  process_buffers(&ct);

  // minimalistic teardown

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
