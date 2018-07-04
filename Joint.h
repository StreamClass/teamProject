#pragma once
#include "Shape.h"

//�֐߃N���X

class Joint
{
private:
	//�֐߂̈ʒu
	ML::Vec3 pos;
	//��]�̌��E�l
	const float limit_X_Minus;
	const float limit_X_Plus;
	const float limit_Y_Minus;
	const float limit_Y_Plus;
	const float limit_Z_Minus;
	const float limit_Z_Plus;
	//�q���ꂽ���̃A�h���X�l(���̖��O)
	Shape* area;
	//���̊֐�(���݂��Ȃ��ꍇ��nullptr)
	Joint* next;

public:
	//�ړ�
	//���� : (�ړ��x�N�g��)
	void Move(const ML::Vec3&);
	//�Ȃ���Ă��鍜����]������
	//���� : (��]�s��̃A�h���X�l)
	void Rotate_Bone(ML::Mat4x4*);
	//��ʂ̊֐߂��疽�߂����ꍇ
	//���� : (��]�s��̃A�h���X�l)
	void Rotated_by_Prev_Joint(ML::Mat4x4*);
	//���̊֐߂��Z�b�g
	void Set_Next_Joint(Joint*);

	//�Q�b�^�[
	ML::Vec3 Get_Pos();
	float Get_Limit_X_Plus();
	float Get_Limit_X_Minus();
	float Get_Limit_Y_Plus();
	float Get_Limit_Y_Minus();
	float Get_Limit_Z_Plus();
	float Get_Limit_Z_Minus();

	//�R���X�g���N�^
	//�[���N���A
	Joint():
		limit_X_Minus(0.0f),
		limit_X_Plus(0.0f),
		limit_Y_Minus(0.0f),
		limit_Y_Plus(0.0f),
		limit_Z_Minus(0.0f),
		limit_Z_Plus(0.0f)
	{
		this->pos = ML::Vec3(0, 0, 0);		
		this->area = nullptr;
		this->next = nullptr;
	}
	//���� : (�֐߂̈ʒu�A��]�ʂ̌��EX-,X+,Y-,Y+,Z-,Z+,�Ȃ����Ă��鍜�̃A�h���X�l)
	Joint(const ML::Vec3& p, const float& xm, const float& xp, const float& ym, const float& yp, const float& zm, const float& zp, Shape* bone) :
		limit_X_Minus(xm),
		limit_X_Plus(xp),
		limit_Y_Minus(ym),
		limit_Y_Plus(yp),
		limit_Z_Minus(zm),
		limit_Z_Plus(zp)
	{
		this->pos = p;
		this->area = bone;
	}

	//�f�X�g���N�^
	~Joint()
	{
		//�q�[�v�̈悩��J��
		delete this->area;
		//�Ȃ����Ă��鎟�̊֐߂�
		//�{�[������J�������
	}
};