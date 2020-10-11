#include "dot.h"
#include <thread>
#include <windows.h>
#include <tchar.h>

#define maxMachine 100

float* v1;
HANDLE hFile = INVALID_HANDLE_VALUE;
answer sumanswer;
void memread(LPCWSTR path) {
	answer sum = 0;
	hFile = CreateFile(path,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwBytesInBlock = GetFileSize(hFile, NULL); //�ļ����� 
		HANDLE hFileMapping = CreateFileMapping(hFile,
			NULL,
			PAGE_READONLY,
			0,//(DWORD)(dwBytesInBlock >> 16), 
			0,//(DWORD)(dwBytesInBlock & 0x0000FFFF), 
			NULL);
		CloseHandle(hFile);  // �ͷ��ļ��ں˶��� 

		if (hFileMapping != 0) {
			LPVOID pbFile = (LPVOID)MapViewOfFile(hFileMapping,
				FILE_MAP_READ,
				0,
				0,
				dwBytesInBlock);// ���ļ�����ӳ�䵽���̵ĵ�ַ�ռ� 
			if (pbFile != 0) {
				int cnt = 20;
				memcpy(&cnt, pbFile, sizeof(int));
				v1 = new float[cnt * 2];
				pbFile = (__int64*)(((__int64)pbFile) + 4);// ƫ�Ƶ�ַ 
				memcpy(v1, pbFile, sizeof(float) * cnt * 2);
				for (int i = 0; i < cnt; i++) {
					sum += v1[i] * v1[cnt + i];
				}
				UnmapViewOfFile(pbFile);
				CloseHandle(hFileMapping);
				delete[] v1;
			}
		}
	}
	sumanswer+=sum;
}
thread* tarray[maxMachine];

void devide(string str) {
	int cores = (int)getCores();//����߳���=CPU�������ٶ�CPU���ܾ��⣩
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
		tarray[(cores - 1)] = new thread(readpointer, str, delta, start1, start2, (cores - 1));
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