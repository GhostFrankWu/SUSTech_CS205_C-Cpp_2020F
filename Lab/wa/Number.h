#pragma once
class Number {
private:
	int num = 0;
public:
	Number();
	Number(int num);
	Number(const Number& src);
	Number operator++();
	Number operator--();
	Number operator++(int);
	Number operator--(int);
	friend std::ostream& operator<< (std::ostream& os, const Number& other);
};
int Number_main();