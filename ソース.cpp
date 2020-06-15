#include "DxLib.h"

struct Rect
{
	float X;
	float Y;
	int W;
	int H;
};

//キャラクターの構造体
struct chara
{
	/*int iX;
	int iY;*/
	float  iYSpeed;
	float  iXSpeed;
	int iCount;
	Rect rect;
};
chara Mario;

struct Block
{
	int No;
	bool hitflag;
	Rect rect;
};
Block Map[16][14];

int MarioGraph1;
int MarioGraph2[4];              //左移動画像変数
int MarioGraphL[4];				 //左加速移動画像変数
int MJ[2];
int BG;
//int Block;
int Block2;
int Ground;
int blue;

int map[14][16] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

bool atari(Rect& a, Rect& b) {
	if (a.X < b.X + b.W &&
		a.X + a.W > b.X &&
		a.Y < b.Y + b.H &&
		a.Y + a.H > b.Y)
	{
		return true;
	}
	return false;
}
bool X(Rect& a, Rect& b)
{
	if (a.X + a.W >= b.X &&
		a.X <= b.X + b.W)
	{
		return true;
	}
	return false;
}
bool Y(Rect& a, Rect& b)
{
	if (a.Y + a.H > b.Y &&
		a.Y < b.Y + b.H)
	{
		return true;
	}
	return false;
}

bool X2(Rect& a, Rect& b)
{
	if (a.X < b.X + b.W)
	{
		return true;
	}
	return false;
}


