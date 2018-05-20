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
	return 50.0f;
		//this->chipSizeX_;
}
float Box::Get_ChipSizeY()
{
	return 300.0f;
		//this->chipSizeY_;
}
float Box::Get_ChipSizeZ()
{
	return 50.0f;
		//this->chipSizeZ_;
}
//スケーリング            (サイズを変更したいboxの種類:wal,floor,ceilingの頭文字を入れる)
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
