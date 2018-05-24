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
	//自分とつながっている全てのブレーカーを検索
	for (auto b : this->cunnected_Breaker)
	{
		//状態を確認する
		if (!b->Get_Now_State())
		{
			return;
		}		
	}
	//全部働いているならドアを開ける
	if (this->Get_Angle() == LR::Left)
	{
		this->pos.x -= 5;
	}
	else
	{
		this->pos.x += 5;
	}
	this->hitBase.Offset(this->pos);
	//開かれる間時間経過
	this->timeCnt++;
	//1秒過ぎたら完全に開かれたことにする
	if (this->Is_Opened_Over())
	{
		this->openFlag = true;
	}
}

//void Door::Door_Open(Breaker* b0, Breaker* b1, Breaker* b2)
//{
//	//引数でもらうブレーカーの状態を確認する
//	if (b0->Get_Now_State() && b1->Get_Now_State() && b2->Get_Now_State())
//	{
//		//全部働いているならドアを開ける
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