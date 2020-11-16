#pragma once
//comp1ex.h --- Complex class after adding friend function

#ifndef _FRIEND_OPERATOR_COMPLEX_H
#define _FRIEND_OPERATOR_COMPLEX_H
class Complex {
private:
	double real;
	double imag;
public:
	Complex();
	Complex(double re, double im);
	Complex operator+(const Complex& other) const;
	Complex operator-(const Complex& other) const;
	Complex operator*(const Complex& other) const;
	Complex operator~();
	bool operator==(const Complex& other) const;
	bool operator!=(const Complex& other) const;
	Complex operator+(double real) const;
	friend Complex operator+(double real, const Complex& other);
	friend Complex operator*(double real, const Complex& other);
	friend std::ostream& operator << (std::ostream& os, const Complex& other);
	friend std::istream& operator >> (std::istream& os, Complex& other);
};
#endif //_FRIEND_OPERATOR_COMPLEX_H

int test_main();