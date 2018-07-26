#include "Bone.h"

//��ɊO���t�@�C������̓ǂݍ��݂Ɏ�������\��
//���݊O���t�@�C���t�H�[�}�b�g�����܂��ĂȂ��̂�
//�͋Z�������l�ł̃��[�V�����f�[�^�𐶐�����(2018/07/25)


//--------------------------------------------------------------------------
//���[�V�����v���Z�b�g����
void Bone::Make_Interaction()
{
	//���[�V�����f�[�^�̃��F�N�^�[����
	std::vector<Motion::Motion_Data> interaction;

	//�e���[�V�����f�[�^�X�e�b�v�𐶐�
	//step0
	//�E�I���グ��
	Motion::Motion_Data step0(ML::Vec3(0, -20, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		2
	);

	Motion::Motion_Data step1(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3((-180), 0, 0), ML::Vec3((-10), 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);

	//step1
	//�I��L�΂��Ȃ������グ��
	//�����Ɍ���O�����
	Motion::Motion_Data step2(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-100), 0, 0), ML::Vec3((180), 0, 0), ML::Vec3((-20), 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);
	//step2, step3
	//�s���������߂�
	Motion::Motion_Data step3(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((100), 0, 0), ML::Vec3((-180), 0, 0), ML::Vec3((20), 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);

	Motion::Motion_Data step4(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, (0), 0), ML::Vec3((180), 0, 0), ML::Vec3((10), 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);

	Motion::Motion_Data step5(ML::Vec3(0, 20, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		2
	);

	//���F�N�^�[�o�^�y�у}�b�s���O
	interaction.push_back(step0);
	interaction.push_back(step1);
	interaction.push_back(step2);
	interaction.push_back(step3);
	interaction.push_back(step4);
	interaction.push_back(step5);

	this->motions.insert({ "InterAction",interaction });
}

void Bone::Make_Running()
{
	std::vector<Motion::Motion_Data> running;

	//step0
	//�I���Ȃ���
	Motion::Motion_Data step0(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3((-90), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3((-90), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-45), 0, 0), ML::Vec3((45), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((30), 0, 0), ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0),
		6
	);

	Motion::Motion_Data step1(ML::Vec3(0, 10, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		4, true
	);

	//step1
	//�E���A������o��
	Motion::Motion_Data step2(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-30), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((70), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((90), 0, 0), ML::Vec3(-45, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-100), 0, 0), ML::Vec3((60), 0, 0), ML::Vec3(0, 0, 0),
		10
	);
	//step2
	//�����ځA�E�����n
	Motion::Motion_Data step3(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-70), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-15), 0, 0), ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((25), 0, 0), ML::Vec3((-45), 0, 0), ML::Vec3(0, 0, 0),
		10
	);

	Motion::Motion_Data step4(ML::Vec3(0, -20, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		10
	);
	/*Motion::Motion_Data step5(ML::Vec3(0, -10, 0), ML::Vec3(0, 0, 0),
	ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
	ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
	ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
	ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
	2
	);*/

	//step3
	//�����A�E����o��
	Motion::Motion_Data step6(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((70), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-30), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-100), 0, 0), ML::Vec3((60), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((90), 0, 0), ML::Vec3((-45), 0, 0), ML::Vec3(0, 0, 0),
		10
	);
	//step4
	//�����ځA�������n
	Motion::Motion_Data step7(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-70), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((25), 0, 0), ML::Vec3((-45), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-15), 0, 0), ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0),
		10
	);

	Motion::Motion_Data step8(ML::Vec3(0, 10, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		4
	);

	//���F�N�^�[�o�^�y�у}�b�s���O
	running.push_back(step0);
	running.push_back(step1);
	running.push_back(step2);
	running.push_back(step3);
	running.push_back(step4);
	//running.push_back(step5);
	running.push_back(step6);
	running.push_back(step7);
	running.push_back(step8);

	this->motions.insert({ "Running",running });
}

//�������[�V����
void Bone::Make_Walking()
{
	std::vector<Motion::Motion_Data> walking;

	//step0
	//���I��O�ɏ����Ȃ���
	Motion::Motion_Data step0(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);

	//�㔼�g�̍������O��
	Motion::Motion_Data step1(ML::Vec3(0, 10, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0),
		5, true
	);

	//step2
	//���r�A�E�����O��
	//�E�r�A����������
	Motion::Motion_Data step2(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(20, 0, 0), ML::Vec3(40, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	//step3
	//�r���ځA�E�����n
	Motion::Motion_Data step3(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(20, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(-30, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	//step4
	//�㔼�g�E���O��
	Motion::Motion_Data step4(ML::Vec3(0, -20, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		10
	);

	//step5
	//�E�r�A�����O��
	//���r�A�E������
	Motion::Motion_Data step5(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(30, 0, 0), ML::Vec3(40, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-20, 0, 0), ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	//step6
	//�r����
	//�������n
	Motion::Motion_Data step6(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(20, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(-30, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	//step7
	//�㔼�g����
	//��������
	Motion::Motion_Data step7(ML::Vec3(0, 10, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	walking.push_back(step0);
	walking.push_back(step1);
	walking.push_back(step2);
	walking.push_back(step3);
	walking.push_back(step4);
	walking.push_back(step5);
	walking.push_back(step6);
	walking.push_back(step7);

	this->motions.insert({ "Walking", walking });
}