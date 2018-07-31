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
	//自分とつながっている全てのブレーカーを検索
	for (auto& b : this->cunnected_Breaker)
	{		
		//状態を確認する
		if (!b->Get_Now_State())
		{
			return;
		}
	}
	//全部働いているならドアを開ける
	if (this->Is_Angle_Left())
	{
		this->pos.x -= DOOR_OPEN_SPEED;
	}
	else
	{
		this->pos.x += DOOR_OPEN_SPEED;
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

//フェーズ移行感知
bool Door::Is_Phase_Offset() const
{
	//活動しているブレーカーの数
	int actived_Breaker_Number = 0;	
	//繋がっているブレーカーを全部確認
	for (auto b : this->cunnected_Breaker)
	{
		if (b->Get_Now_State())
		{
			//活動ブレーカーの数を確認する
			actived_Breaker_Number++;
		}
	}
	//2体以上なら最終フェーズに
	if (actived_Breaker_Number >= 2)
	{
		return true;
	}
	return false;
}


int Door::How_Many_Breaker_Be_Cunnected() const
{
	//活動しているブレーカーの数
	int actived_Breaker_Number = 0;
	//繋がっているブレーカーを全部確認
	for (auto b : this->cunnected_Breaker)
	{
		if (b->Get_Now_State())
		{
			//活動ブレーカーの数を確認する
			actived_Breaker_Number++;
		}
	}

	return actived_Breaker_Number;
}