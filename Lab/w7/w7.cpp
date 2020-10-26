#include "head.h"
#include <iomanip>
using namespace std;
void e1();
void e2();
int main() {
    e1();
    e2();
    system("pause>nul");
}
void e1() {
    cout << setw(30) << cout.fill('=') << "Exercises 1" << setw(30) << cout.fill('=') << endl;
    CandyBar cb;
    cout << "Call the set function of Passing by pointer :" << endl;
    setCandyBar(&cb);
    cout << "Call the show function of Passing by pointer :" << endl;
    showCandyBar(&cb);
    cout << "Call the set function of Passing by reference :" << endl;
    setCandyBar(cb);
    cout << "Call the show function of Passing by reference :" << endl;
    showCandyBar(cb);
}
template <typename T>
T& max5(T arr[]) {
    T max = arr[0];
    for (int i = 1; i < 5; i++) {
        max = max > arr[i] ? max : arr[i];
    }
    return max;
}
void e2() {
    cout << setw(30) << cout.fill('=') << "Exercises 2" << setw(30) << cout.fill('=') << endl;
    int arr1[] = { 1,2,3,4,5 };
    double arr2[] = { 1.1, 2.0, 3.0, 4.0, 5.5 };
    cout << "Max int = " << max5(arr1) << endl;
    cout << "Max int = " << max5(arr2) << endl;
}