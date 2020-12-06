#pragma once
#include "Mat.h"
#include <assert.h>
#include <thread>

#if defined(__linux__) || defined(__linux)   //自动识别系统类型获取CPU核数
#include "unistd.h"
#include <string.h>//memcpy
static auto getCores() {
	return sysconf(_SC_NPROCESSORS_ONLN);
}
#elif (defined(_WIN64) || defined(__WIN64__) || defined(_WIN32) || defined(__WIN32__))
#include <windows.h>
static auto getCores() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return sysInfo.dwNumberOfProcessors;
}
#  endif

template <typename T>
Mat<T>::Mat() {
	width = 0;
	height = 0;
}

template <typename T>
Mat<T>::Mat(const Mat& other) {
	width = other.width;
	height = other.height;
	if (copys.count((unsigned __int64)other.ptr)) {
		copys[(unsigned __int64)other.ptr]++;
	}
	else {
		copys.insert(std::unordered_map<unsigned __int64, unsigned int>::value_type((unsigned __int64)other.ptr, 1));
	}
#ifdef _DEBUG
	std::cout << "copy.c..";
#endif
	ptr = other.ptr;
#ifdef _DEBUG
	std::cout << ptr << std::endl;
#endif
}

template <typename T>
Mat<T> Mat<T>::clone() const{
	Mat<T> c(height, width);
#ifdef _DEBUG
	std::cout << "create.c..";
#endif
	c.ptr = new T[width * height];
#ifdef _DEBUG
	std::cout << "cp"<<ptr<<" to "<<c.ptr << std::endl;
#endif
	memcpy(c.ptr, ptr, sizeof(T) * width * height);
	return c;
}
template <typename T>
Mat<T>& Mat<T>::operator=(const Mat& other){
	if (this != &other){
		if (!ptr) {
			delete[] ptr;
			ptr = NULL;
		}
		width = other.width;
		height = other.height;
#ifdef _DEBUG
		std::cout << "copy.=..";
#endif
		ptr = other.ptr;
#ifdef _DEBUG
		std::cout << ptr << std::endl;
#endif
		if (copys.count((unsigned __int64)other.ptr)) {
			copys[(unsigned __int64)other.ptr]++;
		}
		else {
			copys.insert(std::unordered_map<unsigned __int64,unsigned int>::value_type((unsigned __int64)other.ptr,1));
		}
	}
	return *this;
}

template <typename T>
bool Mat<T>::operator==(const Mat& other) const{
	if (height - other.height || width - other.width)return false;
	return memcmp(ptr, other.ptr, sizeof(T) * width * height);
}

template <typename T>
Mat<T>::Mat(const unsigned int row, const unsigned int col) {
	if (row && col) {
#ifdef _DEBUG
		std::cout << "create...";
#endif
		ptr = new T[row * col];
		memset(ptr, 0, sizeof(T) * row * col);
		height = row;
		width = col;
#ifdef _DEBUG
		std::cout << ptr << std::endl;
#endif
	}
	else {
		width = 0;
		height = 0;
	}
}

//多线程+分块
inline void do_block(int n, float* A, float* B, float* C, int BLOCKSIZE) {
	for (int i = 0; i < BLOCKSIZE; i++) {
		for (int j = 0; j < BLOCKSIZE; j++) {
			float cur = C[i * n + j];
			for (int k = 0; k < BLOCKSIZE - 3; k += 4) {//循环展开
				cur += A[i * n + k] * B[k * n + j];
				cur += A[i * n + k + 1] * B[(k + 1) * n + j];
				cur += A[i * n + k + 2] * B[(k + 2) * n + j];
				cur += A[i * n + k + 3] * B[(k + 3) * n + j];
			}
			for (int k = BLOCKSIZE - BLOCKSIZE % 4; k < BLOCKSIZE; k++) {//不能被4整除的部分
				cur += A[i * n + k] * B[k * n + j];
			}
			C[i * n + j] = cur;
		}
	}
}

inline void block(int n, int BLOCKSIZE, float* A, float* B, float* C, int sj) {
	for (int si = 0; si < n; si += BLOCKSIZE) {
		for (int sk = 0; sk < n; sk += BLOCKSIZE) {
			do_block(n, A + si * n + sk, B + sk * n + sj, C + si * n + sj, BLOCKSIZE);
		}
	}
}

