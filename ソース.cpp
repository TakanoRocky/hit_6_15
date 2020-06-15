#include "DxLib.h"

struct Rect
{
	float X;
	float Y;
	int W;
	int H;
};

//�L�����N�^�[�̍\����
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
int MarioGraph2[4];              //���ړ��摜�ϐ�
int MarioGraphL[4];				 //�������ړ��摜�ϐ�
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


int MarioLeftFrag;                //���ړ��t���O
int MarioRightFrag;				  //�E�ړ��t���O
int MarioLeftFrag2;				  //�������ړ��t���O
int MarioRightFrag2;			  //�E�����ړ��t���O
int JanpFragL;
int JanpFragR;

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	ChangeWindowMode(TRUE);    //
	SetGraphMode(1024, 896, 32);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);	//����ʂ�`��Ώۂ�

	//�}���I�̏���������
	Mario.rect.X = 800;
	Mario.rect.Y = 702;
	Mario.rect.W = 48;
	Mario.rect.H = 64;
	Mario.iXSpeed = 0;
	Mario.iYSpeed = 0;

	//�}�b�v�̏���������
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


	MarioGraph1 = LoadGraph("media\\�`�r�}���I.png");
	LoadDivGraph("media\\�}���I��.png", 4, 4, 1, 60, 64, MarioGraph2);
	LoadDivGraph("media\\LEFT.png", 4, 4, 1, 60, 64, MarioGraphL);
	LoadDivGraph("media\\�}���I�W�����v.png", 2, 2, 1, 64, 64, MJ);

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

		Mario.iCount++;                         //�o�߃t���[�����̃J�E���g�i�A�j���[�V�����p�j
		if (Mario.iCount >= 16)
		{
			Mario.iCount = 0;
		}

		if (CheckHitKey(KEY_INPUT_LEFT) != 0)     //���ړ�����
		{
			Mario.iXSpeed -= 1;
			MarioLeftFrag = TRUE;
			RightFrag = FALSE;
			LeftFrag = TRUE;
			if (Mario.iXSpeed <= -10)               //��������
			{
				Mario.iXSpeed = -10;
			}
		}
		else
		{
			MarioLeftFrag = FALSE;
			if (Mario.iXSpeed < 0)					//��������
			{
				Mario.iXSpeed += 1;
			}
		}

		if (CheckHitKey(KEY_INPUT_B) == 1 && CheckHitKey(KEY_INPUT_LEFT) == 1)      //�������ړ�
		{
			Mario.iXSpeed -= 5;
			MarioLeftFrag2 = TRUE;
		}
		else
		{
			MarioLeftFrag2 = FALSE;
		}

		Mario.rect.X += Mario.iXSpeed;

		if (CheckHitKey(KEY_INPUT_RIGHT) != 0)     //�E�ړ�����
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
			MarioRightFrag = FALSE;               //��������
			if (Mario.iXSpeed > 0)
			{
				Mario.iXSpeed -= 1;
			}
		}

		if (CheckHitKey(KEY_INPUT_B) == 1 && CheckHitKey(KEY_INPUT_RIGHT) == 1)        //�E�����ړ�
		{
			Mario.iXSpeed += 5;
			MarioRightFrag2 = TRUE;
		}
		else
		{
			MarioRightFrag2 = FALSE;
		}

		//��������������W�����v�����֘A������
		if (Mario.rect.Y >= 702)                                        //���W��500�ȏ�Ȃ�
		{
			if (CheckHitKey(KEY_INPUT_SPACE) != 0)                      //�W�����v����
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
		Mario.rect.Y += Mario.iYSpeed;                             //�W�����v��������

		Mario.iYSpeed++;                                       //�d��

		if (Mario.rect.Y >= 702)                                   //�n��
		{
			Mario.iYSpeed = 0;
		}
		//�����������܂ŃW�����v�����֘A������

		//�����������瓖���蔻�菈������
		clsDx();
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 14; y++) {
				if (Map[x][y].hitflag == true) {
					if (X(Mario.rect, Map[x][y].rect))							//�]�͍��E���瓖���������̔���
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

		//���������܂œ����蔻�菈������
		printfDx("�}���I��X���W��%f,�}���I��Y���W��%f,X�X�s�[�h��%f", Mario.rect.X, Mario.rect.Y, Mario.iXSpeed);


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



		if (RightFrag == TRUE && MarioRightFrag == FALSE && JanpFragL == FALSE)   //�E�~���Ă�`��
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MarioGraph1, true);
		}

		if (LeftFrag == TRUE && MarioLeftFrag == FALSE && JanpFragL == FALSE)   //���~���Ă�`��
		{
			DrawTurnGraph(Mario.rect.X, Mario.rect.Y, MarioGraph1, true);
		}

		if (MarioLeftFrag == TRUE && JanpFragL == FALSE)
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MarioGraph2[Mario.iCount / 4], true);   //���ړ�1�`��
		}

		if (MarioLeftFrag2 == TRUE)
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MarioGraphL[Mario.iCount / 4], true);   //���ړ�2�`��
		}

		if (MarioRightFrag == TRUE && JanpFragL == FALSE)
		{
			DrawTurnGraph(Mario.rect.X, Mario.rect.Y, MarioGraph2[Mario.iCount / 4], true);   //�E�ړ�1�`��
		}

		if (MarioRightFrag2 == TRUE)
		{
			DrawTurnGraph(Mario.rect.X, Mario.rect.Y, MarioGraphL[Mario.iCount / 4], true);   //�E�ړ��Q�`��
		}

		if (JanpFragL == TRUE && LeftFrag == TRUE)
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MJ[0], true);
		}
		if (JanpFragR == TRUE && RightFrag == TRUE)
		{
			DrawGraph(Mario.rect.X, Mario.rect.Y, MJ[1], true);
		}

		ScreenFlip();		//����ʂƕ\��ʂ̓���
		ClearDrawScreen();	//����ʂ̕`���S�ď���

	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}