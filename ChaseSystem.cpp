#include "ChaseSystem.h"
#include "MyPG.h"
#include "Task_Player.h"
#include "Task_Map.h"

void ChaseSystem::PushBack_Route(const ML::Vec3& pos)
{
	//������vector�ɑg�ݍ���
	this->player_Route.push_back(pos);
}

void ChaseSystem::SensorCheck(const ML::Box3D& hit, const ML::Vec3& plpos, const ML::Vec3& pos, const float& angle)
{
	//�A���O���ɍ��킹�������x�N�g���Z�o
	ML::Mat4x4 matR;
	ML::Vec3 a(1, 0, 0);
	for (int i = -1; i < 2; i++)
	{
		matR.RotationY(angle + ML::ToRadian(45 * (float)i));
		a = matR.TransformNormal(a);

		//�}�b�v�Ƃ̂����蔻��������Ă���^�X�N�����炤
		auto h = ge->GetTask_One_G<Map::Object>("�t�B�[���h");
		//�A���O�������ɃZ���T�[��`����
		for (float f = 0; f < chipX * 10; f += this->sensor.d / 2.0f)
		{
			ML::Box3D s = this->sensor.OffsetCopy(pos + (a.Normalize()*f));
			//�Z���T�[�̒��S�ɔ͈�1�̋�`�𓯎��ɔ���
			ML::Box3D c(0, 0, 0, 1, 1, 1);
			c.Offset(pos + (a.Normalize()*f));
			//�}�b�v�Ƃ̂����蔻��ł��������珈���I��
			if (h->Map_CheckHit(c))
			{
				break;
			}
			//�Z���T�[�ɓ���������
			if (s.Hit(hit))
			{
				//�`�F�C�X���[�h�ɕύX���A�v���C��ʒu�����[�g�ɓo�^
				this->systemFlag = true;
				this->PushBack_Route(plpos);
				break;
			}
		}
	}
}

ML::Vec3 ChaseSystem::NextRoute()
{
	//�ǐՃ��[�g���I�������ʏ탂�[�h��
	if (this->Is_Any_More_Route())
	{
		//�ʏ탂�[�h�Ԋ�
		this->Shift_to_Routine();
		//�ړI�n�[���x�N�g����Ԃ�
		return ML::Vec3(0, 0, 0);
	}
	ML::Vec3 r = this->player_Route[this->destination];
	r.y = 20;
	this->destination++;
	return r;

}

void ChaseSystem::Shift_to_Routine()
{
	this->systemFlag = false;
	this->destination = 0;
	this->player_Route.clear();
}

bool ChaseSystem::Is_Chase_Mode() const
{
	return this->systemFlag;
}

bool ChaseSystem::Is_Any_More_Route() const
{
	return destination > this->player_Route.size() - 1 ? true : false;
}