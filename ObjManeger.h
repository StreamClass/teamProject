#pragma once
#pragma warning(disable : 4996)
#include "GameEngine_Ver3_7.h"
#include "Door.h"
#include "MapBox.h"
#include "Tablet.h"
#include "Routine.h"

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
	//タブレットのアドレス値
	Tablet* tab;
	//ルーチンワークシステムのアドレス値
	Routine* rou;


	//最初のブレーカーの初期化
	//引数 : (ブレーカーの位置,ブレーカーの表示向き)
	void Init_First_Doors_Breaker(ML::Vec3, int);
	//ブレーカーの初期化
	//引数 : (ブレーカーの位置,ブレーカーの表示向き)
	void Init_Doors_Breaker(ML::Vec3, int);

	//最初のドアの初期化
	//引数 : (ドアの位置,ドアが開く向き)
	void Init_First_Door(ML::Vec3, LR);
	//ドアの初期化
	//引数 : (ドアの位置,ドアが開く向き)
	void Init_Door(ML::Vec3, LR);
public:
	//ブレーカーを作る
	//引数 : (ブレーカーの位置,ブレーカーの表示向き)
	void Create_Breaker(ML::Vec3, Type);
	//ドアを作る
	//引数 : (ドアの位置,ドアが開く向き)
	void Create_Door(ML::Vec3, LR);
	//カメラの生成
	//引数 : (カメラの位置,カメラが向いている向き)
	void Create_Camera(ML::Vec3, Type);
	//ランプの生成
	void Create_Lamp(ML::Vec3, ML::Box3D);
	//タブレットのアドレス値を返す関数(プレイヤの初期化にのみ使うこと)
	Tablet* Create_Tablet();
	//ルーチンワークシステム生成
	Routine* Create_Routine();
	//ルーチンワークシステムに各コーナーを登録
	//引数 : (コーナーの位置,コーナーの番号)
	void Push_Back_Conner(const ML::Vec3&, const int&);
	//コーナー同士の関係性を構築
	void Set_Relationship();
	//エネミーを最終フェーズにする
	void Game_Be_Final_Phase();

	//ファイナライズ,本編が終了する時絶対呼び出すこと
	//ヒープから解放処理をやる
	void Finalize();

	//コンストラクタ・デストラクタ
	ObjManeger();
	~ObjManeger() {}
};