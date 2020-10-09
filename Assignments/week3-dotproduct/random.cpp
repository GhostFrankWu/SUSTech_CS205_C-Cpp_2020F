#include "dot.h"

int rand_upper_bond = 10, rand_low_bond = - 10;

/***
/*分两个文件存储	[OK]
***/
void randomtxt(int n) {
	fstream x1("1.txt", ios::out), x("2.txt", ios::out);
	x1.close(), x.close();
	fstream op("1.txt"), op2("2.txt");
	srand((unsigned)time(NULL));
	float f1, f2;
	for (int i = 0; i < n; i++) {
		f1 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond),
			f2 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond);
		op << f1 << " ";
		op2 << f2 << " ";
	}
	op.close();
	op2.close();
}

/***				[OK]
/*n
/*1-n
/*1-n
***/
void randomtxt1(int n) {
	string str = to_string(n) + ".txt";
	fstream x1(str, ios::out);//创建文件
	x1.close();
	fstream op(str);//清空文件
	srand((unsigned)time(NULL));
	float f1, f2;
	op << n << endl;
	for (int i = 0; i < n; i++) {
		f1 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond);
		op <<  f1 << " ";
	}
	op << endl;
	for (int i = 0; i < n; i++) {
		f1 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond);
		op <<  f1 << " ";
	}
	op.close();
}
///////////////////////////////////两个个二进制文件	[OK]
void randbinary(int n) {
	fstream x1("1.dat", ios::out | ios::binary), x("2.dat", ios::out | ios::binary);
	x1.close(), x.close();
	fstream op("1.dat", ios::out | ios::binary), op2("2.dat", ios::out | ios::binary);
	srand((unsigned)time(NULL));
	float f1, f2;
	for (int i = 0; i < n; i++) {
		f1 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond),
			f2 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond);
		op.write((char*)& f1,sizeof(float));
		op2.write((char*)& f2,sizeof(float));
	}
	op.close();
	op2.close();
}

/***						[OK]
/*n
/*1-n
/*1-n
***/
void randbinary1(int n) {
	string str = to_string(n) + ".dat";
	fstream x1(str, ios::out | ios::binary);//创建文件
	x1.close(); 
	srand((unsigned)time(NULL));
	ofstream op(str, ios::out | ios::binary);
	float f1, f2;
	op.write((char*)& n, sizeof(int));
	for (int i = 0; i < 2 * n; i++) {
		f1 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond);
		op.write((char*)& f1, sizeof(float));
	}
	op.close();
}

void makerand(int cond, int n) {
	switch (cond) {
	case 11:
		randomtxt(n);
		break;
	case 12:
		randomtxt1(n);
		break;
	case 13:
		randbinary(n);
		break;
	case 14:
		randbinary1(n);
		break;
	default:
		break;
	}
}