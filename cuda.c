#include <stdio.h>
#include <time.h>

__device__ float* array;

int isPowerOfTwo(int n) {
    if(n==0) return 0;

    if (ceil(log2(n)) == floor(log2(n)))
        return 1;
    else
        return 0;
}

/**
 * Combines two bitonic sequences together to create a new bitonic sequence.
 * @param a Pointer to the start of the bitonic sequence.
 * @param blockSize The size of each sub-array partition.
 * @param t Determines when to switch between ascending and descending.
 */
__global__ __forceinline__ void bitonicBuild(float* a, int blockSize, int t) {
	int index = threadIdx.x + blockIdx.x * blockSize;
	int x = 0;
	int asc = 1;
	float* b = a + index + (blockSize / 2);
	while(x > index) {
		x += t;
		asc++;
	}
	
	if(asc % 2 == 1) {
		atomicMin(&a[index], atomicMax(&b, a[index]));
	}
	else {
		atomicMax(&a[index], atomicMin(&b, a[index]));
	}
}

void bitonicBuildRunner(float* a, int size) {
	int blocks = size / 2;
	while(blocks != 1) {
		int i = blocks, blockSize = size * (1 - 1 / blocks);
		while(i != 1) {
			dim3 numBlocks = i, threadsPerBlock = blockSize;
			for(j = 0; j < size; j += blockSize, a++) {
				bitonicBuild<<<numBlocks, threadsPerBlock>>>(a, blockSize, i);
			}
			i /= 2;
		}
		blocks /= 2;
	}
}

void bitonicSortRunner(float* a, int size) {
	// Copy over memory
    float* array;
	int mem = sizeof(float) * size;
	cudaMalloc(array, mem);
	cudaMemcpy(array, a, mem, cudaMemcpyHostToDevice);
	
	int blocks = 1;
	while(blocks != size / 2) {
		// Execution config
        dim3 numBlocks = blocks;
		dim3 threadsPerBlock = size / blocks / 2;
		
		bitonicSort<<<numBlocks, threadsPerBlock>>>(array, size / blocks);
		size *= 2;
	}
}

/**
 * Applies the bitonic sorting algorithm to each thread. It swaps two
 * elements in the two lists if they are out of place.
 */
__global__ __forceinline__ void bitonicSort(float* a, int blockSize) {
	// First we need to figure out what index each thread will access
    int index = threadIdx.x + blockIdx.x * blockSize;
	atomicMin(&a[i + index];
	atomicMax(&a[i + blockSize + index], a[i + index]));
	__syncthreads();
}
 
int main() {
	// Input in array...
    float* array;
	
	// BEGIN
    int n, i, s;
	FILE *f = fopen("dane.txt","r");

	fscanf(f,"%d",&n);

	checkIfPowerof2 = isPowerOfTwo(n);

    if (checkIfPowerof2 == 0) {
        printf("The length of a sequence must be a power of 2.\n");
        fclose(f);
        return 0;
    }
	
	array = (float*) malloc(n * sizeof(float));
	for(i = 0; i < n; i++) {
		fscanf(f,"%d",(arr + i));
	}
	// END
    
	// Size of array;
    int size;
	
	/// Start time.
    time_t  = time(NULL);

    bitonicBuildRunner(array, size);
	bitonicSortRunner(array, size);

	// Stop the time;
    time_t end = time(NULL);
	printf("Sequential bitonic took: %.2f sec.\n", difftime(end, start));
}