#include "Mat.cpp"
using namespace std;
int main() {
	Mat<int>a(12, 6);
	Mat<double>b(6, 8);
	a[0] = 1, b[0] = 2;
	a(11,5) = 3;
	b(5, 7) = 4;
	Mat<int>c;
	cout << a << endl;
	cout << b << endl;
	c = (a * b);
	cout << c << endl;
	c = c * 2;
	cout << c << endl;
	c = 2.0 * c;
	cout << c << endl;
	/* 另一段测试代码
	Mat<int>a(2, 6);
	Mat<int>b = a;
	Mat<int>c(a);
	Mat<int>d = a.clone();
	cout << "Mat a\n" << a << endl;
	cout << "Mat b=a\n" << b << endl;
	cout << "Mat c(a)\n" << c << endl;
	cout << "Mat d=a.clone()\n" << d << endl;
	a[0] = 5;
	cout << "==============let a[0] = 5==========="<< endl;
	cout << "Mat a\n" << a << endl;
	cout << "Mat b=a\n" << b << endl;
	cout << "Mat c(a)\n" << c << endl;
	cout << "Mat d=a.clone()\n" << d << endl;
	*/
	return 0;
}