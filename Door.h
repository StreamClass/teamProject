#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"
#include "Breaker.h"

//ドアが開かれる方向
enum LR
{
	CLEAR_LR = -1,//不定(クリア)
	Left = 0,
	Right = 1,
};
class Door
{
private:
	//扉の位置
	ML::Vec3 pos;
	//ドアの開閉フラグ
	bool openFlag;
	//ドアのあたり判定範囲
	ML::Box3D hitBase;
	//関わっているブレーカー
	std::vector<Breaker*> cunnected_Breaker;
	//ドアが開かれる方向
	LR open_Angle;
	//時間
	int timeCnt;
	
public:	
	//ドアを開ける処理 
	void Door_Open();
	//プレイヤとのあたり判定 引数 : (プレイヤのあたり判定範囲)
	bool Player_Hit_the_Door(const ML::Box3D& hit);
	//状態を返す関数、trueを返したらその後当たり判定と開ける処理はしない
	bool Get_State();
	//位置をもらう関数
	ML::Vec3 Get_Pos();
	//開かれる方向をもらう
	LR Get_Angle();
	//ドアが完全に開かれたかを返す関数
	bool Is_Opened_Over();
	//フェーズ移行感知
	bool Is_Phase_Offset();

	//つながっているブレーカーの数確認
	int How_Many_Breaker_Be_Cunnected();


	//コンストラクタ・デストラクタ
	//ゼロクリア
	Door();
	//位置指定 numは1,3どっちかを入れるべき
	Door(ML::Vec3, std::vector<Breaker*> b, LR a);

	~Door(){}
};