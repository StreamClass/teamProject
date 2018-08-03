#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//マップ内のコーナーの数
#define corners 28
//現在の最多選択肢数
//4又が最多
#define choices 4

class Routine
{
private:
	//座標とか移動量、矩形はエネミーが持ってるから要らないかな～～～
	//座標
	ML::Vec3 pos;
	//移動量
	ML::Vec3 moveVec;
	//判定矩形
	ML::Box3D hitBase;
	//最終フェーズなのかを確認するフラグ
	bool final_Phase;

	//現在のコーナー
	int now;
	//選ばれた移動先
	int target;
	//各コーナーの座標配列
	ML::Vec3 cornerPos[corners];
	//各コーナーが持つ移動可能なコーナー
	//選択肢の少ないコーナーには-1の要素を入れ要素数を埋める
	//				[コーナー][最大選択肢数]
	int choiceCorner[corners][choices];
	//目的地検索用矩形の発射
	bool RelationShip_XP(int num_ , int);
	//	 
	bool RelationShip_XM(int num_, int);
	//	 
	bool RelationShip_ZP(int num_, int);
	//
	bool RelationShip_ZM(int num_, int);
public:
	//コンストラクタ
	Routine();
	//ゲッター
	int Get_Now();
	//デモ用ターゲットセッター
	void Set_Target(const int&);
	//近いコーナーを目指して移動する
	ML::Vec3 Move(const ML::Vec3& );
	//自分がコーナーに到達したかの判定
	bool Judge(const ML::Box3D&, const ML::Vec3&);
	//自分のいるコーナーから行けるコーナーを選択する
	void Choice(const int& now_);
	//コーナーの持つ矩形の側がエネミーとの接触に応じて自分の番号を返す
	int Recieved(const ML::Box3D&, const ML::Vec3&);
	//
	void Set_CornerPos(const ML::Vec3& pos_, const int& num_);
	//目的地の検索用
	void Initialize_RelationShip(int num);

	//フェーズ確認
	bool Is_Final_Phase();


	//最終フェーズに移行
	void Set_Final_Phase();
};