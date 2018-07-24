//-----------------------------------------------------------------------------
//�}�b�v�`�b�v�N���X
//-----------------------------------------------------------------------------
#include "MapBox.h"
#include "MyPG.h"

//Box�N���X�̃����o�ϐ��̏�����(�R���X�g���N�^) 
Box::Box()
	: pos_(0.0f, 0.0f, 0.0f)
	, hitBase_(0, 0, 0, 0, 0, 0)
	, type_(Type::clear_Type)
	, chipSizeX_(chipX)
	, chipSizeY_(chipY)
	, chipSizeZ_(chipZ)
	, cornerNum(-1)
	, meshName_("")
{}

//�ǂ̍��W�E�����蔻���`�̎w��
//�����F(���W , �����蔻���`)
Box::Box(ML::Vec3 pos, ML::Box3D hitBase)
	: pos_(pos)
	, hitBase_(hitBase)
	, type_(Type::clear_Type)
	, chipSizeX_(chipX)
	, chipSizeY_(chipY)
	, chipSizeZ_(chipZ)
	, cornerNum(-1)
	, meshName_("")
{}

//���E�V��̍��W�E�����蔻���`�̎w��
//���� : (�`�b�v�T�C�Y , ���W , �����蔻��)
Box::Box(ML::Vec3 chipSize, ML::Vec3 pos, ML::Box3D hitBase)
	: pos_(pos)
	, hitBase_(hitBase)
	, chipSizeX_(chipSize.x)
	, chipSizeY_(chipSize.y)
	, chipSizeZ_(chipSize.z)
	, cornerNum(-1)
	, meshName_("")
{}

//�������
Box::~Box()
{
	if (this->meshName_ != "")//���b�V�������w�肳��Ă���ꍇ
	{
		//�ێ����Ă���摜�����
		DG::Mesh_Erase(this->meshName_);
	}
}

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

//�X�P�[�����O
ML::Vec3 Box::Get_Scaling()
{
	ML::Vec3 scaling
	(
		//�T�C�Y���w��(��l100)
		this->chipSizeX_,
		this->chipSizeY_,
		this->chipSizeZ_
	);
	return scaling;
}

//�����蔻��
bool Box::Map_Hit_Check(const ML::Box3D& hit_)
{
	auto me = this->hitBase_.OffsetCopy(this->pos_);
	return me.Hit(hit_);
}

//�Ȃ���p�̔ԍ���ǂݍ��񂾏��ɐݒ�
void Box::Ini_Corner_Num(const int num)
{
	this->cornerNum = num;
}

//���b�V������ݒ�
void Box::Set_MeshName(const string meshName)
{
	this->meshName_ = meshName;
}

//3D�`�揈��
void Box::Render3D()
{
	//���b�V�������ݒ肳��Ă��Ȃ����
	if (this->meshName_ == "")
	{
		//�I�u�W�F�N�g�ł͂Ȃ����ߏI��
		return;
	}
	//���W�ƃT�C�Y��ݒ�
	ML::Mat4x4 matT, matS;
	matS.Scaling(this->Get_Scaling());
	matT.Translation(this->pos_);
	//���[���h���W�ɐݒ�
	DG::EffectState().param.matWorld = matS * matT;
	//���b�V�����ɍ��킹�ĕ`��
	DG::Mesh_Draw(this->meshName_);
}