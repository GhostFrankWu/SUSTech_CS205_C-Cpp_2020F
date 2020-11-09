#include <iostream>
#include <iomanip>
using namespace std;
void e1();
void e2();
int main() {
    e1();
    e2();
    system("pause>nul");
}
const int LEN = 30;
class CandyBar {
private:
    char brand[LEN];
    double weight;
    int calorie;
public:
    void setCandyBar();
    void showCandyBar();
};
void CandyBar::setCandyBar() {
    cout << "Enter brand name of a Candy bar:";
    cin.getline(this->brand,LEN);//SOF plz
    cout << "Enter weight of the Candy bar:";
    cin >> this->weight;
    cout << "Enter calories(an integer value) in the Candy bar:";
    cin >> this->calorie;
}
void CandyBar::showCandyBar() {
    cout << "Brand: " << this->brand << endl;
    cout << "Weight: " << this->weight << endl;
    cout << "Calories: " << this->calorie << endl;
}
void e1() {
    cout << setw(30) << cout.fill('=') << "Exercises 1" << setw(30) << cout.fill('=') << endl;
    CandyBar cb;
    cb.setCandyBar();
    cb.showCandyBar();
}
class Rectangle {
private:
    double width;
    double height;
public:
    Rectangle() {
        this->width = 1;
        this->height = 1;
    }
    Rectangle(double width, double height) {
        this->width = width;
        this->height = height;
    }
    int getArea() {
        return width * height;
    }
    int getPerimeter() {
        return int(width + height) << 1;
    }
    void display() {
        cout << "Width:     " << width << endl;
        cout << "Height:    " << height << endl;
        cout << "Area:      " << getArea() << endl;
        cout << "Perimeter: " << getPerimeter() << endl;   
    }
};
void e2() {
    cout << setw(30) << cout.fill('=') << "Exercises 2" << setw(30) << cout.fill('=') << endl;
    Rectangle rc1(4,40), rc2(3.5,35.9);
    cout.clear();
    cout.flush();
    cout << "Rectangle 1" << endl << setw(15) << cout.fill('-') << endl;
    rc1.display();
    cout << "Rectangle 2" << endl << setw(15) << cout.fill('-') << endl;
    rc2.display();
}