#include <iostream>
#include "Number.h"
using namespace std;
int Number_main(){
	Number n1(20);
	Number n2 = n1++;
	cout << n1 << endl;
	cout << n2 << endl;
	Number n3 = n2--;
	cout << n2 << endl;
	cout << n3 << endl;
	Number n4 = ++n3;
	cout << n3 << endl;
	cout << n4 << endl;
	Number n5 = --n4;
	cout << n4 << endl;
	cout << n5 << endl;
	return 0;
}

Number::Number() {
	this->num = 0;
}

Number::Number(int num) {
	this->num = num;
}

Number::Number(const Number& src) : num(src.num) {
}

Number Number::operator++(int) {
	Number temp(this->num);
	this->num++;
	return temp;
}

Number Number::operator--(int) {
	Number temp(this->num);
	this->num--;
	return temp;
}

Number Number::operator++() {
	this->num++;
	return *this;
}

Number Number::operator--() {
	this->num--;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Number& other){
	os << other.num;
	return os;
}
