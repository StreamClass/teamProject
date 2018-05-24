#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//マップチップクラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
//壁のサイズ指定
#define chipX 100
#define chipY 300
#define chipZ 100

//マップチップの状態
/*
-1 : なし
 0 : 空気(マップなし)
 1 : 壁
 2 : 遮断機
 3 : プレイヤ初期位置
 4 : 扉
*/
enum Type
{
	clear_Type = -1,
	air	= 0,
	box = 1,
	breaker = 2,
	player = 3,
	door = 4
};
class Box
{
private:
	//壁の座標
	ML::Vec3	pos_;
	//当たり判定矩形
	ML::Box3D	hitBase_;
	//状態
	Type		type_;
	//X方向のチップサイズ
	float		chipSizeX_;
	//Y方向のチップサイズ
	float		chipSizeY_;
	//Z方向のチップサイズ
	float		chipSizeZ_;
public:
	//Boxクラスのメンバ変数の初期化(コンストラクタ) 
	Box()
		: pos_(0.0f, 0.0f, 0.0f)
		, hitBase_(0, 0, 0, 0, 0, 0)
		, type_(Type::clear_Type)
		, chipSizeX_(chipX)
		, chipSizeY_(chipY)
		, chipSizeZ_(chipZ)
	{}
	//壁の座標・当たり判定矩形の指定
	//引数：(座標 , 当たり判定矩形)
	Box(ML::Vec3 pos,ML::Box3D hitBase)
		: pos_(pos)
		, hitBase_(hitBase)
		, type_(Type::clear_Type)
		, chipSizeX_(chipX)
		, chipSizeY_(chipY)
		, chipSizeZ_(chipZ)
	{}
	//床・天井の座標・当たり判定矩形の指定
	//引数 : (ML::vec3 チップサイズ , 座標 , 当たり判定矩形)
	Box(ML::Vec3 chipSize, ML::Vec3 pos, ML::Box3D hitBase)
		: pos_(pos)
		, hitBase_(hitBase)
		, chipSizeX_(chipSize.x)
		, chipSizeY_(chipSize.y)
		, chipSizeZ_(chipSize.z)
	{}
	
	//状態読み取り
	void Type_Read(const int& type);
	//状態を渡す
	int Get_Type();
	//各チップサイズを渡す
	float Get_ChipSizeX();
	float Get_ChipSizeY();
	float Get_ChipSizeZ();
	//現在座標を渡す
	ML::Vec3 Get_Pos();
	//当たり判定矩形を渡す
	ML::Box3D Get_HitBase();
	//スケーリング
	ML::Vec3 Get_Scaling();
	//あたり判定
	bool Map_Hit_Check(const ML::Box3D& hit_);
};