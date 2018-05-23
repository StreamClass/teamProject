#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�}�b�v�`�b�v�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
//�ǂ̃T�C�Y�w��
#define chipX 100
#define chipY 300
#define chipZ 100

//�}�b�v�`�b�v�̏��
/*
-1 : �Ȃ�
 0 : ��C(�}�b�v�Ȃ�)
 1 : ��
 2 : �Ւf�@
 3 : �v���C�������ʒu
 4 : ��
*/
enum Type
{
	clear_Type = -1,
	air	= 0,
	box = 1,
	breaker = 2,
	player = 3,
	door = 4
};
class Box
{
private:
	//�ǂ̍��W
	ML::Vec3	pos_;
	//�����蔻���`
	ML::Box3D	hitBase_;
	//���
	Type		type_;
	//X�����̃`�b�v�T�C�Y
	float		chipSizeX_;
	//Y�����̃`�b�v�T�C�Y
	float		chipSizeY_;
	//Z�����̃`�b�v�T�C�Y
	float		chipSizeZ_;
public:
	//Box�N���X�̃����o�ϐ��̏�����(�R���X�g���N�^) 
	Box()
		: pos_(0.0f, 0.0f, 0.0f)
		, hitBase_(0, 0, 0, 0, 0, 0)
		, type_(Type::clear_Type)
		, chipSizeX_(chipX)
		, chipSizeY_(chipY)
		, chipSizeZ_(chipZ)
	{}
	//�ǂ̍��W�E�����蔻���`�̎w��
	//�����F(���W , �����蔻���`)
	Box(ML::Vec3 pos,ML::Box3D hitBase)
		: pos_(pos)
		, hitBase_(hitBase)
		, type_(Type::clear_Type)
		, chipSizeX_(chipX)
		, chipSizeY_(chipY)
		, chipSizeZ_(chipZ)
	{}
	//���E�V��̍��W�E�����蔻���`�̎w��
	//���� : (ML::vec3 �`�b�v�T�C�Y , ���W , �����蔻���`)
	Box(ML::Vec3 chipSize, ML::Vec3 pos, ML::Box3D hitBase)
		: pos_(pos)
		, hitBase_(hitBase)
		, chipSizeX_(chipSize.x)
		, chipSizeY_(chipSize.y)
		, chipSizeZ_(chipSize.z)
	{}
	
	//��ԓǂݎ��
	void Type_Read(const int& type);
	//��Ԃ�n��
	int Get_Type();
	//�e�`�b�v�T�C�Y��n��
	float Get_ChipSizeX();
	float Get_ChipSizeY();
	float Get_ChipSizeZ();
	//���ݍ��W��n��
	ML::Vec3 Get_Pos();
	//�����蔻���`��n��
	ML::Box3D Get_HitBase();
	//�X�P�[�����O
	ML::Vec3 Get_Scaling();
};