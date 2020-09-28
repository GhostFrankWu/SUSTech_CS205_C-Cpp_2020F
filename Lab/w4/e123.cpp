#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
using namespace std;
struct CandyBar {
	string name;
	double weight;
	int calories;
	CandyBar() {
		name = "";
		weight = 0;
		calories = 0;
	}
}; 
string f_getline(){
	string res;
	while (!getline(cin, res) || res.empty());
	return res;
}
void e1();
void e2();
void e3();
void main(void) {
	e1();
	e2();
	e3();
	system("pause>nul");
}
void e1() {
	cout << setw(30) << cout.fill('=') << "Exercises 1"
		<< setw(30) << cout.fill('=') << endl;
	CandyBar c1;
	cout << "Enter brand name of a Candy bar: ";
	c1.name = f_getline();
	cout << "Enter the weight of Candy bar: ";
	cin >> c1.weight;
	cout << "Enter calories (an integer value) in the Candy bar: ";
	cin >> c1.calories;
	cout << "Brand: " << c1.name << endl << "weight: " << c1.weight
		<< endl << "Calores: " << c1.calories << endl;
}
void e2() {
	cout << endl << setw(30) << cout.fill('=')
		<< "Exercises 2" << setw(30) << cout.fill('=') << endl;
	CandyBar arr[3];
	int index = 0, tmp = 0;
	double max = 0;
	cout << "Please input three Candybars information: " << endl;
	for (int i = 0; i < 3; i++) {
		CandyBar c1;
		cout << "Enter brand name of a Candy bar: ";
		c1.name = f_getline();
		cout << "Enter the weight of Candy bar: ";
		tmp = scanf("%lf", &c1.weight);
		cout << "Enter calories (an integer value) in the Candy bar: ";
		tmp = scanf("%d", &c1.calories);
		if ((c1.calories / c1.weight) > max) {
			max = c1.calories / c1.weight;
			index = i;
		}
		arr[i] = c1;
	}
	cout << endl << "Displaying the CandyBar array contents" << endl;
	for (int i = 0; i < 3; i++) {
		cout << "Brand: " << arr[i].name << endl << "weight: " << arr[i].weight
			<< endl << "Calores: " << arr[i].calories << endl;
	}
	cout << endl << "The greatest calories per weight is:" << endl
		<< "Brand name: " << arr[index].name << endl << "Calories pre weight:"
		<< setprecision(6) << max << endl;
}
void e3() {
	cout << endl << setw(30) << cout.fill('=')
		<< "Exercises 3" << setw(30) << cout.fill('=') << endl;
	string str;
	cout << "Please input a string: ";
	str= f_getline();/*//Hi! I am Candy, 18 years old.
	for (int i = 0; i < str.length(); i++) {
		if (!((str[i] > 64 && str[i] < 91) || (str[i] > 96 && str[i] < 123))) {
			str[i] = 32;
		}
	}//*/
	ofstream of1;
	of1.open("f1.txt", ios::out);
	of1 << str;
	of1.close();
	for (int i = 0; i < str.length(); i++) {
		if (str[i] > 96 && str[i] < 123) {
			str[i] -= 32;
		}
	}
	of1.open("f2.txt", ios::out);
	of1 << str;
	of1.close();
	cout << endl << endl;
	ifstream if1;
	if1.open("f1.txt", ios::in);
	if (if1.good()) {
		cout << "The contents of f1.txt : ";
		while (!if1.eof()) {
			getline(if1, str);
			cout << str << endl;
		}
	}
	if1.close();
	if1.open("f2.txt", ios::in);
	if (if1.good()) {
		cout << "The contents of f2.txt : ";
		while (!if1.eof()) {
			getline(if1, str);
			cout << str << endl;
		}
	}
	if1.close();
}