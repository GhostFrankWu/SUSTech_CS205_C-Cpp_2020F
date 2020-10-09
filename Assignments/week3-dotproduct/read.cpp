#include "dot.h"

/*****************************************************************************
sizeof可否通过宏定义优化？
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
float sum;
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
	float sum = 0, tmp;
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
void readbinary(string path1, string path2) {//	[OK]
	ifstream vec1(path1, ios::in | ios::binary),
		vec2(path2, ios::in | ios::binary);
	float f1, f2, sum=0;
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
	float f1, f2, sum = 0;
	int n = 0;
	if (vec1.good()) {
		vec1.read((char*)& n, sizeof(int));
		vec2.seekg( n * sizeof(float)+ sizeof(int));
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
void readData(int mode, string path1, string path2) {
	switch (mode) {
	case 11:
		readtxt(path1, path2);
		break;
	case 13:
		readbinary(path1, path2);
		break;
	default:
		break;
	}
}
//
void readData(int mode, string path) {
	switch (mode) {
	case 12:
		readtxt1(path);
		break;
	case 14:
		readbinary1(path);
		break;
	default:
		break;
	}
}