#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�}�b�v�`�b�v�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
//�ǂ̃T�C�Y�w��
#define chipX 150
#define chipY 300
#define chipZ 150

//�}�b�v�`�b�v�̏��
/*
-1 : �Ȃ�
 0 : ��C(�}�b�v�Ȃ�)
 1 : ��
 2 : �Ւf�@�k����
 3 : �Ւf�@�����
 4 : �v���C�������ʒu
 5 : ��
 6 : �J�����@�k
 7 : �J�����@�k��
 8 : �J�����@��
 9 : �J�����@�쓌
 10 : �J�����@��
 11: �J�����@�쐼
 12: �J�����@��
 13: �J�����@�k��
 14: �R�[�i�[
 15: �u���[�J�[�����v
 16: �e�[�u��
*/

enum Type
{
	clear_Type = -1,
	air	= 0,
	box = 1,
	breakerN = 2,
	breakerS = 3,
	player = 4,
	door = 5,
	camera_North = 6,
	camera_North_East = 7,
	camera_East	= 8,
	camera_South_East = 9,
	camera_South = 10,
	camera_South_West = 11,
	camera_West = 12,
	camera_North_West = 13,
	corner = 14,
	lamp = 15,
	table = 16
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
	//
	int cornerNum;
public:
	//Box�N���X�̃����o�ϐ��̏�����(�R���X�g���N�^) 
	Box()
		: pos_(0.0f, 0.0f, 0.0f)
		, hitBase_(0, 0, 0, 0, 0, 0)
		, type_(Type::clear_Type)
		, chipSizeX_(chipX)
		, chipSizeY_(chipY)
		, chipSizeZ_(chipZ)
		, cornerNum(-1)
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
	//�����蔻��
	bool Map_Hit_Check(const ML::Box3D& hit_);
	//�Ȃ���p�̔ԍ���ǂݍ��񂾏��ɐݒ�
	void Ini_Corner_Num(const int num);
};