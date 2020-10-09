#include "dot.h"
#include "timeTick.cpp"


//////////////////////////////////////////////
//////主函数
int main() {
	cout << setw(30) << cout.fill('=') << "向量点乘" << setw(30) << cout.fill('=') << endl;
	
	//可选参数：ONE_BIN TWO_BIN ONE_TXT TWO_TXT
	makerand(ONE_BIN, 2000020);
	TimePoint nowTime;
	readData(ONE_BIN, "2000020.dat");


	cout << "the program used\t" << nowTime.timeDffer() << "us.\n";
	system("pause>nul");
}
