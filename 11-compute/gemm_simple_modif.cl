//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Matrix multiplication kernel -- simplest possible
//
// Here type substituted to float for offline compilation
//
//-----------------------------------------------------------------------------

#define TYPE float

__kernel void matrix_multiply(
    __global TYPE *A, __global TYPE *B, __global TYPE *C, int AX, int AY, int BY) {
  int row = get_global_id(0);
  int col = get_global_id(1);
  TYPE sum = 0;

  for (int k = 0; k < AY; k++)
    sum += A[row * AY + k] * B[k * BY + col];

  C[row * BY + col] = sum;
}