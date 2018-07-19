#pragma once
#include "Joint.h"
#include "Motion.h"

//骨格クラス

//idleは直立
//視線の正面はZ+

//モーションデータと同じく関節を番号で割り当てて管理する

class Bone
{
private:
	//体全体の中心
	ML::Vec3 center_of_Body;
	//背の高さ
	const float tall;

	//全関節のアドレス値
	Joint* joint[JOINT_ON_HUMAN];

	//名前を別々に持っていたver0.1
	/*
	Joint* waist;
	Joint* neck;

	Joint* left_Sholder;	
	Joint* left_Elbow;
	Joint* left_Wrist;

	Joint* right_Sholder;
	Joint* right_Elbow;
	Joint* right_Wrist;

	Joint* left_Hip;
	Joint* left_Knee;
	Joint* left_Ankle;
	
	Joint* right_Hip;
	Joint* right_Knee;
	Joint* right_Ankle;
	*/

	//全体のY回転値
	float All_RotY;

	//モーション用カウンタ
	int motionCnt;
	//ヴェクターの[]演算子で利用する識別子
	int motion_Index;
	//今何のモーションなのかを確認するキー
	string now_Motion;
	//予約かけてる次に動くモーションを確認するキー
	string next_Motion;
	//モーションが一回り終わっても繰り返すフラグ(走るモーションとかに利用)
	bool repeat_Flag;

	//モーションデータのマッピング
	//実体持ち？ヒープに宣言？(2018/07/05)
	std::map < string, std::vector<Motion::Motion_Data> > motions;


	//メソッド

	//動きのプリセット
	//歩き
	//void Make_Walking();
	//走り
	void Make_Running();
	//相互作用
	void Make_Interaction();

	//直立に整頓
	void To_Standing(bool ASAP);

	//予約モーションにするか空の状態にする
	bool Next_Motion_or_None();

public:
	//ゲッター
	ML::Vec3 Get_Center();

	//座標移動
	void Moving(const ML::Vec3& vec);

	//体全体をY軸回転を行う
	//ほかの軸も必要に応じて追加可能性あり(2018/07/04)
	//引数 : (Y軸回転量)
	void Bone_RotateY_All(const float& radian);

	//次のモーション予約
	void Set_Next_Motion(const string& next);
	//連続行動フラグを立てる
	void Repeat_Now_Motioin();
	//アニメーションアップデート
	void UpDate();
	//アニメーションレンダリング
	void Render();
	

	//コンストラクタ
	//ゼロクリア
	Bone() :
		tall(0.0f)
	{
		this->center_of_Body = ML::Vec3(0, 0, 0);
		this->All_RotY = 0.0f;
		/*this->neck = nullptr;
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
		this->right_Ankle = nullptr;*/

		for (int i = 0; i < JOINT_ON_HUMAN; i++)
		{
			this->joint[i] = nullptr;
		}

		this->motions.clear();
		this->motionCnt = 0.0f;
		this->motion_Index = 0;
		this->now_Motion = "";
		this->next_Motion = "";
		this->repeat_Flag = false;
	}
	//引数 : (身長)
	Bone(const float& tall, const string& owner);

	//デストラクタ
	~Bone()
	{
		//ヒープ領域から開放
		/*delete this->waist;
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
		delete this->right_Wrist;*/

		for (int i = 0; i < JOINT_ON_HUMAN; i++)
		{
			delete this->joint[i];
		}

		this->motions.clear();
	}

};