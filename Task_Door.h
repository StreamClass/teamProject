#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ドアタスク
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Door.h"

namespace Task_Door
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ドア");	//グループ名
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
		//仮のメッシュ名
		string meshName;
		string shadowMesh;
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
		static  Object::SP  Create(bool flagGameEnginePushBack_, Door* d);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(Door* d);
		bool  B_Finalize();
		bool  Initialize(Door* d);	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	
		//追加したい変数・メソッドはここに追加する	
		//変数
		//ドアの回路
		Door* circuit;
		//

	public:
		//メソッド
		//プレイヤとのあたり判定(プレイヤ側で呼ぶ処理)
		bool Hit_Check(const ML::Box3D& hit);
		//つながっているブレーカーの数確認
		int How_Many_Breaker_Be_Cunnected();
		//
		void Render3DShadow();
	};
}