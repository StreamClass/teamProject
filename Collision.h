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
	bool Check_Collision_Cube_to_Point(const Shape*, const ML::Vec3&);

public:
};