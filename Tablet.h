#pragma once
#pragma warning (disable : 4996)
#include "GameEngine_Ver3_7.h"

class Tablet
{
private:
	//カメラの位置
	std::vector<ML::Vec3> camera_Pos;
	//主視点の位置
	std::vector<ML::Vec3> target_Pos;
	//現在選択したカメラ
	int Select;
	//タブレットを使用中なのかを確認するフラグ
	bool be_used_now;	


	//選択範囲を超えないようにする処理
	void Is_Select_Range_Over();
	//カメラ変更
	void Change_Camera();
	//デモの時のカメラの動き
	bool Update_On_Demo();

public:

	//カメラ生成の時の位置登録
	//引数 : (登録するカメラの位置、登録するカメラの主視点)
	void PushBack_Camera(const ML::Vec3&, const ML::Vec3&);
	//使用フラグを反転させる関数
	void Open_or_Close_Tablet();
	//入力反応
	void Select_Camera(const unsigned int& );

	//使用されているかを返す関数
	bool Is_Used_Now() const;

	//コンストラクタ・デストラクタ
	//セロクリア
	Tablet();

	~Tablet() {}
};