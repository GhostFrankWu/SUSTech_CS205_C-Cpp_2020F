#include "cuda_runtime.h" 
#include "device_launch_parameters.h" 
#include<memory> 
#include<iostream>
#include<chrono>
using namespace std; 
#define answer double
const int threadsPerBlock = 1024;
__global__ void dot(float* a, float* b, answer* c,int N) {//CUDA加速
	__shared__ answer cache[threadsPerBlock];//共享内存
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
	FILE* fp = fopen("100000000.dat", "rb");//C风格的文件读入，不占主机内存了..
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
		_fread_nolock(b, N * sizeof(float), 1, fp2);//读入到缓存，占一次主机内存
	}
	cudaMalloc((void**)& dev_a, N * sizeof(float));//new显存空间
	cudaMalloc((void**)& dev_b, N * sizeof(float));
	cudaMalloc((void**)& dev_partial_c, sizeof(answer));
	cudaMemcpy(dev_a, a, N * sizeof(float), cudaMemcpyHostToDevice);//主机拷贝到显存，同属于IO
	cudaMemcpy(dev_b, b, N * sizeof(float), cudaMemcpyHostToDevice); 
	printf("=========CUDA读入数据耗时: %lld us==========\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - ss).count());
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	dot << <1, threadsPerBlock >> > (dev_a, dev_b, dev_partial_c, N);
	dot << <1, threadsPerBlock >> > (dev_a, dev_b, dev_partial_c, N);//优化内存
	now = std::chrono::system_clock::now();
	dot << <1, threadsPerBlock >> > (dev_a, dev_b, dev_partial_c, N);//计算
	printf("=========CUDA计算耗时: %lld us==========\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - now).count());
	now = std::chrono::system_clock::now();
	cudaMemcpy(partial_c, dev_partial_c, sizeof(answer), cudaMemcpyDeviceToHost);//获取答案
	if (sizeof(answer) > 5) {//根据精度设置输出答案
		printf("Ans is %llf\n", *partial_c);
	}
	else {
		printf("Ans is %f\n", *partial_c);
	}
	cudaFree(dev_a);//清空缓存
	cudaFree(dev_b);
	cudaFree(dev_partial_c);
	free(a);
	free(b);
	free(partial_c);
	printf("=========输出及缓存清空耗时: %lld ms==========\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - now).count() / 1000);
	system("pause>nul");
}