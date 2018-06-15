#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲームオーバー画面
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace Over
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ゲームオーバー");	//グループ名
	const  string  defName("NoName");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		//背景のイメージ名 
		string bImgName;
		//キャラクタのイメージ名
		string cImgName;
		//エフェクトのイメージ名
		string eImgName;
		//ゲームオーバーロゴのイメージ名
		string lImgName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		//使用イージング名
		string easingName;
		//ロゴの不透明度の指定
		float al;
		//フレーム数カウント
		int timeCnt;
		//キャラクタの2次元座標
		ML::Vec2 cPos;
		//エフェクトが出てロゴが出きっているか判断用
		bool endFlag;
		//endFlagがtrueになってからのフレーム数カウント
		int endCnt;
	};
}