#include<cstdio>
#include<cstring>
#include<iostream>
//#include<windows.h>                                     #for time caculate
using namespace std;

bool flag, flag_a, flag_b;
const int MAX = 2047; //WINDOWS 
int* _multiplier = new int[MAX];
int* _multipliee = new int[MAX];
int* _result = new int[MAX * MAX];
int len_a, len_b, len_s1, len_s2, len;  //opening on stack, default int = 0

bool check(char* data);
void stringToInt(char* s1, char* s2);
void Calculate();

int main() {
	string s1, s2;
	printf("Please input two integers\n");
	cin >> s1 >> s2;
	//long t1 = GetTickCount64();                         #for time caculate
	len_s1 = s1.length();
	len_s2 = s2.length();
	char* c1 = new char[len_s1];
	char* c2 = new char[len_s2];
	strcpy(c1, s1.c_str());
	strcpy(c2, s2.c_str());

	if (!(check(c1) && check(c2))) {
		printf("不合法的输入（课件中为同一行的两个整数，空格分隔）\n");
		std::system("pause>nul");
		exit(-1);
	}
	if (flag_a) {//remove minus-sign
		strcpy(c1, s1.c_str() + 1);
	}
	if (flag_b) {
		strcpy(c2, s2.c_str() + 1);
	}
	flag = flag_a ^ flag_b;  //str a, b are unsigned, use flag to storge sign
	stringToInt(c1, c2); //change data form
	Calculate();  //multiple and output
	//long t2 = GetTickCount64();                          #for time caculate
	//cout << "程序执行时间：" << (t2 - t1) << "ms" << endl;
	system("pause>nul");
	return 0;
}

void out(int x) { //fast output
	if (x > 9) {
		out(x / 10);
	}
	putchar(x % 10 + '0');
}

bool check(char* data) {  //judge weather input is legal & storge sign
	for (int i = 0; i < strlen(data); i++) {
		if (i == 0 && data[0] == 48) {
			cout << "0不是一个合法整数的开头" << endl;
		}
		if (data[i] < 48 || data[i]>57) {
			if (i == 0 && data[0] == 45) {
				if (!flag) {
					flag_a = true;
				}
				else {
					flag_b = true;
				}
			}
			else {
				return false;
			}
		}
	}
	flag = true;
	return true;
}
//below algorithm referenced form cnblogs, rewrite and optimized.
void stringToInt(char* s1, char* s2) { //each 4 num to one int; ensure no overflow:[log(INT_FAST32_MAX)]/2=4 
	char temp[6];
	int len_s1 = strlen(s1);
	int len_s2 = strlen(s2);
	while (len_s1 > 4){
		strncpy(temp, &s1[len_s1 - 4], 4);
		temp[5] = '\0';//no over flow and no warrning
		_multiplier[len_b++] = atoi(temp);
		len_s1 -= 4;
		s1[len_s1] = '\0';//cut
	}
	if (len_s1 > 0) {
		_multiplier[len_b++] = atoi(s1);
	}

	while (len_s2 > 4){
		strncpy(temp, &s2[len_s2 - 4], 4);
		temp[5] = '\0';
		_multipliee[len_a++] = atoi(temp);
		len_s2 -= 4;
		s2[len_s2] = '\0';
	}
	if (len_s2 > 0) {
		_multipliee[len_a++] = atoi(s2);
	}
}
void Calculate() {
	int temp, carry = 0, j, count=0;
	memset(_result, 0, MAX * MAX * 4);
	for (int i = 0; i < len_a; i++) {
		for (j = 0, count = j + i ; j < len_b; j++) {
			temp = _multipliee[i] * _multiplier[j] + carry + _result[count];
			_result[count++] = temp % 10000;
			carry = temp / 10000;  //add to next tern
		}
		while (carry) {  //at most twice
			_result[count++] += (carry % 10000);
			carry /= 10000;
		}
	}
	//multiple end
	//output start
	putchar('\n');
	if (flag) {
		putchar('-');
	}
	out(_result[count - 1]); //remove first nx0
	for (int i = count - 2; i >= 0; i--) {
		if (_result[i] == 0) {
			printf("0000");//e.g. 1234+0000 ->12340000
		}
		else {
			for (j = 4, temp = _result[i]; temp > 0; j--) {
				temp /= 10;
			}
			while (--j >= 0) {
				putchar('0');//e.g. 12340012 0->0->12
			}
			out(_result[i]);
		}
	}
	putchar('\n');
}