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
		//サイズの％でサイズを指定
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
