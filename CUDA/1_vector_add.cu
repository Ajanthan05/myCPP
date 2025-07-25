#include <iostream>
#include <cuda_runtime.h>

#define N 512  // number of elements

/*   
    CONFIURE LAUNCH PARAMETERS
Block and Grid Dimensions
dim3 grid_size(x,y,z);
dim3 block_size(x,y,z);

dom3 is cuda data structure. Default val (1,1,1)


    LAUNCH_KERNEL
kernelName<<< grid_size, block_size >>>(...)

cudaMalloc( location, size);
cudaFree();

cudaMemcpy( dst, src, numBytes, direction);
numBytes = N * sizeof(type)
direction
    cudaMemcpyHostToDevice
    cudaMemcpyDeviceToHost
*/



// Kernel definition
__global__ void VecAdd(float* A, float* B, float* C)
{
    int i = threadIdx.x;
    if (i < N) {
        C[i] = A[i] + B[i];
    } 
}

// KERNEL DEFINITION
/*  __global__ is a "declaration Specifier" that alerts the compiler 
that a function should be compiled to run in device 

KERNELS must be return typed void
    => Variable operated on in the kernel need to be passed by reference 

    We simulate "pass by reference":- Pass the address of the parameter to the kernel.


*/
__global__ void kernel(int *d_out, int *d_in ) 
{
    // Perform this operation for every thread
    d_out[0] = d_in[0];
}
/*  THREAD INDEX
Each thread has it own thread index
    Accessible within a kernel through the build in threadIdx variable

Thread Block can have as many as 3-dimension
Index of a Thread
    threadIdx.x
    threadIdx.y`
    threadIdx.z

Index of a Block
    dim3 blockIdx;
    int blockIdx.x;
    int blockIdx.y;
    int blockIdx.z;

Dimension of a Grid
    dim3 gridDim;
    int gridDim.x;    
    int gridDim.y;    
    int gridDim.z;
    
Dimension of a Block
    dim3 blockDim;
    int blockDim.x;
    int blockDim.y;
    int blockDim.z;


    INDEXING WITHIN GRID

    *   threadIdx IS ONLY Unique within its own Thread Block
    
    i = threadIdx.x + blockInd.x * blockDim.x;


    Keyword     ExexuteOn      CallOn
    ------------------------------
    __device__  Device         Device
    __global__  Device         Host
    __host__    Host           Host


    Type    Variable    Description
    -------------------------------
    dim3    gridDim     Dimensions of grid
    uint3   blockIdx    Index of block within grid
    dim3    blockDim    Dimensions of block
    unit3   threadIdx   Index of thread within block
*/

// PARALLELIZE FOR LOOP
__global__ void increment_gpu(int *a, int n) {
    int i = threadIdx.x;
    if (i < N)  //Ensure that Kernel doesnot execute more Thread than the length of the array
        a[i] = a[i] + 1;
}

void T_increment_gpu() {
    int h_a[N];

    int *d_a;
    cudaMalloc( (void**)&d_a, N * sizeof(int) );

    cudaMemcpy(d_a, h_a, N*sizeof(int), cudaMemcpyHostToDevice );

    dim3 grid_size(1);
    dim3 block_size(N);

    increment_gpu<<<grid_size, block_size>>>( d_a, N );
}

int main()
{
    /*  Declare variables 
    Variable that live on the host      h_
    Variable that live on the device    d_
    */ 
    int *h_c, *d_c;

    // Allocate memory on the devise
    cudaMalloc( (void**)&d_c, sizeof(int) );

    // Assume h_c all ready initialize with data

    cudaMemcpy(d_c, h_c, sizeof(int), cudaMemcpyHostToDevice);

    // Configuration Parameters
    dim3 grid_size(1);   // Grid dimension 1 X 1 X 1 = 1 Block
    dim3 block_size(1);  // Blpck Dimension 1 X 1 X 1 = 1 Thread

    // kernel<<< grid_size, block_size >>>(...);


    cudaMemcpy(h_c, d_c, sizeof(int), cudaMemcpyDeviceToHost);
    // De-allocatememory
    cudaFree( d_c );
    free( h_c );


    // Host vectors
    float h_A[N], h_B[N], h_C[N];

    // Initialize host data
    for (int i = 0; i < N; i++) {
        h_A[i] = i * 1.0f;
        h_B[i] = i * 2.0f;
    }

    // Device vectors
    float *d_A, *d_B, *d_C;
    size_t size = N * sizeof(float);

    // Allocate device memory
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy data from host to device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Launch kernel with 1 block of N threads
    VecAdd<<<1, N>>>(d_A, d_B, d_C);

    // Copy result back to host
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost );

    // Verify results
    for (int i = 0; i < 10; i++) {
        std::cout << "h_C[" << i << "] = " << h_C[i] << std::endl;
    }

    // Free device memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
