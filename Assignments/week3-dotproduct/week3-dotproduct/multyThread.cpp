#include "dot.h"
#include <thread>

//自动识别系统类型获取CPU核数
#if defined(__linux__) || defined(__linux)
#include "unistd.h"
auto getCores() {
	return sysconf(_SC_NPROCESSORS_ONLN));
}
#elif (defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#include <windows.h>
auto getCores() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return sysInfo.dwNumberOfProcessors;
}
#  endif


//answer sumarray[maxMachine];
answer sumanswer;
thread* tarray[maxMachine];
void readpointer(string path1,int n,long long start1,long long start2,int index) {
	FILE* fp = fopen(path1.c_str(), "rb");
	FILE* fp2 = fopen(path1.c_str(), "rb");
	float f1=0, f2=0;
	answer sum = 0;
	if (fp) {
		_fseeki64_nolock(fp, start1, SEEK_SET);
		_fseeki64_nolock(fp2, start2, SEEK_SET);
		for (int i = 0; i < n; i++) {
			//_fread_nolock_s(&f1, sizeof(float), sizeof(float), 1, fp);
			//_fread_nolock_s(&f2, sizeof(float), sizeof(float), 1, fp2);
			_fread_nolock(&f1, sizeof(float), 1, fp);
			_fread_nolock(&f2, sizeof(float), 1, fp2);
			sum += f1 * f2;
		}
		fclose(fp);
		fclose(fp2);
	}
	sumanswer += sum;
	//sumarray[index] = sum;
}

void devide(string str) {
	int cores = (int)getCores();//最佳线程数=CPU核数（假定CPU性能均衡）
	//memset(sumarray, 0, 100 * sizeof(float));
	FILE* fp = fopen(str.c_str(), "rb");
	float f;
	//float sum = 0;
	if (fp) {
		int n = 0, delta = 0;
		fread(&n, sizeof(int), 1, fp);
		fclose(fp);
		delta = n / cores;
		long long start1, start2, shiftx = delta * sizeof(float);
		for (int i = 0; i < cores - 1; i++) {
			start1 = sizeof(int) + (long long)i * shiftx;
			start2 = start1 + (long long)n * sizeof(float);
			tarray[i] = new thread(readpointer, str, delta, start1, start2, i);
			//tarray[i]->detach();
		}
		start1 = sizeof(int) + (long long)(cores - 1) * shiftx;
		start2 = start1 + (long long)n * sizeof(float);
		delta = n - delta * (cores - 1);
		tarray[(cores - 1)] = new thread(readpointer,
			str, delta, start1, start2, (cores - 1));
		for (int i = 0; i < cores; i++) {
			tarray[i]->join();
			delete tarray[i];
			//sum += sumarray[i];
		}
	}
	//cout << setprecision(15) << sum << endl;
	cout << setprecision(15) << sumanswer << endl;
	sumanswer = 0;
}