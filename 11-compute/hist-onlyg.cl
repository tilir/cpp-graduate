__kernel void histogram(
    __global uchar *data, int num_data, __global int *histogram,
    __local int *local_hist, int num_bins) {  
  int gid = get_global_id(0);
  int gsize = get_global_size(0);

  for (int i = gid; i < num_data; i += gsize)
    histogram[data[i]] += 1;
}