#include "Joint.h"

//�ړ�

void Joint::Move(const ML::Vec3& vec)
{
	this->pos += vec;
}

void Joint::Set_Next_Joint(Joint* next)
{
	this->next = next;
}

void Joint::Rotate_Bone(ML::Mat4x4* matR)
{
	//�����ƂȂ����Ă��鍜����]
	this->area->Rotate(matR);
	//�Ȃ���������Ă���ԐڂȂ玟�̂����]������
	if (this->next != nullptr)
	{
		next->Rotated_by_Prev_Joint(matR);
	}
}

void Joint::Rotated_by_Prev_Joint(ML::Mat4x4* matR)
{
	//��]�����Ĉʒu���X�V
	this->pos = matR->TransformCoord(this->pos);

	//���̎��ɂȂ����Ă�����̂���]	
	this->Rotate_Bone(matR);	
}

//getter

//�֐߂̌��݈ʒu��Ԃ�(��΍��W)
ML::Vec3 Joint::Get_Pos()
{
	return this->pos;
}

//�e��]�ʂ̌��E�l��Ԃ�
//X
float Joint::Get_Limit_X_Minus()
{
	return this->limit_X_Minus;
}

float Joint::Get_Limit_X_Plus()
{
	return this->limit_X_Plus;
}

//Y
float Joint::Get_Limit_Y_Minus()
{
	return this->limit_Y_Minus;
}

float Joint::Get_Limit_Y_Plus()
{
	return this->limit_Y_Plus;
}

//Z
float Joint::Get_Limit_Z_Minus()
{
	return this->limit_Z_Minus;
}

float Joint::Get_Limit_Z_Plus()
{
	return this->limit_Z_Plus;
}