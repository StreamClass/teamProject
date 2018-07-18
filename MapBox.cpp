//-----------------------------------------------------------------------------
//マップチップクラス
//-----------------------------------------------------------------------------
#include "MapBox.h"
#include "MyPG.h"

//状態を読み込み	   (状態を数値で)
void Box::Type_Read(const int& type)
{
	//自分の状態に設定
	this->type_ = (Type)type;
}

//状態を渡す
int Box::Get_Type()
{
	return (int)this->type_;
}

//位置を渡す
ML::Vec3 Box::Get_Pos()
{
	return this->pos_;
}

//あたり判定矩形を渡す
ML::Box3D Box::Get_HitBase()
{
	return this->hitBase_;
}

//チップサイズを渡す
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

//スケーリング
ML::Vec3 Box::Get_Scaling()
{
	ML::Vec3 scaling
	(
		//サイズを指定(基準値100)
		this->chipSizeX_,
		this->chipSizeY_,
		this->chipSizeZ_
	);
	return scaling;
}

//あたり判定
bool Box::Map_Hit_Check(const ML::Box3D& hit_)
{
	auto me = this->hitBase_.OffsetCopy(this->pos_);
	return me.Hit(hit_);
}

//曲がり角の番号を読み込んだ順に設定
void Box::Ini_Corner_Num(const int num)
{
	this->cornerNum = num;
}

//メッシュ名を設定
void Box::Set_MeshName(const string meshName)
{
	this->meshName_ = meshName;
}

//3D描画処理
void Box::Render3D()
{
	//メッシュ名が設定されていなければ
	if (this->meshName_ == "")
	{
		//オブジェクトではないため終了
		return;
	}
	//座標とサイズを設定
	ML::Mat4x4 matT, matS;
	matS.Scaling(this->Get_Scaling());
	matT.Translation(this->pos_);
	//ワールド座標に設定
	DG::EffectState().param.matWorld = matS * matT;
	//メッシュ名に合わせて描画
	DG::Mesh_Draw(this->meshName_);
}