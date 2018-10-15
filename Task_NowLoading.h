#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//フェードイン・アウト
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace Loading
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ローディング");	//グループ名
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
		//時間カウント
		int timeCnt;
		//不透明度
		float alpha;
		//RGBの値(一括)
		float rgb;
		//現在のタスク名
		string nowTask;
		//次のタスク名
		string nextTask;
	public:
		//メソッド
		//現状のタスクを指定
		void Set_NowTask(const string);
		//次のタスクを指定
		void Set_NextTask(const string);
		//他のタスクから白か黒を指定
		void Set_Color(float rgb);
		//フェードインアウトの画像の不透明度の指定
		void FadeInOut();
	};
}