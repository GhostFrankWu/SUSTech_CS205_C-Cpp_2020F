#include "dot.h"
#include<fstream>

/*****************************************************************************
此代码文件为初期版本中使用，在最终集成的版本中可以忽视，
但是对于TXT文件仍有不可替代的鲁棒性

====sizeof可否通过宏定义优化？

以下记录了程序的耗时，T为相对单位。
**********************************************************************************/
struct vectous {
	float v;
}vec[2];
char ch_in[129];
vectous& operator>>(istream& in, vectous& b){
	in >> ch_in;
	int dot = 0,len= strlen(ch_in);
	for (int i=0; i < len;i++){
		if (ch_in[i] < 48 || ch_in[i]>57) {
			if (ch_in[i] == 46 && !dot) {//允许存在一个小数点
				dot = i;
			}else if (i!=0 || ch_in[0] != 45) {//忽略所有不合理输入
				memmove(ch_in+i, ch_in + i+1, len + 1 - i);
				i--;
				len--;
			}
		}
	}
	b.v= atof(ch_in);
	return b;
}
answer sum;
/*************************************************************
TXT文本读取，耗时为 1000T
****************************************************************/
////////////////////////////////双文本读取 [OK]
void readtxt(string path1, string path2) {
	ifstream vec1, vec2;
	vec1.open(path1, ios::in);
	vec2.open(path2, ios::in);
	if (vec1.good() && vec2.good()) {
		vec1 >> vec[0];
		vec2 >> vec[1];
		while (!vec1.eof() && !vec2.eof()) {
			sum += vec[0].v * vec[1].v;
			vec1 >> vec[0];
			vec2 >> vec[1];
		}
	}
	cout << setprecision(15) << sum << endl;
	vec1.close();
	vec2.close();
}
////////////////////////////////单文本读取 [OK]
void readtxt1(string path1) {
	ifstream vecI, vec2;
	int n;
	std::streamoff y;
	float tmp;
	vecI.open(path1, ios::in);//两个文件指针都是顺序阅读
	vec2.open(path1, ios::in);
	if (vecI.good()) {
		vecI >> n;
		y = vecI.tellg();
		for (int i = 0; i < n; i++) {
			vecI >> tmp;//fixme 能不能空读加速？
		}
		vec2.seekg(vecI.tellg());//文件随机访问 *报告特别说明
		vecI.seekg(y);
		for (int i = 0; i < n; i++) {
			vecI >> vec[0];
			vec2 >> vec[1];
			sum += vec[0].v * vec[1].v;
		}
	}
	cout << setprecision(15) << sum << endl;
	vecI.close();
}
/*************************************************************
C++风格二进制读取，耗时为 100T ，比TXT快了接近10倍且节省硬盘
****************************************************************/
void readbinary(string path1, string path2) {//	[OK]
	ifstream vec1(path1, ios::in | ios::binary),
		vec2(path2, ios::in | ios::binary);
	float f1, f2;
	if (vec1.good() && vec2.good()) {
		while (vec1.peek() != EOF && vec2.peek() != EOF){
			vec1.read((char*)& f1, sizeof(float));
			vec2.read((char*)& f2, sizeof(float));
			sum += f1 * f2;
		}
	}
	cout << setprecision(15) << sum << endl;
	vec1.close();
	vec2.close();
}
void readbinary1(string path1) {	//[OK]
	ifstream vec1(path1, ios::in | ios::binary);
	ifstream vec2(path1, ios::in | ios::binary);
	float f1, f2;
	int n = 0;
	if (vec1.good()) {
		vec1.read((char*)& n, sizeof(int));
		vec2.seekg(n * sizeof(float) + sizeof(int));
		for (int i = 0; i < n; i++) {
			vec1.read((char*)& f1, sizeof(float));
			vec2.read((char*)& f2, sizeof(float));
			sum += f1 * f2;
		}
	}
	cout << setprecision(15) << sum << endl;
	vec1.close();
	vec2.close();
}
/*************************************************************
C风格二进制读取，耗时为 30T ，比TXT快了约30倍
****************************************************************/
void readbinary1_advance(string path1) { //		[OK]
	FILE* fp = fopen(path1.c_str(), "rb");
	FILE* fp2 = fopen(path1.c_str(), "rb");
	float f1,f2;
	if (fp) {
		int n = 0;
		fread(&n, sizeof(int), 1, fp);
		fseek(fp2, sizeof(int) + n * sizeof(float), SEEK_SET);
		for (int i = 0; i < n; i++) {
			fread(&f1, sizeof(float), 1, fp);
			fread(&f2, sizeof(float), 1, fp2);
			sum += f1 * f2;
		}
		cout << setprecision(15) << sum << endl;
		fclose(fp);
		fclose(fp2);
	}
}

void readData(int mode, string path1, string path2) {
	sum = 0;
	switch (mode) {
	case 11:
		readtxt(path1, path2);
		break;
	case 13:
		readbinary(path1, path2);
		break;
	}
}
void readData(int mode, string path) {
	sum = 0;
	switch (mode) {
	case 12:
		readtxt1(path);
		break;
	case 14:
		readbinary1(path);
		break;
	case 15:
		readbinary1_advance(path);
	}
}


/*************************************************************
对应上述版本的文件生成方法
****************************************************************/

const int rand_upper_bond = 10, rand_low_bond = -10;

/***
/*分两个文件存储	[OK]
***/
float randomfloat() {
	return (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond);
}
void randomtxt(int n) {
	fstream x1("1.txt", ios::out), x("2.txt", ios::out);
	x1.close(), x.close();
	fstream op("1.txt"), op2("2.txt");
	srand((unsigned)time(NULL));
	float f1, f2;
	for (int i = 0; i < n; i++) {
		f1 = randomfloat(),
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
	float f1;
	op << n << endl;
	for (int i = 0; i < n; i++) {
		f1 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond);
		op << f1 << " ";
	}
	op << endl;
	for (int i = 0; i < n; i++) {
		f1 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond);
		op << f1 << " ";
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
		op.write((char*)& f1, sizeof(float));
		op2.write((char*)& f2, sizeof(float));
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
	float f1;
	op.write((char*)& n, sizeof(int));
	for (int i = 0; i < 2 * n; i++) {
		f1 = (float)(rand() % 101) / 101 + (float)((rand() % (rand_upper_bond - rand_low_bond + 1)) + rand_low_bond);
		op.write((char*)& f1, sizeof(float));
	}
	op.close();
}

void random(int n) {  //		[OK]
	string str = to_string(n) + ".dat";
	fstream x1(str, ios::out | ios::binary);//创建文件
	x1.close();
	FILE* fp = fopen(str.c_str(), "wb");
	srand((unsigned)time(NULL));
	float f1;
	if (fp) {
		fwrite(&n, sizeof(int), 1, fp);
		n *= 2;
		for (int i = 0; i < n; i++) {
			f1 = randomfloat();
			fwrite(&f1, sizeof(float), 1, fp);
		}
		fclose(fp);
	}
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
	case 15:
		random(n);
		break;
	default:
		break;
	}
}