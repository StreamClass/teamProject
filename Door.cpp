#include "Door.h"
#include "MyPG.h"

#define DOOR_OPEN_SPEED 5

Door::Door()
{
	this->pos = ML::Vec3(0, 0, 0);
	this->hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->timeCnt = 0;
	this->openFlag = false;
	this->cunnected_Breaker.clear();
	this->open_Angle = LR::CLEAR_LR;
}

Door::Door(ML::Vec3 pos, std::vector<Breaker*> b, LR a)
{
	this->pos = pos;
	this->hitBase = ML::Box3D(-chipX / 2, -chipY / 2, -25, chipX, chipY, 50);
	this->timeCnt = 0;
	this->openFlag = false;
	this->cunnected_Breaker = b;
	this->open_Angle = a;
}

void Door::Door_Open()
{
	//�����ƂȂ����Ă���S�Ẵu���[�J�[������
	for (auto& b : this->cunnected_Breaker)
	{		
		//��Ԃ��m�F����
		if (!b->Get_Now_State())
		{
			return;
		}
		this->Sound_Play();
	}
	//�S�������Ă���Ȃ�h�A���J����
	if (this->Is_Angle_Left())
	{
		this->pos.x -= DOOR_OPEN_SPEED;
	}
	else
	{
		this->pos.x += DOOR_OPEN_SPEED;
	}
	this->hitBase.Offset(this->pos);
	//�J�����Ԏ��Ԍo��
	this->timeCnt++;
	//1�b�߂����犮�S�ɊJ���ꂽ���Ƃɂ���
	if (this->Is_Opened_Over())
	{
		this->openFlag = true;
	}
}


bool Door::Player_Hit_the_Door(const ML::Box3D& hit) const
{
	return hit.Hit(this->hitBase.OffsetCopy(this->pos));
}

bool Door::Get_State() const
{
	return this->openFlag;
}

ML::Vec3 Door::Get_Pos() const
{
	return this->pos;
}

bool Door::Is_Angle_Left() const
{
	return this->open_Angle == LR::Left ? true : false;
}

bool Door::Is_Opened_Over() const
{
	return this->timeCnt > 60;
}

//�t�F�[�Y�ڍs���m
bool Door::Is_Phase_Offset() const
{
	//�������Ă���u���[�J�[�̐�
	int actived_Breaker_Number = 0;	
	//�q�����Ă���u���[�J�[��S���m�F
	for (auto b : this->cunnected_Breaker)
	{
		if (b->Get_Now_State())
		{
			//�����u���[�J�[�̐����m�F����
			actived_Breaker_Number++;
		}
	}
	//2�̈ȏ�Ȃ�ŏI�t�F�[�Y��
	if (actived_Breaker_Number >= 2)
	{
		return true;
	}
	return false;
}


int Door::How_Many_Breaker_Be_Cunnected() const
{
	//�������Ă���u���[�J�[�̐�
	int actived_Breaker_Number = 0;
	//�q�����Ă���u���[�J�[��S���m�F
	for (auto b : this->cunnected_Breaker)
	{
		if (b->Get_Now_State())
		{
			//�����u���[�J�[�̐����m�F����
			actived_Breaker_Number++;
		}
	}

	return actived_Breaker_Number;
}

void Door::Set_SoundName(const string& seName)
{
	this->soundName = seName;
}

void Door::Sound_Play()
{
	DM::Sound_Play(this->soundName, false);
}