int MarioLeftFrag;                //左移動フラグ
int MarioRightFrag;				  //右移動フラグ
int MarioLeftFrag2;				  //左加速移動フラグ
int MarioRightFrag2;			  //右加速移動フラグ
int JanpFragL;
int JanpFragR;

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	ChangeWindowMode(TRUE);    //
	SetGraphMode(1024, 896, 32);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);	//裏画面を描画対象へ

	//マリオの初期化処理
	Mario.rect.X = 800;
	Mario.rect.Y = 702;
	Mario.rect.W = 48;
	Mario.rect.H = 64;
	Mario.iXSpeed = 0;
	Mario.iYSpeed = 0;

	//マップの初期化処理
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 14; y++)
		{
			Map[x][y].No = 0;
			Map[x][y].hitflag = false;
			Map[x][y].rect.X = x * 64;
			Map[x][y].rect.Y = y * 64;
			Map[x][y].rect.W = 64;
			Map[x][y].rect.H = 64;
		}
	}
	Map[10][11].No = 1;
	Map[10][11].hitflag = true;


	MarioGraph1 = LoadGraph("media\\チビマリオ.png");
	LoadDivGraph("media\\マリオ左.png", 4, 4, 1, 60, 64, MarioGraph2);
	LoadDivGraph("media\\LEFT.png", 4, 4, 1, 60, 64, MarioGraphL);
	LoadDivGraph("media\\マリオジャンプ.png", 2, 2, 1, 64, 64, MJ);

	BG = LoadGraph("media\\background1.png");
	//Block = LoadGraph("media\\BG.png");
	Block2 = LoadGraph("media\\BrickBlock.png");
	Ground = LoadGraph("media\\Ground.png");
	blue = LoadGraph("media\\blue.png");

	MarioLeftFrag = FALSE;
	MarioRightFrag = FALSE;
	MarioLeftFrag2 = FALSE;
	MarioRightFrag2 = FALSE;
	JanpFragL = FALSE;
	JanpFragR = FALSE;


	int RightFrag = TRUE;
	int LeftFrag = FALSE;

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		Mario.iCount++;                         //経過フレーム数のカウント（アニメーション用）
		if (Mario.iCount >= 16)
		{
			Mario.iCount = 0;
		}

		if (CheckHitKey(KEY_INPUT_LEFT) != 0)     //左移動処理
		{
			Mario.iXSpeed -= 1;
			MarioLeftFrag = TRUE;
			RightFrag = FALSE;
			LeftFrag = TRUE;
			if (Mario.iXSpeed <= -10)               //加速処理
			{
				Mario.iXSpeed = -10;
			}
		}
		else
		{
			MarioLeftFrag = FALSE;
			if (Mario.iXSpeed < 0)					//減速処理
			{
				Mario.iXSpeed += 1;
			}
		}

		if (CheckHitKey(KEY_INPUT_B) == 1 && CheckHitKey(KEY_INPUT_LEFT) == 1)      //左加速移動
		{
			Mario.iXSpeed -= 5;
			MarioLeftFrag2 = TRUE;
		}
		else
		{
			MarioLeftFrag2 = FALSE;
		}

		Mario.rect.X += Mario.iXSpeed;

		if (CheckHitKey(KEY_INPUT_RIGHT) != 0)     //右移動処理
		{
			Mario.iXSpeed += 1;
			MarioRightFrag = TRUE;
			RightFrag = TRUE;
			LeftFrag = FALSE;
			if (Mario.iXSpeed >= 10)
			{
				Mario.iXSpeed = 10;
			}
		}
		else
		{
			MarioRightFrag = FALSE;               //減速処理
			if (Mario.iXSpeed > 0)
			{
				Mario.iXSpeed -= 1;
			}
		}

		if (CheckHitKey(KEY_INPUT_B) == 1 && CheckHitKey(KEY_INPUT_RIGHT) == 1)        //右加速移動
		{
			Mario.iXSpeed += 5;
			MarioRightFrag2 = TRUE;
		}
		else
		{
			MarioRightFrag2 = FALSE;
		}

		//↓↓↓ここからジャンプ処理関連↓↓↓
		if (Mario.rect.Y >= 702)                                        //座標が500以上なら
		{
			if (CheckHitKey(KEY_INPUT_SPACE) != 0)                      //ジャンプ処理
			{
				Mario.iYSpeed = -30;
				JanpFragL = TRUE;
				JanpFragR = TRUE;
				MarioLeftFrag = FALSE;
				MarioRightFrag = FALSE;
			}
			else
			{
				JanpFragL = FALSE;
				JanpFragR = FALSE;
			}
		}
		Mario.rect.Y += Mario.iYSpeed;                             //ジャンプし続ける

		Mario.iYSpeed++;                                       //重力

		if (Mario.rect.Y >= 702)                                   //地面
		{
			Mario.iYSpeed = 0;
		}
		//↑↑↑ここまでジャンプ処理関連↑↑↑

		//↓↓ここから当たり判定処理↓↓
		clsDx();
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 14; y++) {
				if (Map[x][y].hitflag == true) {
					if (X(Mario.rect, Map[x][y].rect))							//Ⅹは左右から当たった時の判定
					{
						if (Y(Mario.rect, Map[x][y].rect))
						{
							Mario.iYSpeed = 0;
							printfDx("Hit\n");
						}
					}
					if (Y(Mario.rect, Map[x][y].rect))
					{
						if (X(Mario.rect, Map[x][y].rect))
						{
							Mario.iXSpeed = 0;
							printfDx("Hit\n");
						}
					}
					
				}
			}
		}

		//↑↑ここまで当たり判定処理↑↑
		printfDx("マリオのX座標は%f,マリオのY座標は%f,Xスピードは%f", Mario.rect.X, Mario.rect.Y, Mario.iXSpeed);


		DrawGraph(0, 0, BG, true);

		for (int x = 0; x < 16; x++)
		{
			for (int y = 0; y < 14; y++)
			{
				if (Map[x][y].hitflag == true)
					DrawGraph(Map[x][y].rect.X, Map[x][y].rect.Y, Block2, false);
				if (map[y][x] == 1)
					DrawGraph(x * 64, y * 64 + 32, Ground, false);
			}
		}



		if (RightFrag == TRUE && MarioRightFrag == FALSE && JanpFragL == FALSE)   //右止ってる描画
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MarioGraph1, true);
		}

		if (LeftFrag == TRUE && MarioLeftFrag == FALSE && JanpFragL == FALSE)   //左止ってる描画
		{
			DrawTurnGraph(Mario.rect.X, Mario.rect.Y, MarioGraph1, true);
		}

		if (MarioLeftFrag == TRUE && JanpFragL == FALSE)
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MarioGraph2[Mario.iCount / 4], true);   //左移動1描画
		}

		if (MarioLeftFrag2 == TRUE)
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MarioGraphL[Mario.iCount / 4], true);   //左移動2描画
		}

		if (MarioRightFrag == TRUE && JanpFragL == FALSE)
		{
			DrawTurnGraph(Mario.rect.X, Mario.rect.Y, MarioGraph2[Mario.iCount / 4], true);   //右移動1描画
		}

		if (MarioRightFrag2 == TRUE)
		{
			DrawTurnGraph(Mario.rect.X, Mario.rect.Y, MarioGraphL[Mario.iCount / 4], true);   //右移動２描画
		}

		if (JanpFragL == TRUE && LeftFrag == TRUE)
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MJ[0], true);
		}
		if (JanpFragR == TRUE && RightFrag == TRUE)
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MJ[1], true);
		}

		ScreenFlip();		//裏画面と表画面の入替
		ClearDrawScreen();	//裏画面の描画を全て消去

	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}