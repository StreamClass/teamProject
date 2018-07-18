//-----------------------------------------------------------------------------
//�}�b�v�I�u�W�F�N�g�N���X
//-----------------------------------------------------------------------------
#include "MapObject.h"
#include "MyPG.h"

//�������R���X�g���N�^
MapObj::MapObj()
	: pos_(0, 0, 0)
	, angle_(0)
	, hitBase_(0, 0, 0, 0, 0, 0)
	, scale_(0)
	, meshName_("")
{}

//�R�s�[�R���X�g���N�^
MapObj::MapObj(ML::Vec3& pos, ML::QT& angle, ML::Box3D& hitBase, int& scale, string& meshName)
	: pos_(pos)
	, angle_(angle)
	, hitBase_(hitBase)
	, scale_(scale)
	, meshName_(meshName)
{}

//�f�X�g���N�^
MapObj::~MapObj()
{
	//�ێ����Ă���摜�����
	DG::Mesh_Erase(this->meshName_);
}


//���W��Ԃ�
ML::Vec3 MapObj::Get_Pos()
{
	return this->pos_;
}

//�����蔻���Ԃ�
ML::Box3D MapObj::Get_HitBase()
{
	return this->hitBase_;
}

//�`�揈��
void MapObj::Render3D()
{
	ML::Mat4x4 matW;
	D3DXMatrixAffineTransformation(&matW, this->scale_, NULL, &this->angle_, &this->pos_);
	DG::EffectState().param.matWorld = matW;
	DG::Mesh_Draw(this->meshName_);
}