#pragma once
#include "myLib.h"

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

		//���O��ʁX�Ɏ����Ă���ver0.1
		/*ML::Vec3 waist_Rot;
		ML::Vec3 neck_Rot;		

		ML::Vec3 left_Sholder_Rot;
		ML::Vec3 left_Elbow_Rot;
		ML::Vec3 left_Wrist_Rot;

		ML::Vec3 right_Sholder_Rot;
		ML::Vec3 right_Elbow_Rot;
		ML::Vec3 right_Wrist_Rot;

		ML::Vec3 left_Hip_Rot;
		ML::Vec3 left_Knee_Rot;
		ML::Vec3 left_Ankle_Rot;
		
		ML::Vec3 right_Hip_Rot;
		ML::Vec3 right_Knee_Rot;
		ML::Vec3 right_Ankle_Rot;*/

		//��̉�]�ʃf�[�^�܂ł��ǂ蒅������
		const int duration;

		

		//�R���X�g���N�^
		//���� : (�e�֐߂����ǂ蒅����]�� ��A���A�����A���I�A�����A�E���A�E�I�A�E���A�����K�A���G�A������A�E���K�A�E�G�A�E����A���ǂ蒅���܂ł̎���)
		Motion_Data(const const ML::Vec3&, const const ML::Vec3&,
			const ML::Vec3&, const ML::Vec3&, const ML::Vec3&,
			const ML::Vec3&, const ML::Vec3&, const ML::Vec3&,
			const ML::Vec3&, const ML::Vec3&, const ML::Vec3&, 
			const ML::Vec3&, const ML::Vec3&, const ML::Vec3&, 
			const int&);
	};


	Motion_Data::Motion_Data(const ML::Vec3& neck, const ML::Vec3& waist,
		const ML::Vec3& l_sholder, const ML::Vec3& l_elbow, const ML::Vec3& l_wrist,
		const ML::Vec3& r_sholder, const ML::Vec3& r_elbow, const ML::Vec3& r_wrist,
		const ML::Vec3& l_hip, const ML::Vec3& l_knee, const ML::Vec3& l_ankle,
		const ML::Vec3& r_hip, const ML::Vec3& r_knee, const ML::Vec3& r_ankle,
		const int& du)
		: duration(du)
	{
		this->neck_Rot = neck;
		this->waist_Rot = waist;

		this->left_Sholder_Rot = l_sholder;
		this->left_Elbow_Rot = l_elbow;
		this->left_Wrist_Rot = l_wrist;

		this->right_Sholder_Rot = r_sholder;
		this->right_Elbow_Rot = r_elbow;
		this->right_Wrist_Rot = r_wrist;

		this->left_Hip_Rot = l_hip;
		this->left_Knee_Rot = l_knee;
		this->left_Ankle_Rot = l_ankle;
		
		this->right_Hip_Rot = r_hip;
		this->right_Knee_Rot = r_knee;
		this->right_Ankle_Rot = r_ankle;
	}
}