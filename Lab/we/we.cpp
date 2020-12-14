#include<iostream>
using namespace std;
class OutOfRangeException : exception {
private:
	int index, val;
public:
	OutOfRangeException(int index, int val) {
		this->index = index;
		this->val = val;
	}
	void printStackTrace() {//this is bad
		clog << "The parameter " << index << " is " << val << " which out of range (0-100)." << endl;
	}
};
double calculateAverage(int a,int b,int c,int d) {
	if (a >> 7 || a > 100) {
		throw OutOfRangeException(1,a);
	}else if (b >> 7 || b > 100) {
		throw OutOfRangeException(2, b);
	}
	else if (c >> 7 || c > 100) {
		throw OutOfRangeException(3, c);
	}
	else if (d >> 7 || d > 100) {
		throw OutOfRangeException(4, d);
	}
	else {
		return ((double)a + b + c + d) / 4;
	}
}
int main() {
	int a, b, c, d;
	char ch;
	while (1) {
		cout << "Please enter marks for 4 courses:";
		cin >> a >> b >> c >> d;
		try {
			cout << "The average of the four courses is " << calculateAverage(a, b, c, d) << endl;
		}
		catch (OutOfRangeException e) {
			e.printStackTrace();
		}
		cout << "Would you want to enter another marks for 4 courses(y / n)";
		cin >> ch;
		if (ch != 89 && ch != 121)break;
	}
	cout << "Bye, see you next time." << endl;
}