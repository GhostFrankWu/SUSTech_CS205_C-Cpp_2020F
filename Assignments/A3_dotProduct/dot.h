#pragma once 
//#pragma GCC optimize(3)

#include<iomanip>
#include<iostream>
#include<string> 
using namespace std;

//�궨��
#define TWO_TXT 11
#define ONE_TXT 12
#define TWO_BIN 13
#define ONE_BIN 14
#define ONE_BIN_QUICK 15

#define answer double
#define maxMachine 100

//#define FASTEST //����ٶȣ����ͷſռ�
#define ACC4    //��·ѭ�� ACC4���� SSE���� SSE
// ȫ�ֱ���:
//extern int rand_bond;//�����������

// ǰ������:
void makerand(int cond, int n);
void readData(int cond, string path1, string path2);
void readData(int cond, string path);
void devide(string str);
void devideQuick(string str);
void devideQuickC(string str);