//-----------------------------------------------------------------------------
//マップオブジェクトクラス
//-----------------------------------------------------------------------------
#include "MapObject.h"
#include "MyPG.h"

//初期化コンストラクタ
MapObj::MapObj()
	: pos_(0, 0, 0)
	, angle_(0)
	, hitBase_(0, 0, 0, 0, 0, 0)
	, scale_(0)
	, meshName_("")
{}

//コピーコンストラクタ
MapObj::MapObj(ML::Vec3& pos, ML::QT& angle, ML::Box3D& hitBase, int& scale, string& meshName)
	: pos_(pos)
	, angle_(angle)
	, hitBase_(hitBase)
	, scale_(scale)
	, meshName_(meshName)
{}

//デストラクタ
MapObj::~MapObj()
{
	//保持している画像を解放
	DG::Mesh_Erase(this->meshName_);
}


//座標を返す
ML::Vec3 MapObj::Get_Pos()
{
	return this->pos_;
}

//あたり判定を返す
ML::Box3D MapObj::Get_HitBase()
{
	return this->hitBase_;
}

//
bool MapObj::HitCheck(const ML::Box3D& hit)
{
	if (this->hitBase_.w == 0 || this->hitBase_.h == 0 || this->hitBase_.d == 0 ||
		hit.w == 0 || hit.h == 0 || hit.d == 0)
	{
		return false;
	}
	return this->hitBase_.OffsetCopy(this->pos_).Hit(hit);
}

//描画処理
void MapObj::Render3D()
{
	if (this->meshName_ == "fireModel")
	{
		DG::EffectState().param.lightAmbient = ML::Color(1, 1, 1, 1);
	}
	ML::Mat4x4 matW;
	D3DXMatrixAffineTransformation(&matW, float(this->scale_), NULL, &this->angle_, &this->pos_);
	DG::EffectState().param.matWorld = matW;
	DG::Mesh_Draw(this->meshName_);
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
}