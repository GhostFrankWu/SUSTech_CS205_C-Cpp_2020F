#include "matrix.h"

//暴力算法，O(n^3)
inline void bf(float* a, float* b, float* c, const unsigned int wa, const unsigned int ha, const unsigned int wb) {
	for (int i = 0; i < ha; i++){
		for (int j = 0; j < wb; j++){
			for (int k = 0; k < wa; k++){
				c[wb * i + j] += a[wa * i + k] * b[wb * k + j];
			}
		}
	}
}

#include <thread>
#if defined(__linux__) || defined(__linux)   //自动识别系统类型获取CPU核数
#include "unistd.h"
auto getCores() {
	return sysconf(_SC_NPROCESSORS_ONLN));
}
#elif (defined(_WIN64) || defined(__WIN64__) || defined(_WIN32) || defined(__WIN32__))
#include <windows.h>
auto getCores() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return sysInfo.dwNumberOfProcessors;
}
#  endif
thread* tarray[maxMachine];

//普通多线程
inline void dbf(float* a, float* b, float* c, const unsigned int wa, const unsigned int s, const unsigned int e, const unsigned int wb) {
	for (int i = s; i < e; i++) {
		for (int j = 0; j < wb; j++) {
			for (int k = 0; k + 3 < wa; k += 4) {
				c[wb * i + j] += a[wa * i + k] * b[wb * k + j];
				c[wb * i + j] += a[wa * i + k + 1] * b[wb * (k + 1) + j];
				c[wb * i + j] += a[wa * i + k + 2] * b[wb * (k + 2) + j];
				c[wb * i + j] += a[wa * i + k + 3] * b[wb * (k + 3) + j];
			}
			for (int k = wa - wa % 4; k < wa; k++) {
				c[wb * i + j] += a[wa * i + k] * b[wb * k + j];
			}
		}
	}
}
void simpleDevide(float* a, float* b, float* c, const unsigned int wa, const unsigned int ha, const unsigned int wb) {
	int cores = (int)getCores();
	int threads = ha / cores;
	for (int i = 0; i < cores; i ++) {
		tarray[i] = new thread(dbf, a, b, c, wa, i * threads, i * threads+threads, wb);
	}
	for (int i = cores * threads; i < ha; i++) {
		for (int j = 0; j < wb; j++) {
			for (int k = 0; k < wa; k++) {
				c[wb * i + j] += a[wa * i + k] * b[wb * k + j];
			}
		}
	}
	for (int i = 0; i < cores; i++) {
		tarray[i]->join();
	}
}

//多线程+分块
inline void do_block(int n, float* A, float* B, float* C,int BLOCKSIZE) {
	for (int i = 0; i < BLOCKSIZE; i++) {
		for (int j = 0; j < BLOCKSIZE; j++) {
			float cur = C[i * n + j];
			for (int k = 0; k < BLOCKSIZE-3; k+=4) {
				cur += A[i * n + k] * B[k * n + j];
				cur += A[i * n + k + 1] * B[(k + 1) * n + j];
				cur += A[i * n + k + 2] * B[(k + 2) * n + j];
				cur += A[i * n + k + 3] * B[(k + 3) * n + j];
			}
			for (int k = BLOCKSIZE - BLOCKSIZE % 4; k < BLOCKSIZE; k++) {
				cur += A[i * n + k] * B[k * n + j];
			}
			C[i * n + j] = cur;
		}
	}
}
inline void block(int n,int BLOCKSIZE,float *A,float*B,float*C,int sj) {
	for (int si = 0; si < n; si += BLOCKSIZE) {
		for (int sk = 0; sk < n; sk += BLOCKSIZE) {
			do_block(n, A + si * n + sk, B + sk * n + sj, C + si * n + sj, BLOCKSIZE);
		}
	}
}
void blockDevide(int n, float* A, float* B, float* C) {
	int BLOCKSIZE = 200;//n / (int)getCores();//最佳线程数=CPU核数（假定CPU性能均衡）
	while (n % BLOCKSIZE != 0)BLOCKSIZE--;
	int cnt = 0;
	for (int sj = 0; sj+ BLOCKSIZE <= n; sj += BLOCKSIZE) {
		tarray[cnt++] = new thread(block, n, BLOCKSIZE, A, B, C, sj);
		if (cnt == 50) {
			while (cnt--) {
				tarray[cnt]->join();
			}
		}
	}
	while (cnt-->0) {
		tarray[cnt]->join();
	}
}

/// <summary>
/// 核验矩阵是否可以计算，格式规范则进行计算
/// </summary>
/// <param name="A">原矩阵A</param>
/// <param name="B">原矩阵B</param>
/// <param name="C">结果矩阵</param>
/// <param name="mode">计算模式</param>
/// <returns>输入的矩阵格式正确则计算后返回ture，否则直接返回false</returns>
bool matMul(Matrix A, Matrix B, Matrix C,int mode) {
	return matMul(A.getRaw(), A.getRow(), A.getCol(), B.getRaw(), B.getRow(), B.getCol(), C.getRaw(), C.getRow(), C.getCol(),mode);
}

bool matMul(float* a, const unsigned int hA, const unsigned int wA, float* b, const unsigned int hB,
	const unsigned int wB, float* c, const unsigned int hC, const unsigned int wC, const unsigned int mode) {
	if (hB - wA || wC - wB || hC - hA) {
		cout << "输入矩阵的大小不匹配,请检查" << endl;
		return false;
	}
	else {
		switch (mode){
		case BruteForce://1000 
			bf(a, b, c, wA, hA, wB);
			break;
		case DevideBlock:
			blockDevide(hA, a, b, c);
			break;
		case DevideBruteForce://5000 150s
			simpleDevide(a, b, c, wA, hA, wB);
			break;
		default:
			return false;
		}
	}
	return true;
}