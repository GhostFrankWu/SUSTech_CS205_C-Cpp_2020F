// Cal.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Cal.h"
#include <atlstr.h> 
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <cstdlib>
#include <vector>
#include <sstream>
using namespace std;

// 全局变量:
#define MAX_LOADSTRING 100 //WIN32程序相关定义
#define MAX_DIGIT 100      //最大支持输入
#define ARCH_D 4           //压位
#define MAXN 9999          //对应压位的最大数
//
char data1[MAX_DIGIT];
char data2[MAX_DIGIT];
string FX = "", FY = "", FZ = "";                        //自定义函数缓存
int index1 = MAX_DIGIT-1, index2 = MAX_DIGIT-1;          
bool dot_flag = false, first_flag = false,
move_flag = false, free_flag = false;  //合法输入判别
int cnt_ld = 0, cnt_rd = 0;                              //括号计数器
string str_formar="" , str_temp="", str_ld="", str_rd="";//文本框
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
///////////////////////////////////////////
//实例化主函数（win32程序入口函数）
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow){
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CAL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow)){
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAL));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0)){
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
///////////////////////////////////////////////////////////////
//大整数相关结构体
struct bign {
	int data_float[MAX_DIGIT], data_int[MAX_DIGIT], data[MAX_DIGIT];//浮点左右
	int f_len, int_len, sign, len;    //sign正负 len浮点位
	char buff[MAX_DIGIT];
	bign() {//初始化
		memset(data_float, 0, sizeof(data_float));
		memset(data, 0, sizeof(data));
		memset(data_int, 0, sizeof(data_int));
		int_len = 1, f_len = 0, len = 1, sign = 0;
	}
	string get() {
		string str = sign ? "-" : "";
		stringstream temp;
		temp << data_int[int_len - 1];
		for (int i = int_len - 2; i >= 0; i--){
			temp.width(ARCH_D);
			temp.fill('0');
			temp << data_int[i];
		}
		if (f_len > 0){
			temp << ".";
			for (int i = 0; i < f_len; i++)
			{
				temp.width(ARCH_D);
				temp.fill('0');
				temp << data_float[i];
			}
		}
		str = str + temp.str();
		return str;
	}
	void insert(string str) {//初始化
		int l, i, j, k, index, t, len1;
		char s[MAX_DIGIT];
		memset(data_float, 0, sizeof(data_float));
		memset(data, 0, sizeof(data));
		memset(data_int, 0, sizeof(data_int));
		memset(s, 0, sizeof(s));
		l = str.length();
		int_len = 0; sign = 0;
		for (i = 0, j = 0; i < l; i++) {
			if (str[i] == '-') {
				sign = 1;
				continue;
			}
			if (str[i] == '.') {
				int_len = j;
				continue;
			}
			s[j++] = str[i] - '0';
		}
		l = j;
		if (int_len == 0)
			int_len = l;
		len1 = int_len;
		j = 0;
		f_len = l - int_len;

		l = int_len;
		int_len = l / ARCH_D;
		if (l % ARCH_D) {
			int_len++;
		}
		index = 0;
		for (i = l - 1; i >= 0; i -= ARCH_D) {
			t = 0;
			k = i - ARCH_D + 1;
			if (k < 0)
				k = 0;
			for (int j = k; j <= i; j++)
				t = t * 10 + s[j];
			data_int[index++] = t;
		}
		l = f_len;
		if (l == 0) {
			f_len = 0;
		}
		else {
			f_len = l / ARCH_D;
			if (l % ARCH_D)
				f_len++;
			index = 0;
			for (i = 0; i <= l - 1; i += ARCH_D) {
				t = 0;
				k = ARCH_D - 1 + i;
				for (j = i; j <= k; j++)
					t = t * 10 + s[len1 + j];
				data_float[index] = t;
				index++;
			}
			f_len = index;
		}
		for (i = 0, j = 0; i < f_len; i++) {
			data[j++] = data_float[f_len - 1 - i];
		}
		for (i = 0; i < int_len; i++) {
			data[j++] = data_int[i];
		}
		len = int_len + f_len;
	}

	void FrontErase0(){
		int i, j;
		while ((data[len - 1] == 0) && ((len - 1) > 0)) {
			len--;
		}
		memset(data_int, 0, sizeof(data_int));
		for (i = f_len, j = 0; i < len; i++, j++) {
			data_int[j] = data[i];
		}
		int_len = j;
	}
	void BackErase0(){
		int i, j;
		while (data_float[f_len - 1] == 0 && f_len > 0)
			f_len--;
		memset(data, 0, sizeof(data));
		if (f_len == 0)
		{
			for (i = 0; i < int_len; i++)
				data[i] = data_int[i];
			len = int_len;
		}
		else
		{
			for (i = f_len - 1, j = 0; i >= 0; i--, j++)
				data[j] = data_float[i];
			for (i = 0; i < int_len; i++)
				data[j++] = data_int[i];
			len = int_len + f_len;
		}

	}
	void MoveLeftOne(){
		int i;
		len++;
		for (i = len - 1; i >= 1; i--)
			data[i] = data[i - 1];
		data[0] = 0;
		f_len = 0, int_len = len;
		for (i = 0; i < int_len; i++)
			data_int[i] = data[i];
	}
	void MoveRightOne(){
		int i;
		len--;
		for (i = 0; i < len; i++) {
			data[i] = data[i + 1];
		}
		data[len] = 0;
		f_len = 0, int_len = len;
		for (i = 0; i < int_len; i++) {
			data_int[i] = data[i];
		}
	}
};
///////////////////////////////结构体定义结束
///////////////////////////////////////////////
///////结构体运算
//方法名对应作用同汇编
bool tgt(bign &b, bign &a) {
	int i;
	if (b.len > a.len) {
		return true;
	}
	if (b.len < a.len) {
		return false;
	}
	for (i = b.len - 1; i > 0; i--) {
		if (b.data[i] > a.data[i]) {
			return true;
		}
		else if (b.data[i] == a.data[i]) {
			continue;
		}
	}
	if (b.data[i] < a.data[i]) {
		return false;
	}
	return (b.data[i] > a.data[i] && i >= 0);
}
bool tlt(bign& b, bign& a) {
	return tgt(a, b);
}
bool teq(bign& b, bign& a) {
	int i;
	if (b.len < a.len) {
		return false;
	}
	if (b.len > a.len) {
		return false;
	}
	if (b.len == a.len) {
		for (i = b.len - 1; i > 0; i--) {
			if (b.data[i] < a.data[i]) {
				return false;
			}
			if (b.data[i] == a.data[i]) {
				continue;
			}
			if (b.data[i] > a.data[i]) {
				return false;
			}
		}
	}
	return (b.data[i] == a.data[i] && i >= 0);
}
//////////////////////////////基本比较定义结束
void Align(bign &b, bign &a) {
	int i, j, Plen;
	memset(b.data, 0, sizeof(b.data));
	memset(a.data, 0, sizeof(a.data));
	Plen = b.f_len > a.f_len ? b.f_len : a.f_len;
	for (i = 0; i < Plen; i++) {
		b.data[i] = b.data_float[Plen - 1 - i], a.data[i] = a.data_float[Plen - 1 - i];
	}
	for (i = Plen, j = 0; i < Plen + b.int_len; i++, j++) {
		b.data[i] = b.data_int[j];
	}
	for (i = Plen, j = 0; i < Plen + a.int_len; i++, j++) {
		a.data[i] = a.data_int[j];
	}
	b.len = b.int_len + Plen; a.len = a.int_len + Plen;
	b.f_len = Plen; a.f_len = Plen;
}
/////////////////////////////////////
//四则运算
bign bsub(bign a, bign b);
bign badd(bign a, bign b) {
	int i, j, pl;
	bign res;
	res = b;
	Align(res, a);
	pl = res.len > a.len ? res.len : a.len;
	if ((res.sign == 1 && a.sign == 1) || (res.sign == 0 && a.sign == 0)) {
		for (i = 0; i < pl; i++) {
			res.data[i] = res.data[i] + a.data[i];
			if (res.data[i] > (MAXN)) {
				res.data[i] -= (MAXN + 1);
				res.data[i + 1]++;
			}
		}
		while (res.data[i] != 0) {
			pl++;
		}
		res.len = pl;
		res.f_len = res.f_len > a.f_len ? res.f_len : a.f_len;
		for (i = 0; i < res.f_len; i++) {
			res.data_float[i] = res.data[res.f_len - 1 - i];
		}
		for (i = res.f_len, j = 0; i < res.len; i++, j++) {
			res.data_int[j] = res.data[i];
		}
		res.int_len = res.len - res.f_len;
		return res;
	}
	if (b.sign == 1 && a.sign == 0) {
		b.sign = 0, a.sign = 0;
		return bsub(a, b);
	}
	b.sign = 0, a.sign = 0;
	return bsub(b, a);
}
bign bsub(bign a, bign b) {
	int i, j;
	int flag = 0;
	bign result, temp;
	Align(a, b);
	b.FrontErase0();
	if ((a.sign == 0 && b.sign == 0) || (a.sign == 1 && b.sign == 1)) {
		if (tgt(b, a)) {
			temp = b, b = a, a = temp;
			flag = 1;
		}
		j = 0;
		for (i = 0; i < a.len; i++) {
			if (a.data[i] < b.data[i]) {
				j = i + 1;
				while (a.data[j++] == 0) {
				}
				a.data[j--]--;
				while (j > i) {
					a.data[j--] += MAXN;
				}
				a.data[i] = a.data[i] + MAXN + 1 - b.data[i];
			}
			else
				a.data[i] = a.data[i] - b.data[i];
		}

		while ( a.len > 1 && a.data[a.len - 1] == 0 ) {
			a.len--;
		}
		if (a.f_len > 0) {
			for (i = 0; i < a.f_len; i++)
				a.data_float[i] = a.data[a.f_len - 1 - i];
			for (i = a.f_len, j = 0; i < a.len; i++, j++)
				a.data_int[j] = a.data[i];

			a.int_len = j; a.sign = flag;
			result = a;
		}
		else {
			for (i = 0; i < a.len; i++) {
				a.data_int[i] = a.data[i];
			}
			a.int_len = a.len, a.sign = flag;
			result = a;
		}
		return result;
	}
	if (b.sign == 1) {
		b.sign = 0;
	}
	else {
		b.sign = 1;
	}
	return badd(a, b);
}
bign bmul(bign a, bign b){
	bign ret;
	int i, j;
	if ((b.sign == 0 && a.sign == 0) || (b.sign == 1 && a.sign == 1)) {
		ret.sign = 0;
	}
	else {
		ret.sign = 1;
	}
	Align(b, a);
	for (i = 0; i < a.len; i++){
		for (j = 0; j < b.len; j++){
			ret.data[i + j] = ret.data[i + j] + a.data[i] * b.data[j];
			while (ret.data[i + j] > MAXN){
				ret.data[i + j + 1] += ret.data[i + j] / (MAXN + 1);
				ret.data[i + j] -= (MAXN + 1) * (ret.data[i + j] / (MAXN + 1));
			}
		}
	}
	ret.len = a.len + b.len + 1; ret.f_len = b.f_len + a.f_len;
	for (i = 0; i < ret.f_len; i++) {
		ret.data_float[i] = ret.data[ret.f_len - 1 - i];
	}
	for (i = ret.f_len, j = 0; i < ret.len; i++, j++) {
		ret.data_int[j] = ret.data[i];
	}
	ret.int_len = j;
	ret.FrontErase0();//对齐（删去多余的）预留的0位
	ret.BackErase0();//fixme
	return  ret;
}

bign bdiv(bign a, bign b) {//减法的整数除法
	bign result, one;   
	result.sign = a.sign ^ b.sign;
	a.sign = b.sign = 0;
	a = bsub(a, b);
	one.insert(result.sign ? "-1" : "1");
	while (!a.sign) {
		a = bsub(a, b);
		result = badd(result, one);
	}
	return result;
}

bign bmod(bign a, bign b) {
	return bsub(a, bmul(b, bdiv(a, b)));
}

bign bpow(bign a, bign b) {
	bign one, res;
	res = badd(a,one);
	res = bsub(res, one);//=的重载没写
	one.insert("1");
	while (tgt(b, one)) {
		res = bmul(a, res);
		b = bsub(b, one);
	}
	return res;
}

//////////////////////////////////////////////////////////////
//主处理函数
//
vector<string> Transfer(string& str);
string Calculate(vector<string> result);
string sabs(string str) {
	str = Calculate(Transfer(str));
	if (str[0] == 45) {
		str.erase(0, 1);
	}
	return str;
}
string slog(string str) {
	str = Calculate(Transfer(str));
	bign len;
	len.insert(str);
	stringstream temp;
	temp << len.int_len;
	return temp.str();
}
string sneg(string str) {
	str = Calculate(Transfer(str));
	if (str[0] == 45) {
		str.erase(0, 1);
		str = "(0+" + str + ')';
	}
	else {
		str = "(0-" + str + ")";
	}
	return str;
}
string sfact(string str) {
	str = Calculate(Transfer(str));
	bign u, tmp,l;
	tmp.insert(str);
	l.insert("1"); 
	u = bsub(tmp, l);
	for (; tgt(u,l); u = bsub(u, l)) {
		tmp = bmul(tmp, u);
	}
	return tmp.get();
}
string ssqrt(string str){
	str = Calculate(Transfer(str));
	double A = atof(str.c_str());
	double x0 = A + 0.25, x1, x2 = x0;
	while (1) {
		x1 = (x0 * x0 + A) / (2 * x0);
		if (fabs(x1 - x0) <= 0.00000001 || x2 == x1) break;  //to break two value cycle.
		x2 = x0;
		x0 = x1;
	}
	stringstream temp;
	temp << x1;
	return temp.str();
	/*  大数的除法通过取整实现，所以目前不能进行的大数运算
	bign x0, x1, x2, A, delta,temp,min;
	A.insert(str);
	delta.insert("0.25");
	min.insert("0.001");
	x0 = badd(A, delta);
	x2 = badd(A, delta);
	while(1){
		x1 = bdiv(badd(bmul(x0,x0) , A), badd(x0, x0)) ;
		temp = bsub(x1, x0);
		if (tlt(temp, min)) break;
		if (teq(x2,x1)) break;  //to break two value cycle.
		x2 = x0;
		x0 = x1;
	}
	return x1.get();
	*/
}
bool findAcut(string str) {
	string data[] = { "abs(" , "log(" ,"sqrt(" ,"sqrt(","negative(" ,"fact(" };
	for (string f : data) {
		string::size_type index = str.find(f);
		if (index != str.npos) {
			return false;
		}
	}
	return true;
}

string ca(string experission) {//多个函数的叠加处理
	while (!findAcut(experission)) {
		string::size_type index = experission.find("negative(");
		if (index != experission.npos) {//neg()函数预处理
			int start = index;
			int i = index + 9;
			int cnt = 1;
			string res = "";
			while (cnt) {
				if (experission[i] == '(') {//闭合括号
					cnt++;
				}
				else if (experission[i] == ')') {
					cnt--;
					if (cnt == 0) {
						i++;
						break;
					}
				}
				res += experission[i++];
			}
			if (findAcut(res)) {
				string temp = sneg(res);
				experission.replace(start, i - start, temp);
			}
		}
		index = experission.find("abs(");
		if (index != experission.npos) {//neg()函数预处理
			int start = index;
			int i = index + 4;
			int cnt = 1;
			string res = "";
			while (cnt) {
				if (experission[i] == '(') {//闭合括号
					cnt++;
				}
				else if (experission[i] == ')') {
					cnt--;
					if (cnt == 0) {
						i++;
						break;
					}
				}
				res += experission[i++];
			}
			if (findAcut(res)) {
				string temp = sabs(res);
				experission.replace(start, i - start, temp);
			}
		}
		index = experission.find("fact(");
		if (index != experission.npos) {//fact()函数预处理
			int start = index;
			int i = index + 5;
			int cnt = 1;
			string res = "";
			while (cnt) {
				if (experission[i] == '(') {//闭合括号
					cnt++;
				}
				else if (experission[i] == ')') {
					cnt--;
					if (cnt == 0) {
						i++;
						break;
					}
				}
				res += experission[i++];
			}
			if (findAcut(res)) {
				string temp = sfact(res);
				experission.replace(start, i - start, temp);
			}
		}
		index = experission.find("sqrt(");
		if (index != experission.npos) {//sqrt()函数预处理
			int start = index;
			int i = index + 5;
			int cnt = 1;
			string res = "";
			while (cnt) {
				if (experission[i] == '(') {//闭合括号
					cnt++;
				}
				else if (experission[i] == ')') {
					cnt--;
					if (cnt == 0) {
						i++;
						break;
					}
				}
				res += experission[i++];
			}
			if (findAcut(res)) {
				string temp = ssqrt(res);
				experission.replace(start, i - start, temp);
			}
		}
		index = experission.find("log(");
		if (index != experission.npos) {//log()函数预处理
			int start = index;
			int i = index + 4;
			int cnt = 1;
			string res = "";
			while (cnt) {
				if (experission[i] == '(') {//闭合括号
					cnt++;
				}
				else if (experission[i] == ')') {
					cnt--;
					if (cnt == 0) {
						i++;
						break;
					}
				}
				res += experission[i++];
			}
			if (findAcut(res)) {
				string temp = slog(res);
				experission.replace(start, i - start, temp);
			}
		}
	}
	return Calculate(Transfer(experission));
}

//////////////////////
//中缀转逆波兰
vector<string> Transfer(string& str){
	map<string, int> precedence;//优先级定义
	precedence["+"] = precedence["-"] = 0,
		precedence["*"] = precedence["/"] = precedence["%"] = 1,
		precedence["^"] = 2,
		precedence["("] = precedence[")"] = -1;
	vector<string> result;
	stack<string> operators;
	if (str.empty()) return result;
	str.insert(0, 1, '(');
	str.push_back(')');
	int size = str.size();
	for (int i = 0; i < size; ++i) {// 忽略空格 
		if (str[i] == ' ') continue;// 提取数字
		if (isdigit(str[i])||str[i]=='.') {
			string temp;
			while (i < size && (isdigit(str[i])) || str[i] == '.') {
				temp += str[i];
				++i;
			}
			result.push_back(temp);
			--i;
			continue;
		}
		if (str[i] == 'x') {//插入（替换）X Y Z
			result.push_back(FX);
			continue;
		}
		if (str[i] == 'y') {
			result.push_back(FY);
			continue;
		}
		if (str[i] == 'z') {
			result.push_back(FZ);
			continue;
		}
		if (str[i] == '(') {//压入（
			operators.push(string(1, str[i]));
		}
		else if (str[i] == ')') {
			while (operators.top() != "(") {
				result.push_back(operators.top());
				operators.pop();
			}
			operators.pop(); // pop（
		}
		else if (operators.empty() || precedence[string(1, str[i])] > precedence[operators.top()]) {
			operators.push(string(1, str[i]));
		}
		else  { // pop高于当前优先级的运算符
			while (!operators.empty()){
				string op = operators.top();
				if (precedence[op] >= precedence[string(1, str[i])]){
					result.push_back(op);
					operators.pop();
				}
				else break;
			}
			operators.push(string(1, str[i])); // 压入当前操作符
		}
	}
	return result;
}
/////////////////
//计算逆波兰式

string Calculate(vector<string> result){
	stack<string> nums;
		for (int i = 0; i < result.size(); ++i) {
			if (isdigit(result[i][0])) {
				nums.push(result[i]);
			}
			else {
				bign num2;
				num2.insert(nums.top());
				nums.pop();
				bign num1;
				num1.insert(nums.top());
				nums.pop();
				if (result[i] == "+") {
					nums.push(badd(num1, num2).get());
				}
				else if (result[i] == "-") {
					nums.push(bsub(num1, num2).get());
				}
				else if (result[i] == "*") {
					nums.push(bmul(num1, num2).get());
				}
				else if (result[i] == "/") {
					nums.push(bdiv(num1, num2).get());
				}
				else if (result[i] == "%") {
					nums.push(bmod(num1, num2).get());
				}
				else {
					nums.push(bpow(num1, num2).get());
				}
			}
		}
	return nums.top();
}
////////////////////////////////////////////////////////////
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance){
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CAL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

/////////////////////////////////////////////
//   函数: InitInstance(HINSTANCE, int)
//   目标: 保存实例句柄并创建主窗口
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)WndProc);

	//用来居中窗口
	int scrWidth, scrHeight;
	RECT rect;
	//获得屏幕尺寸
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//取得窗口尺寸
	GetWindowRect(hWnd, &rect);
	//重新设置rect里的值
	rect.left = (scrWidth - rect.right) / 2;
	rect.top = (scrHeight - rect.bottom) / 2;
	//移动窗口到指定的位置
	SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);


	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

