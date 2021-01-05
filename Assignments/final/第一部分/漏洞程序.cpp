#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include <iostream>
#define EMPTY 0
#define BLACK 1
#define WHITE 2

#define COMPUTER BLACK
#define HUMAN WHITE

#define MIN_SCORE -100
int board[3][3] = {};

int check(void) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY) {
            return board[i][0];
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY) {
            return board[0][i];
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY) {
        return board[0][2];
    }
    return false;
}

bool full(void) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

void print(void) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            char out = '_';
            if (board[i][j] == WHITE) {
                out = 'X';
            } else if (board[i][j] == BLACK) {
                out = 'O';
            } else if (board[i][j] != EMPTY) {
                out = '?';  // well if you see this something may be wrong.
            }
            printf("%c", out);
        }
        puts("");
    }
}

int opp(int p) {
    if (p == BLACK)
        return WHITE;
    else if (p == WHITE)
        return BLACK;
    assert(0);
}

int minimax(int player) {
    int winColor = check();
    if (winColor != EMPTY) {
        return winColor == player ? 1 : -1;
    }
    int movex = -1, movey = -1;
    int max_score = MIN_SCORE;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = player;
                int score = -minimax(opp(player));
                if (score > max_score) {
                    max_score = score;
                    movex = i; movey = j;
                }
                board[i][j] = EMPTY;
            }
        }
    }
    if (movex == -1) {
        return 0;
    }
    return max_score;
}

void ai(int *x, int *y) {// Minimax
    int movex = -1, movey = -1;
    int max_score = MIN_SCORE;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = COMPUTER;
                int score = -minimax(HUMAN);
                if (score > max_score) {
                    max_score = score;
                    movex = i; movey = j;
                }
                board[i][j] = EMPTY;
            }
        }
    }
    *x = movex;
    *y = movey;
}
int main(void) {
    bool success = false;  // human wins?
    char pause[32] = "pause > nul";
    char input[128] = {};  // input is large and it will not be ok.
    char buffer[512] = {};
    system("title Tictactoe");
    puts("Welcome to Tic Tac Toe! Computer first!");
    puts("You're 'X' and I'm 'O'!");
    while (!success) {
        // computer: BLACK
        int x, y;
        puts("Now computer goes!");
        ai(&x, &y);
        board[x][y] = BLACK;
        print();
        if (check()) {
            success = false;
            break;
        }
        if (full()) {
            break;
        }
        puts("Now human goes!");
        bool fl = false;
        while (true) {
            printf("Your turn. Input like (x,y), such as (0,1): ");
            scanf("%s", &buffer);
            strcpy(input, buffer);
            //gets(input);
            x = input[1] - '0';
            y = input[3] - '0';
            printf("You wanna put X on (%d,%d)...\n", x, y);
            if (!(x >= 0 && x < 3 && y >= 0 && y < 3)) {
                puts("Wrong input! Please try again!");
                continue;
            }
            if (board[x][y] != EMPTY) {
                puts("This pos has already been occupied!");
                continue;
            }
            break;
        }
        board[x][y] = WHITE;
        print();
        if (check()) {
            success = true;
            break;
        }
        if (full()) {
            break;
        }
    }
    if (success) {
        puts("What? You win! Here is my secret:");
        puts("Some secret here, e.g. pwd flag etc\n return 1 coin!");
    } else {
        puts("You failed! See you next time~");
    }
    system(pause);
    return 0;
}