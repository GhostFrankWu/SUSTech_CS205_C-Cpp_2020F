#pragma once
#ifndef M_MAT_INCLUDED
#define M_MAT_INCLUDED   //ͷ�ļ��ڱ�
#include<iostream>
#include<unordered_map>
static std::unordered_map<long long, unsigned int> copys;
template <typename T>
class Mat {
private:
	T* ptr = NULL;
	unsigned int width;
	unsigned int height;
public:
	Mat();
	Mat(const Mat&);
	~Mat();
	Mat(unsigned int, unsigned int);
	//Mat operator*(const Mat&);
	Mat& operator=(const Mat&);
	bool operator==(const Mat&) const;
	Mat& operator*(const double&);
	Mat clone() const;
	T& operator[](const int&);
#ifndef RAW_USE  //[i][j]���ض�ά����
#ifndef NORMAL_USE//[i]��һά����ȡֵ,(x,y)����ά�������
#define NORMAL_USE
#endif 
#ifdef NORMAL_USE
	T& operator()(const int&, const int&);
#endif
#endif
	T* getRaw();
	const unsigned int getWidth();
	const unsigned int getHeight();

	template <typename T0>
	friend Mat<T0> operator*(double, const Mat<T0>&);
	template <typename T1,typename T2>
	friend Mat<T1> operator*(const Mat<T1>&, const Mat<T2>&);
	template <typename T0>
	friend std::ostream& operator << (std::ostream&, Mat<T0>&);
};
namespace myMat {

}
#endif // M_MAT_INCLUDED