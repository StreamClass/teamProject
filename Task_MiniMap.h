#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "MapBox.h"

#define NORMALMAGNI 5.0f
#define NORMALMAPSIZE 500
#define TABLETMAGNI 8.0f
#define TABLETMAPSIZE 800

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
		//イメージ名
		string imageName;
		string plImgName;
		string caImgName;
		string anImgName;
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
		//座標参照用倍率
		float magni;
		//
		int mapSize;
		//デバックモードの有無
		bool debugMode;
	public:

		//メソッド
		//ミニマップを表示するか判断
		bool MiniMap_View();
		//プレイヤからミニマップを出すか指定
		void Set_MiniMap_View();
		//ミニマップ描画用
		void MiniMap_Render();

		//デバッグ時用変数
		//エネミーの座標
		ML::Vec2 epos;
		//エネミーの向き
		float eangle;

	};
}