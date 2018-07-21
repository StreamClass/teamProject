#pragma once
#include "Shape.h"
#include "GameEngine_Ver3_7.h"

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
	//現在までの回転量クォータニオン
	ML::QT rotated;
	//繋がれた骨のアドレス値(仮の名前)
	Shape* area;
	//下の関節(存在しない場合はnullptr)
	std::vector<Joint*> next;

	//描画するメッシュの名前
	string mesh_Name;

public:
	//移動
	//引数 : (移動ベクトル)
	void Move(const ML::Vec3&);
	//つながれている骨を回転させる
	//引数 : (回転行列のアドレス値, 掛け算するクォータニオン)
	void Rotate_Bone(ML::Mat4x4*, const ML::QT&);
	//上位の関節から命令される場合
	//引数 : (回転行列のアドレス値, 掛け算するクォータニオン)
	void Rotated_by_Prev_Joint(ML::Mat4x4*, const ML::QT&);
	//次の関節をセット
	void Set_Next_Joint(Joint*);
	//回転量アップデート
	void Quartanion_Update(const ML::QT&);

	//レンダリング
	//引数 : (倍率用背の高さ)
	void Render(const float& tall);

	
	//現在関節とボーンまでのベクトルを返す
	ML::Vec3 Get_To_Bone() const;



	//デバッグ用
	void DEBUG();


	//ゲッター
	ML::Vec3 Get_Pos() const;
	float Get_Limit_X_Plus() const;
	float Get_Limit_X_Minus()const;
	float Get_Limit_Y_Plus()const;
	float Get_Limit_Y_Minus()const;
	float Get_Limit_Z_Plus()const;
	float Get_Limit_Z_Minus()const;

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
		this->rotated = ML::QT();
		this->pos = ML::Vec3(0, 0, 0);		
		this->area = nullptr;
		this->next.clear();
		this->mesh_Name = "";
	}
	//引数 : (関節の位置、回転量の限界X-,X+,Y-,Y+,Z-,Z+,つながっている骨のアドレス値,メッシュ名)
	Joint(const ML::Vec3& p, const float& xm, const float& xp, const float& ym, const float& yp, const float& zm, const float& zp, Shape* bone,const string& name) :
		limit_X_Minus(xm),
		limit_X_Plus(xp),
		limit_Y_Minus(ym),
		limit_Y_Plus(yp),
		limit_Z_Minus(zm),
		limit_Z_Plus(zp)
	{
		//左手法右手法を治すために
		this->rotated = ML::QT(ML::Vec3(0,1,0),ML::ToRadian(180));
		this->pos = p;
		this->area = bone;
		this->next.clear();
		this->mesh_Name = name;
		DG::Mesh_CreateFromSOBFile(this->mesh_Name, "./data/mesh/bone/" + this->mesh_Name + ".sob");
	}

	//デストラクタ
	~Joint()
	{
		//ヒープ領域から開放
		delete this->area;
		//メッシュ開放
		DG::Mesh_Erase(this->mesh_Name);
		//つながっている次の関節は
		//ボーンから開放される
	}
};