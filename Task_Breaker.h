#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ブレーカー
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Breaker.h"

namespace Task_Breaker
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ブレーカー");	//グループ名
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
		string meshName;
		string buttonMeshName;
		string soundName;
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
		static  Object::SP  Create(bool flagGameEnginePushBack_, Breaker*, int);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(Breaker*, int);
		bool  B_Finalize();
		bool  Initialize(Breaker*, int);	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	
		//追加したい変数・メソッドはここに追加する
		//変数
		//ブレーカー回路
		Breaker* circuit;
		//ブレーカーの向き(2 or 3)
		int angle;
		//ボタン座標
		ML::Vec3 pos;
		//すでに押されているか
		bool pushedButton;
		//プレイヤがボタンを押したことをエネミーに教える処理
		void Now_Be_Pushed();
	public:
		//メソッド
		//プレイヤとのあたり判定
		bool Hit_Check(const ML::Box3D& hit);
		//ブレーカー起動
		void ActivateBreaker();
		//向きの設定
		float RotationY_Angle(int& angle);
		//ボタンの移動
		ML::Vec3 Move_Button();
		//
		bool Get_Now_State();
	};
}