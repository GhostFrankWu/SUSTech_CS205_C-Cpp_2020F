#include "dot.h"
#include <thread>
#include <windows.h>
#include <tchar.h>
#include <atlstr.h> 
#include "timeTick.cpp"

inline answer SSE(const float* a, const float* b, unsigned int size){
	float z = 0.0f;
	answer fres = 0.0f;
	float ftmp[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	__m128 mres;
	if ((size / 4) != 0) {
		mres = _mm_load_ss(&z);
		for (unsigned int i = 0; i < size / 4; i++)
			mres = _mm_add_ps(mres, _mm_mul_ps(_mm_loadu_ps(&a[4 * i]),
				_mm_loadu_ps(&b[4 * i])));
		__m128 mv1 = _mm_movelh_ps(mres, mres);
		__m128 mv2 = _mm_movehl_ps(mres, mres);    
		mres = _mm_add_ps(mv1, mv2);     
		_mm_store_ps(ftmp, mres);
		fres = (double)ftmp[0] + (double)ftmp[1];
	}
	return fres;
}//8路循环要比4路慢，SSE再慢一些，应该是数据量的问题
inline answer dotProduct(float* a, float* b, unsigned int size) { // 四路循环展开
	answer ret = 0.0, dot1 = 0.0, dot2 = 0.0, dot3 = 0.0, dot4 = 0.0;
	for (unsigned i = 0; i < size / 4; ++i) {
		dot1 += (double)a[4 * i] * (double)b[4 * i];
		dot2 += (double)a[4 * i + 1] * (double)b[4 * i + 1];
		dot3 += (double)a[4 * i + 2] * (double)b[4 * i + 2];
		dot4 += (double)a[4 * i + 3] * (double)b[4 * i + 3];
	}
	ret = dot1 + dot2 + dot3 + dot4;
	for (unsigned int i = size / 4 * 4; i < size; ++i) {
		ret += (double)a[i] * (double)b[i];
	}
	return ret;
}

auto getCoresa() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return sysInfo.dwNumberOfProcessors;
}

answer sumanswerx;
thread* tarrayx[maxMachine];
void calculate(float* v1, float* v2, int n) {
	answer sumx = 0;
	sumanswerx += dotProduct(v1, v2, n);
}

void devideQuick(string str) {
	TimePoint nowTime;
	CString cstr = str.c_str();//类型转换
	LPCWSTR lpcwStr = cstr.AllocSysString();
	int cores = (int)getCoresa();//最佳线程数=CPU核数（假定CPU性能均衡）
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(lpcwStr,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwBytesInBlock = GetFileSize(hFile, NULL); //文件长度 
		HANDLE hFileMapping = CreateFileMapping(hFile,
			NULL, PAGE_READONLY, 0, 0, NULL);
		CloseHandle(hFile);  // 释放文件内核对象 
		if (hFileMapping != 0) {
			LPVOID pbFile = (LPVOID)MapViewOfFile(hFileMapping,
				FILE_MAP_READ, 0, 0, dwBytesInBlock);// 将文件数据映射到进程的地址空间 
			if (pbFile != 0) {
				int n = 0;
				memcpy(&n, pbFile, sizeof(int));
				pbFile= (__int64*)(((__int64)pbFile) + sizeof(int));// 偏移地址 
				float* floatdata1 = (float*)pbFile;
				pbFile = (__int64*)(((__int64)pbFile) + sizeof(float) * n);
				float* floatdata2 = (float*)pbFile;
				int delta = n / cores;
				__int64 offset = sizeof(float) * delta;
				for (int i = 0; i < cores - 1; i++) {
					tarrayx[i] = new thread(calculate,
						&(floatdata1[i * delta]),
						&(floatdata2[i * delta]),
						delta);
				}
				tarrayx[(cores - 1)] = new thread(calculate,
					&(floatdata1[(cores - 1) * delta]),
					&(floatdata2[(cores - 1) * delta]),
					n - (cores - 1) * delta);
				cout << "**********读取数据IO用时：" <<
					nowTime.timeDffer() / 1000 << "ms.\n";
				nowTime.setTime();
				for (int i = 0; i < cores; i++) {
					tarrayx[i]->join();
					delete tarrayx[i];
				}
				cout << setprecision(15) << sumanswerx << endl;
				sumanswerx = 0;
				cout << "**********多路循环计算用时：" <<
					nowTime.timeDffer() / 1000 << "ms.\n";
				nowTime.setTime();
			}
		}
	}
}

void devideQuickC(string str) {
	TimePoint nowTime;
	CString cstr = str.c_str();//类型转换
	LPCWSTR lpcwStr = cstr.AllocSysString();
	int cores = (int)getCoresa();//最佳线程数=CPU核数（假定CPU性能均衡）
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(lpcwStr,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwBytesInBlock = GetFileSize(hFile, NULL); //文件长度 
		HANDLE hFileMapping = CreateFileMapping(hFile,
			NULL, PAGE_READONLY, 0, 0, NULL);
		CloseHandle(hFile);  // 释放文件内核对象 
		if (hFileMapping != 0) {
			LPVOID pbFile = (LPVOID)MapViewOfFile(hFileMapping,
				FILE_MAP_READ, 0, 0, dwBytesInBlock);// 将文件数据映射到进程的地址空间 
			if (pbFile != 0) {
				int n = 0;
				memcpy(&n, pbFile, sizeof(int));
				pbFile = (__int64*)(((__int64)pbFile) + sizeof(int));// 偏移地址
				float* floatdata1 = (float*)malloc(sizeof(float) * n);
				memcpy(floatdata1, pbFile, sizeof(float) * n);
				pbFile = (__int64*)(((__int64)pbFile) + sizeof(float) * n);
				float* floatdata2 = (float*)malloc(sizeof(float) * n);
				memcpy(floatdata2, pbFile, sizeof(float) * n);
				int delta = n / cores;
				__int64 offset = sizeof(float) * delta;
				for (int i = 0; i < cores - 1; i++) {
					tarrayx[i] = new thread(calculate,
						&(floatdata1[i * delta]),
						&(floatdata2[i * delta]),
						delta);
				}
				tarrayx[(cores - 1)] = new thread(calculate,
					&(floatdata1[(cores - 1) * delta]),
					&(floatdata2[(cores - 1) * delta]),
					n - (cores - 1) * delta);
				cout << "**********读取数据IO用时：" <<
					nowTime.timeDffer() / 1000 << "ms.\n";
				nowTime.setTime();
				for (int i = 0; i < cores; i++) {
					tarrayx[i]->join();
					delete tarrayx[i];
				}
				cout << setprecision(15) << sumanswerx << endl;
				sumanswerx = 0;
				cout << "**********多路循环计算用时：" <<
					nowTime.timeDffer() / 1000 << "ms.\n";
				nowTime.setTime();
				free(floatdata1);
				free(floatdata2);
				UnmapViewOfFile(pbFile);//清理
				CloseHandle(hFileMapping);
				cout << "**********清空内存用时：" <<
					nowTime.timeDffer() / 1000 << "ms.\n";
			}
		}
	}
}