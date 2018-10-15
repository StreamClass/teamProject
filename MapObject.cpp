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
//���� : (���W, �N�H�[�^�j�I���ł̌���, �����蔻��, �\���{��, ���b�V����) 
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

//�����蔻�菈��
bool MapObj::HitCheck(const ML::Box3D& hit)
{	
	//�������O�̕ӂ��Ȃ�������
	if (this->hitBase_.w == 0 || this->hitBase_.h == 0 || this->hitBase_.d == 0 ||
		hit.w == 0 || hit.h == 0 || hit.d == 0)
	{
		//�������ꍇ������Ȃ������Ə���
		return false;
	}
	//�����Ԃ�
	return this->hitBase_.OffsetCopy(this->pos_).Hit(hit);
}

//�`�揈��
void MapObj::Render3D()
{
	//�w��̃��b�V�������ǂ����𔻒�
	if (this->meshName_ == "fireModel")
	{
		//�������ő�ɂ��Ė��邭������
		DG::EffectState().param.lightAmbient = ML::Color(1, 1, 1, 1);
	}
	ML::Mat4x4 matW;
	//matW�ɒl������
	D3DXMatrixAffineTransformation(&matW, float(this->scale_), NULL, &this->angle_, &this->pos_);
	DG::EffectState().param.matWorld = matW;
	DG::Mesh_Draw(this->meshName_);
	//��Ŏw��̃��b�V���������ꍇ�Ɋ������ύX�����̂Ō��̐��l�ɖ߂�
	DG::EffectState().param.lightAmbient = ML::Color(1, 0.2f, 0.2f, 0.2f);
}