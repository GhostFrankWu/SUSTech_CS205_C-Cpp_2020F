#include <windows.h>
#include <stdio.h>

int main() {
	HWND h = FindWindow(NULL, L"Tictactoe");//通过程序窗口名找到程序
	DWORD processid;
	GetWindowThreadProcessId(h, &processid);//通过窗口名获取程序ID
	HANDLE hprocess = 0;
	hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processid);//获取对应程序的权限
	if (hprocess == 0) {
		puts("Failed open process.");
		system("pause");
		return 1;
	}
	puts("Successfully opened process");
	if (getchar() == 49) {//直接获胜
		DWORD success = 1;
		DWORD addr = 1703651;
		bool res = WriteProcessMemory(hprocess, (LPVOID)addr, &success, 4, 0);
		if (res) {
			puts("Successfully changed memery.");
		}
		else {
			puts("Failed write menery, please check authority.");
		}
	}
	else {//输出当前棋盘情况
		int board[3][3];
		while (1) {
			DWORD addr = 4524360;
			ReadProcessMemory(hprocess, (LPVOID)addr, board, 36, 0);
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					char out = '_';
					if ((int)board[i][j] == 2) {
						out = 'X';
					}
					else if ((int)board[i][j] == 1) {
						out = 'O';
					}
					else if ((int)board[i][j] != 0) {
						out = '?';
					}
					printf("%c", out);
				}
				puts("");
			}

			if (getchar() != 10) {//修改棋子
				int x, y, val;
				scanf("%d%d%d", &x, &y, &val);
				board[x][y] = val;
				bool res = WriteProcessMemory(hprocess, (LPVOID)addr, &board, 36, 0);
				if (res) {
					puts("Successfully changed memery.");
				}
				else {
					puts("Failed write menery, please check authority.");
				}
			}
		}
	}
	system("pause");
	return 0;
}