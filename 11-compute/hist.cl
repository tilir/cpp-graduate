//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Histogram kernel with local memory, barriers and atomics
//
//-----------------------------------------------------------------------------

__kernel void histogram(
    __global uchar *data, int num_data, __global int *histogram,
    __local int *local_hist, int num_bins) {
  int i;
  int lid = get_local_id(0);
  int gid = get_global_id(0);
  int lsize = get_local_size(0);
  int gsize = get_global_size(0);

  for (i = lid; i < num_bins; i += lsize)
    local_hist[i] = 0;

  barrier(CLK_LOCAL_MEM_FENCE);

  for (i = gid; i < num_data; i += gsize)
    atomic_add(&local_hist[data[i]], 1);

  barrier(CLK_LOCAL_MEM_FENCE);

  for (i = lid; i < num_bins; i += lsize)
    atomic_add(&histogram[i], local_hist[i]);
}