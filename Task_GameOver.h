#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲームオーバー画面
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Bone.h"
#include "Motion.h"

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
		string bgMeshName;
		//エフェクトのイメージ名
		string eImgName;
		//ゲームオーバーロゴのイメージ名
		string lImgName;

		//足音
		string footSoundName;
		//BGM
		string gameOver_Bgm;
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
		//ロゴの不透明度の指定
		float al;
		//フレーム数カウント
		int timeCnt;
		//エネミーのボーン情報
		Bone* enBone;
		//エネミーのモーション名
		string motionName;
		//座標
		ML::Vec3 pos;
		//エフェクトが出てロゴが出きっているか判断用
		bool endFlag;
		//endFlagがtrueになってからのフレーム数カウント
		int endCnt;
		//エネミーのアニメーション初めのフレームか判断
		bool iniFlag;
	};
}