////////////////////////////////////////////////////////////////
///////////文本框编辑操作
///////////
void clsStr(HWND hWnd) {//清屏（下）
	dot_flag = false;
	first_flag = false;
	CString ch1 = _T("");
	SetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();//
}

void clsStrU(HWND hWnd) {//清屏（U）
	CString ch1 = _T("");
	str_formar = "";
	str_ld = "";
	str_rd = "";
	str_temp = "";
	cnt_ld = 0;
	cnt_rd = 0;
	SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();//
}

void addStr(HWND hWnd, string str) {//添加文本（下）
	first_flag = true;
	CString ch1;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	str1 += str;
	ch1.ReleaseBuffer();
	ch1 = str1.c_str();
	SetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
}

void delStr(HWND hWnd) {///删除文本（下）
	CString ch1;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	if (str1.length() > 1) {
		if (str1[str1.length() - 1] == '.') {//如果删除了点，可以再次键入
			dot_flag = false;
		}
		str1.erase(str1.end() - 1);
	}
	else if(str1.length() == 1){
		str1.erase(str1.end() - 1);
		dot_flag = false;
		first_flag = false;
	}
	ch1.ReleaseBuffer();
	ch1 = str1.c_str();
	SetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
}
/////////////////////////////////////////////////////////////////
////////自定义函数
////////
void neg(HWND hWnd) {//正负
	CString ch1;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	if (str_temp.length() > 1) {
		if (str_temp.find("negative(") != 0)//负负得正
			str_temp = "negative(" + str_temp ;
		else {
			str_temp.erase(str_temp.end() - 1);
			str_temp.erase(0, 9);
		}
	}
	else {
		str_temp = "negative(" + str1 ;
	}
	cnt_ld++;
	str_formar = str_formar + str_ld + str_temp + str_rd;
	str_temp = "";
	ch1 = str_formar.c_str();
	SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
	clsStr(hWnd);
}

