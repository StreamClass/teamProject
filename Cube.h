#pragma once
#include "Shape.h"

//3次元矩形

class Cube : public Shape
{
private:
	//始点
	ML::Vec3 leftBottom;
	//各辺の長さ
	ML::Vec3 lengthX;
	ML::Vec3 lengthY;
	ML::Vec3 lengthZ;


public:
	//ゲッター
	ML::Vec3 Get_LeftBottom();
	ML::Vec3 Get_LengthX();
	ML::Vec3 Get_LengthY();
	ML::Vec3 Get_LengthZ();

	//移動・回転
	//引数 : (回転行列のアドレス値)
	void Rotate(ML::Mat4x4* matR) override;
	//引数 : (移動ベクトル)
	void Move(const ML::Vec3& vec) override;
	//引数 : (結果を保存するML::Vec3型のvectorのアドレス値)
	void Get_All_Points(std::vector<ML::Vec3>* result) override;
	//引数 : (結果を保存するTriangle型のvectorのアドレス値)
	void Get_All_Triangle(std::vector<Triangle>* resultr) override;

	//コンストラクタ
	//ゼロクリア
	Cube():Shape()
	{
		this->leftBottom = ML::Vec3(0, 0, 0);
		this->lengthX = ML::Vec3(0, 0, 0);
		this->lengthY = ML::Vec3(0, 0, 0);
		this->lengthZ = ML::Vec3(0, 0, 0);
	}
	//引数 : (中心点、左端っこの点、Xの長さ、Yの長さ、Zの長さ)
	Cube(const ML::Vec3& c, const ML::Vec3& lb, const float& x, const float& y, const float&z)
		: Shape(c)
	{
		this->leftBottom = lb;
		this->lengthX = ML::Vec3(x,0,0);
		this->lengthY = ML::Vec3(0,y,0);
		this->lengthZ = ML::Vec3(0,0,z);
	}
	//すかライブラリーでも生成できる
	Cube(const ML::Box3D& b)
		: Shape((ML::Vec3(b.x, b.y, b.z) + ML::Vec3(b.w / 2.0f, b.h / 2.0f, b.d / 2.0f)))
	{
		this->leftBottom = ML::Vec3(b.x, b.y, b.z);
		this->lengthX = ML::Vec3(b.x, 0, 0);
		this->lengthY = ML::Vec3(0, b.y, 0);
		this->lengthZ = ML::Vec3(0, 0, b.z);
	}

	//デストラクタ
	~Cube(){}
};