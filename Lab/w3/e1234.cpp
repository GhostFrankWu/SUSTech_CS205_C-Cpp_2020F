﻿#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <iomanip>  
using namespace std;
void e2();
void e3();
void e4();
int main(){
}

	printf("You entered a total of %d words .\n", cnt);
	cout << "Enter the quiz, mid-term and final scores:";
	float sum = 0, tmp = 0,i=0;
	while (i++ < 3) {
		cin >> tmp;
		sum += tmp;
	}
	sum /= 3;
	cout << "average score = " << setprecision(6) << sum <<" grade = ";
	if (sum >= 90) {
		cout << "A" << endl;
	}
	else if (sum >= 70) {
		cout << "B" << endl;
	}
	else if (sum >= 50) {
		cout << "C" << endl;
	}
	else {
		cout << "F" << endl;
	}
