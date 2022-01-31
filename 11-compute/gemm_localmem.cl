//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Matrix multiplication kernel with local memory
//
// external define TYPE required (added by framwork after reading kernel)
// external define TS required (added by framwork after reading kernel)
//
//-----------------------------------------------------------------------------

__kernel void matrix_multiply(
    __global TYPE *A, __global TYPE *B, __global TYPE *C, int AX, int AY, int BY) {
  int k, t;
  const int row = get_local_id(0);                  // Local row ID (max: TS)
  const int col = get_local_id(1);                  // Local col ID (max: TS)
  const int globalRow = TS * get_group_id(0) + row; // Row ID of C (0..M)
  const int globalCol = TS * get_group_id(1) + col; // Col ID of C (0..N)

  __local TYPE Asub[TS][TS];
  __local TYPE Bsub[TS][TS];

  TYPE acc = 0;

  const int numTiles = AY / TS;

  for (t = 0; t < numTiles; t++) {
    const int tiledRow = TS * t + row;
    const int tiledCol = TS * t + col;
    Asub[col][row] = A[globalRow * AY + tiledCol];
    Bsub[col][row] = B[tiledRow * BY + globalCol];

    // Synchronise to make sure the tile is loaded
    barrier(CLK_LOCAL_MEM_FENCE);

    for (k = 0; k < TS; k++)
      acc += Asub[k][row] * Bsub[col][k];

    // Synchronise before loading the next tile
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  // Store the final result in C
  C[globalRow * BY + globalCol] = acc;
}
