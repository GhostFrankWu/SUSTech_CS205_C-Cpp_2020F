#include <helper_string.h> 
#include <cuda_runtime.h>
#include <cublas_v2.h>
#include <helper_functions.h>
#include <helper_cuda.h>
#define uiHB uiWA
#define uiWC uiWB
#define uiHC uiHA
int main() {
    printf("��ʼ����ʼ...\n");
    const unsigned int block_size = 16, devID = 0;
    const unsigned int uiWA = 125 * block_size * 5;//������󳤿�
    const unsigned int uiHA = 125 * block_size * 5;
    const unsigned int uiWB = 125 * block_size * 5;
    cudaDeviceProp deviceProp;
    cudaGetDeviceProperties(&deviceProp, devID);//
    const unsigned int size_A = uiWA * uiHA, size_B = uiWB * uiHB, size_C = uiWC * uiHC;//�����ܴ�С
    const unsigned int mem_size_A = sizeof(float) * size_A, mem_size_B = sizeof(float) * size_B, mem_size_C = sizeof(float) * size_C;//����byte
    float* h_A = (float*)malloc(mem_size_A);//��ʼ���ڴ�
    float* h_B = (float*)malloc(mem_size_B);
    float* h_C = (float*)malloc(mem_size_C);//C:�𰸾���
    float* h_CUBLAS = (float*)malloc(mem_size_C);//CUBLAS:��ż������ľ���
    memset(h_A, 0, mem_size_A);//malloc����֤������ڴ�Ϊ0
    memset(h_B, 0, mem_size_B);
    memset(h_C, 0, mem_size_C);
    h_A[0] = 1.1;///////////////������β��ֵ
    h_A[size_A - 1] = 2.2;
    h_B[0] = 3;
    h_B[size_B - 1] = 4;
    h_C[0] = h_A[0] * h_B[0];////////////////��ȷ�𰸵ľ���
    h_C[uiHA * uiWB - 1] = h_A[uiHA * uiWA - 1] * h_B[uiWA * uiWB - 1];
    float* d_A, * d_B, * d_C;
    cudaMalloc((void**)&d_A, mem_size_A);//GPU�ڴ��ʼ��
    cudaMalloc((void**)&d_B, mem_size_B);
    cudaMemcpy(d_A, h_A, mem_size_A, cudaMemcpyHostToDevice);//���ƾ���AB
    cudaMemcpy(d_B, h_B, mem_size_B, cudaMemcpyHostToDevice);
    cudaMalloc((void**)&d_C, mem_size_C);
    dim3 threads(block_size, block_size);
    dim3 grid(uiWC / threads.x, uiHC / threads.y);
    constexpr int nIter = 9;//��������ƽ��
    printf("��ʼ���� A[%d*%d] * B[%d*%d] %d����ƽ����ʱ...\n", uiHA, uiWA, uiHB, uiWB, nIter + 1);
    const float alpha = 1.0f, beta = 0.0f;
    cublasHandle_t handle;
    cudaEvent_t start, stop;//cuda����ʱ���
    cublasCreate(&handle);
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, uiWB, uiHA, uiWA, &alpha, d_B, uiWB, d_A, uiWA, &beta, d_C, uiWB);//Ԥ�ȼ���һ��
    cudaEventRecord(start, NULL);//��ʱ��ʼ
    for (int j = 0; j < nIter; j++) {
        cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, uiWB, uiHA, uiWA, &alpha, d_B, uiWB, d_A, uiWA, &beta, d_C, uiWB);
    }
    cudaEventRecord(stop, NULL);//��ʱ����
    cudaEventSynchronize(stop);
    float msecTotal = 0.0f;
    cudaEventElapsedTime(&msecTotal, start, stop);
    printf("*ÿ�μ��㣺\nCUDA������ʱ %.3f ms, ������� %.0f\n", msecTotal / nIter, 2.0 * (double)uiHC * (double)uiWC * (double)uiHB);
    cudaMemcpy(h_CUBLAS, d_C, mem_size_C, cudaMemcpyDeviceToHost);
    printf("�������C[0,0]=%f C[%d,%d]=%f\n", h_CUBLAS[0], uiHA, uiWB, h_CUBLAS[uiHA * uiWB - 1]);//Ψ����������λ
    cublasDestroy(handle);
    printf("����������Ƿ���ȷ������λ��Ϊ0��: %s\n", sdkCompareL2fe(h_C, h_CUBLAS, size_C, 1.0e-6f) ? "��ȷ" : "FAIL");//�ȽϷ��صľ����Ƿ������۴𰸾�����ͬ
    free(h_A);//�ͷ��ڴ漰�Դ�Ļ���
    free(h_B);
    free(h_C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}