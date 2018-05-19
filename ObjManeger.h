#pragma once
#pragma warning(disable : 4996)
#include "GameEngine_Ver3_7.h"
#include "Door.h"

//^-----------------------------------------------------------
//���̂̃A�h���X�l���Ǘ�����N���X
//--------------------------------------------------------------
class ObjManeger
{
private:
	//�ŏ��̃h�A�̃u���[�J�[�̃A�h���X�l
	std::vector<Breaker*> first_Door_Connencted_Breaker;
	//�u���[�J�[�̃A�h���X�l
	std::vector<Breaker*> door_Connencted_Breaker;
	//�h�A�̃A�h���X�l
	std::vector<Door*> door;

public:
	//�ŏ��̃u���[�J�[�̏�����
	void Init_First_Doors_Breaker(ML::Vec3);
	//�u���[�J�[�̏�����
	void Init_Doors_Breaker(ML::Vec3);

	//�ŏ��̃h�A�̏�����
	void Init_First_Door(ML::Vec3, LR);
	//�h�A�̏�����
	void Init_Door(ML::Vec3, LR);

	//�t�@�C�i���C�Y�{�҂��I�����鎞��ΌĂяo������
	//�q�[�v���������������
	void Finalize();

	//�R���X�g���N�^�E�f�X�g���N�^
	ObjManeger();
	~ObjManeger() {}
};