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

	//全体のY回転値
	float All_RotY;
	//正面ベクトル(初期化の時はZ+)
	ML::Vec3 front_Vec;

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
	//歩く
	void Make_Walking();

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

	//モーションをボーンに登録
	void Registrate_Motion(const std::vector<Motion::Motion_Data>& d, const string& motions_Name);
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

		for (int i = 0; i < JOINT_ON_HUMAN; i++)
		{
			this->joint[i] = nullptr;
		}

		this->motions.clear();
		this->motionCnt = 0.0f;
		this->motion_Index = 0;
		this->front_Vec = ML::Vec3(0, 0, 0);
		this->now_Motion = "";
		this->next_Motion = "";
		this->repeat_Flag = false;
	}
	//引数 : (身長)
	Bone(const float& tall, const string& owner);

	//デストラクタ
	~Bone()
	{		

		for (int i = 0; i < JOINT_ON_HUMAN; i++)
		{
			delete this->joint[i];
		}

		this->motions.clear();
	}

};