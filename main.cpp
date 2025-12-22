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
int level = 1;
int baseDelay = 200;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

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

int getBlockColor(char c) {
    switch (c) {
    case 'I': return 11; // Cyan
    case 'O': return 14; // Yellow
    case 'T': return 13; // Magenta
    case 'S': return 10; // Green
    case 'Z': return 12; // Red
    case 'J': return 9;  // Blue
    case 'L': return 6;  // Brown
    case '#': return 8;  // Gray
    default: return 7;
    }
}

class Piece {
public:
    char shape[4][4];
    int x, y;

    Piece() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';

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

    void rotate() {
        char temp[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[j][3 - i] = shape[i][j];

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (temp[i][j] != ' ') {
                    int tx = x + j;
                    int ty = y + i;
                    if (tx < 1 || tx >= W - 1 || ty >= H - 1) return;
                    if (ty >= 0 && board[ty][tx] != ' ') return;
                }

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = temp[i][j];
    }
};

Piece* createPiece(int id) {
    Piece* p = new Piece();
    switch (id) {
    case 0: p->shape[1][0] = p->shape[1][1] = p->shape[1][2] = p->shape[1][3] = 'I'; break;
    case 1: p->shape[1][1] = p->shape[1][2] = p->shape[2][1] = p->shape[2][2] = 'O'; break;
    case 2: p->shape[1][1] = p->shape[2][0] = p->shape[2][1] = p->shape[2][2] = 'T'; break;
    case 3: p->shape[1][1] = p->shape[1][2] = p->shape[2][0] = p->shape[2][1] = 'S'; break;
    case 4: p->shape[1][0] = p->shape[1][1] = p->shape[2][1] = p->shape[2][2] = 'Z'; break;
    case 5: p->shape[1][0] = p->shape[2][0] = p->shape[2][1] = p->shape[2][2] = 'J'; break;
    case 6: p->shape[1][2] = p->shape[2][0] = p->shape[2][1] = p->shape[2][2] = 'L'; break;
    }
    return p;
}

Piece* currentPiece = nullptr;
Piece* nextPiece = nullptr;

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
            char cell = board[i][j];
            setColor(getBlockColor(cell));

            if (cell == '#') {
                if (i == H - 1) cout << "==";
                else cout << "||";
            }
            else if (cell != ' ') {
                cout << "[]";
            }
            else {
                cout << " .";
            }
        }
    }
    setColor(7);
}

void removeFullLines() {
    for (int i = H - 2; i > 0; i--) {
        bool full = true;
        for (int j = 1; j < W - 1; j++) {
            if (board[i][j] == ' ') {
                full = false;
                break;
            }
        }

        if (full) {
            for (int r = i; r > 0; r--) {
                for (int c = 1; c < W - 1; c++) {
                    board[r][c] = board[r - 1][c];
                }
            }

            for (int c = 1; c < W - 1; c++)
                board[0][c] = ' ';

            score += 100;
            i++;
        }
    }
}

void drawScore() {
    gotoxy(START_X + W * 2 + 2, START_Y + 2);
    setColor(14);
    cout << "SCORE:";
    gotoxy(START_X + W * 2 + 2, START_Y + 3);
    setColor(15);
    cout << score;
    setColor(7);
}

void drawNextPiece() {
    int nx = START_X + W * 2 + 2;
    int ny = START_Y + 6;

    setColor(15);
    gotoxy(nx, ny - 1);
    cout << "NEXT:";

    for (int i = 0; i < 4; i++) {
        gotoxy(nx, ny + i);
        cout << "        ";
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (nextPiece->shape[i][j] != ' ') {
                setColor(getBlockColor(nextPiece->shape[i][j]));
                gotoxy(nx + j * 2, ny + i);
                cout << "[]";
            }
        }
    }
    setColor(7);
}

bool isGameOver() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentPiece->shape[i][j] != ' ') {
                int by = currentPiece->y + i;
                int bx = currentPiece->x + j;
                if (by >= 0 && board[by][bx] != ' ')
                    return true;
            }
    return false;
}

void drawPause() {
    gotoxy(START_X + W - 4, START_Y + H / 2);
    setColor(12);
    cout << " PAUSED ";
    setColor(7);
}

