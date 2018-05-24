//-----------------------------------------------------------------------------
//�}�b�v�`�b�v�N���X
//-----------------------------------------------------------------------------
#include "MapBox.h"
#include "MyPG.h"

//��Ԃ�ǂݍ���	   (��Ԃ𐔒l��)
void Box::Type_Read(const int& type)
{
	//�����̏�Ԃɐݒ�
	this->type_ = (Type)type;
}
//��Ԃ�n��
int Box::Get_Type()
{
	return (int)this->type_;
}
//�ʒu��n��
ML::Vec3 Box::Get_Pos()
{
	return this->pos_;
}
//�����蔻���`��n��
ML::Box3D Box::Get_HitBase()
{
	return this->hitBase_;
}
//�`�b�v�T�C�Y��n��
float Box::Get_ChipSizeX()
{
	return this->chipSizeX_;
}
float Box::Get_ChipSizeY()
{
	return this->chipSizeY_;
}
float Box::Get_ChipSizeZ()
{
	return this->chipSizeZ_;
}
//�X�P�[�����O            (�T�C�Y��ύX������box�̎��:wal,floor,ceiling�̓�����������)
ML::Vec3 Box::Get_Scaling()
{
	ML::Vec3 scaling
	(
		this->chipSizeX_ / 100.0f,
		this->chipSizeY_ / 100.0f,
		this->chipSizeZ_ / 100.0f
	);
	return scaling;
}
//�����蔻��
bool Box::Map_Hit_Check(const ML::Box3D& hit_)
{
	auto me = this->hitBase_.OffsetCopy(this->pos_);
	return me.Hit(hit_);
}
