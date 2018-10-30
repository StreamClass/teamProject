#include "Tablet.h"
#include "Task_Camera.h"
#include "MyPG.h"
#include "Task_Enemy.h"
#include "Task_MiniMap.h"

Tablet::Tablet()
{
	this->be_used_now = true;
	this->Select = 0;
	this->camera_Pos.clear();
	this->target_Pos.clear();
}

void Tablet::Select_Camera(const unsigned int& index)
{
	auto in1 = DI::GPad_GetState(ge->controllerName);
	auto cm = ge->GetTask_One_G<Camera::Object>("�J�����}��");
	//�f���̍X�V�ƕ��ʂ̍X�V�𕪂���
	if (ge->state == MyPG::MyGameEngine::State::demo)
	{
		if (this->Update_On_Demo())
		{
			cm->Noise_Reset();
		}
	}
	else if(index != this->Select)
	{
		//���͂őI��ԍ��X�V
		/*if (in1.HL.down || in1.LStick.L.down)
		{
			this->Select--;
			cm->Noise_Reset();
		}
		else if (in1.HR.down || in1.LStick.R.down)
		{
			this->Select++;
			cm->Noise_Reset();
		}*/
		cm->Noise_Reset();
		this->Select = index;
	}

	//�͈͂𒴂��Ȃ��悤�ɂ��鏈��
	this->Is_Select_Range_Over();
	//�J�����ύX
	this->Change_Camera();
}

void Tablet::Change_Camera()
{
	//�J�����̈ʒu�Ǝ压�_�X�V
	ge->camera[0]->pos = this->camera_Pos[this->Select];
	ge->camera[0]->target = this->target_Pos[this->Select];
	ge->camera[0]->UpDate();
}

void Tablet::PushBack_Camera(const ML::Vec3& pos, const ML::Vec3& target)
{
	//�J�����̈ʒu�A�压�_�o�^
	this->camera_Pos.push_back(pos);
	this->target_Pos.push_back(target);
}

bool Tablet::Is_Used_Now() const
{
	return this->be_used_now;
}

void Tablet::Open_or_Close_Tablet()
{
	this->be_used_now ? this->be_used_now = false : this->be_used_now = true;
	//�J�����ύX
	this->Change_Camera();
	//�m�C�����Z�b�g
	auto cm = ge->GetTask_One_G<Camera::Object>("�J�����}��");
	cm->Noise_Reset();	
}

void Tablet::Is_Select_Range_Over()
{
	//�͈͂𒴂�����e�I�[�_�ŏ㏑������
	if (this->Select > (int)this->camera_Pos.size()-1)
	{
		this->Select = 0;
	}
	if (this->Select < 0)
	{
		this->Select = (int)this->camera_Pos.size() - 1;
	}
}

bool Tablet::Update_On_Demo()
{
	//�G�l�~�[�^�X�N�̍��W���Ƃ�
	ML::Vec3 epos = ge->GetTask_One_G<Enemy::Object>("�G�l�~�[")->Get_Pos();
	//���ʂ��鋗��
	const float to_Length = 1200.0f;

	//�S�Ă̊Ď��J�����Ƃ̋������Ƃ��Ĉ�苗���ȓ��̃J������I��
	for (size_t i = 0; i < this->camera_Pos.size(); i++)
	{
		ML::Vec3 evec = this->camera_Pos[i] - epos;

		if (evec.Length() < to_Length && this->Select != i)
		{
			this->Select = i;
			return true;
		}
	}
	return false;
}