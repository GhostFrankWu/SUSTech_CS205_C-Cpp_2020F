#include <iostream>
using namespace std;
void print() {
                cout << "Smile!" ;
}
int main() {
        for (int i=0; i < 3; i++) {
                for (int j = 0; j < 3 - i; j++) {
                        print();
                }
                cout << endl;
        }
        return 0;
}