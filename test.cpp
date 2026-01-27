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
<<<<<<< HEAD
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
    "####################",
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
=======
	"####################",
	"#...#..............#",
	"#...#..............#",
	"#...############...#",
	"#..............#...#",
	"#..............#...#",
	"#..................#",
	"#..................#",
	"#..................#",
	"#........#.........#",
	"#........#.........#",
	"#........#.........#",
	"#...##########.....#",
	"#..................#",
	"#.....###..........#",
	"#..................#",
	"#......####........#",
	"#..................#",
	"#..................#",
	"####################",
};

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
>>>>>>> origin/zen
{
    ChangeWindowMode(TRUE);               // �� DxLib_Init �O
    SetGraphMode(WINDOWSIZE, WINDOWSIZE, 32);

<<<<<<< HEAD
    if (DxLib_Init() == -1) return -1;

    SetMouseDispFlag(TRUE);

    initMap();
=======
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	ChangeWindowMode(TRUE);		// ウィンドウモードに設定
	SetMouseDispFlag(TRUE);
	SetGraphMode(WINDOWSIZE, WINDOWSIZE, 32); // 画面サイズを640x480、色深度32ビットに設定

	WaitKey();				// キー入力待ち
>>>>>>> origin/zen

    int x = 1, y = 1;     // �����ʒu
    int a = 0;            // �����i0:�E 1:�� 2:�� 3:��j

<<<<<<< HEAD
    int handle = LoadGraph("chara1.png");

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
            int color;
            int fill;

            if (board[x][y] == CELL_WALL) {
                color = GetColor(100, 100, 255);
                fill = TRUE;
            }
            else {
                color = GetColor(50, 50, 100);
                fill = FALSE;
            }

            DrawBox(
                x * CELLSIZE,
                y * CELLSIZE,
                x * CELLSIZE + CELLSIZE,
                y * CELLSIZE + CELLSIZE,
                color,
                fill
            );
        }
    }
}

void drawCharacter(int x, int y, int a, int handle)
{
    double rad = (double)a * PI / 2.0;
    int turn = (a == 2); // �������������]

    DrawRotaGraph(
        x * CELLSIZE + CELLSIZE / 2,
        y * CELLSIZE + CELLSIZE / 2,
        (double)CELLSIZE / 300.0,
        rad,
        handle,
        TRUE,
        turn
    );
}

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

    up = u; down = d; left = l; right = r;
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
    default: break;
    }

    if (*x < 0 || *x >= CELLS || *y < 0 || *y >= CELLS ||
        board[*x][*y] == CELL_WALL) {
        *x = px;
        *y = py;
    }
=======
	int x = 1, y = 2; //　キャラクターの位置（座標ではなく、マス目の位置）
	int a = 0; // 0:右、1:下、2:左、3:上

	int handle = LoadGraph("chara1.png");
	
	do {
		ClearDrawScreen(); // 画面をクリア
		drawBoard();
		drawCharacter(x, y, a, handle);

		int key = keyCheck();
		
		moveCharacter(key, &x, &y, &a);
		ProcessMessage();        // メッセージ処理
	} while (!CheckHitKey(KEY_INPUT_ESCAPE));

	WaitKey();				// キー入力待ち

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

void initMap(void) {
	for (int y = 0; y < CELLS; y++) {
		for (int x = 0; x < CELLS; x++) {
			switch (level[y][x]) {
			case '#': board[x][y] = CELL_WALL; break;
			case '.': board[x][y] = CELL_PATH; break;
			default:  board[x][y] = CELL_PATH; break;
			}
		}
	}
}

void drawBoard() {
	unsigned int c;
	int fillFlag;
		
	for (int i = 0; i < CELLS; i++)
		for (int j = 0; j < CELLS; j++) {
			if (board[j][i] == CELL_WALL) {
			 c = GetColor(255, 100, 255);
				fillFlag = TRUE;
			}
			else {
				c = GetColor(50, 50, 100);
				fillFlag = FALSE;
			}
			DrawBox(j * CELLSIZE, i * CELLSIZE, j * CELLSIZE + CELLSIZE, i * CELLSIZE + CELLSIZE, c, fillFlag);    // 四角形を描画
		}

}

void drawCharacter(int x, int y, int a, int handle) {
	double rad = a == 2 ? 0 : (double)a * PI / 2.0;
	int turn = a == 2 ? TRUE : FALSE; // 左向きのときだけ反転
	DrawRotaGraph3(x * CELLSIZE + CELLSIZE / 2, y * CELLSIZE + CELLSIZE / 2, 150, 150, (double)CELLSIZE / 300, (double)CELLSIZE / 300, rad, handle, TRUE, turn);
}

// 戻り値：　０：押されていない、1：上、2：下、3：左、4：右
int keyCheck() {
	static int upKey = 0, downKey = 0, leftKey = 0, rightKey = 0;

	int key = 0;
	int upKey0 = CheckHitKey(KEY_INPUT_UP);
	int downKey0 = CheckHitKey(KEY_INPUT_DOWN);
	int leftKey0 = CheckHitKey(KEY_INPUT_LEFT);
	int rightKey0 = CheckHitKey(KEY_INPUT_RIGHT);

	if (!upKey && upKey0) {
		key = 1;
	}
	if (!downKey && downKey0) {
		key = 2;
	}
	if (!leftKey && leftKey0) {
		key = 3;
	}
	if (!rightKey && rightKey0) {
		key = 4;
	}

	upKey = upKey0;
	downKey = downKey0;
	leftKey = leftKey0;
	rightKey = rightKey0;

	return key;
}

void moveCharacter(int key, int* x, int* y, int *a)
{
	int prevX = *x;
	int prevY = *y;
	switch (key) {
	case 1: // UP
		(*y)--;
		*a = 3;
		break;
	case 2: // DOWN
		(*y)++;
		*a = 1;
		break;
	case 3: // LEFT
		(*x)--;
		*a = 2;
		break;
	case 4: // RIGHT
		(*x)++;
		*a = 0;
		break;
	default:
		break;
	}
	if (*x < 0 || *x >= CELLS || *y < 0 || *y >= CELLS) {
		*x = prevX;
		*y = prevY;
	}
	if (board[*x][*y] == CELL_WALL) {
		*x = prevX;
		*y = prevY;
	}
>>>>>>> origin/zen
}
