#include <iostream>
#include <string>
#include "matrix.h"
using namespace std;
template<typename T1>
void useMatrixTemplate(Matrix<T1>& M, T1 array1[][MAXCOLS], T1 array2[][MAXCOLS]) {
	M.setMatrix(array1);
	cout << "Matrix set first array" << endl;
	M.printMatrix();
	cout << "Matrix incremented second array" << endl;
	M.addMatrix(array2);
	M.printMatrix();
}
static int matrix_main() {
	string str1[MAXROWS][MAXCOLS] = { {"Congra","y" , "ar"},{"a1m", "don","La"} };
	string str2[MAXROWS][MAXCOLS] = { {"tulations" , "ou", "e"},{"ost","e the","b!"} };
	int num1[MAXROWS][MAXCOLS] = { {1,2,3},{4,5,6} };
	int num2[MAXROWS][MAXCOLS] = { {6,5,4},{3,2,1} };
	Matrix<string> stringMartix(2, 3);
	Matrix<int> intMatrix(2, 3);
	cout<< "Demonstrating with string matrix:" << endl;
	useMatrixTemplate(stringMartix, str1, str2);
	cout<< "\nDemonstrating with int matrix:" << endl;
	useMatrixTemplate(intMatrix, num1, num2);
	cout << "\n" << endl;
	return 0;
}
