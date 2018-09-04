#pragma once
#include "Shape.h"
#include "MyMath.h"

//�p�x�������Ă����Ԑ}�`�̂����蔻��p�N���X

class Collision
{
private:
	//360�x�𔻒肷�邽�߂̃��W�A���l
	const float judge_360;
	//�L���[�u�Ɠ_1�̂����蔻��
	bool Check_Collision_Cube_to_Point(Shape*, const ML::Vec3&);

public:
	//�O�ŗ��p���郁�\�b�h
	//�|�����t�����N���X���m�̂����蔻��
	//���� : (���肷�鑤�A���肳��鑤)
	bool Hit_Check(Shape*, Shape*);//���g�͖�����

	//�|�����t�����N���X�ƁA�_�̂����蔻��
	//���� : (���肷��N���X�A���肳���_)
	bool Hit_Check(Shape*, const ML::Vec3&);



	//�R���X�g���N�^�E�f�X�g���N�^
	Collision() :
		judge_360(cosf(359))
	{

	}
};