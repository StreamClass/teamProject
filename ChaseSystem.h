#pragma once
#include "GameEngine_Ver3_7.h"

//�G�l�~�[�̎���ƒǐՃ��[�g�����߂�N���X

class ChaseSystem
{
private:
	//�v���C���̈ړ��ʒu��ۑ�����
	std::vector<ML::Vec3> player_Route;
	//�Z���T�[�̂����蔻���`
	ML::Box3D sensor;
	//�V�X�e������(false : ���� true : �ǐ�)
	bool systemFlag;
	//���ݖړI�n��T���ԍ�
	unsigned int destination;

	//�v���C���̈ʒu�����[�g�ɓo�^
	void PushBack_Route(const ML::Vec3& pos);
	void Shift_to_Routine();
public:
	//�����蔻��(���� : �v���C���̂����蔻���`,�v���C��̈ʒu,�G�l�~�[�̌��݈ʒu,�G�l�~�[�̃A���O��)
	void SensorCheck(const ML::Box3D& hit,const ML::Vec3& plpos, const ML::Vec3& pos, const float& angle);
	//���̒ǐՏꏊ��Ԃ�
	ML::Vec3 NextRoute();
	//�V�X�e�����[�h���m�F����֐�
	bool Is_Chase_Mode();



	//�R���X�g���N�^�E�f�X�g���N�^
	ChaseSystem() :
		sensor(-300, -100, -300, 600, 200, 600),
		systemFlag(false),
		destination(0)
	{
		this->player_Route.clear();
	}
	~ChaseSystem(){}
};