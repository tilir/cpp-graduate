// external define TYPE required (added by framwork after reading kernel)

__kernel void matrix_multiply(
    __global TYPE *A, __global TYPE *B, __global TYPE *C, int AX, int AY, int BY) {
  int row = get_global_id(0);
  int col = get_global_id(1);
  TYPE sum = 0;

  for (int k = 0; k < AY; k++)
    sum += A[row * AY + k] * B[k * BY + col];

  C[row * BY + col] = sum;
}