#pragma once
#pragma warning(disable:4996)
#include "GameEngine_Ver3_7.h"

//マップ内のコーナーの数
#define corners 24
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
public:
	//コンストラクタ
	Routine();
	//近いコーナーを目指して移動する
	void Move();
	//自分がコーナーに到達したかの判定
	bool Judge();
	//自分のいるコーナーから行けるコーナーを選択する
	int Choice(const int& now_);
	//コーナーの持つ矩形の側がエネミーとの接触に応じて自分の番号を返す
	int Recieved();
};