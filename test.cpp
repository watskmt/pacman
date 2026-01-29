#include "DxLib.h"

typedef enum {
    CELL_PATH = 0,
    CELL_WALL = 1
} CellType;

int keyCheck(void);
void moveCharacter(int key, int* x, int* y, int* a);
void drawBoard(void);
void drawCharacter(int x, int y, int a, int handle);
void initMap(void);

#define WINDOWSIZE 800
#define CELLSIZE 40
#define CELLS (WINDOWSIZE / CELLSIZE)
#define PI 3.14159265358979323846

CellType board[CELLS][CELLS];

const char level[CELLS][CELLS + 1] = {
    "####################",
    "#..................#",
    "#..................#",
    "#..................#",
    "#........##........#",
    "#.......#..#......#",
    "#.....#......#.....#",
    "#...#..........#...#",
    "#....#........#....#",
    "#......#.....#.....#",
    "#.........##.......#",
    "#..................#",
    "#...################",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "####################"
};

// プログラムはここから
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(WINDOWSIZE, WINDOWSIZE, 32);

    if (DxLib_Init() == -1) return -1;

    SetMouseDispFlag(TRUE);

    initMap();

    int handle = LoadGraph("chara1.png");
    int x = 1, y = 1; // キャラ位置
    int a = 0;        // 向き

    while (!CheckHitKey(KEY_INPUT_ESCAPE)) {
        ClearDrawScreen();

        drawBoard();
        drawCharacter(x, y, a, handle);

        int key = keyCheck();
        moveCharacter(key, &x, &y, &a);

        ScreenFlip();
        ProcessMessage();
    }

    DxLib_End();
    return 0;
}

void initMap(void)
{
    for (int y = 0; y < CELLS; y++) {
        for (int x = 0; x < CELLS; x++) {
            if (level[y][x] == '#')
                board[x][y] = CELL_WALL;
            else
                board[x][y] = CELL_PATH;
        }
    }
}

void drawBoard(void)
{
    for (int y = 0; y < CELLS; y++) {
        for (int x = 0; x < CELLS; x++) {
            unsigned int c;
            int fill;

            if (board[x][y] == CELL_WALL) {
                c = GetColor(100, 100, 255);
                fill = TRUE;
            }
            else {
                c = GetColor(50, 50, 100);
                fill = FALSE;
            }

            DrawBox(
                x * CELLSIZE,
                y * CELLSIZE,
                x * CELLSIZE + CELLSIZE,
                y * CELLSIZE + CELLSIZE,
                c,
                fill
            );
        }
    }
}

void drawCharacter(int x, int y, int a, int handle)
{
    double rad = (a == 2) ? 0.0 : a * PI / 2.0;
    int turn = (a == 2);

    DrawRotaGraph3(
        x * CELLSIZE + CELLSIZE / 2,
        y * CELLSIZE + CELLSIZE / 2,
        150,
        150,
        (double)CELLSIZE / 300,
        (double)CELLSIZE / 300,
        rad,
        handle,
        TRUE,
        turn
    );
}

// 戻り値：0=なし 1=上 2=下 3=左 4=右
int keyCheck(void)
{
    static int up = 0, down = 0, left = 0, right = 0;

    int key = 0;
    int u = CheckHitKey(KEY_INPUT_UP);
    int d = CheckHitKey(KEY_INPUT_DOWN);
    int l = CheckHitKey(KEY_INPUT_LEFT);
    int r = CheckHitKey(KEY_INPUT_RIGHT);

    if (!up && u) key = 1;
    if (!down && d) key = 2;
    if (!left && l) key = 3;
    if (!right && r) key = 4;

    up = u;
    down = d;
    left = l;
    right = r;

    return key;
}

void moveCharacter(int key, int* x, int* y, int* a)
{
    int px = *x;
    int py = *y;

    switch (key) {
    case 1: (*y)--; *a = 3; break;
    case 2: (*y)++; *a = 1; break;
    case 3: (*x)--; *a = 2; break;
    case 4: (*x)++; *a = 0; break;
    default: return;
    }

    if (*x < 0 || *x >= CELLS || *y < 0 || *y >= CELLS ||
        board[*x][*y] == CELL_WALL) {
        *x = px;
        *y = py;
    }
}
