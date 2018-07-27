#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Tablet.h"
#include "Bone.h"

#define NORMALSPEED 10
#define TIRED_SPEED 3
#define DASHSPEED 20
#define MAX_STAMINA 240

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
		//状態管理
		enum Motion
		{
			neutral,//標準速度
			dash,	//ダッシュ中
			tired	//疲労中
		};
		Motion motion;
		//追加変数
		//プレイヤの座標
		ML::Vec3 pos;
		//移動速度
		float speed;
		//エネミーとの判定矩形
		ML::Box3D hitBase;
		//マップとの判定矩形
		ML::Box3D moveBase;
		//プレイやの向き
		ML::Vec3 angle;
		//プレイヤの視点(頭の高さ)
		int headHeight;
		//プレイやの視点基準
		int headHeight_std;
		//注視点の高さ
		int adjust_TG;
		//注視点の高さ基準
		int adjust_TG_std;
		//注視点の増減量
		float add_adjust;
		//視点移動速度
		float adjust_Speed;
		//注視点高さの下限
		int adjust_Min;
		//注視点高さの上限
		int adjust_Max;
		//視点揺れカウンタ
		int cnt_TG;
		//視点揺れの速度
		int cnt_SP;
		//画面揺れ下限
		float trm_Min;
		//画面揺れ上限
		float trm_Max_std;
		//ダッシュ用上限
		float trm_Max_dash;
		//疲労時用上限
		float trm_Max_trd;
		//視点の揺れ幅
		float tremor;		
		//画面揺れ上限
		float heightMax;
		//プレイヤの移動量
		ML::Vec3 moveVec;
		//プレイヤの移動量保存
		float moveVecRec;
		//コントローラネーム
		string controllerName;
		//クリア判定
		bool clearFlag;
		//タブレット
		Tablet* tab;

		//ボーンアニメーション用
		Bone* plBone;

		//スタミナ
		float stamina;
		//スタミナ回復フラグ
		bool recovery_Flag;
		bool debugMode;
		//
		int breakerOnCnt;
		//サウンド管理フラグ
		bool neutralSoundFlag;
		bool dashSoundFlag;
		bool tiredSoundFlag;
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
		//あたり判定範囲をもらう関数
		ML::Box3D Get_HitBase();
		//移動速度をfloatで返す
		float Get_MoveSpeed();
		//プレイヤの初期値指定
		void Ini_Pos(const ML::Vec3& pos);
		//めり込まない処理
		//引数：（プレイヤの移動量）
		void Player_CheckMove(ML::Vec3& est_);
		//
		void Turn_Angle();
		//正面が壁に近づきすぎない処理
		bool Check_Front();
		//ギミックへの干渉
		void Touch();
		//
		bool Touch_AimToBreaker();
		//クリアしているか判定
		void Check_Clear();
		//クリア情報を渡す
		bool Get_ClearFlag();
		//タブレットを使用中を確認する関数
		bool Is_Used_Tablet();
		//疲れているかを渡す
		bool Is_Tired();
		//スタミナの残量を渡す
		float Get_Stamina();
		//デバッグ用無敵モードのオンオフ
		bool Get_DebugOnOff();
	};
}