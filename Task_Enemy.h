#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//エネミー
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "ChaseSystem.h"
#include "Routine.h"
#include "Bone.h"

namespace Enemy
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("エネミー");	//グループ名
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
		string meshName,searchMesh;
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
	public:
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//追加変数
		//自身の座標
		ML::Vec3 pos;
		//徘徊時の移動速度
		float speed;
		//
		ML::Box3D hitBase;
		//向き
		ML::Vec3 angle;
		//追跡用移動量
		ML::Vec3 toVec;
		//速度
		float chasing_Speed;
		float final_Phase_Speed;
		//探知用矩形
		//ML::Box3D searchBase;

		//ボーンアニメ用ボーン
		Bone* ebone;

		//ルーチーンワーク
		Routine* rou;

		//追跡システム
		ChaseSystem system;
		int timeCnt;
		//追加したい変数・メソッドはここに追加する
		//
		void Player_HitCheck();
		//
		Bone* Get_EnemyBonePtr();
		//
		ML::Vec3 Get_Pos();
	};
}