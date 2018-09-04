#include "Collision.h"

bool Collision::Check_Collision_Cube_to_Point(Shape* c, const ML::Vec3& p)
{
	//点をキューブから全部取り出す
	vector<ML::Vec3> all_Points;
	c->Get_All_Points(&all_Points);
	
	for (size_t i = 0; i<all_Points.size()/2; i++)
	{
		//対角線どうしで判定する点にベクトルを繋ぐ
		ML::Vec3 a, b;

		a = all_Points[i] - p;
		b = all_Points[i+4] - p;

		//両方の外積のコサイン値が-値(90<x<=180)が全ての点で出るんだったら
		//判定される点はキューブの中に内包されている
		float cos = 0.0f;
		MyMath::Vector_Cross(&cos, a, b);
		//ひとつでも条件に合ってないんだったら当たらないことにする
		if (cos < 0)
		{
			return true;
		}
	}
	return false;
}


//空間図形と点のあたり判定
//現在は物理演算が必要ないので、戻り値をboolにする
bool Collision::Hit_Check(Shape* you, const ML::Vec3& me)
{
	//判定するメソッドを判別
	switch (you->Get_Type())
	{
	case Shape_Type::Shape_Cube:
		this->Check_Collision_Cube_to_Point(you, me);
		break;

	default:
		//判定用メソッドがない場合はfalseで返す
		return false;
		break;
	}
}