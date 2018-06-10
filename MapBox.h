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
 5 : �J�����@�k
 6 : �J�����@�k��
 7 : �J�����@��
 8 : �J�����@�쓌
 9 : �J�����@��
 10: �J�����@�쐼
 11: �J�����@��
 12: �J�����@�k��
 13: �R�[�i�[
*/
enum Type
{
	clear_Type = -1,
	air	= 0,
	box = 1,
	breaker = 2,
	player = 3,
	door = 4,
	camera_North = 5,
	camera_North_East = 6,
	camera_East	= 7,
	camera_South_East = 8,
	camera_South = 9,
	camera_South_West = 10,
	camera_West = 11,
	camera_North_West = 12,
	corner = 13
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