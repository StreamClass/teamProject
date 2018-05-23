#include "Door.h"

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
	this->hitBase = ML::Box3D(-25, -100, -25, 50, 100, 50);
	this->timeCnt = 0;
	this->openFlag = false;
	this->cunnected_Breaker = b;
	this->open_Angle = a;
}

void Door::Door_Open()
{
	//�����ƂȂ����Ă���S�Ẵu���[�J�[������
	for (auto b : this->cunnected_Breaker)
	{
		//��Ԃ��m�F����
		if (!b->Get_Now_State())
		{
			return;
		}		
	}
	//�S�������Ă���Ȃ�h�A���J����
	if (this->Get_Angle() == LR::Left)
	{
		this->pos.x -= 5;
	}
	else
	{
		this->pos.x += 5;
	}
	//�J�����Ԏ��Ԍo��
	this->timeCnt++;
	//1�b�߂����犮�S�ɊJ���ꂽ���Ƃɂ���
	if (this->Is_Opened_Over())
	{
		this->openFlag = true;
	}
}

//void Door::Door_Open(Breaker* b0, Breaker* b1, Breaker* b2)
//{
//	//�����ł��炤�u���[�J�[�̏�Ԃ��m�F����
//	if (b0->Get_Now_State() && b1->Get_Now_State() && b2->Get_Now_State())
//	{
//		//�S�������Ă���Ȃ�h�A���J����
//		this->openFlag = true;
//	}
//}

bool Door::Player_Hit_the_Door(const ML::Box3D& hit)
{
	return hit.Hit(this->hitBase.OffsetCopy(this->pos));
}

bool Door::Get_State()
{
	return this->openFlag;
}

ML::Vec3 Door::Get_Pos()
{
	return this->pos;
}

LR Door::Get_Angle()
{
	return this->open_Angle;
}

bool Door::Is_Opened_Over()
{
	return this->timeCnt > 60;
}