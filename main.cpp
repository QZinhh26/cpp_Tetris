#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

#define H 20
#define W 20

#define START_X 4
#define START_Y 2

char board[H][W] = {} ;
int score = 0;
int dropDelay = 200;
bool isPaused = false;


void hideCursor() {
    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = 1;
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class Piece {
public:
    char shape[4][4];
    int x, y;

    Piece() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';

        shape[1][1] = 'X';
        shape[1][2] = 'X';
        shape[2][1] = 'X';
        shape[2][2] = 'X';

        x = W / 2 - 2;
        y = 0;
    }

    bool canMove(int dx, int dy) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (shape[i][j] != ' ') {
                    int tx = x + j + dx;
                    int ty = y + i + dy;

                    if (tx < 1 || tx >= W - 1 || ty >= H - 1)
                        return false;
                    if (ty >= 0 && board[ty][tx] != ' ')
                        return false;
                }
            }
        }
        return true;
    }
};

Piece* currentPiece = nullptr;

void initBoard() {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (i == H - 1 || j == 0 || j == W - 1)
                board[i][j] = '#';
            else
                board[i][j] = ' ';
        }
    }
}

void boardDelPiece() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->shape[i][j] != ' ') {
                int by = currentPiece->y + i;
                int bx = currentPiece->x + j;
                if (by >= 0 && by < H && bx >= 0 && bx < W)
                    board[by][bx] = ' ';
            }
}

void pieceToBoard() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->shape[i][j] != ' ') {
                int by = currentPiece->y + i;
                int bx = currentPiece->x + j;
                if (by >= 0 && by < H && bx >= 0 && bx < W)
                    board[by][bx] = currentPiece->shape[i][j];
            }
}

void drawBoard() {
    for (int i = 0; i < H; i++) {
        gotoxy(START_X, START_Y + i);
        for (int j = 0; j < W; j++) {
            if (board[i][j] == '#') {
                if (i == H - 1) cout << "==";
                else cout << "||";
            }
            else {
                cout << " .";
            }
        }
    }
}

// bool canMove(int dx, int dy){
//     for (int i = 0 ; i < 4 ; i++)
//         for (int j = 0 ; j < 4 ; j++)
//             if (blocks[b][i][j] != ' '){
//                 int tx = x + j + dx;int ty = y + i + dy;
//                 if ( tx<1 || tx >= W-1 || ty >= H-1) return false;
//                 if ( board[ty][tx] != ' ') return false;
//             }
//     return true;
// }

// void removeLine(){
//     int j;
//     for (int i = H-2; i >0 ; i-- ){
//         for (j = 0; j < W-1 ; j++)
//             if (board[i][j] == ' ') break;
//         if (j == W-1){
//             for (int ii = i; ii >0 ; ii-- )
//                 for (int j = 0; j < W-1 ; j++ ) board[ii][j] = board[ii-1][j];
//             i++;
//             draw();
//             _sleep(200);
//         }
//     }
// }

int main()
{
    hideCursor();
    srand((unsigned)time(0));

    system("cls");
    initBoard();

    currentPiece = new Piece();
    pieceToBoard();

    while (true) {
        if (_kbhit()) {
            char c = _getch();
            if (c == 'q' || c == 'Q')
                break;
        }

        boardDelPiece();

        if (currentPiece->canMove(0, 1)) {
            currentPiece->y++;
        }

        pieceToBoard();
        drawBoard();

        Sleep(dropDelay);
    }

    delete currentPiece;
    return 0;
}