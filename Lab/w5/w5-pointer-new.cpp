#include<iostream>
#include<iomanip>
#include<string>
using namespace std;
struct Pizza {
	string name;
	int diameter;
	float weight;
}; 
struct CandyBar {
	string name;
	double weight;
	int calories;
};
string f_getline() {
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
	Pizza* p1 = new Pizza;
	cout << "Enter the name of pizza company: ";
	p1->name = f_getline();
	cout << "Enter the diameter of pizza (inches): ";
	cin >> p1->diameter;
	cout << "Enter the weight of the pizza (g): ";
	cin >> p1->weight;
	cout << "Displaying the information of the pizza:" << endl <<
		"Company name:" << p1->name << endl <<
		"Pizza diameter: " << p1->diameter << " inches" << endl <<
		"Pizza weight: " << setprecision(4) << p1->weight << " g" << endl;
	delete p1;
}
void e2() {
	cout << endl << setw(30) << cout.fill('=')
		<< "Exercises 2" << setw(30) << cout.fill('=') << endl;
	CandyBar* arr=new CandyBar[3];
	int index = 0;
	double max = 0;
	cout << "Please input three Candybars information: " << endl;
	for (int i = 0; i < 3; i++) {
		CandyBar c1;
		cout << "Enter brand name of a Candy bar: ";
		c1.name = f_getline();
		cout << "Enter the weight of Candy bar: ";
		scanf("%lf", &c1.weight);
		cout << "Enter calories (an integer value) in the Candy bar: ";
		scanf("%d", &c1.calories);
		arr[i] = c1;
	}
	cout << endl << "Displaying the CandyBar array contents" << endl;
	for (int i = 0; i < 3; i++) {
		cout << "Brand: " << arr[i].name << endl << "weight: " << arr[i].weight
			<< endl << "Calores: " << arr[i].calories << endl;
	}
	delete[] arr;
}
void e3() {
	cout << "Enter 5 integers : " << endl;
	int* arr = (int*)malloc(5 * sizeof(int));
	for (int i = 0; i < 5; i++) {
		scanf("%d", &arr[i]);
	}
	cout <<"The elements of the array in reverse order are : "<< endl;
	for (int i = 0; i < 5; i++) {
		cout << arr[4 - i] << " ";
	}
	free(arr);
}
