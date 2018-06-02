#include "ChaseSystem.h"

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

	matR.RotationY(angle);
	a = matR.TransformCoord(a);
	//�A���O�������ɃZ���T�[��`����
	for (int i = 0; i < 1000; i += 50)
	{
		ML::Box3D s =this->sensor.OffsetCopy(pos + (a.Normalize()*i));
		//�Z���T�[�ɓ���������
		if (s.Hit(hit))
		{
			//�`�F�C�X���[�h�ɕύX���A�v���C��ʒu�����[�g�ɓo�^
			this->systemFlag = true;
			this->PushBack_Route(plpos);
		}
	}
}

ML::Vec3 ChaseSystem::NextRoute()
{
	//�ǐՃ��[�g���I�������ʏ탂�[�h��
	if (destination > this->player_Route.size() - 1)
	{
		//�ʏ탂�[�h�Ԋ�
		this->Shift_to_Routine();
		//�ړI�n�[���x�N�g����Ԃ�
		return ML::Vec3(0,0,0);
	}
	ML::Vec3 r = this->player_Route[this->destination];
	r.y = 50;
	this->destination++;
	return r;
	
}

void ChaseSystem::Shift_to_Routine()
{
	this->systemFlag = false;
	this->destination = 0;
	this->player_Route.clear();
}

bool ChaseSystem::Is_Chase_Mode()
{
	return this->systemFlag;
}