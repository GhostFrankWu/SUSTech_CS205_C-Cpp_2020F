// complex.cpp --- implementing Complex class methods
#include <iostream>
#include "complex.h"

Complex::Complex() : real(0), imag(0) {
}

Complex::Complex(double re, double im) : real(re), imag(im){
}

Complex Complex::operator+(double real)const{
	double result_real = real + this->real;
	double result_imag = this->imag;
	return Complex(result_real, result_imag);
}

Complex Complex::operator+(const Complex& other) const {
	double result_real = this->real + other.real;
	double result_imag = this->imag + other.imag;
	return Complex(result_real, result_imag);
}

Complex Complex::operator-(const Complex& other) const {
	double result_real = this->real - other.real;
	double result_imag = this->imag - other.imag;
	return Complex(result_real, result_imag);
}

Complex Complex::operator*(const Complex& other) const {
	double result_real = this->real * other.real - this->imag * other.imag;
	double result_imag = this->real * other.imag + this->imag * other.real;
	return Complex(result_real, result_imag);
}

Complex Complex::operator~() {
	return Complex(this->real*-1, this->imag * -1);
}
bool Complex::operator==(const Complex& other) const {
	return this->real == other.real && this->imag == other.imag;
}

bool Complex::operator!=(const Complex& other) const {
	return !(this->real == other.real && this->imag == other.imag);
}

Complex operator+(double real, const Complex& other) {
	double result_real = real + other.real;
	double result_imag = other.imag;
	return Complex(result_real, result_imag);
}

Complex operator*(double real, const Complex& other) {
	double result_real = real * other.real;
	double result_imag = real * other.imag;
	return Complex(result_real, result_imag);
}

std::ostream& operator<< (std::ostream& os, const Complex& other) {
	os << other.real << " + " << other.imag << "i";
	return os;
}

std::istream& operator>> (std::istream& os, Complex& other) {
	std::cout << "real: ";
	os >> other.real;
	std::cout << "imaginary: ";
	os >> other.imag;
	return os;
}