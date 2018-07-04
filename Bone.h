#pragma once
#include "Joint.h"
#include "Motion.h"

//���i�N���X

//idle�͒���
//�����̐��ʂ�Z+


class Bone
{
private:
	//�̑S�̂̒��S
	ML::Vec3 center_of_Body;
	//�w�̍���
	const float tall;

	//�S�֐߂̃A�h���X�l
	Joint* waist;
	Joint* neck;
	Joint* left_Sholder;	
	Joint* left_Elbow;
	Joint* left_Wrist;
	Joint* left_Hip;
	Joint* left_Knee;
	Joint* left_Ankle;
	Joint* right_Sholder;
	Joint* right_Elbow;
	Joint* right_Wrist;
	Joint* right_Hip;
	Joint* right_Knee;
	Joint* right_Ankle;

	//���[�V�����p�J�E���^
	float motionCnt;
	//���F�N�^�[��[]���Z�q�ŗ��p���鎯�ʎq
	int motion_Index;
	//�����̃��[�V�����Ȃ̂����m�F����L�[
	string now_Motion;

	//���[�V�����f�[�^�̃}�b�s���O
	std::map < string, std::vector<Motion::Motion_Data> > motions;

	//�����̃v���Z�b�g
	//����
	void Walking();
	//����
	void Running();
	//���ݍ�p
	void Interaction();

public:
	//�Q�b�^�[
	ML::Vec3 Get_Center();

	//�̑S�̂�Y����]���s��
	//�ق��̎����K�v�ɉ����Ēǉ��\������(2018/07/04)
	//���� : (Y����]��)
	void Bone_RotateY_All(float radian);

	

	//�R���X�g���N�^
	//�[���N���A
	Bone() :
		tall(0.0f)
	{
		this->center_of_Body = ML::Vec3(0, 0, 0);
		this->neck = nullptr;
		this->waist = nullptr;
		this->left_Sholder = nullptr;
		this->left_Elbow = nullptr;
		this->left_Wrist = nullptr;
		this->left_Hip = nullptr;
		this->left_Knee = nullptr;
		this->left_Ankle = nullptr;
		this->right_Sholder = nullptr;
		this->right_Elbow = nullptr;
		this->right_Wrist = nullptr;
		this->right_Hip = nullptr;
		this->right_Knee = nullptr;
		this->right_Ankle = nullptr;

		this->motions.clear();
		this->motionCnt = 0.0f;
		this->motion_Index = 0;
		this->now_Motion = "";
	}
	//���� : (�g��)
	Bone(const float& tall);

	//�f�X�g���N�^
	~Bone()
	{
		//�q�[�v�̈悩��J��
		delete this->waist;
		delete this->neck;
		delete this->left_Ankle;
		delete this->left_Elbow;
		delete this->left_Hip;
		delete this->left_Knee;
		delete this->left_Sholder;
		delete this->left_Wrist;
		delete this->right_Ankle;
		delete this->right_Elbow;
		delete this->right_Hip;
		delete this->right_Knee;
		delete this->right_Sholder;
		delete this->right_Wrist;

		this->motions.clear();
	}

};