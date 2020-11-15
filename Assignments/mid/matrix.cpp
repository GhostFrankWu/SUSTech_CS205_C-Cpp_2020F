///////////////////////////////////////////////////////////
////����ļ��еĴ������ھ����� �����������A,B
////����˻��Ľ������C (C=A*B)�У�
////A1 ... .      B1 ... .       C1 ... .  
//// . ... .   *   . ... .   =    . ... .  
//// . ... A2      . ... B2       . ... C2  
////����C1=A1*B1, C2=A2*B2 ���������
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

void Matrix::print(){//��ӡ����
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cout << dat[i*col+j] << " ";
		}
		cout << endl;
	}
}

void Matrix::init() {//��ʼ����β
	dat[0] = 2;
	dat[row * col - 1] = 3;
}


const unsigned int Matrix::getCol() {//ֻ�����Ե�����
	return col;
}
const unsigned int Matrix::getRow() {//ֻ�����Ե�����
	return row;
}

float* Matrix::getRaw() {//��������
	return dat;
}

float& Matrix::operator[](int i) {//����[]����һά�����������
	if (i < col * row) {
		return dat[i];
	}
	else {
		return dat[i %(col * row)];//����Զ�ȡ��
	}
}


float& Matrix::operator()(int row, int col) {//����()���ն�ά�����������
	if (this->row > row && this->col > col) {
		return dat[this->col * row + col];
	}
	else {
		return dat[(this->col * row + col) % (col * row)];//����Զ�ȡ��
	}
}