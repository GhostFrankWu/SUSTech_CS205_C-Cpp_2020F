///////////////////////////////////////////////////////////
////这个文件中的代码用于矩阵类 对两个随机的A,B
////及其乘积的结果矩阵C (C=A*B)有：
////A1 ... .      B1 ... .       C1 ... .  
//// . ... .   *   . ... .   =    . ... .  
//// . ... A2      . ... B2       . ... C2  
////且有C1=A1*B1, C2=A2*B2 其余项都置零
////////////////////////////////////////////////////////////
#include "matrix.h"

Matrix::Matrix() { 
	*dat = 1;
}

Matrix::Matrix(const unsigned int row, const unsigned int col) {
	if (row && col) {//makesure row&col != 0
		dat =(float*) malloc(sizeof(float)*row * col);
		memset(dat, 0, sizeof(float) * row * col);
		this->row = row;
		this->col = col;
	}
}

Matrix::~Matrix() {
	//free(dat);
}

void Matrix::print(){//打印矩阵
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cout << dat[i*col+j] << " ";
		}
		cout << endl;
	}
}

void Matrix::init() {//初始化首尾
	dat[0] = 2;
	dat[row * col - 1] = 3;
}


const unsigned int Matrix::getCol() {//只读属性的列数
	return col;
}
const unsigned int Matrix::getRow() {//只读属性的行数
	return row;
}

float* Matrix::getRaw() {//常量传递
	return dat;
}

float& Matrix::operator[](int i) {//重载[]按照一维数组操作矩阵
	if (i < col * row) {
		return dat[i];
	}
	else {
		return dat[i %(col * row)];//溢出自动取余
	}
}


float& Matrix::operator()(int row, int col) {//重载()按照二维数组操作矩阵
	if (this->row > row && this->col > col) {
		return dat[this->col * row + col];
	}
	else {
		return dat[(this->col * row + col) % (col * row)];//溢出自动取余
	}
}