//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Histogram kernel -- naive and wrong (do you see why wrong?)
//
//-----------------------------------------------------------------------------

__kernel void histogram(
    __global uchar *data, int num_data, __global int *histogram,
    __local int *local_hist, int num_bins) {  
  int gid = get_global_id(0);
  int gsize = get_global_size(0);

  for (int i = gid; i < num_data; i += gsize)
    histogram[data[i]] += 1;
}