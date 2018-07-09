#pragma once
#include "Shape.h"
#include "GameEngine_Ver3_7.h"

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
	//���݂܂ł̉�]�ʃN�H�[�^�j�I��
	ML::QT rotated;
	//�q���ꂽ���̃A�h���X�l(���̖��O)
	Shape* area;
	//���̊֐�(���݂��Ȃ��ꍇ��nullptr)
	Joint* next;

	//�`�悷�郁�b�V���̖��O
	string mesh_Name;

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
	//��]�ʃA�b�v�f�[�g
	void Quartanion_Update(const ML::QT&);

	//�����_�����O
	void Render();

	
	//���݊֐߂ƃ{�[���܂ł̃x�N�g����Ԃ�
	ML::Vec3 Get_To_Bone() const;



	//�f�o�b�O�p
	void DEBUG();


	//�Q�b�^�[
	ML::Vec3 Get_Pos() const;
	float Get_Limit_X_Plus() const;
	float Get_Limit_X_Minus()const;
	float Get_Limit_Y_Plus()const;
	float Get_Limit_Y_Minus()const;
	float Get_Limit_Z_Plus()const;
	float Get_Limit_Z_Minus()const;

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
		this->rotated = ML::QT();
		this->pos = ML::Vec3(0, 0, 0);		
		this->area = nullptr;
		this->next = nullptr;
		this->mesh_Name = "";
	}
	//���� : (�֐߂̈ʒu�A��]�ʂ̌��EX-,X+,Y-,Y+,Z-,Z+,�Ȃ����Ă��鍜�̃A�h���X�l,���b�V����)
	Joint(const ML::Vec3& p, const float& xm, const float& xp, const float& ym, const float& yp, const float& zm, const float& zp, Shape* bone,const string& name) :
		limit_X_Minus(xm),
		limit_X_Plus(xp),
		limit_Y_Minus(ym),
		limit_Y_Plus(yp),
		limit_Z_Minus(zm),
		limit_Z_Plus(zp)
	{
		//����@�E��@���������߂�
		this->rotated = ML::QT(ML::Vec3(0,1,0),ML::ToRadian(180));
		this->pos = p;
		this->area = bone;
		this->next = nullptr;
		this->mesh_Name = name;
		DG::Mesh_CreateFromSOBFile(this->mesh_Name, "./data/mesh/bone/" + this->mesh_Name + ".sob");
	}

	//�f�X�g���N�^
	~Joint()
	{
		//�q�[�v�̈悩��J��
		delete this->area;
		//���b�V���J��
		DG::Mesh_Erase(this->mesh_Name);
		//�Ȃ����Ă��鎟�̊֐߂�
		//�{�[������J�������
	}
};