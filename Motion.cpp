#include "Motion.h"

//�O���t�@�C������̃��[�V�����f�[�^����
Motion::Motion_Data Motion::Create_Step_From_File(const string& fn)
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
		//��������f�[�^��S���������ɂ���
		Motion::Replace_to_Key(key_Data);
		//�L�[�f�[�^�Ɣ�r���Ă����ꏊ�𔻕�
		if (Motion::motion_Data_Key.count(key_Data) >= 1)
		{
			auto& index = Motion::motion_Data_Key.find(key_Data);
			file >> md[(*index).second].x >> md[(*index).second].y >> md[(*index).second].z;
		}
		else if (key_Data.at(0) == 't')
		{
			flag = true;
		}
		else if (key_Data.at(0) == 'f')
		{
			flag = false;
		}
		else if (key_Data.at(0) == 'd')
		{
			file >> du;
		}
		//�ǂ���ɂ�����Ȃ��ꍇ�t�H�[�}�b�g�ɍ���Ȃ��̂Ń[���f�[�^��Ԃ�
		else
		{
			file.close();
			return Motion::Motion_Data();
		}
	}

	//�t�@�C���N���[�Y
	file.close();

	//�߂�l���쐬���ĕԂ�
	return Motion_Data(md[0], md[1],
		md[2], md[3], md[4],
		md[5], md[6], md[7],
		md[8], md[9], md[10],
		md[11], md[12], md[13],
		du, flag);
}

void Motion::Make_Motion(std::vector<Motion_Data>* result, const string& motion_Name)
{
	//���Ԗڂ����m�F����C���f�b�N�X
	int i = 0;
	while(true)
	{	
		//�O���t�@�C������̓ǂݍ���
		Motion::Motion_Data step = Motion::Create_Step_From_File(motion_Name + "/Step" + to_string(i) + ".txt");
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

void Motion::Replace_to_Key(string& s)
{
	for (unsigned int i = 0; i < s.length(); i++)
	{
		//�������ɒu��������
		if (s.at(i) >= 'A' && s.at(i) <= 'Z')
		{
			s.at(i) += 32;
		}
		//�F�𔭌�������A�����܂ł̃f�[�^���L�[�f�[�^�Ƃ���
		if (s.at(i) == ':')
		{
			s.resize(i);
		}
	}
}