#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"
//-----------------------------------------------------------------
//遮断機の回路クラス
//----------------------------------------------------------------
class Breaker
{
private:
	//遮断機の位置
	ML::Vec3 pos; 
	//あたり判定範囲
	ML::Box3D active_Base;
	//オンオフのフラグ
	bool active_Falg;

public:
	//プレイヤとのあたり判定
	//引数 : (プレイヤのあたり判定範囲)
	bool Player_Touch_Breaker(const ML::Box3D& hit) const;
	//ブレーカーを起動
	void Activate_Breaker();
	//今のブレーカーの状態をもらう
	bool Get_Now_State() const;
	//位置取得
	ML::Vec3 Get_Pos() const;

	//コンストラクタ・デストラクタ
	//ゼロクリア
	Breaker();
	//位置指定コンストラクタ
	Breaker(ML::Vec3 pos);

	~Breaker() {}
};