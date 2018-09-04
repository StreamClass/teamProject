#pragma once
#include "Shape.h"
#include "MyMath.h"

//角度を持っている空間図形のあたり判定用クラス

class Collision
{
private:
	//360度を判定するためのラジアン値
	const float judge_360;
	//キューブと点1個のあたり判定
	bool Check_Collision_Cube_to_Point(Shape*, const ML::Vec3&);

public:
	//外で利用するメソッド
	//ポリモフしたクラス同士のあたり判定
	//引数 : (判定する側、判定される側)
	bool Hit_Check(Shape*, Shape*);//中身は未実装

	//ポリモフしたクラスと、点のあたり判定
	//引数 : (判定するクラス、判定される点)
	bool Hit_Check(Shape*, const ML::Vec3&);



	//コンストラクタ・デストラクタ
	Collision() :
		judge_360(cosf(359))
	{

	}
};