#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//マップオブジェクトクラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class MapObj
{
private:
	//座標
	ML::Vec3 pos_;
	//向きのクォータニオン
	ML::QT angle_;
	//あたり判定
	ML::Box3D hitBase_;
	//サイズ(基準値は100)
	int scale_;
	//メッシュ名
	string meshName_;
public:
	//初期化コンストラクタ
	MapObj();
	//データ設定用コピーコンストラクタ
	//引数：( ML::Vec3型 座標 ,ML::QT型 向き ,ML::Box3D型 あたり判定 ,int型 スケーリング ,string型 メッシュ名 )
	MapObj(ML::Vec3&, ML::QT&, ML::Box3D&, int&, string&);
	//保持画像解放用デストラクタ
	~MapObj();

	//メソッド---------------------------------------------
	//座標を返す
	ML::Vec3 Get_Pos();
	//あたり判定を返す
	ML::Box3D Get_HitBase();
	//
	bool HitCheck(const ML::Box3D&);
	//描画処理
	void Render3D();
};