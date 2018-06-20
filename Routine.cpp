#include "Routine.h"
#include "MapBox.h"
#include "Task_Player.h"
#include "MyPG.h"

//�}�b�v���̃R�[�i�[�̐�
#define corners 28
//���݂̍ő��I������
//4�����ő�
#define choices 4

//�[���N���A
Routine::Routine()
{
	srand((unsigned int)time(NULL));
	this->pos = ML::Vec3(0.0f, 0.0f, 0.0f);
	this->moveVec = ML::Vec3(0, 0, 0);
	this->hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->now = 19;
	this->final_Phase = false;
	this->target = 0;
	for (int i = 0; i < corners; ++i)
	{
		this->cornerPos[i] = ML::Vec3(0,0,0);
	}
	for (int i = 0; i < corners; ++i)
	{
		for (int j = 0; j < choices; ++j)
		{
			this->choiceCorner[i][j]=-1;
		}
	}
}

//
int Routine::Get_Now()
{
	return this->now;
}

//�߂��R�[�i�[��ڎw���Ĉړ�����
//toVec���G�l�~�[�̈ړ��ʂɑ�����Ďg�p
ML::Vec3 Routine::Move(const ML::Vec3& ePos_)
{
	ML::Vec3 toVec = this->cornerPos[this->target] - ePos_;
	return toVec.Normalize();
}
//�������R�[�i�[�ɓ��B�������̔���
bool Routine::Judge(const ML::Box3D& eHit_, const ML::Vec3& ePos_)
{
	//����p�ϐ����R�[�i�[�̔ԍ��O�Ő錾
	this->now = this->Recieved(eHit_,ePos_);
	//judge�̒l�������ꂩ�̃R�[�i�[�̔ԍ��ɂȂ����Ƃ�
	if (this->now >= 0)
	{
		/*this->Choice(this->now);*/
		return true;
	}
	else
	{
		return false;
	}
}
//�����̂���R�[�i�[����s����R�[�i�[��I������
void Routine::Choice(const int& now_)
{
	this->target = -1;
	//�R�[�i�[�ɂ���đI�����̐����Ⴄ����
	//���R�����o��܂őI����J��Ԃ�
	if (this->Is_Final_Phase() == false)
	{
		while (this->target < 0)
		{
			this->target = this->choiceCorner[this->now][rand() % choices];
		}
	}
	//�ŏI�t�F�[�Y�̏���
	//�I�ׂ�I�����̒��Ńv���C���ƈ�ԋ߂��Ƃ����I��
	else
	{
		//�v���C���̏������炤
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		ML::Vec3 p_pos = pl->Get_Pos();
		int m = 0;
		ML::Vec3 min = p_pos - this->cornerPos[this->choiceCorner[now_][m]];

		for (int i = 1; i < choices; i++)
		{
			int check = this->choiceCorner[now_][i];
			//-1�͕s��A�N�Z�X�����珈����������
			if (check == -1)
			{
				continue;
			}

			ML::Vec3 compare = p_pos - this->cornerPos[check];
			if (min.Length() > compare.Length())
			{
				min = compare;
				m = i;
			}
		}

		this->target = this->choiceCorner[now_][m];
	}
}
//�R�[�i�[�̎���`�̑����G�l�~�[�Ƃ̐ڐG�ɉ����Ď����̔ԍ���Ԃ�
int Routine::Recieved(const ML::Box3D& eHit_, const ML::Vec3& ePos_)
{
	ML::Box3D me = eHit_.OffsetCopy(ePos_);
	//�R�[�i�[�ɋ�`���������A�ڐG������s��
	//for (int i = 0; i < corners; ++i)
	//{
	//	ML::Box3D you = ML::Box3D(this->cornerPos[i].x, this->cornerPos[i].y, this->cornerPos[i].z,
	//		1, 1, 1);
	//	if (you.Hit(me))
	//	{
	//		//�ڐG������`�̔ԍ���Ԃ��Ĕ�����
	//		return i;
	//	}
	//}
	ML::Box3D you = ML::Box3D(this->cornerPos[this->target].x, this->cornerPos[this->target].y, this->cornerPos[this->target].z,
		1, 1, 1);
	if (you.Hit(me))
	{
		//�ڐG������`�̔ԍ���Ԃ��Ĕ�����
		return this->target;
	}
	return -1;
}
void Routine::Set_CornerPos(const ML::Vec3& pos_,const int& num_)
{
	this->cornerPos[num_] = pos_;
}
//�ړI�n�̌����p
//�}�b�v�ǂݍ��݂̌�ɌĂяo��
void Routine::Initialize_RelationShip(int num)
{
	int c = 0;
	if (this->RelationShip_XP(num, c))
	{
		c++;
	}
	if(this->RelationShip_XM(num, c))
	{
		c++;
	}
	if (this->RelationShip_ZP(num, c))
	{
		c++;
	}
	if (this->RelationShip_ZM(num, c))
	{
		c++;
	}
}

