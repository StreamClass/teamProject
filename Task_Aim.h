#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤエイム
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace Aiming
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("エイム");	//グループ名
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
		//エイム用の画像名
		string imageName[3];
		//通常時の操作説明用画像名
		string normalModeImg[2];
		//監視カメラ時の操作説明画像名
		string tabletModeImg[2];
		//スタミナ残量描画用画像名
		string staminaImgName[2];
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
		//追加したい変数・メソッドはここに追加する
		//あたり判定
		ML::Box3D hitBase;
		//時間カウンタ
		int timeCnt;
		//コントローラー入力時間
		float moveCnt;
		//エイムの2次元座標
		ML::Vec2 aimPosC;
		ML::Vec2 aimPosT;
		ML::Vec2 aimPosB; 
		ML::Vec2 aimPosL; 
		ML::Vec2 aimPosR;
		//あたり判定の３次元座標
		ML::Vec3 pos;
		//エイムの動く速さ
		float aimMoveSpeed;
		//エイムの動く幅
		float aimMovetremor;
	public:
		//各状態での操作説明
		void TabletModeRrender();
		void NormalModeRrender();
		//エイムのアニメーション
		void AimingRender();
		//スタミナの描画
		void StaminaRender();
		//当たり判定を渡す
		ML::Box3D Get_HitBase();
		//あたり判定の座標設定
		void Set_Pos(ML::Vec3& pos_);
	};
}