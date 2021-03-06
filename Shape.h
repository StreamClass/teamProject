#pragma once
#include "myLib.h"

//継承先の列挙型
enum Shape_Type
{
	Shape_Cube,
	Shape_Sphere,
};

//三角形の基本情報
//頂点と法線ベクトル
struct Triangle
{
	ML::Vec3 a, b, c;
	ML::Vec3 normal;
};

//あたり判定の後に返す構造体
struct After_Collision
{
	bool collision_Flag;
	ML::Vec3 normal;

	After_Collision()
	{
		this->collision_Flag = false;
		this->normal = ML::Vec3(0, 0, 0);
	}
};

//3次元図形基本形

class Shape
{
private:
	//中心点
	ML::Vec3 center;

public:
	//中心点のコピーをもらう
	ML::Vec3 GetCenter() const;
	
	//回転
	void Rotate_Center(ML::Mat4x4* matR);
	//動く
	void Move_Center(const ML::Vec3& vec);

	//仮想関数
	//継承先の回転
	//引数 : (回転行列)
	virtual void Rotate(ML::Mat4x4*) = 0;
	//継承先の移動
	//引数 : (移動ベクトル)
	virtual void Move(const ML::Vec3&) = 0;
	//頂点で分割する
	//引数 : (結果を格納するヴェクター)
	virtual void Get_All_Points(std::vector<ML::Vec3>*) = 0;
	//三角形で分割する
	//引数 : (結果を格納するヴェクター)
	virtual void Get_All_Triangle(std::vector<Triangle>*) = 0;
	//継承先のパラメータのコピーをもらう
	//引数 : (パラメータ識別用キーデータ,結果を格納する変数)
	virtual void Get_Parameter(const string, ML::Vec3&) = 0;
	//空間図形のタイプをもらう
	virtual Shape_Type Get_Type() = 0;

	//コンストラクタ
	//ゼロクリア
	Shape()
	{
		this->center = ML::Vec3(0, 0, 0);
	}
	//中心点を指定したコンストラクタ
	Shape(const ML::Vec3& c)
	{
		this->center = c;
	}

	//デストラクタ
	virtual ~Shape(){}


};