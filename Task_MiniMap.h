#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace MiniMap
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ミニマップ");	//グループ名
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

		ML::Vec2 epos;
		float eangle;
		//イメージ名
		string imageName;
		string plImgName;
		string caImgName;
		//ミニマップのプレイヤ用変数
		//座標
		ML::Vec2 plpos;
		//向き
		float plAngle;
		//ミニマップのカメラ用変数
		//座標
		ML::Vec2 capos;
		//向き
		float caAngle;
		//ミニマップを表示するか
		bool viewFlag;
		//タブレットを使用しているか
		bool tab_use_now;

		//ミニマップを表示するか判断
		bool MiniMap_View();
		//プレイヤからミニマップを出すか指定
		void Set_MiniMap_View();
	};
}