void sqrt(HWND hWnd) {//开根号
	CString ch1;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	if (str_temp.length() > 1) {
		str_temp = "sqrt(" + str_temp ;
	}
	else {
		str_temp = "sqrt(" + str1 ;
	}
	cnt_ld++;
	str_formar = str_formar + str_ld + str_temp + str_rd;
	str_temp = "";
	ch1 = str_formar.c_str();
	SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
	clsStr(hWnd);
}

void log(HWND hWnd) {//取对数
	CString ch1;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	if (str_temp.length() > 1) {
		str_temp = "log(" + str_temp ;
	}
	else {
		str_temp = "log(" + str1 ;
	}
	cnt_ld++;
	str_formar = str_formar + str_ld + str_temp + str_rd;
	str_temp = "";
	ch1 = str_formar.c_str();
	SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
	clsStr(hWnd);
}

void mod(HWND hWnd) {//取余
	CString ch1, ch2;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	GetDlgItemText(hWnd, IDC_EDIT2, ch2.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	string str2(CW2A(ch2.GetString()));
	ch1.ReleaseBuffer();
	ch2.ReleaseBuffer();
	clsStr(hWnd);
	if (str1.length() > 0) {
		if (str2.length() == 0) {
			str_temp = str_formar + str1 + " % ";
		}
		else {
			str_temp = str_formar + str2 + str1 + " % ";
		}
		str_formar = str_formar + str_ld + str_temp + str_rd;
		str_temp = "";
		ch1 = str_formar.c_str();
		SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
		ch1.ReleaseBuffer();
	}
	clsStr(hWnd);
}

void abs(HWND hWnd) {//绝对值
	CString ch1;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	if (str_temp.length() > 1 ) {
		str_temp = "abs(" + str_temp ;
	}
	else {
		str_temp = "abs(" + str1 ;
	}
	cnt_ld++;
	str_formar = str_formar + str_ld + str_temp + str_rd;
	str_temp = "";
	ch1 = str_formar.c_str();
	SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
	clsStr(hWnd);
}

void lev(HWND hWnd) {//阶乘
	CString ch1;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	if (str_temp.length() > 1) {
		str_temp = "fact(" + str_temp;
	}
	else {
		str_temp = "fact(" + str1;
	}
	str_formar = str_formar + str_ld + str_temp + str_rd;
	str_temp = "";
	ch1 = str_formar.c_str();
	SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
	clsStr(hWnd);
}

void pow(HWND hWnd) {//次方
	CString ch1, ch2;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	GetDlgItemText(hWnd, IDC_EDIT2, ch2.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	string str2(CW2A(ch2.GetString()));
	ch1.ReleaseBuffer();
	ch2.ReleaseBuffer();
	clsStr(hWnd);
	if (str1.length() > 0) {
		if (str2.length() == 0) {
			str_temp = str_formar + str1 + " ^ ";
		}
		else {
			str_temp = str_formar + str2 + str1 + " ^ ";
		}
		str_formar = str_formar + str_ld + str_temp + str_rd;
		str_temp = "";
		ch1 = str_formar.c_str();
		SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
		ch1.ReleaseBuffer();
	}
	clsStr(hWnd);
}

void ld(HWND hWnd) {//左括号
	CString ch1;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	str_ld = str_ld + "(";
	cnt_ld++;
	str1 = str_formar + str_ld+ str_temp+ str_rd;
	ch1 = str1.c_str();
	SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
}

void rd(HWND hWnd) {//右括号
	CString ch1;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	clsStr(hWnd);//自动补全（闭合）
	str_rd = str_rd + ")";
	cnt_rd++;
	str_temp = str_temp + str1;
	str1 = str_formar + str_ld + str_temp + str_rd;
	ch1 = str1.c_str();
	SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
}
///////////////////////////////////////////////////////////////////////

void ms(HWND hWnd) {//记忆清空，重置XYZ
	FX = "";
	FY = "";
	FZ = "";
	CString ch = "x=";
	SetDlgItemText(hWnd, IDC_BUTTON_DEFX, ch.GetBuffer(MAX_DIGIT));
	ch.ReleaseBuffer();
	ch = "y=";
	SetDlgItemText(hWnd, IDC_BUTTON_DEFY, ch.GetBuffer(MAX_DIGIT));
	ch.ReleaseBuffer();
	ch = "z=";
	SetDlgItemText(hWnd, IDC_BUTTON_DEFZ, ch.GetBuffer(MAX_DIGIT));
	ch.ReleaseBuffer();
}
void defx(HWND hWnd) {//定义X,Y,Z
	CString ch1,ch2;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	FX = str1;
	str1 = "x=" + str1;
	ch2 = str1.c_str();
	clsStr(hWnd);
	SetDlgItemText(hWnd, IDC_BUTTON_DEFX, ch2.GetBuffer(MAX_DIGIT));
	ch2.ReleaseBuffer();
}
void defy(HWND hWnd) {
	CString ch1, ch2;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	FY = str1;
	str1 = "y=" + str1;
	ch2 = str1.c_str();
	clsStr(hWnd);
	SetDlgItemText(hWnd, IDC_BUTTON_DEFY, ch2.GetBuffer(MAX_DIGIT));
	ch2.ReleaseBuffer();
}
void defz(HWND hWnd) {
	CString ch1, ch2;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	ch1.ReleaseBuffer();
	FZ = str1;
	str1 = "z=" + str1;
	ch2 = str1.c_str();
	clsStr(hWnd);
	SetDlgItemText(hWnd, IDC_BUTTON_DEFZ, ch2.GetBuffer(MAX_DIGIT));
	ch2.ReleaseBuffer();
}
void getx(HWND hWnd) {//输入X,Y,Z
	if (!free_flag) {
		str_formar = str_formar + str_ld + "x" + str_temp + str_rd;
		CString ch1 = str_formar.c_str();
		SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
		ch1.ReleaseBuffer();
	}
}
void gety(HWND hWnd) {
	if (!free_flag) {
		str_formar = str_formar + str_ld + "y" + str_temp + str_rd;
		CString ch1 = str_formar.c_str();
		SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
		ch1.ReleaseBuffer();
	}
}
void getz(HWND hWnd) {
	if (!free_flag) {
		str_formar = str_formar + str_ld + "z" + str_temp + str_rd;
		CString ch1 = str_formar.c_str();
		SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
		ch1.ReleaseBuffer();
	}
}
///////////////////////////////////////////////////////////////////
//算术操作符
//
void add(HWND hWnd) {//加法
	CString ch1,ch2;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	GetDlgItemText(hWnd, IDC_EDIT2, ch2.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	string str2(CW2A(ch2.GetString()));
	ch1.ReleaseBuffer();
	ch2.ReleaseBuffer();
	char ch = str2[str2.length() == 0 ? 0 : str2.length() - 1];
	bool flag = (ch - 47 > 0 && ch - 57 < 0) ||
		ch == 41 || ch == 'x' || ch == 'y' || ch == 'z';
	if (flag || str1.length()!=0) {
		clsStr(hWnd);
		str_formar = str_formar + str_ld + str_temp + str_rd + str1 + "+";
		str_ld = "";
		str_rd = "";
		str1 = str_formar;
		ch1 = str1.c_str();
		SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
		ch1.ReleaseBuffer();
	}
}
void sub(HWND hWnd) {//减法
	CString ch1, ch2;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	GetDlgItemText(hWnd, IDC_EDIT2, ch2.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	string str2(CW2A(ch2.GetString()));
	ch1.ReleaseBuffer();
	ch2.ReleaseBuffer();
	char ch = str2[str2.length() == 0 ? 0 : str2.length() - 1];
	bool flag = (ch - 47 > 0 && ch - 57 < 0) ||
		ch == 41 || ch=='x' || ch=='y'|| ch=='z';
	if (flag || str1.length() != 0) {
		clsStr(hWnd);
		str_formar = str_formar + str_ld + str_temp + str_rd + str1 + "-";
		str_ld = "";
		str_rd = "";
		str1 = str_formar;
		ch1 = str1.c_str();
		SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
		ch1.ReleaseBuffer();
	}

}
void mul(HWND hWnd) {//乘法
	CString ch1, ch2;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	GetDlgItemText(hWnd, IDC_EDIT2, ch2.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	string str2(CW2A(ch2.GetString()));
	ch1.ReleaseBuffer();
	ch2.ReleaseBuffer();
	char ch = str2[str2.length() == 0 ? 0 : str2.length() - 1];
	bool flag = (ch - 47 > 0 && ch - 57 < 0) ||
		ch == 41 || ch == 'x' || ch == 'y' || ch == 'z';
	if (flag || str1.length() != 0) {
		clsStr(hWnd);
		str_formar = str_formar + str_ld + str_temp + str_rd + str1 + "*";
		str_ld = "";
		str_rd = "";
		str1 = str_formar;
		ch1 = str1.c_str();
		SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
		ch1.ReleaseBuffer();
	}

}
void dev(HWND hWnd) {//除法
	CString ch1, ch2;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	GetDlgItemText(hWnd, IDC_EDIT2, ch2.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	string str2(CW2A(ch2.GetString()));
	ch1.ReleaseBuffer();
	ch2.ReleaseBuffer();
	char ch = str2[str2.length() == 0 ? 0 : str2.length() - 1];
	bool flag = (ch - 47 > 0 && ch - 57 < 0) ||
		ch == 41 || ch == 'x' || ch == 'y' || ch == 'z';
	if (flag || str1.length() != 0) {
		clsStr(hWnd);
		str_formar = str_formar + str_ld + str_temp + str_rd + str1 + "/";
		str_ld = "";
		str_rd = "";
		str1 = str_formar;
		ch1 = str1.c_str();
		SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
		ch1.ReleaseBuffer();
	}

}
void equ(HWND hWnd) {//等于号
	CString ch1, ch2,ch3;
	GetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	GetDlgItemText(hWnd, IDC_EDIT2, ch2.GetBuffer(MAX_DIGIT), MAX_DIGIT);
	string str1(CW2A(ch1.GetString()));
	string str2(CW2A(ch2.GetString()));
	string str3 = str2 + str1;
	ch3 = str3.c_str();
	SetDlgItemText(hWnd, IDC_EDIT2, ch3.GetBuffer(MAX_DIGIT));
	clsStr(hWnd);
	ch1.ReleaseBuffer();
	ch2.ReleaseBuffer();
	ch3.ReleaseBuffer();
	string str = str2+str1;
	if (cnt_ld > cnt_rd) {//闭合括号
		while ((cnt_ld--) - cnt_rd) {
			str = str + ")";
		}
	}else if (cnt_ld < cnt_rd) {
		while ((cnt_rd--) - cnt_ld) {
			str = "(" + str;
		}
	}
	cnt_ld = 0;
	cnt_rd = 0;
	ch1 = str.c_str();
	SetDlgItemText(hWnd, IDC_EDIT2, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
	str = ca(str);
	ch1 = str.c_str();
	SetDlgItemText(hWnd, IDC_EDIT1, ch1.GetBuffer(MAX_DIGIT));
	ch1.ReleaseBuffer();
}


///////////////////////////////////////////////
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message){
		/*  键盘消息，入口点不对，考虑更换入口点或者使用全局钩子
	case WM_KEYDOWN: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case VK_NUMPAD0:
			addStr(hWnd, 0);
			break;

		}
	}*/
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId){
		case IDC_BUTTON800:
			if (first_flag)
				addStr(hWnd, "0");
			break;
		case IDC_BUTTON801:
			addStr(hWnd, "1");
			break;
		case IDC_BUTTON802:
			addStr(hWnd, "2");
			break;
		case IDC_BUTTON803:
			addStr(hWnd, "3");
			break;
		case IDC_BUTTON804:
			addStr(hWnd, "4");
			break;
		case IDC_BUTTON805:
			addStr(hWnd, "5");
			break;
		case IDC_BUTTON806:
			addStr(hWnd, "6");
			break;
		case IDC_BUTTON807:
			addStr(hWnd, "7");
			break;
		case IDC_BUTTON808:
			addStr(hWnd, "8");
			break;
		case IDC_BUTTON809:
			addStr(hWnd, "9");
			break;
		case IDC_BUTTON_DOT:
			if (!dot_flag && first_flag) {
				addStr(hWnd, ".");
				dot_flag = true;
			}
			break;
		case IDC_BUTTON_E:
			clsStr(hWnd);
			addStr(hWnd, "2.7182818284590452353602874713527");
			dot_flag = true;
			break;
		case IDC_BUTTON_PAI:
			clsStr(hWnd);
			addStr(hWnd, "3.1415926535897932384626433832795");
			dot_flag = true;
			break;

		case IDC_BUTTON_NEG:
				neg(hWnd);
			break;
		case IDC_BUTTON_CLS:
			clsStr(hWnd);
			clsStrU(hWnd);
			break;
		case IDC_BUTTON_POW:
			if (first_flag)
				pow(hWnd);
			break;
		case IDC_BUTTON_ABS:
				abs(hWnd);
			break;
		case IDC_BUTTON_MOD:
			if (first_flag)
				mod(hWnd);
			break;
		case IDC_BUTTON_SQRT:
				sqrt(hWnd);
			break;
		case IDC_BUTTON_LOG:
				log(hWnd);
			break;
		case IDC_BUTTON_LEV:
			if (first_flag)
				lev(hWnd);
			break;

		case IDC_BUTTON_MUL:
			mul(hWnd);
			break;
		case IDC_BUTTON_SUB:
			sub(hWnd);
			break;
		case IDC_BUTTON_ADD:
			add(hWnd);
			break;
		case IDC_BUTTON_DEV:
			dev(hWnd);
			break;
		case IDC_BUTTON_EQU:
			free_flag = first_flag;
			equ(hWnd);
			first_flag = free_flag;
			break;
		case IDC_BUTTON_DEFX:
			if (first_flag)
				defx(hWnd);
			break;
		case IDC_BUTTON_DEFY:
			if (first_flag)
				defy(hWnd);
			break;
		case IDC_BUTTON_DEFZ:
			if (first_flag)
				defz(hWnd);
			break;

		case IDC_BUTTON_DEL:
			delStr(hWnd);
			break;
		case IDC_BUTTON_LT:
			ld(hWnd);
			break;
		case IDC_BUTTON_RT:
			rd(hWnd);
			break;
		case IDC_BUTTON_MS:
			ms(hWnd);
			break;
		case IDC_BUTTON_X:
			getx(hWnd);
			break;
		case IDC_BUTTON_Y:
			gety(hWnd);
			break;
		case IDC_BUTTON_Z:
			getz(hWnd);
			break;
			/*
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;*/
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		//  使用 hdc 的绘图代码
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
    return 0;
}
