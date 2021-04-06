#include "dot.h"
#include "timeTick.cpp"

void welcome();

//////////////////////////////////////////////
//////主函数
	//可选参数：ONE_BIN TWO_BIN ONE_TXT TWO_TXT 
	//          ONE_BIN_QUICK devide devideQuick devideQuickC
	//对于低要求的数据(TXT,BIN,CSV...)，接受多种文件输入类型。
	//对于高要求数据BIN，只接受ACM赛制的标准输入
/////////////////////////////////////////////////////////////////////
int main() {
	welcome();
	int n = 100000000;
	string str = to_string(n) + ".dat";
	TimePoint NowTime;
	
	//makerand(ONE_BIN_QUICK, n);
	//cout << "生成数据用时：\t" << NowTime.timeDffer() / 1000 << "ms.\n";
	readData(ONE_BIN, str);
	cout << "==========传统IO单线程：" <<
		NowTime.timeDffer() / 1000 << "ms." << setw(40) << cout.fill('=') << "\n\n";
	NowTime.setTime();
	readData(ONE_BIN_QUICK,str);
	cout << "==========快速IO单线程：" <<
		NowTime.timeDffer() / 1000 << "ms." << setw(40) << cout.fill('=') << "\n\n";
	NowTime.setTime();
	devide(str);
	cout << "==========快速IO多线程：" <<
		NowTime.timeDffer() / 1000 << "ms." << setw(40) << cout.fill('=') << "\n\n";
	NowTime.setTime();
	devideQuick(str);
	cout << "==========加载IO多线程四路 [加IO总时间最短] ：" <<
		NowTime.timeDffer() / 1000 << "ms." << setw(40) << cout.fill('=') << "\n\n";
	NowTime.setTime();
	devideQuickC(str);
	cout << "==========复制IO多线程四路 [纯计算时间最短] ：" <<
		NowTime.timeDffer() / 1000 << "ms." << setw(40) << cout.fill('=') << "\n\n";
	NowTime.setTime();
	system("pause > nul");
	return 0;
}
void welcome() {
	cout <<
		" ___  __      __                _____            _     ___ " << endl <<
		"|  _| \\ \\    / /               |  __ \\          | |   |_  |" << endl <<
		"| |    \\ \\  / /    ___    ___  | |  | |   ___   | |_    | |" << endl <<
		"| |     \\ \\/ /    / _ \\  / __| | |  | |  / _ \\  | __|   | |" << endl <<
		"| |      \\  /    |  __/ | (__  | |__| | | (_) | | |_    | |" << endl <<
		"| |_      \\/      \\___|  \\___| |_____/   \\___/   \\__|  _| |" << endl <<
		"|___|                                                 |___|" << endl;
	cout << setw(30) << cout.fill('=') << "向量点乘" << setw(30) <<
		cout.fill('=') << "\n\n";
}