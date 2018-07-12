#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//マップチップクラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
//壁のサイズ指定
#define chipX 150
#define chipY 300
#define chipZ 150

//マップチップの状態
/*
-1 : なし
 0 : 空気(マップなし)
 1 : 壁
 2 : 遮断機北向き
 3 : 遮断機南向き
 4 : プレイヤ初期位置
 5 : 扉
 6 : カメラ　北
 7 : カメラ　北東
 8 : カメラ　東
 9 : カメラ　南東
 10 : カメラ　南
 11: カメラ　南西
 12: カメラ　西
 13: カメラ　北西
 14: コーナー
 15: ブレーカーランプ
 16: テーブル
*/

enum Type
{
	clear_Type = -1,
	air	= 0,
	box = 1,
	breakerN = 2,
	breakerS = 3,
	player = 4,
	door = 5,
	camera_North = 6,
	camera_North_East = 7,
	camera_East	= 8,
	camera_South_East = 9,
	camera_South = 10,
	camera_South_West = 11,
	camera_West = 12,
	camera_North_West = 13,
	corner = 14,
	lamp = 15,
	table = 16
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
	//
	int cornerNum;
public:
	//Boxクラスのメンバ変数の初期化(コンストラクタ) 
	Box()
		: pos_(0.0f, 0.0f, 0.0f)
		, hitBase_(0, 0, 0, 0, 0, 0)
		, type_(Type::clear_Type)
		, chipSizeX_(chipX)
		, chipSizeY_(chipY)
		, chipSizeZ_(chipZ)
		, cornerNum(-1)
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
	//曲がり角の番号を読み込んだ順に設定
	void Ini_Corner_Num(const int num);
};