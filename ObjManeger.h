#pragma once
#pragma warning(disable : 4996)
#include "GameEngine_Ver3_7.h"
#include "Door.h"
#include "MapBox.h"
#include "Tablet.h"
#include "Routine.h"

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
	//�^�u���b�g�̃A�h���X�l
	Tablet* tab;
	//
	Routine* rou;


	//�ŏ��̃u���[�J�[�̏�����
	void Init_First_Doors_Breaker(ML::Vec3);
	//�u���[�J�[�̏�����
	void Init_Doors_Breaker(ML::Vec3);

	//�ŏ��̃h�A�̏�����
	void Init_First_Door(ML::Vec3, LR);
	//�h�A�̏�����
	void Init_Door(ML::Vec3, LR);
public:
	//�u���[�J�[�����
	void Create_Breaker(ML::Vec3);
	//�h�A�����
	void Create_Door(ML::Vec3, LR);
	//�J�����̐���
	void Create_Camera(ML::Vec3, Type);
	//�^�u���b�g�̃A�h���X�l��Ԃ��֐�(�v���C���̏������ɂ̂ݎg������)
	Tablet* Create_Tablet();

	Routine* Create_Routine();

	void Push_Back_Conner(const ML::Vec3&, const int&);

	void Set_Relationship();

	//�t�@�C�i���C�Y,�{�҂��I�����鎞��ΌĂяo������
	//�q�[�v���������������
	void Finalize();

	//�R���X�g���N�^�E�f�X�g���N�^
	ObjManeger();
	~ObjManeger() {}
};