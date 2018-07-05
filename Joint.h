#pragma once
#include "Shape.h"

//関節クラス

class Joint
{
private:
	//関節の位置
	ML::Vec3 pos;
	//回転の限界値
	const float limit_X_Minus;
	const float limit_X_Plus;
	const float limit_Y_Minus;
	const float limit_Y_Plus;
	const float limit_Z_Minus;
	const float limit_Z_Plus;
	//繋がれた骨のアドレス値(仮の名前)
	Shape* area;
	//下の関節(存在しない場合はnullptr)
	Joint* next;

public:
	//移動
	//引数 : (移動ベクトル)
	void Move(const ML::Vec3&);
	//つながれている骨を回転させる
	//引数 : (回転行列のアドレス値)
	void Rotate_Bone(ML::Mat4x4*);
	//上位の関節から命令される場合
	//引数 : (回転行列のアドレス値)
	void Rotated_by_Prev_Joint(ML::Mat4x4*);
	//次の関節をセット
	void Set_Next_Joint(Joint*);

	//ゲッター
	ML::Vec3 Get_Pos();
	float Get_Limit_X_Plus();
	float Get_Limit_X_Minus();
	float Get_Limit_Y_Plus();
	float Get_Limit_Y_Minus();
	float Get_Limit_Z_Plus();
	float Get_Limit_Z_Minus();

	//コンストラクタ
	//ゼロクリア
	Joint():
		limit_X_Minus(0.0f),
		limit_X_Plus(0.0f),
		limit_Y_Minus(0.0f),
		limit_Y_Plus(0.0f),
		limit_Z_Minus(0.0f),
		limit_Z_Plus(0.0f)
	{
		this->pos = ML::Vec3(0, 0, 0);		
		this->area = nullptr;
		this->next = nullptr;
	}
	//引数 : (関節の位置、回転量の限界X-,X+,Y-,Y+,Z-,Z+,つながっている骨のアドレス値)
	Joint(const ML::Vec3& p, const float& xm, const float& xp, const float& ym, const float& yp, const float& zm, const float& zp, Shape* bone) :
		limit_X_Minus(xm),
		limit_X_Plus(xp),
		limit_Y_Minus(ym),
		limit_Y_Plus(yp),
		limit_Z_Minus(zm),
		limit_Z_Plus(zp)
	{
		this->pos = p;
		this->area = bone;
	}

	//デストラクタ
	~Joint()
	{
		//ヒープ領域から開放
		delete this->area;
		//つながっている次の関節は
		//ボーンから開放される
	}
};