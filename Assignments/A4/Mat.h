#pragma once
#ifndef M_MAT_INCLUDED
#define M_MAT_INCLUDED   //头文件哨兵
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
#ifndef RAW_USE  //[i][j]重载二维矩阵
#ifndef NORMAL_USE//[i]按一维矩阵取值,(x,y)按二维矩阵访问
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