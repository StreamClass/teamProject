#include "ChaseSystem.h"
#include "MyPG.h"
#include "Task_Player.h"
#include "Task_Map.h"
#include "MyMath.h"

void ChaseSystem::PushBack_Route(const ML::Vec3& pos)
{
	//������vector�ɑg�ݍ���	
	this->player_Route.push_back(pos);
}

void ChaseSystem::SensorCheck(const ML::Box3D& hit, const ML::Vec3& plpos, const ML::Vec3& pos, const float& angle)
{		
	for (int i = -5; i < 10; i++)
	{
		//�A���O���ɍ��킹�������x�N�g���Z�o
		ML::Mat4x4 matR;
		ML::Vec3 a(1, 0, 0);
		matR.RotationY(-angle + ML::ToRadian(10 * (float)i));
		a = matR.TransformNormal(a);

		//����̊�ɂȂ���ϒl���ɋ��߂Ă���
		//���ϒl��ۑ�����ꏊ
		float cos = 0.0f;
		//���΃x�N�g��
		ML::Vec3 tovec = (plpos - pos);
		//���蒆�̎����x�N�g���Ƒ��΃x�N�g������ς���
		MyMath::Vector_Dot(&cos, a, tovec);

		//�}�b�v�Ƃ̂����蔻��������Ă���^�X�N�����炤
		auto h = ge->GetTask_One_G<Map::Object>("�t�B�[���h");
		//�A���O�������ɃZ���T�[��`����
		for (float f = 0; f <= chipX * 10; f += chipX/3.0f)
		{
			//�Z���T�[�̒��S�ɔ͈�1�̋�`�𓯎��ɔ���			
			ML::Box3D c(-3,-3,-3,6,6,6);
			c.Offset(pos + (a.Normalize()*f));
			//�}�b�v�Ƃ̂����蔻��ł��������珈���I��
			if (h->Map_CheckHit(c))
			{
				break;
			}

			//�Z���T�[�ɓ���������
			if(cos > cosf(ML::ToRadian(10)) && (tovec.Length() < f))
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
	ML::Vec3 r = ML::Vec3(0, 0, 0);
	//�s��ꏊ���o�Ȃ��܂ŌJ��Ԃ�
	while (r.Is_Zero_Vec())
	{
		r = this->player_Route[this->destination];		
		this->destination++;
	}
	r.y = 20;
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