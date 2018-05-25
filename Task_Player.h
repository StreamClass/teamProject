#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace Player
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤ");	//グループ名
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
		//オフスクリーン名
		string meshName;
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
		//プレイヤの座標
		ML::Vec3 pos;
		//プレイヤの矩形
		ML::Box3D hitBase;
		//プレイやの向き
		ML::Vec3 angle;
		//プレイヤの視点(頭の高さ)
		int headHeight;
		//注視点の高さ
		int adjust_TG;
		//注視点高さの下限
		int adjust_Min;
		//注視点高さの上限
		int adjust_Max;
		//プレイヤの移動量
		ML::Vec3 moveVec;
		//コントローラネーム
		string controllerName;
		//クリア判定
		bool		clearFlag;
	public:
		//追加メソッド
		//プレイヤの視点をint型で返す
		int Get_PointView();
		//注視点の高さ(adJust_TG)を返す
		int Get_Adjust();
		
		//プレイヤの座標をML::Vec3型で返す
		ML::Vec3 Get_Pos();
		//プレイヤの向きをML::Vec3型で返す
		ML::Vec3 Get_Angle();
		//プレイヤの初期値指定
		void Ini_Pos(const ML::Vec3& pos);
		//チップサイズをML::Vec3型に変換
		//引数：（チップサイズX, チップサイズY, チップサイズZ）
		//ML::Vec3 Chip_Size(/*const float& cSizeX, const float& cSizeY, const float& cSizeZ*/);
		//マップとの接触判定
		//引数：（マップの矩形, プレイヤの矩形, マップのチップサイズ）
		bool Map_CheckHit(/*const ML::Box3D& mHit, */const ML::Box3D& pHit/*, ML::Vec3& cSize*/);
		//めり込まない処理
		//引数：（プレイヤの座標, プレイヤの矩形, プレイヤの移動量）
		void Player_CheckMove(/*const ML::Vec3& pPos, const ML::Box3D& pHit,*/ML::Vec3& est_);
		//ギミックへの干渉
		void Touch();
		//クリアしているか判定
		void Check_Clear();
		//クリア情報を渡す
		bool Get_ClearFlag();
	};
}