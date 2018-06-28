#include "Tablet.h"
#include "Task_Camera.h"
#include "MyPG.h"

Tablet::Tablet()
{
	this->be_used_now = true;
	this->Select = 0;
	this->camera_Pos.clear();
	this->target_Pos.clear();
}

void Tablet::Select_Camera()
{
	auto in1 = DI::GPad_GetState("P1");
	auto cm = ge->GetTask_One_G<Camera::Object>("カメラマン");
	//入力で選択番号更新
	if (in1.HL.down || in1.LStick.L.down)
	{
		this->Select--;
		cm->Noise_Reset();
	}
	else if (in1.HR.down || in1.LStick.R.down)
	{
		this->Select++;		
		cm->Noise_Reset();
	}
	//範囲を超えないようにする処理
	this->Is_Select_Range_Over();
	this->Change_Camera();
}

void Tablet::Change_Camera()
{
	//カメラの位置と主視点更新
	ge->camera[0]->pos = this->camera_Pos[this->Select];
	ge->camera[0]->target = this->target_Pos[this->Select];
	//ge->camera[0]->forePlane = 10000.0f;
	ge->camera[0]->UpDate();
}

void Tablet::PushBack_Camera(const ML::Vec3& pos, const ML::Vec3& target)
{
	//カメラの位置、主視点登録
	this->camera_Pos.push_back(pos);
	this->target_Pos.push_back(target);
}

bool Tablet::Is_Used_Now()
{
	return this->be_used_now;
}

void Tablet::Open_or_Close_Tablet()
{
	this->be_used_now ? this->be_used_now = false : this->be_used_now = true;
	//ノイずリセット
	auto cm = ge->GetTask_One_G<Camera::Object>("カメラマン");
	cm->Noise_Reset();
}

void Tablet::Is_Select_Range_Over()
{
	if (this->Select > (int)this->camera_Pos.size()-1)
	{
		this->Select = 0;
	}
	if (this->Select < 0)
	{
		this->Select = (int)this->camera_Pos.size() - 1;
	}
}
