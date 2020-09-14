#include<iostream>
#include<iomanip>
using namespace std;
const int x = 10, y = 5;
int main() {//1. Write a program to produce the output as shown below
	cout.setf(ios_base::left, ios_base::adjustfield);
	cout.width(2);
	cout << "x";
	cout << setw(6) << "value" << setw(2) << "y" << setw(6) << "value" << setw(16) << "Expressions" << setw(10) << "Result" << endl;
	printf("%d |\t%d |\tx=y+3\t\t|x=%d\n", x, y, y + 3);
	printf("%d |\t%d |\tx=y-2\t\t|x=%d\n", x, y, y - 2);
	printf("%d |\t%d |\tx=y*5\t\t|x=%d\n", x, y, y * 5);
	printf("%d |\t%d |\tx=x/y\t\t|x=%d\n", x, y, x / y);
	printf("%d |\t%d |\tx=x%y\t\t|x=%d\n", x, y, x % y);
	system("pause>nul");
}