#pragma once 
//#pragma GCC optimize(3)

#include<iomanip>
#include<iostream>
#include<string> 
using namespace std;

//宏定义
#define TWO_TXT 11
#define ONE_TXT 12
#define TWO_BIN 13
#define ONE_BIN 14
#define ONE_BIN_QUICK 15

#define answer double
#define maxMachine 100

//#define FASTEST //最快速度，不释放空间
#define ACC4    //四路循环 ACC4或者 SSE加速 SSE
// 全局变量:
//extern int rand_bond;//随机数上下限

// 前向声明:
void makerand(int cond, int n);
void readData(int cond, string path1, string path2);
void readData(int cond, string path);
void devide(string str);
void devideQuick(string str);
void devideQuickC(string str);