static void blockDevide(int n, float* A, float* B, float* C) {
	int cores = (int)getCores();
	int BLOCKSIZE = n / cores;//最佳线程数=CPU核数（假定CPU性能均衡）
	std::thread* tarray[128];//
	while (n % BLOCKSIZE != 0)BLOCKSIZE--;
	int cnt = 0;
	for (int sj = 0; sj + BLOCKSIZE <= n; sj += BLOCKSIZE) {
		tarray[cnt++] = new std::thread(block, n, BLOCKSIZE, A, B, C, sj);
		if (cnt == cores+1) {
			while (cnt--) {
				tarray[cnt]->join();
			}
		}
	}
	while (cnt-- > 0) {
		tarray[cnt]->join();
	}
}
template<typename T,typename T2>
inline void bf(const T* a,const T2* b, T* c, const unsigned int wa, const unsigned int ha, const unsigned int wb) {
	for (int i = 0; i < ha; i++) {
		for (int j = 0; j < wb; j++) {
			for (int k = 0; k < wa; k++) {
				c[wb * i + j] += a[wa * i + k] * b[wb * k + j];
			}
		}
	}
}
/*
template<typename T>
Mat<T>& Mat<T>::operator*(const Mat& other) {
	if (width - other.height) {
		std::cout << "矩阵的大小不匹配,请检查" << std::endl;
		assert(0);
	}
	Mat<T> c(height, other.width);
	if (typeid(float) == typeid(T)) {
		blockDevide(height, (float *)ptr, (float*)other.ptr, (float*)c.ptr);
	}
	else {
		bf(ptr, other.ptr, c.ptr, width, height, other.width);
	}
	return c;
}*/

template<typename T>
Mat<T>& Mat<T>::operator*(const double& x){
	int m = height * width;
	for (int i = 0; i < m; i++) {
		ptr[i] *= x;
	}
	return *this;
}

template <typename T>
Mat<T>::~Mat() {
	if (!copys.count((unsigned __int64)ptr)||!copys[(unsigned __int64)ptr]) {
		if (ptr) {
#ifdef _DEBUG
			std::cout << "delete..." << ptr;
#endif
			delete[] ptr;
#ifdef _DEBUG
			std::cout << "  done" << std::endl;
#endif
		}
		else {
#ifdef _DEBUG
			std::cout << "  skip.null.." << std::endl;
#endif
		}
	}
	else {
		copys[(unsigned __int64)ptr]--;
#ifdef _DEBUG
		std::cout << ptr<< "  skip.copy.."<< copys[(unsigned __int64)ptr]  << std::endl;
#endif
	}
}

template <typename T>
const unsigned int Mat<T>::getWidth() {//只读属性的列数
	return width;
}

template <typename T>
const unsigned int Mat<T>::getHeight() {//只读属性的列数
	return height;
}

template <typename T>
T* Mat<T>::getRaw() {//常量传递
	return ptr;
}

#ifdef NORMAL_USE
template <typename T>
T& Mat<T>::operator[](const int& i) {//重载[]按照一维数组操作矩阵
	if (i < width * height) {
		return ptr[i];
	}
	else {
		return ptr[i % (width * height)];//溢出自动取余
	}
}

template <typename T>
T& Mat<T>::operator()(const int& row, const int& col) {//重载()按照二维数组操作矩阵
	if (this->width > col && this->height > row) {
		return ptr[this->height * col + row];
	}
	else {
		return ptr[(this->height * col + row) % (this->height * this->width)];//溢出自动取余
	}
}
#else
template <typename T>
T& Mat<T>::operator[](const int& i) {//返回对应行的首位指针
	if (i < height) {
		return ptr[i * width];
	}
	else {
		return ptr[(i % height) * width];//溢出自动取余
	}
}
#endif

template <typename T>
std::ostream& operator<< (std::ostream& os, Mat<T>& other) {
	for (int i = 0; i < other.getHeight(); i++) {
		for (int j = 0; j < other.getWidth(); j++) {
			os << other(i, j) << " ";
		}
		os << std::endl;
	}
	return os;
}

template <typename T1, typename T2>
Mat<T1> operator*(const Mat<T1>& self, const Mat<T2>& other) {
	if (self.width - other.height) {
		std::cout << "矩阵的大小不匹配,请检查" << std::endl;
		assert(0);
	}
	Mat<T1> c(self.height, other.width);
	if (typeid(float) == typeid(T1)) {
		blockDevide(self.height, (float*)self.ptr, (float*)other.ptr, (float*)c.ptr);
	}
	else {
		bf(self.ptr, other.ptr, c.ptr, self.width, self.height, other.width);
	}
	return c;
}



template<typename T>
Mat<T> operator*(double x, const Mat<T>& other) {
	int m = other.height * other.width;
	Mat<T>c = other.clone();
	for (int i = 0; i < m; i++) {
		c.ptr[i] *= x;
	}
	return c;
}