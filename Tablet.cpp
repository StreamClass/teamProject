#include "Tablet.h"
#include "Task_Camera.h"
#include "MyPG.h"

Tablet::Tablet()
{
	this->be_used_now = false;
	this->Select = 0;
	this->camera_Pos.clear();
	this->target_Pos.clear();
}

void Tablet::Select_Camera()
{
	auto in1 = DI::GPad_GetState("P1");
	auto cm = ge->GetTask_One_G<Camera::Object>("�J�����}��");
	//���͂őI��ԍ��X�V
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
	//�͈͂𒴂��Ȃ��悤�ɂ��鏈��
	this->Is_Select_Range_Over();
	this->Change_Camera();
}

void Tablet::Change_Camera()
{
	//�J�����̈ʒu�Ǝ压�_�X�V
	ge->camera[0]->pos = this->camera_Pos[this->Select];
	ge->camera[0]->target = this->target_Pos[this->Select];
	//ge->camera[0]->forePlane = 10000.0f;
	ge->camera[0]->UpDate();
}

void Tablet::PushBack_Camera(const ML::Vec3& pos, const ML::Vec3& target)
{
	//�J�����̈ʒu�A�压�_�o�^
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
	//�m�C�����Z�b�g
	auto cm = ge->GetTask_One_G<Camera::Object>("�J�����}��");
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
