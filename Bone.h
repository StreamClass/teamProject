#pragma once
#include "Joint.h"
#include "Motion.h"

//骨格クラス

//idleは直立
//視線の正面はZ+


class Bone
{
private:
	//体全体の中心
	ML::Vec3 center_of_Body;
	//背の高さ
	const float tall;

	//全関節のアドレス値
	Joint* waist;
	Joint* neck;
	Joint* left_Sholder;	
	Joint* left_Elbow;
	Joint* left_Wrist;
	Joint* left_Hip;
	Joint* left_Knee;
	Joint* left_Ankle;
	Joint* right_Sholder;
	Joint* right_Elbow;
	Joint* right_Wrist;
	Joint* right_Hip;
	Joint* right_Knee;
	Joint* right_Ankle;

	//モーション用カウンタ
	float motionCnt;
	//ヴェクターの[]演算子で利用する識別子
	int motion_Index;
	//今何のモーションなのかを確認するキー
	string now_Motion;

	//モーションデータのマッピング
	std::map < string, std::vector<Motion::Motion_Data> > motions;

	//動きのプリセット
	//歩き
	void Walking();
	//走り
	void Running();
	//相互作用
	void Interaction();

public:
	//ゲッター
	ML::Vec3 Get_Center();

	//体全体をY軸回転を行う
	//ほかの軸も必要に応じて追加可能性あり(2018/07/04)
	//引数 : (Y軸回転量)
	void Bone_RotateY_All(float radian);

	

	//コンストラクタ
	//ゼロクリア
	Bone() :
		tall(0.0f)
	{
		this->center_of_Body = ML::Vec3(0, 0, 0);
		this->neck = nullptr;
		this->waist = nullptr;
		this->left_Sholder = nullptr;
		this->left_Elbow = nullptr;
		this->left_Wrist = nullptr;
		this->left_Hip = nullptr;
		this->left_Knee = nullptr;
		this->left_Ankle = nullptr;
		this->right_Sholder = nullptr;
		this->right_Elbow = nullptr;
		this->right_Wrist = nullptr;
		this->right_Hip = nullptr;
		this->right_Knee = nullptr;
		this->right_Ankle = nullptr;

		this->motions.clear();
		this->motionCnt = 0.0f;
		this->motion_Index = 0;
		this->now_Motion = "";
	}
	//引数 : (身長)
	Bone(const float& tall);

	//デストラクタ
	~Bone()
	{
		//ヒープ領域から開放
		delete this->waist;
		delete this->neck;
		delete this->left_Ankle;
		delete this->left_Elbow;
		delete this->left_Hip;
		delete this->left_Knee;
		delete this->left_Sholder;
		delete this->left_Wrist;
		delete this->right_Ankle;
		delete this->right_Elbow;
		delete this->right_Hip;
		delete this->right_Knee;
		delete this->right_Sholder;
		delete this->right_Wrist;

		this->motions.clear();
	}

};