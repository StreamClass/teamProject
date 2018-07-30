#pragma once
#include "myLib.h"
#include "Joint.h"

#define JOINT_ON_HUMAN 14

namespace Motion
{
	struct Motion_Data
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
			this->joint[0] = waist;
			this->joint[1] = neck;

			this->joint[2] = l_sholder;
			this->joint[3] = l_elbow;
			this->joint[4] = l_wrist;

			this->joint[5] = r_sholder;
			this->joint[6] = r_elbow;
			this->joint[7] = r_wrist;

			this->joint[8] = l_hip;
			this->joint[9] = l_knee;
			this->joint[10] = l_ankle;

			this->joint[11] = r_hip;
			this->joint[12] = r_knee;
			this->joint[13] = r_ankle;
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
			return this->duration == 0 ? true : false;
		}
	}; 

	//�O���t�@�C������̃��[�V�����f�[�^����
	//���� : (�t�@�C���� + .txt)
	Motion_Data Create_Step_From_File(const string&);

	void Make_Motion(std::vector<Motion_Data>* result, const string& motion_Name);
}