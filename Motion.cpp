#include "Motion.h"

//�O���t�@�C������̃��[�V�����f�[�^����
Motion::Motion_Data& Motion::Create_Step_From_File(const string& fn)
{
	//�O���t�@�C������̃��[�V�����f�[�^����
	//�t�@�C���I�[�v��
	ifstream file("./data/Motion_Data/" + fn);

	//�ǂݍ��ݎ��s�̎��͖����f�[�^��Ԃ�
	if (file.fail())
	{
		return Motion::Motion_Data();
	}

	//�i�[����ꏊ�錾
	ML::Vec3 md[JOINT_ON_HUMAN];
	int du;
	bool flag;

	while (!file.eof())
	{
		//�t�H�[�}�b�g�͏��Ԓʂ�ɍ���Ă邯��
		//�������̂��߂�if���Ŋm�F����
		string key_Data;
		
		file >> key_Data;
		//��
		if (key_Data.at(0) == 'W' || key_Data.at(0) == 'w')
		{
			file >> md[0].x >> md[0].y >> md[0].z;
		}
		//��
		else if (key_Data.at(0) == 'N' || key_Data.at(0) == 'n')
		{
			file >> md[1].x >> md[1].y >> md[1].z;
		}
		//�����g
		else if (key_Data.at(0) == 'L' || key_Data.at(0) == 'l')
		{
			//����
			if (key_Data.at(1) == 'S' || key_Data.at(1) == 's')
			{
				file >> md[2].x >> md[2].y >> md[2].z;
			}
			//���I
			if (key_Data.at(1) == 'E' || key_Data.at(1) == 'e')
			{
				file >> md[3].x >> md[3].y >> md[3].z;
			}
			//�����
			if (key_Data.at(1) == 'W' || key_Data.at(1) == 'w')
			{
				file >> md[4].x >> md[4].y >> md[4].z;
			}
			//�����K
			if (key_Data.at(1) == 'H' || key_Data.at(1) == 'h')
			{
				file >> md[8].x >> md[8].y >> md[8].z;
			}
			//���G
			if (key_Data.at(1) == 'K' || key_Data.at(1) == 'k')
			{
				file >> md[9].x >> md[9].y >> md[9].z;
			}
			// ������
			if (key_Data.at(1) == 'A' || key_Data.at(1) == 'a')
			{
				file >> md[10].x >> md[10].y >> md[10].z;
			}
		}
		//�E���g
		else if (key_Data.at(0) == 'R' || key_Data.at(0) == 'r')
		{
			//�E��
			if (key_Data.at(1) == 'S' || key_Data.at(1) == 's')
			{
				file >> md[5].x >> md[5].y >> md[5].z;
			}
			//�E�I
			if (key_Data.at(1) == 'E' || key_Data.at(1) == 'e')
			{
				file >> md[6].x >> md[6].y >> md[6].z;
			}
			// �E���
			if (key_Data.at(1) == 'W' || key_Data.at(1) == 'w')
			{
				file >> md[7].x >> md[7].y >> md[7].z;
			}
			// �E���K
			if (key_Data.at(1) == 'H' || key_Data.at(1) == 'h')
			{
				file >> md[11].x >> md[11].y >> md[11].z;
			}
			// �E�G
			if (key_Data.at(1) == 'K' || key_Data.at(1) == 'k')
			{
				file >> md[12].x >> md[12].y >> md[12].z;
			}
			// �E����
			if (key_Data.at(1) == 'A' || key_Data.at(1) == 'a')
			{
				file >> md[13].x >> md[13].y >> md[13].z;
			}
		}
		//�t���[����
		else if (key_Data.at(0) == 'D' || key_Data.at(0) == 'd')
		{
			file >> du;
		}
		//repeat flag
		else if (key_Data.at(0) == 'T' || key_Data.at(0) == 't')
		{
			flag = true;
		}
		else
		{
			flag = false;
		}
	}

	//�t�@�C���N���[�Y
	file.close();

	//�߂�l���쐬���ĕԂ�
	return Motion::Motion_Data(md[0], md[1],
		md[2], md[3], md[4],
		md[5], md[6], md[7],
		md[8], md[9], md[10],
		md[11], md[12], md[13],
		du, flag
	);
}

void Motion::Make_Motion(std::vector<Motion_Data>* result, const string& motion_Name)
{
	//���Ԗڂ����m�F����C���f�b�N�X
	int i = 0;
	while(true)
	{	
		//�O���t�@�C������̓ǂݍ���
		Motion::Motion_Data step = Motion::Create_Step_From_File(motion_Name + "/step" + to_string(i) + ".txt");
		//�����f�[�^�Ȃ烋�[�v�𔲂���
		if (step.Is_Zero_Data())
		{
			break;
		}
		else
		{
			//�����f�[�^�ł͂Ȃ����f�[�^�o�^
			result->push_back(step);
		}
		//�C���f�b�N�X�㏸
		i++;
	}


}