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
//引数 : (座標, クォータニオンでの向き, あたり判定, 表示倍率, メッシュ名) 
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

//あたり判定処理
bool MapObj::HitCheck(const ML::Box3D& hit)
{	
	//長さが０の辺がないか判定
	if (this->hitBase_.w == 0 || this->hitBase_.h == 0 || this->hitBase_.d == 0 ||
		hit.w == 0 || hit.h == 0 || hit.d == 0)
	{
		//あった場合当たらなかったと処理
		return false;
	}
	//判定を返す
	return this->hitBase_.OffsetCopy(this->pos_).Hit(hit);
}

//描画処理
void MapObj::Render3D()
{
	//指定のメッシュ名かどうかを判定
	if (this->meshName_ == "fireModel")
	{
		//環境光を最大にして明るく見せる
		DG::EffectState().param.lightAmbient = ML::Color(1, 1, 1, 1);
	}
	ML::Mat4x4 matW;
	//matWに値を合成
	D3DXMatrixAffineTransformation(&matW, float(this->scale_), NULL, &this->angle_, &this->pos_);
	DG::EffectState().param.matWorld = matW;
	DG::Mesh_Draw(this->meshName_);
	//上で指定のメッシュだった場合に環境光が変更されるので元の数値に戻す
	DG::EffectState().param.lightAmbient = ML::Color(1, 0.2f, 0.2f, 0.2f);
}