void drawGameOver() {
    gotoxy(START_X + W - 5, START_Y + H / 2);
    setColor(12);
    cout << " GAME OVER ";

    gotoxy(START_X + W - 7, START_Y + H / 2 + 1);
    setColor(14);
    cout << " SCORE: " << score << " ";

    gotoxy(START_X + W - 9, START_Y + H / 2 + 3);
    setColor(11);
    cout << " R: Restart  Q: Quit ";
    setColor(7);
}


int showMenu() {
    system("cls");
    setColor(14);
    gotoxy(10, 5);  cout << "TETRIS CONSOLE";
    setColor(7);

    gotoxy(10, 7);  cout << "1. Start Game";
    gotoxy(10, 8);  cout << "2. Speed: Slow";
    gotoxy(10, 9);  cout << "3. Speed: Normal";
    gotoxy(10,10);  cout << "4. Speed: Fast";
    gotoxy(10,11);  cout << "5. Quit";

    while (true) {
        char c = _getch();
        if (c >= '1' && c <= '5')
            return c - '0';
    }
}

void updateLevel() {
    level = score / 500 + 1;

    dropDelay = baseDelay - (level - 1) * 20;
    if (dropDelay < 50)
        dropDelay = 50;
}

void drawLevel() {
    gotoxy(START_X + W * 2 + 2, START_Y + 5);
    setColor(11);
    cout << "LEVEL:";
    gotoxy(START_X + W * 2 + 2, START_Y + 6);
    setColor(15);
    cout << level;
    setColor(7);
}

void resetGame() {
    score = 0;
    level = 1;
    dropDelay = baseDelay;
    isPaused = false;

    initBoard();

    if (currentPiece) delete currentPiece;
    if (nextPiece) delete nextPiece;

    currentPiece = createPiece(rand() % 7);
    nextPiece = createPiece(rand() % 7);
    pieceToBoard();
}

int main()
{
    hideCursor();
    srand((unsigned)time(0));

    system("cls");
    initBoard();

    currentPiece = createPiece(rand() % 7);
    nextPiece = createPiece(rand() % 7);
    pieceToBoard();

    int choice;
    while (true) {
        choice = showMenu();
        if (choice == 1) break;
        if (choice == 2) baseDelay = 400;
        if (choice == 3) baseDelay = 200;
        if (choice == 4) baseDelay = 80;
        if (choice == 5) return 0;
        dropDelay = baseDelay;
    }
    restart_game:
    while (true) {

        if (_kbhit()) {
            char c = _getch();

            if (c == 'p' || c == 'P') {
                isPaused = !isPaused;
            }

            if (isPaused) continue;

            boardDelPiece();

            if (c == 'a' || c == 'A')
                if (currentPiece->canMove(-1, 0)) currentPiece->x--;
            if (c == 'd' || c == 'D')
                if (currentPiece->canMove(1, 0)) currentPiece->x++;
            if (c == 's' || c == 'S')
                if (currentPiece->canMove(0, 1)) currentPiece->y++;
            if (c == 'w' || c == 'W')
                currentPiece->rotate();
            if (c == ' ') { // HARD DROP
                while (currentPiece->canMove(0, 1)) {
                    currentPiece->y++;
                }
                pieceToBoard();
                removeFullLines();
                updateLevel();
                delete currentPiece;
                currentPiece = nextPiece;
                nextPiece = createPiece(rand() % 7);
            }
            if (c == 'q' || c == 'Q')
                break;

            pieceToBoard();
        }

        if (isPaused) {
            drawPause();
            Sleep(100);
            continue;
        }

        boardDelPiece();

        if (currentPiece->canMove(0, 1))
            currentPiece->y++;
        else {
            pieceToBoard();
            removeFullLines();
            updateLevel();
            delete currentPiece;
            currentPiece = nextPiece;
            nextPiece = createPiece(rand() % 7);

            if (isGameOver()) {
                pieceToBoard();
                drawBoard();
                drawGameOver();

                while (true) {
                    char c = _getch();
                    if (c == 'r' || c == 'R') {
                        resetGame();
                        goto restart_game;
                    }
                    if (c == 'q' || c == 'Q') {
                        return 0;
                    }
                }
            }
        }

        pieceToBoard();
        drawBoard();
        drawScore();
        drawLevel();
        drawNextPiece();
        Sleep(dropDelay);
    }

    delete currentPiece;
    delete nextPiece;
    return 0;
}