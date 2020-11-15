#include "matrix.h"
#include "timer.cpp"
#define q 8192
void test(int m) {
	Matrix A(q, q);
	Matrix B(q, q);
	A.init();
	B.init();
	Matrix C(q, q);
	cout << "start.." << endl;
	TimePoint now;
	now.setTime();
	matMul(A, B, C, m);
	cout << "用时：\t" << now.timeDffer() / 1000 << "ms.\n";
	cout << C[0] << " " << C[C.getRow() * C.getCol() - 1] << endl;
}
int main() {
	test(DevideBlock);//DevideBruteForce DevideBlock BruteForce
	system("pause>nul");
}
