#pragma once


#include<fstream>
#include<iomanip>
#include<iostream>
#include<string> 
using namespace std;

//宏定义
#define TWO_TXT 11
#define ONE_TXT 12
#define TWO_BIN 13
#define ONE_BIN 14

// 全局变量:
extern int rand_upper_bond;//随机数上下限
extern int rand_low_bond;

// 前向声明:
void makerand(int cond, int n);
void readData(int cond, string path1, string path2);
void readData(int cond, string path);
