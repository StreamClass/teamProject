#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//カメラマン
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace Camera
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("カメラマン");	//グループ名
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
		string tablet_Img_Name;
		string display_Noise_Img_Name;
		string hearts_Sound_Name;
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
		//追加変数

		//カメラの座標
		ML::Vec3 pos;
		//カメラマンの向き
		ML::Vec3 angle;
		//注視点
		ML::Vec3 dist;
		//ノイズの動き用のカウント
		int noise_Cnt;

		//test flag
		bool test_flag;

	public:
		//追加メソッド
		//
		void Noise_Reset();
		//心音の大きさを調整
		void Change_Volume_Hearts_Sound();
	};
}