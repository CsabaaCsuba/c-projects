#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

#define KEPERNYO_SZELESSEGE 6
#define KEPERNYO_MAGASSAGA 4

__global__ void run_gpu(int maxelemszam) {
    int x = threadIdx.x + (blockIdx.x * blockDim.x);
    int y = threadIdx.y + (blockIdx.y * blockDim.y);
    int aktualis_index = x + (y * blockDim.x * gridDim.x);

    if (aktualis_index >= maxelemszam)return;
    printf("Aktualis keppont: %d, %d\n", x, y);

}

int main() {
    int szalakX = 3, szalakY = 2;

    dim3 blokktomb((KEPERNYO_SZELESSEGE + szalakX - 1) / szalakX, (KEPERNYO_MAGASSAGA + szalakY - 1) / szalakY);
    dim3 szaltomb(szalakX, szalakY);

    run_gpu <<< blokktomb, szaltomb >>> (KEPERNYO_SZELESSEGE * KEPERNYO_MAGASSAGA);
    cudaDeviceSynchronize();
    printf("Végrehajtás befejezve");


    return 0;
}