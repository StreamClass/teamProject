#pragma once
#pragma warning(disable : 4996)
#include "GameEngine_Ver3_7.h"
#include "Door.h"

//^-----------------------------------------------------------
//物体のアドレス値を管理するクラス
//--------------------------------------------------------------
class ObjManeger
{
private:
	//最初のドアのブレーカーのアドレス値
	std::vector<Breaker*> first_Door_Connencted_Breaker;
	//ブレーカーのアドレス値
	std::vector<Breaker*> door_Connencted_Breaker;
	//ドアのアドレス値
	std::vector<Door*> door;


	//最初のブレーカーの初期化
	void Init_First_Doors_Breaker(ML::Vec3);
	//ブレーカーの初期化
	void Init_Doors_Breaker(ML::Vec3);

	//最初のドアの初期化
	void Init_First_Door(ML::Vec3, LR);
	//ドアの初期化
	void Init_Door(ML::Vec3, LR);
public:
	//ブレーカーを作る
	void Create_Breaker(ML::Vec3);
	//ドアを作る
	void Create_Door(ML::Vec3, LR);
	//ファイナライズ,本編が終了する時絶対呼び出すこと
	//ヒープから解放処理をやる
	void Finalize();

	//コンストラクタ・デストラクタ
	ObjManeger();
	~ObjManeger() {}
};