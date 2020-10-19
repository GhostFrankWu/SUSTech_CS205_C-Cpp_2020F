#include <iostream> 
#include <iomanip>
using namespace std;
int e1();
int e2();
int e3();
int main() {
    e1();
    e2();
    e3();
    system("pause>nul");
}
////////////////////////////////////////////////////
/// e1:
void Displaymenu();
int Add(int a, int b);
int Substract(int a, int b);
int Multiply(int a, int b);
int Divide(int a, int b);
int Modulus(int a, int b);

int e1() {
    cout << setw(30) << cout.fill('=') << "Exercises 1" << setw(30) << cout.fill('=') << endl;
    Displaymenu();
    int YourChoice;
    int a, b;
    char confirm;
    do {
        cout << "Enter your choice(1~5):";
        cin >> YourChoice;
        cout << "Enter your integer numbers:";
        cin >> a >> b;
        cout << "\n";
        switch (YourChoice) {
        case 1:
            cout << "Result:" << Add(a, b) << endl;
            break;
        case 2:
            cout << "Result:" << Substract(a, b) << endl;
            break;
        case 3:
            cout << "Result:" << Multiply(a, b) << endl;
            break;
        case 4:
            cout<< "Result:" << Divide(a, b)<<endl;
            break;
        case 5:
            cout <<"Result:"<< Modulus(a, b) << endl;
            break;
        default:
            cout << "You die!" << endl;
            break;
        }
        cout << "Press y or Y to continue:";
        cin >> confirm;
    } while (confirm == 'y' || confirm == 'Y');
    return 0;
}
void Displaymenu() {
    cout << setw(70) << cout.fill('=') << endl;
    cout << setw(30) << cout.fill(' ') << "MENU" << endl;
    cout << setw(70) << cout.fill('=') << endl;
    cout << "\t1.Add\n\t2.Substract\n\t3.Multiply\n\t4.Divide\n\t5.Modulus\n";
}

int Add(int a, int b) {
    return a + b;
}

int Substract(int a, int b) {
    return a - b;
}

int Multiply(int a, int b) {
    return a * b;
}

int Divide(int a, int b) {
    return a / b;
}

int Modulus(int a, int b) {
    return a % b;
}
////////////////////////////////////////////////////
/// e2:
struct box {
    char maker[40]="Yu shiqi and SAs are good!";
    float height=3.4;
    float width=4.5;
    float length=5.6;
    float volume=0;
};
void displayBox(box b) {
    cout << "Maker:" << b.maker << "\nheight:" << b.length << "\nwidth:" << b.width << "\nlength:" << b.length << "\nvolume:" << b.volume << endl;
}
void cal(box *b) {
    b->volume = b->height * b->length * b->width;
}
int e2() {
    cout << setw(30) << cout.fill('=') << "Exercises 2" << setw(30) << cout.fill('=') << endl;
    box bb;
    cout << "Before setting volume:" << endl;
    displayBox(bb);
    cal(&bb);
    cout << "After setting volume:" << endl;
    displayBox(bb);
    return 0;
}
////////////////////////////////////////////////////
/// e3:
int Fill_array(double arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << "Enter value #" << i+1 << ":";
        cin >> arr[i];
        if (!cin.good()) {
            cin.clear();
            return i;
        }
    }
    return size;
}
void Show_array(double* arr, int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}
void Reverse_array(double* arr, int size){
    if (size/2) {
        double tmp = arr[0];
        arr[0] = arr[size  - 1];
        arr[size - 1] = tmp;
        Reverse_array(&arr[1], size - 2);
    }
    else {
        return;
    }
}
int e3() {
    cout << setw(30) << cout.fill('=') << "Exercises 3" << setw(30) << cout.fill('=') << endl;
    int size;
    cout << "Enter the size of an array:";
    cin >> size;
    double* arr = new double[size];
    size=Fill_array(arr, size);
    Show_array(arr, size);
    Reverse_array(arr, size);
    Show_array(arr, size);
    Reverse_array(arr + 1, size - 2);
    Show_array(arr, size);
    return 0;
}