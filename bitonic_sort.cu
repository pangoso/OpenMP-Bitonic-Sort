#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* Every thread gets exactly one value in the unsorted array. */
#define THREADS 512 // 2^9
#define BLOCKS 262144 // 2^15
#define NUM_VALS THREADS*BLOCKS

void print_time(clock_t start, clock_t stop)
{
  double timed = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("Elapsed time: %.8fs\n", timed);
}

void print_array(int *array, int length) 
{
  for (int i = 0; i < length; ++i) {printf("%d ",  array[i]);}
  printf("\n");
}

void fill_array(int *array, int length)
{
  srand(time(NULL));
  for (int i = 0; i < length; ++i) {array[i] = (int)rand();}
}

__global__ void bitonic_sort_step(int *dev_values, int j, int k)
{
  unsigned int i, ixj; 
  i = threadIdx.x + blockDim.x * blockIdx.x;
  ixj = i^j;

  if ((ixj)>i) {
    if ((i&k)==0) {
      if (dev_values[i]>dev_values[ixj]) {
        int temp = dev_values[i];
        dev_values[i] = dev_values[ixj];
        dev_values[ixj] = temp;
      }
    }
    if ((i&k)!=0) {
      if (dev_values[i]<dev_values[ixj]) {
        int temp = dev_values[i];
        dev_values[i] = dev_values[ixj];
        dev_values[ixj] = temp;
      }
    }
  }
}

void bitonic_sort(int *values)
{
  int *dev_values;
  size_t size = NUM_VALS * sizeof(int);

  cudaMalloc((void**) &dev_values, size);
  cudaMemcpy(dev_values, values, size, cudaMemcpyHostToDevice);

  dim3 blocks(BLOCKS,1);
  dim3 threads(THREADS,1);

  int j, k;
  for (k = 2; k <= NUM_VALS; k <<= 1) {
    for (j=k>>1; j>0; j=j>>1) {
      bitonic_sort_step<<<blocks, threads>>>(dev_values, j, k);
    }
  }
  cudaMemcpy(values, dev_values, size, cudaMemcpyDeviceToHost);
  cudaFree(dev_values);
}

int main(void)
{
  clock_t start, stop;

  int *values = (int*) malloc( NUM_VALS * sizeof(int));
  fill_array(values, NUM_VALS);

  start = clock();
  bitonic_sort(values);
  stop = clock();
  print_array(values, NUM_VALS);

  print_time(start, stop);
}