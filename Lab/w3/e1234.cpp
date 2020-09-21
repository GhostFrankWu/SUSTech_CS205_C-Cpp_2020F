#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <iomanip>  
using namespace std;void e1();
void e2();
void e3();
void e4();
int main(){	for (int i = 0; i < 4096; i++) {		cout << 0;	}	e1();	e2();	e3();	e4();	system("pause > nul");	return 0;
}
void e1(){	int sum = 0, tmp = 0;	do {		cout << "Enter an integer number:";		cin >> tmp;		sum += tmp;		cout << "The cumulative sum of the entries to date is :" << sum << endl;	} while (tmp != 0);}void e2() {	int cnt = 0, flag = 1;	cout << "Enter words(to stop, type the word done)" << endl;	while(1){		string str;		getline(cin, str);		cnt += count(str.begin(), str.end(), ' ');		if (str.find("done", 0) != string::npos){			cnt--; 			break;		}	} 
	printf("You entered a total of %d words .\n", cnt);}void e3() {	for (int i = 0; i < 7; i++) {		for (int j = 0; j < 7; j++) {			if (j <= i)				cout << j + 1;			else				cout << '*';		}		cout << endl;	}}void e4() {
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
}