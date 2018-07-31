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
	//パラメータを返すメソッド
	//引数の名前で戻せるものを決める
	//引数 : (判明するキー、収納するコンテナ)
	void Get_Parameter(const string key, ML::Vec3& cont) override;

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
		: Shape((ML::Vec3((float)b.x, (float)b.y, (float)b.z) + ML::Vec3(b.w / 2.0f, b.h / 2.0f, b.d / 2.0f)))
	{
		this->leftBottom = ML::Vec3((float)b.x, (float)b.y, (float)b.z);
		this->lengthX = ML::Vec3((float)b.w, 0, 0);
		this->lengthY = ML::Vec3(0, (float)b.h, 0);
		this->lengthZ = ML::Vec3(0, 0, (float)b.d);
	}

	//デストラクタ
	~Cube(){}
};