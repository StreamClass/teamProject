#include "Motion.h"

//�O���t�@�C������̃��[�V�����f�[�^����
Motion::Motion_Data& Motion::Create_Step_From_File(const string& fn)
{
	//�O���t�@�C������̃��[�V�����f�[�^����

	ifstream file("./data/Motion_Data/" + fn);

	//�ǂݍ��ݎ��s�̎��͖����f�[�^��Ԃ�
	if (file.fail())
	{
		return Motion::Motion_Data();
	}


	
}