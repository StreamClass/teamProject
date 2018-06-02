#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//�}�b�v���̃R�[�i�[�̐�
#define corners 24
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
public:
	//�R���X�g���N�^
	Routine();
	//�߂��R�[�i�[��ڎw���Ĉړ�����
	void Move();
	//�������R�[�i�[�ɓ��B�������̔���
	bool Judge();
	//�����̂���R�[�i�[����s����R�[�i�[��I������
	int Choice(const int& now_);
	//�R�[�i�[�̎���`�̑����G�l�~�[�Ƃ̐ڐG�ɉ����Ď����̔ԍ���Ԃ�
	int Recieved();
};