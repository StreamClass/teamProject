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
	//���[�`�����[�N�V�X�e���̃A�h���X�l
	Routine* rou;


	//�ŏ��̃u���[�J�[�̏�����
	//���� : (�u���[�J�[�̈ʒu,�u���[�J�[�̕\������)
	void Init_First_Doors_Breaker(ML::Vec3, int);
	//�u���[�J�[�̏�����
	//���� : (�u���[�J�[�̈ʒu,�u���[�J�[�̕\������)
	void Init_Doors_Breaker(ML::Vec3, int);

	//�ŏ��̃h�A�̏�����
	//���� : (�h�A�̈ʒu,�h�A���J������)
	void Init_First_Door(ML::Vec3, LR);
	//�h�A�̏�����
	//���� : (�h�A�̈ʒu,�h�A���J������)
	void Init_Door(ML::Vec3, LR);
public:
	//�u���[�J�[�����
	//���� : (�u���[�J�[�̈ʒu,�u���[�J�[�̕\������)
	void Create_Breaker(ML::Vec3, Type);
	//�h�A�����
	//���� : (�h�A�̈ʒu,�h�A���J������)
	void Create_Door(ML::Vec3, LR);
	//�J�����̐���
	//���� : (�J�����̈ʒu,�J�����������Ă������)
	void Create_Camera(ML::Vec3, Type);
	//�����v�̐���
	void Create_Lamp(ML::Vec3, ML::Box3D);
	//�^�u���b�g�̃A�h���X�l��Ԃ��֐�(�v���C���̏������ɂ̂ݎg������)
	Tablet* Create_Tablet();
	//���[�`�����[�N�V�X�e������
	Routine* Create_Routine();
	//���[�`�����[�N�V�X�e���Ɋe�R�[�i�[��o�^
	//���� : (�R�[�i�[�̈ʒu,�R�[�i�[�̔ԍ�)
	void Push_Back_Conner(const ML::Vec3&, const int&);
	//�R�[�i�[���m�̊֌W�����\�z
	void Set_Relationship();
	//�G�l�~�[���ŏI�t�F�[�Y�ɂ���
	void Game_Be_Final_Phase();

	//�ق��̃^�X�N����^�u���b�g�ɃA�N�Z�X���邽�߂ɃA�h���X��Ԃ����\�b�h
	const Tablet* Get_Tablet() const;

	//�t�@�C�i���C�Y,�{�҂��I�����鎞��ΌĂяo������
	//�q�[�v���������������
	void Finalize();

	//�R���X�g���N�^�E�f�X�g���N�^
	ObjManeger();
	~ObjManeger() {}
};