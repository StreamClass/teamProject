#pragma once
#include "GameEngine_Ver3_7.h"
#include "Cube.h"

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
	//引数 : (発見したプレイヤの位置)
	void PushBack_Route(const ML::Vec3& pos);
	//ルーチンワークに戻る
	void Shift_to_Routine();
	//追跡ルートが終わったのかを確認
	bool Is_Any_More_Route() const;
public:
	//あたり判定(引数 : プレイヤのあたり判定矩形,プレイやの位置,エネミーの現在位置,エネミーのアングル)
	void SensorCheck(const ML::Box3D& hit,const ML::Vec3& plpos, const ML::Vec3& pos, const float& angle);
	//次の追跡場所を返す
	ML::Vec3 NextRoute();
	//システムモードを確認するメソッド
	bool Is_Chase_Mode() const;



	//コンストラクタ・デストラクタ
	ChaseSystem() :
		sensor(-500, -100, -50, 1000, 200, 100),
		systemFlag(false),
		destination(0)
	{
		this->player_Route.clear();
	}
	~ChaseSystem(){}
};