bool Routine::RelationShip_XP(int num_, int c)
{
	ML::Box3D me = ML::Box3D(-50, -50, -50, 100, 100, 100);	
	
	for (int xp = 0; xp < 200; xp++)
	{	
		for (int i = 0; i<corners; i++)
		{
			if (num_ == i)
			{
				continue;
			}
			if (me.OffsetCopy(this->cornerPos[num_] + ML::Vec3(xp * 50, 0, 0)).Hit(this->cornerPos[i]))
			{
				this->choiceCorner[num_][c] = i;				
				return true;
			}
		}
	}
	return false;
}

bool Routine::RelationShip_XM(int num_, int c)
{
	ML::Box3D me = ML::Box3D(-50, -50, -50, 100, 100, 100);
	for (int xm = 0; xm < 200; xm++)	
	{
		
		for (int i = 0; i<corners; i++)
		{
			if (num_ == i)
			{
				continue;
			}
			if (me.OffsetCopy(this->cornerPos[num_] + ML::Vec3(xm * -50, 0, 0)).Hit(this->cornerPos[i]))
			{
				this->choiceCorner[num_][c] = i;				
				return true;
			}
		}
	}
	return false;
}

bool Routine::RelationShip_ZP(int num_, int c)
{
	ML::Box3D me = ML::Box3D(-50, -50, -50, 100, 100, 100);
	
	for (int zp = 0; zp < 200; zp++)	
	{
		
		for (int i = 0; i<corners; i++)
		{	
			if (num_ == i)
			{
				continue;
			}
			if (me.OffsetCopy(this->cornerPos[num_] + ML::Vec3(0, 0, zp * 50)).Hit(this->cornerPos[i]))
			{
				this->choiceCorner[num_][c] = i;				
				return true;
			}
		}
	}
	return false;
}

bool Routine::RelationShip_ZM(int num_, int c)
{
	ML::Box3D me = ML::Box3D(-50, -50, -50, 100, 100, 100);
	for (int zm = 0; zm < 200; zm++)
	{
		
		for (int i = 0; i<corners; i++)
		{
			if (num_ == i)
			{
				continue;
			}

			if (me.OffsetCopy(this->cornerPos[num_] + ML::Vec3(0, 0, zm * -50)).Hit(this->cornerPos[i]))
			{
				this->choiceCorner[num_][c] = i;				
				return true;
			}
		}
	}
	return false;
}

//void Routine::Initialize_Corner();
//{
//	this->choiceCorner[0][0] = 1;
//	this->choiceCorner[0][1] = 9;
//
//	this->choiceCorner[1][0] = 2;
//	this->choiceCorner[1][1] = 7;
//
//	this->choiceCorner[2][0] = 1;
//	this->choiceCorner[2][1] = 3;
//
//	this->choiceCorner[3][0] = 2;
//	this->choiceCorner[3][1] = 4;
//	this->choiceCorner[3][2] = 8;
//
//	this->choiceCorner[4][0] = 3;
//	this->choiceCorner[4][1] = 5;
//
//	this->choiceCorner[5][0] = 4;
//	this->choiceCorner[5][1] = 18;
//
//	this->choiceCorner[6][0] = 7;
//	this->choiceCorner[6][1] = 11;
//
//	this->choiceCorner[7][0] = 1;
//	this->choiceCorner[7][1] = 6;
//	this->choiceCorner[7][2] = 8;
//	this->choiceCorner[7][3] = 12;
//
//	this->choiceCorner[8][0] = 3;
//	this->choiceCorner[8][1] = 7;
//	this->choiceCorner[8][2] = 17;
//
//	this->choiceCorner[9][0] = 0;
//	this->choiceCorner[9][1] = 10;
//	this->choiceCorner[9][2] = 19;
//
//	this->choiceCorner[10][0] = 9;
//	this->choiceCorner[10][1] = 11;
//	this->choiceCorner[10][2] = 14;
//
//	this->choiceCorner[11][0] = 6;
//	this->choiceCorner[11][1] = 10;
//	this->choiceCorner[11][2] = 12;
//	this->choiceCorner[11][3] = 15;
//
//	this->choiceCorner[12][0] = 7;
//	this->choiceCorner[12][1] = 11;
//	this->choiceCorner[12][2] = 13;
//
//	this->choiceCorner[13][0] = 7;
//}


//�ŏI�t�F�[�Y�Ɉڍs
void Routine::Set_Final_Phase()
{
	this->final_Phase = true;
}

//�t�@�C�i���t�F�[�Y�����m�F����
bool Routine::Is_Final_Phase()
{
	return this->final_Phase;
}