#pragma once 
//#pragma GCC optimize(3)
///////////////////////////////////////////////////////////
//////头文件包含了全局宏定义、全局变量和一些方法的前向声明
#include<iomanip>
#include<iostream>
#include<string> 
using namespace std;

//宏定义
#define BruteForce 1
#define DevideBlock 2
#define DevideBruteForce 3
#define Net 99

#define answer double
#define maxMachine 100

// 矩阵结构体
class Matrix {
private:
	float* dat=new float;
	unsigned int col=1;
	unsigned int row=1;
public:
	Matrix(const unsigned int row, const unsigned int col = 1);
	Matrix();
	~Matrix();
	float* getRaw();
	const unsigned int getRow();
	const unsigned int getCol();
	void init();
	void print();
	float& operator[](int i);
	float& operator()(int row,int col);
};
// 前向声明:
bool matMul(Matrix A, Matrix B, Matrix C, int mode);
bool matMul(float* a, const unsigned int hA, const unsigned int wA, float* b, const unsigned int hB,
	const unsigned int wB, float* c, const unsigned int hC, const unsigned int wC, const unsigned int mode);