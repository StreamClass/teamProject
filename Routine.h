#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//�}�b�v���̃R�[�i�[�̐�
#define corners 28
//���݂̍ő��I������
//4�����ő�
#define choices 4

class Routine
{
private:
	//���W�Ƃ��ړ��ʁA��`�̓G�l�~�[�������Ă邩��v��Ȃ����ȁ`�`�`
	//���W
	ML::Vec3 pos;
	//�ړ���
	ML::Vec3 moveVec;
	//�����`
	ML::Box3D hitBase;
	//�ŏI�t�F�[�Y�Ȃ̂����m�F����t���O
	bool final_Phase;

	//���݂̃R�[�i�[
	int now;
	//�I�΂ꂽ�ړ���
	int target;
	//�e�R�[�i�[�̍��W�z��
	ML::Vec3 cornerPos[corners];
	//�e�R�[�i�[�����ړ��\�ȃR�[�i�[
	//�I�����̏��Ȃ��R�[�i�[�ɂ�-1�̗v�f�����v�f���𖄂߂�
	//				[�R�[�i�[][�ő�I������]
	int choiceCorner[corners][choices];
	//�ړI�n�����p��`�̔���
	bool RelationShip_XP(int num_ , int);
	//	 
	bool RelationShip_XM(int num_, int);
	//	 
	bool RelationShip_ZP(int num_, int);
	//
	bool RelationShip_ZM(int num_, int);
public:
	//�R���X�g���N�^
	Routine();
	//�Q�b�^�[
	int Get_Now();
	//�߂��R�[�i�[��ڎw���Ĉړ�����
	ML::Vec3 Move(const ML::Vec3& );
	//�������R�[�i�[�ɓ��B�������̔���
	bool Judge(const ML::Box3D&, const ML::Vec3&);
	//�����̂���R�[�i�[����s����R�[�i�[��I������
	void Choice(const int& now_);
	//�R�[�i�[�̎���`�̑����G�l�~�[�Ƃ̐ڐG�ɉ����Ď����̔ԍ���Ԃ�
	int Recieved(const ML::Box3D&, const ML::Vec3&);
	//
	void Set_CornerPos(const ML::Vec3& pos_, const int& num_);
	//�ړI�n�̌����p
	void Initialize_RelationShip(int num);

	//�t�F�[�Y�m�F
	bool Is_Final_Phase();


	//�ŏI�t�F�[�Y�Ɉڍs
	void Set_Final_Phase();
};