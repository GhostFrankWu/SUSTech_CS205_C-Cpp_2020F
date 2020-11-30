#include <iostream>
using namespace std;
class CStereoShape {
	static unsigned int numberOfObject;
public:
	CStereoShape() {
		numberOfObject++;
	}
	virtual double GetArea() {
		cout << "CStereoShape::GetArea()" << endl;
		return 0.0;
	}
	virtual double GetVolume() {
		cout << "CStereoShape::GetVolume()" << endl;
		return 0.0;
	}
	virtual void Show() {
		cout << "CStereoShape::Show()" << endl;
	}
	static unsigned int GetNumOfObject() {
		return numberOfObject;
	}
};
unsigned int CStereoShape::numberOfObject = 0;
class CCube : public CStereoShape {
	double length, width, height;
public:
	CCube() {
		length = 0;
		width = 0;
		height = 0;
	}
	CCube(double length, double width, double height) {
		this->length = length;
		this->width = width;
		this->height = height;
	}
	CCube(const CCube& other) {
		length = other.length;
		width = other.width;
		height = other.height;
	}
	double GetArea() {
		return length * width * 2 + length * height * 2 + height * width * 2;
	}
	double GetVolume() {
		return length * width * height;
	}
	void Show() {
		cout << "length:" << length << "  width:" << width << "  height:" << height 
			<< "  Area:" << GetArea() << "  Volume:" << GetVolume() << endl;
	}
};
class CSphere : public CStereoShape {
	double radius;
public:
	CSphere() {
		radius = 0;
	}
	CSphere(double radius) {
		this->radius = radius;
	}
	CSphere(const CSphere& other) {
		radius = other.radius;
	}
	double GetArea() {
		return radius * radius * 6;
	}
	double GetVolume() {
		return radius * radius * radius;
	}
	void Show() {
		cout << "radius:" << radius << "  Area:" << GetArea() << 
			"  Volume:" << GetVolume() << endl;
	}
};
int main() {
	CCube a_cube(4.0, 5.0, 6.0);
	CSphere c_sphere(7.9);
	CStereoShape* p = &a_cube;
	p->Show();
	p = &c_sphere;
	p->Show();
	cout << CStereoShape::GetNumOfObject() << endl;
}

/*#include <iostream>
using namespace std;
class base {
private:
	int privateint = 1;
protected:
	int protectedint = 2;
public:
	base() {
		cout << "base -> ";
	}
	virtual ~base() {
		cout << "-> base" << endl;
	}
	int publicint = 3;
	int getprivateint() {
		return privateint;
	}
	int getprotectedint() {
		return protectedint;
	}
	void setprivateint(int i) {
		privateint = i;
	}
	void setprotectedint(int i) {
		protectedint = i;
	}
	virtual void show() {
		cout << "hello from base!" << endl;
	}
};
class A :private base {
public:
	A() {
		cout << "A" << endl;
	}
	virtual ~A() {
		cout << "A";
	}
	virtual void show() {
		cout << "hello from A!" << endl;
	}
};
class B :protected base {
public:
	B() {
		cout << "B" << endl;
	}
	virtual ~B() {
		cout << "B";
	}
	virtual void show() {
		cout << "hello from B!" << endl;
	}
};
class C :public base {
public:
	C() {
		cout << "C" << endl;
	}
	virtual ~C() {
		cout << "C";
	}
	virtual void show() {
		cout << "hello from C!" << endl;
	}
};
int main() {
	base* test=new base;
	cout << test->getprivateint() << " " << test->getprotectedint() << " " << test->publicint << endl;
	test->setprivateint(5), test->setprotectedint(5), test->publicint = 5;
	cout << test->getprivateint() << " " << test->getprotectedint() << " " << test->publicint << endl;
	test->show();
	delete test;

	test = (base*)new A;
	cout << test->getprivateint() << " " << test->getprotectedint() << " " << test->publicint << endl;
	test->setprivateint(5), test->setprotectedint(5), test->publicint = 5;
	cout << test->getprivateint() << " " << test->getprotectedint() << " " << test->publicint << endl;
	test->show();
	delete test;

	test = (base*)new B;
	cout << test->getprivateint() << " " << test->getprotectedint() << " " << test->publicint << endl;
	test->setprivateint(5), test->setprotectedint(5), test->publicint = 5;
	cout << test->getprivateint() << " " << test->getprotectedint() << " " << test->publicint << endl;
	test->show();
	delete test;

	test = (base*)new C;
	cout << test->getprivateint() << " " << test->getprotectedint() << " " << test->publicint << endl;
	test->setprivateint(5), test->setprotectedint(5), test->publicint = 5;
	cout << test->getprivateint() << " " << test->getprotectedint() << " " << test->publicint << endl;
	test->show();
	delete test;
}*/