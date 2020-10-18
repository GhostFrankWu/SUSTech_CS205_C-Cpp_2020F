#include "cuda_runtime.h" 
#include "device_launch_parameters.h" 
#include<memory> 
#include<iostream>
#include<chrono>
using namespace std; 
#define answer double
const int threadsPerBlock = 1024;
__global__ void dot(float* a, float* b, answer* c,int N) {//CUDA����
	__shared__ answer cache[threadsPerBlock];//�����ڴ�
	int tid = threadIdx.x + blockIdx.x * blockDim.x;
	int cacheIndex = threadIdx.x;
	answer temp = 0;
	while (tid < N) {
		temp += a[tid] * b[tid];
		tid += blockDim.x * gridDim.x;
	}
	cache[cacheIndex] = temp;
	__syncthreads();
	int i = blockDim.x / 2;
	while (i) {
		if (cacheIndex < i)
			cache[cacheIndex] += cache[cacheIndex + i];
		__syncthreads();
		i /= 2;
	}
	if (!cacheIndex)
		c[blockIdx.x] = cache[0];
}
int main() {
	std::chrono::time_point<std::chrono::system_clock> ss = std::chrono::system_clock::now();
	int N=0;
	FILE* fp = fopen("100000000.dat", "rb");//C�����ļ����룬��ռ�����ڴ���..
	FILE* fp2 = fopen("100000000.dat", "rb");
	if (fp) {
		_fread_nolock(&N, sizeof(int), 1, fp);
	}
	float* a, * b, * dev_a, * dev_b;;
	answer* partial_c, * dev_partial_c;
	a = (float*)malloc(N * sizeof(float));
	b = (float*)malloc(N * sizeof(float));
	partial_c = (answer*)malloc(sizeof(answer));
	if (fp) {
		_fread_nolock(a, N * sizeof(float), 1, fp);
		_fread_nolock(b, N * sizeof(float), 1, fp2);//���뵽���棬ռһ�������ڴ�
	}
	cudaMalloc((void**)& dev_a, N * sizeof(float));//new�Դ�ռ�
	cudaMalloc((void**)& dev_b, N * sizeof(float));
	cudaMalloc((void**)& dev_partial_c, sizeof(answer));
	cudaMemcpy(dev_a, a, N * sizeof(float), cudaMemcpyHostToDevice);//�����������Դ棬ͬ����IO
	cudaMemcpy(dev_b, b, N * sizeof(float), cudaMemcpyHostToDevice); 
	printf("=========CUDA�������ݺ�ʱ: %lld us==========\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - ss).count());
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	dot << <1, threadsPerBlock >> > (dev_a, dev_b, dev_partial_c, N);
	dot << <1, threadsPerBlock >> > (dev_a, dev_b, dev_partial_c, N);//�Ż��ڴ�
	now = std::chrono::system_clock::now();
	dot << <1, threadsPerBlock >> > (dev_a, dev_b, dev_partial_c, N);//����
	printf("=========CUDA�����ʱ: %lld us==========\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - now).count());
	now = std::chrono::system_clock::now();
	cudaMemcpy(partial_c, dev_partial_c, sizeof(answer), cudaMemcpyDeviceToHost);//��ȡ��
	if (sizeof(answer) > 5) {//���ݾ������������
		printf("Ans is %llf\n", *partial_c);
	}
	else {
		printf("Ans is %f\n", *partial_c);
	}
	cudaFree(dev_a);//��ջ���
	cudaFree(dev_b);
	cudaFree(dev_partial_c);
	free(a);
	free(b);
	free(partial_c);
	printf("=========�����������պ�ʱ: %lld ms==========\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - now).count() / 1000);
	system("pause>nul");
}