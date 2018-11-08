#pragma once
#include "myLib.h"
#include "Joint.h"

#define JOINT_ON_HUMAN 14

namespace Motion
{
	/*
	�e�֐߂ɔԍ������蓖�ĂĊǗ�����
	0 = ��
	1 = ��
	2 = ����
	3 = ���I
	4 = �����
	5 = �E��
	6 = �E�I
	7 =  �E���
	8 = �����K
	9 = ���G
	10 = ������
	11 = �E���K
	12 = �E�G
	13 = �E����
	*/
	enum Joint_Name
	{
		Waist			= 0,
		Neck			= 1,
		Left_Sholder	= 2,
		Left_Elbow		=3,
		Left_Wrist		=4,
		Right_Sholder	=5,
		Right_Elbow		=6,
		Right_Wrist		=7,
		Left_Hip		=8,
		Left_Knee		= 9,
		Left_Ankle		= 10,
		Right_Hip		= 11,
		Right_Knee		= 12,
		Right_Ankle		= 13,
	};
	struct Motion_Data
	{		
		//�e���̉�]�f�[�^���x�N�g���ɐ錾
		ML::Vec3 joint[JOINT_ON_HUMAN];
		

		//��̉�]�ʃf�[�^�܂ł��ǂ蒅������
		const int duration;
		//repeat flag�������Ă��鎞�A���Ă���ꏊ�m�F�p
		const bool repeat_Point;

		//�R���X�g���N�^(�����s���t���O��0�Ԃɂ��Ȃ�����)
		/*���� : (�e�֐߂����ǂ蒅����]�� ��A���A
		�����A���I�A�����A
		�E���A�E�I�A�E���A
		�����K�A���G�A������A
		�E���K�A�E�G�A�E����A
		���ǂ蒅���܂ł̎��ԁA�����s����T���t���O)*/
		Motion_Data(const ML::Vec3& waist, const ML::Vec3& neck,
			const ML::Vec3& l_sholder, const ML::Vec3& l_elbow, const ML::Vec3& l_wrist,
			const ML::Vec3& r_sholder, const ML::Vec3& r_elbow, const ML::Vec3& r_wrist,
			const ML::Vec3& l_hip, const ML::Vec3& l_knee, const ML::Vec3& l_ankle,
			const ML::Vec3& r_hip, const ML::Vec3& r_knee, const ML::Vec3& r_ankle,
			const int& du, bool r = false)
			: duration(du),
			repeat_Point(r)
		{
			this->joint[Joint_Name::Waist] = waist;
			this->joint[Joint_Name::Neck] = neck;

			this->joint[Joint_Name::Left_Sholder] = l_sholder;
			this->joint[Joint_Name::Left_Elbow] = l_elbow;
			this->joint[Joint_Name::Left_Wrist] = l_wrist;

			this->joint[Joint_Name::Right_Sholder] = r_sholder;
			this->joint[Joint_Name::Right_Elbow] = r_elbow;
			this->joint[Joint_Name::Right_Wrist] = r_wrist;

			this->joint[Joint_Name::Left_Hip] = l_hip;
			this->joint[Joint_Name::Left_Knee] = l_knee;
			this->joint[Joint_Name::Left_Ankle] = l_ankle;

			this->joint[Joint_Name::Right_Hip] = r_hip;
			this->joint[Joint_Name::Right_Knee] = r_knee;
			this->joint[Joint_Name::Right_Ankle] = r_ankle;
		}

		//�[���N���A
		Motion_Data()
			: duration(0)
			, repeat_Point(false)
		{
			for (int i = 0; i < JOINT_ON_HUMAN; i++)
			{
				this->joint[i] = ML::Vec3(0, 0, 0);
			}
		}

		//��]�s���������Ă��̕���]����
		void Rotation_Matrix(ML::Mat4x4* matR)
		{
			for (int i =0; i<JOINT_ON_HUMAN; i++)
			{
				this->joint[i] = matR->TransformNormal(this->joint[i]);
			}
		}

		//Y����]�ɑ΂��鑫���Z
		Motion_Data& operator +(const float& val)
		{
			for (auto& j : this->joint)
			{
				if (!j.Is_Zero_Vec())
				{
					j.y += val;
				}
			}
			return *this;
		}

		Motion_Data& operator +=(const float& val)
		{
			return *this + val;
		}
		
		//�����f�[�^�����m�F
		bool Is_Zero_Data()
		{
			//�[���t���[���s���Ȃ�
			//�����f�[�^�ƔF������
			return this->duration == 0;
		}
	}; 
	const std::map<string,Joint_Name> motion_Data_Key = 
	{
		{"waist",	Waist },
		{ "neck",	Neck },
		{ "lsholder",Left_Sholder },
		{ "lelbow",	Left_Elbow },
		{ "lwrist",	Left_Wrist },
		{ "rsholder",Right_Sholder },
		{ "relbow",	Right_Elbow },
		{ "rwrist",	Right_Wrist },
		{ "lhip",	Left_Hip },
		{ "lknee",	Left_Knee },
		{ "lankle",	Left_Ankle },
		{ "rhip",	Right_Hip },
		{ "rknee",	Right_Knee },
		{ "rankle",	Right_Ankle }
	};
	//�O���t�@�C������̃��[�V�����f�[�^����
	//���� : (�t�@�C���� + .txt)
	Motion_Data Create_Step_From_File(const string&);

	void Make_Motion(std::vector<Motion_Data>* result, const string& motion_Name);

	void Replace_to_Key(string& s);
}