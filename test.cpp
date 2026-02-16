#include "DxLib.h"

/*--------------------
  定数・型定義
--------------------*/
typedef enum {
	CELL_PATH = 0,
	CELL_WALL = 1
} CellType;

/*--------------------
  関数プロトタイプ
--------------------*/
int keyCheck(void);
void drawBoard();
void moveCharacter(int key, int* x, int* y, int* a, double* scale);
void drawCharacter(int x, int y, int a, int handle, double scale);
void drawItems(int powerHandle, int smallHandle);
void initMap(void);

#define WINDOWSIZE 800
#define CELLSIZE 40
#define CELLS (WINDOWSIZE / CELLSIZE)
#define PI 3.1415926535897932384626433832795f
CellType board[CELLS][CELLS] = { CELL_PATH };


/*--------------------
  グローバル変数
--------------------*/

char level[CELLS][CELLS + 1] = {
	"##########..########",
	"#...#..............#",
	"#...#............1.#",
	"#...############...#",
	"#..............#...#",
	"#.....1........#...#",
	"#..................#",
	"#...##########.....#",
	"#..................#",
	"#.....###......1...#",
	"#........######....#",
	"#..2...............#",
	"#...####....####...#",
	"#..#.....##.....#..#",
	"#...#..........#...#",
	"#....#........#....#",
	"#......#.....#.....#",
	"#.........#....2...#",
	"#..................#",
	"##########..########",
};

int itemPowerHandle;
int itemSmallHandle;

/*--------------------
  メイン関数
--------------------*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	initMap();

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	ChangeWindowMode(TRUE);		// ウィンドウモードに設定
	SetMouseDispFlag(TRUE);
	SetGraphMode(WINDOWSIZE, WINDOWSIZE, 32); // 画面サイズを640x480、色深度32ビットに設定

	int charHandle = LoadGraph("chara1.png");
	itemPowerHandle = LoadGraph("item_power.png");
	itemSmallHandle = LoadGraph("item_small.png");

	double scale = 1.0; // 大きさ

	int x = 1, y = 2; //　キャラクターの位置（座標ではなく、マス目の位置）
	int a = 0; // 0:右、1:下、2:左、3:上
	while (!CheckHitKey(KEY_INPUT_ESCAPE)) {
		ClearDrawScreen();

		drawBoard();
		drawItems(itemPowerHandle, itemSmallHandle);
		drawCharacter(x, y, a, charHandle, scale);

		int key = keyCheck();
		moveCharacter(key, &x, &y, &a, &scale);

		ProcessMessage();
	}

	DxLib_End();
	return 0;
}

/*--------------------
  マップ初期化
--------------------*/
void initMap(void)
{
	for (int y = 0; y < CELLS; y++) {
		for (int x = 0; x < CELLS; x++) {
			if (level[y][x] == '#') {
				board[x][y] = CELL_WALL;
			}
			else {
				board[x][y] = CELL_PATH;
			}
		}
	}
}

/*--------------------
  マップ描画
--------------------*/
void drawBoard(void)
{
	for (int y = 0; y < CELLS; y++) {
		for (int x = 0; x < CELLS; x++) {

			int color;
			int fill;

			if (board[x][y] == CELL_WALL) {
				color = GetColor(255, 100, 255);
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

/*--------------------
  アイテム描画
--------------------*/
void drawItems(int powerHandle, int smallHandle)
{
	for (int y = 0; y < CELLS; y++) {
		for (int x = 0; x < CELLS; x++) {

			int px = x * CELLSIZE + CELLSIZE / 2;
			int py = y * CELLSIZE + CELLSIZE / 2;

			if (level[y][x] == '1') {
				DrawRotaGraph(px, py, 0.5, 0.0, powerHandle, TRUE);
			}
			else if (level[y][x] == '2') {
				DrawRotaGraph(px, py, 0.5, 0.0, smallHandle, TRUE);
			}
		}
	}
}

/*--------------------
  キャラ描画
--------------------*/
void drawCharacter(int x, int y, int a, int handle, double scale)
{
	double rad = (a == 2) ? 0.0 : (double)a * PI / 2.0;
	int turn = (a == 2);

	DrawRotaGraph3(
		x * CELLSIZE + CELLSIZE / 2,
		y * CELLSIZE + CELLSIZE / 2,
		150, 150,
		(double)CELLSIZE / 300 * scale,
		(double)CELLSIZE / 300 * scale,
		rad,
		handle,
		TRUE,
		turn
	);
}

/*--------------------
  キー入力
--------------------*/
int keyCheck(void)
{
	static int u, d, l, r;
	int key = 0;

	if (!u && CheckHitKey(KEY_INPUT_UP)) key = 1;
	if (!d && CheckHitKey(KEY_INPUT_DOWN)) key = 2;
	if (!l && CheckHitKey(KEY_INPUT_LEFT)) key = 3;
	if (!r && CheckHitKey(KEY_INPUT_RIGHT)) key = 4;

	u = CheckHitKey(KEY_INPUT_UP);
	d = CheckHitKey(KEY_INPUT_DOWN);
	l = CheckHitKey(KEY_INPUT_LEFT);
	r = CheckHitKey(KEY_INPUT_RIGHT);

	return key;
}

/*--------------------
  キャラ移動＆アイテム取得
--------------------*/
void moveCharacter(int key, int* x, int* y, int* a, double* scale)
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
		return;
	}

	if (level[*y][*x] == '1') {
		*scale += 0.2;
		if (*scale > 2.0) *scale = 2.0;
		((char*)level)[*y * (CELLS + 1) + *x] = '.';
	}
	else if (level[*y][*x] == '2') {
		*scale -= 0.2;
		if (*scale < 0.4) *scale = 0.4;
		((char*)level)[*y * (CELLS + 1) + *x] = '.';
	}
}
