#include "DxLib.h"

typedef enum {
	CELL_PATH = 0,
	CELL_WALL = 1,
	CELL_ITEM = 2,
	CELL_POWER = 3
} CellType;

int keyCheck(void);
void moveCharacter(int key, int* x, int* y, int* a, int* itemCount);
void drawBoard();
void drawCharacter(int x, int y, int a, int handle);
void drawteki(int xe, int ye, int ea, int handle);
void initMap(void);
int getTotalItems(void);

#define WINDOWSIZE 800
#define CELLSIZE 40
#define CELLS WINDOWSIZE / CELLSIZE
#define PI    3.1415926535897932384626433832795f
#define TIME_LIMIT 30   // 制限時間（秒）

int startTime;   // 開始時刻（ミリ秒）
int remainTime;  // 残り時間（秒）

CellType board[CELLS][CELLS] = { CELL_PATH };


const char level[CELLS][CELLS + 1] = {
	"##########..########",
	"#p..#.*............#",
	"#...#..............#",
	"#...############...#",
	"#..............#...#",
	"#.....*........#...#",
	"#..................#",
	"#...##########.....#",
	"#.................p#",
	".....###..*.........",
	".........p..........",
	"#.....*............#",
	"#...####....####...#",
	"#..#.....##.....#..#",
	"#...#..........#...#",
	"#....#...*....#....#",
	"#......#.....#.....#",
	"#.........#........#",
	"#.p............*...#",
	"##########..########",
};

int startX = 1, startY = 2; //　キャラクター初期位置
int x = startX, y = startY; //　キャラクターの位置（座標ではなく、マス目の位置）
int ex = 6, ey = 13; //敵の初期値
int powerMode = 0;
int powerStartTime = 0;

// プログラムは WinMain から始まります
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

	WaitKey();				// キー入力待ち

	unsigned int cy = GetColor(200, 200, 100);

	int x = 1, y = 2; //　キャラクターの位置（座標ではなく、マス目の位置）
	int a = 0; // 0:右、1:下、2:左、3:上

	int itemCount = getTotalItems(); // マップ内に配置されたアイテムの総数を取得

	int handle = LoadGraph("chara1.png");
	startTime = GetNowCount();
	int teki = LoadGraph("teki.png");
	do {
		ClearDrawScreen(); // 画面をクリア

		// ==== タイマー更新 ====
		int now = GetNowCount();
		int elapsed = (now - startTime) / 1000;
		remainTime = TIME_LIMIT - elapsed;
		if (remainTime < 0) remainTime = 0;

		drawBoard();
		drawCharacter(x, y, a, handle);

		// ==== タイマー表示 ====
		DrawFormatString(10, 10, GetColor(255, 255, 255),
			"TIME : %d", remainTime);

		if (remainTime == 0) {
			DrawFormatString(300, 380, GetColor(255, 0, 0),
				"TIME UP!");
			ScreenFlip();
			WaitTimer(2000);
			break;
		}

		if (itemCount == 0) {	//ゲームクリア判定
			SetFontSize(64);
			const char* str = "GAME CLEAR!";
			int w = GetDrawStringWidth(str, strlen(str));
			DrawString((WINDOWSIZE - w) / 2, (WINDOWSIZE - 64) / 2, str, GetColor(255, 255, 0));
			ScreenFlip();
			break;
		}

		int key = keyCheck();

		moveCharacter(key, &x, &y, &a, &itemCount);
		ProcessMessage();        // メッセージ処理


		if (powerMode) {
			if (GetNowCount() - powerStartTime > 10000) {
				powerMode = 0;
			}
		}
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
			case '*': board[x][y] = CELL_ITEM; break;
			case 'p': board[x][y] = CELL_POWER; break;
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
			else if (board[j][i] == CELL_ITEM) {  
				c = GetColor(255, 255, 0);          
				fillFlag = TRUE;
			}

			else if(board[j][i] == CELL_POWER) {
				c = GetColor(255, 0, 0);
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
void drawteki(int xe, int ye, int a, int handle) {
	double rad = a == 2 ? 0 : (double)a * PI / 2.0;
	int turn = a == 2 ? TRUE : FALSE; // 左向きのときだけ反転
	DrawRotaGraph3(xe * CELLSIZE + CELLSIZE / 2, ye * CELLSIZE + CELLSIZE / 2, 150, 150, (double)CELLSIZE / 400, (double)CELLSIZE / 400, rad, handle, TRUE, turn);
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

void moveCharacter(int key, int* x, int* y, int* a, int* itemCount)
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
	/*壁の当たり判定*/
	if (board[*x][*y] == CELL_WALL) {
		*x = prevX;
		*y = prevY;
	}

	/*パワーエサの取得*/
	if (board[*x][*y] == CELL_POWER) {
		board[*x][*y] = CELL_PATH;
		powerMode = 1;
		powerStartTime = GetNowCount();
	}

	/*アイテム取得の処理*/
	if (board[*x][*y] == CELL_ITEM) {
		board[*x][*y] = CELL_PATH;
		(*itemCount)--;
	}
	/* 敵との当たり判定 */
	if (*x == ex && *y == ey) {
		if (powerMode) {/*パワーモード中は敵を初期値に戻す*/
			ex = 6;
			ey = 13;
		}
		else {/*それ以外はプレイヤーが初期値に戻る*/
			*x = startX;
			*y = startY;
			*a = 0;
		}

	}


}

int getTotalItems()
{
	int count = 0;
	for (int y = 0; y < CELLS; y++) {
		for (int x = 0; x < CELLS; x++) {
			if (board[x][y] == CELL_ITEM) {
				count++;
			}
		}
	}

  return count;
}

