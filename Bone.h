#pragma once
#include "Joint.h"
#include "Motion.h"

//���i�N���X

//idle�͒���
//�����̐��ʂ�Z+

//���[�V�����f�[�^�Ɠ������֐߂�ԍ��Ŋ��蓖�ĂĊǗ�����

class Bone
{
private:
	//�̑S�̂̒��S
	ML::Vec3 center_of_Body;
	//�w�̍���
	const float tall;

	//�S�֐߂̃A�h���X�l
	Joint* joint[JOINT_ON_HUMAN];	

	//�S�̂�Y��]�l
	float All_RotY;
	//���ʃx�N�g��(�������̎���Z+)
	ML::Vec3 front_Vec;

	//���[�V�����p�J�E���^
	int motionCnt;
	//���F�N�^�[��[]���Z�q�ŗ��p���鎯�ʎq
	unsigned int motion_Index;
	//repeat Point��ۑ�����
	unsigned int repeat_Index;
	//�����̃��[�V�����Ȃ̂����m�F����L�[
	string now_Motion;
	//�\�񂩂��Ă鎟�ɓ������[�V�������m�F����L�[
	string next_Motion;
	//���[�V����������I����Ă��J��Ԃ��t���O(���郂�[�V�����Ƃ��ɗ��p)
	bool repeat_Flag;

	//���[�V�����f�[�^�̃}�b�s���O	
	std::map < string, std::vector<Motion::Motion_Data> > motions;


	//���\�b�h
	//�����ɐ���
	//���� : (���������ŏI��点�邩�ǂ����̃t���O)
	void To_Standing(bool ASAP) const;

	//�\�񃂁[�V�����ɂ��邩��̏�Ԃɂ���
	bool Next_Motion_or_None();
	//�A���s�������Ԃ����m�F����
	bool Is_Ok_to_Repeat();
public:
	//�Q�b�^�[
	ML::Vec3 Get_Center() const;

	//���W�ړ�
	//���� : (�ړ�����x�N�g��)
	void Moving(const ML::Vec3& vec);

	//�̑S�̂�Y����]���s��(�����̕���������)
	//�ق��̎����K�v�ɉ����Ēǉ��\������(2018/07/04)
	//���� : (�U������������p�x)
	void Bone_RotateY_All(const float& radian);

	//���[�V�������{�[���ɓo�^
	//���� : (���[�V�����f�[�^���W�߂����F�N�^�[�A�V�������[�V�����̖��O)
	void Registrate_Motion(const std::vector<Motion::Motion_Data>& d, const string& motions_Name);
	//���̃��[�V�����\��
	//���� : (�\�񂷂郂�[�V�����̖��O)
	void Set_Next_Motion(const string& next);
	//�A���s���t���O�𗧂Ă�
	void Repeat_Now_Motioin();
	//�A�j���[�V�����A�b�v�f�[�g
	void UpDate();
	//�A�j���[�V���������_�����O
	void Render() const;	


	//�R���X�g���N�^
	//�[���N���A
	Bone() :
		tall(0.0f)
	{
		this->center_of_Body = ML::Vec3(0, 0, 0);
		this->All_RotY = 0.0f;		

		for (int i = 0; i < JOINT_ON_HUMAN; i++)
		{
			this->joint[i] = nullptr;
		}

		this->motions.clear();
		this->motionCnt = 0;
		this->motion_Index = 0;
		this->repeat_Index = 0;
		this->front_Vec = ML::Vec3(0, 0, 0);
		this->now_Motion = "";
		this->next_Motion = "";
		this->repeat_Flag = false;
	}
	//���� : (�g��)
	Bone(const float& tall, const string& owner);

	//�f�X�g���N�^
	~Bone()
	{		

		for (int i = 0; i < JOINT_ON_HUMAN; i++)
		{
			delete this->joint[i];
		}

		this->motions.clear();
	}

};