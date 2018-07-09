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
		Motion_Data(const ML::Vec3& waist, const ML::Vec3& neck,
			const ML::Vec3& l_sholder, const ML::Vec3& l_elbow, const ML::Vec3& l_wrist,
			const ML::Vec3& r_sholder, const ML::Vec3& r_elbow, const ML::Vec3& r_wrist,
			const ML::Vec3& l_hip, const ML::Vec3& l_knee, const ML::Vec3& l_ankle,
			const ML::Vec3& r_hip, const ML::Vec3& r_knee, const ML::Vec3& r_ankle,
			const int& du)
			: duration(du)
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
	}; 

	
	////��������Ă��郂�[�V�����f�[�^�̏���������m�F����
	//void Set_Upper_Lower_Limits(const Joint* j, Motion_Data* md, const int index)
	//{
	//	//�������̕s��A�N�Z�X��h�����߂�
	//	if (index >= JOINT_ON_HUMAN)
	//	{
	//		return;
	//	}
	//	//��]�ʂ��������܂ꂽ���̂̂ݔ�ׂ�
	//	if (md->joint[index].Is_Zero_Vec())
	//	{
	//		return;
	//	}

	//	//�ő�l�ŏ��l�𒴂���ꍇ�͏㏑������
	//	//X���
	//	if (md->joint[index].x < 0.0f)
	//	{

	//		if (md->joint[index].x < j->Get_Limit_X_Minus())
	//		{
	//			md->joint[index].x = j->Get_Limit_X_Minus();
	//		}
	//	}
	//	else if (md->joint[index].x > 0.0f)
	//	{
	//		if (md->joint[index].x > j->Get_Limit_X_Plus())
	//		{
	//			md->joint[index].x = j->Get_Limit_X_Plus();
	//		}
	//	}

	//	//Y���
	//	if (md->joint[index].y < 0.0f)
	//	{

	//		if (md->joint[index].y < j->Get_Limit_Y_Minus())
	//		{
	//			md->joint[index].y = j->Get_Limit_Y_Minus();
	//		}
	//	}
	//	else if (md->joint[index].y > 0.0f)
	//	{
	//		if (md->joint[index].y > j->Get_Limit_Y_Plus())
	//		{
	//			md->joint[index].y = j->Get_Limit_Y_Plus();
	//		}
	//	}

	//	//Z���
	//	if (md->joint[index].z < 0.0f)
	//	{

	//		if (md->joint[index].z < j->Get_Limit_Z_Minus())
	//		{
	//			md->joint[index].z = j->Get_Limit_Z_Minus();
	//		}
	//	}
	//	else if (md->joint[index].z > 0.0f)
	//	{
	//		if (md->joint[index].z > j->Get_Limit_Z_Plus())
	//		{
	//			md->joint[index].z = j->Get_Limit_Z_Plus();
	//		}
	//	
	//	}
	//}

}