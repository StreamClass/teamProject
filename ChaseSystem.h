#pragma once
#include "GameEngine_Ver3_7.h"

//エネミーの視野と追跡ルートを決めるクラス

class ChaseSystem
{
private:
	//プレイ屋の移動位置を保存する
	std::vector<ML::Vec3> player_Route;
	//センサーのあたり判定矩形
	ML::Box3D sensor;
	//システム判別(false : 普通 true : 追跡)
	bool systemFlag;
	//現在目的地を探す番号
	unsigned int destination;

	//プレイヤの位置をルートに登録
	void PushBack_Route(const ML::Vec3& pos);
	void Shift_to_Routine();
public:
	//あたり判定(引数 : プレイヤのあたり判定矩形,プレイやの位置,エネミーの現在位置,エネミーのアングル)
	void SensorCheck(const ML::Box3D& hit,const ML::Vec3& plpos, const ML::Vec3& pos, const float& angle);
	//次の追跡場所を返す
	ML::Vec3 NextRoute();
	//システムモードを確認する関数
	bool Is_Chase_Mode();



	//コンストラクタ・デストラクタ
	ChaseSystem() :
		sensor(-300, -100, -300, 600, 200, 600),
		systemFlag(false),
		destination(0)
	{
		this->player_Route.clear();
	}
	~ChaseSystem(){}
};