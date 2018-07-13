#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"
#include "Breaker.h"

//�h�A���J��������
enum LR
{
	CLEAR_LR = -1,//�s��(�N���A)
	Left = 0,
	Right = 1,
};
class Door
{
private:
	//���̈ʒu
	ML::Vec3 pos;
	//�h�A�̊J�t���O
	bool openFlag;
	//�h�A�̂����蔻��͈�
	ML::Box3D hitBase;
	//�ւ���Ă���u���[�J�[
	std::vector<Breaker*> cunnected_Breaker;
	//�h�A���J��������
	LR open_Angle;
	//����
	int timeCnt;
	
public:	
	//�h�A���J���鏈�� 
	void Door_Open();
	//�v���C���Ƃ̂����蔻�� ���� : (�v���C���̂����蔻��͈�)
	bool Player_Hit_the_Door(const ML::Box3D& hit);
	//��Ԃ�Ԃ��֐��Atrue��Ԃ����炻�̌㓖���蔻��ƊJ���鏈���͂��Ȃ�
	bool Get_State();
	//�ʒu�����炤�֐�
	ML::Vec3 Get_Pos();
	//�J�������������炤
	LR Get_Angle();
	//�h�A�����S�ɊJ���ꂽ����Ԃ��֐�
	bool Is_Opened_Over();
	//�t�F�[�Y�ڍs���m
	bool Is_Phase_Offset();

	//�Ȃ����Ă���u���[�J�[�̐��m�F
	int How_Many_Breaker_Be_Cunnected();


	//�R���X�g���N�^�E�f�X�g���N�^
	//�[���N���A
	Door();
	//�ʒu�w�� num��1,3�ǂ�����������ׂ�
	Door(ML::Vec3, std::vector<Breaker*> b, LR a);

	~Door(){}
};