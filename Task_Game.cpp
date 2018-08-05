//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_NowLoading.h"
#include  "Task_Map.h"
#include  "Task_GameClear.h"
#include  "Task_GameOver.h"
#include  "Task_Title.h"
#include  "Task_Camera.h"
#include  "Task_Player.h"
#include  "Task_MiniMap.h"
#include  "Task_Enemy.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//BGM
		this->bgmName = "GameBGM";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/GameBGM.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DM::Sound_Erase(this->bgmName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化		
		this->pushButton = false;
		this->timeCnt = 0;
		this->stanbyCnt = 0;
		this->startTime = 60 * 2;

		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//ライティングの設定
		//ライティング有効化
		DG::EffectState().param.lightsEnable = true;
		//環境光の強さを設定する
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.5f, 0.5f, 0.5f);
		////平行光源の設定
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//光源の種類
		DG::EffectState().param.light[0].direction = ML::Vec3(1, 0, 1).Normalize();//照射方向
		DG::EffectState().param.light[0].color = ML::Color(1, 0.2f,0.2f,0.2f);//色と強さ

		//BGM再生
		DM::Sound_Play(this->res->bgmName, true);
		//大きかったため音量を少し下げる(800 / 1000)
		DM::Sound_Volume(this->res->bgmName, 800);

		//★タスクの生成
		auto pl = Player::Object::Create(true);
		auto cam = Camera::Object::Create(true);
		auto en = Enemy::Object::Create(true);
		auto map = Map::Object::Create(true);
		auto mm = MiniMap::Object::Create(true);
		/*
		ミニマップで監視カメラの座標を保存するために
		ミニマップを生成したあとにマップをファイルから読み込む
		(オブジェクトはミニマップの前でも可)
		*/
		map->Load_Map();
		map->Load_Objects();

		//DG::Mesh_CreateFromSOBFile("bbb", "./data/mesh/.SOB");
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//DG::Mesh_Erase("bbb");
		//★データ＆タスク解放
		ge->KillAll_G("フィールド");
		ge->KillAll_G("カメラマン");
		ge->KillAll_G("プレイヤ");
		ge->KillAll_G("ドア");
		ge->KillAll_G("ブレーカー");
		ge->KillAll_G("ミニマップ");
		ge->KillAll_G("エネミー");
		ge->KillAll_G("ランプ基盤");

		ge->OM.Finalize();
		//BGMを停止
		DM::Sound_Stop(this->res->bgmName);

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			//フェードインアウトに移行
			//auto nextTask = Clear::Object::Create(true);
			//if (ge->state == ge->clear)
			//{
			//	auto nextTask = Clear::Object::Create(true);
			//}
			//else if (ge->state == ge->over)
			//{
			//	auto nextTask = Over::Object::Create(true);
			//	nextTask->Set_Bone_Ptr(this->eBone);
			//}
			//else
			//{
			//	auto nextTask = Title::Object::Create(true);
			//}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{		
		auto in = DI::GPad_GetState(ge->controllerName);
		//最初のフレームなら
		if (this->stanbyCnt == 0)
		{
			//エネミーを停止
			ge->StopAll_G("エネミー", true);
			this->stanbyCnt++;
		}
		//スタンバイ時の処理
		else if (this->stanbyCnt == 1)//カメラの初期位置を設定するように1フレームのみ動かす
		{
			ge->StopAll_G("プレイヤ", true);
			this->stanbyCnt++;
		}
		//スタートするタイミングになるまで
		else if(this->stanbyCnt < this->startTime)
		{
			this->stanbyCnt++;
			//デバッグ用
			//B1(Xor□)を押したら
			if (in.B1.down)
			{
				//スタートするタイミングまで飛ばす
				this->stanbyCnt = this->startTime - 1;
			}
		}
		//スタートするタイミングなら
		else if(this->stanbyCnt == this->startTime)
		{
			//ゲームを開始
			this->Start();
		}
		//スタートとセレクトを押すと
		if (in.ST.down && in.SE.down)
		{
			//ゲームオーバーに
			ge->state = ge->over;
		}
		//クリア状態で初めてのフレームなら
		if (ge->state == ge->clear && !this->pushButton)
		{
			//フェードインアウト
			auto lo = Loading::Object::Create(true);
			lo->Set_NowTask(defGroupName);
			lo->Set_Color(1);
			//状態遷移待機状態へ
			this->pushButton = true;
		}
		//ゲームオーバー状態で初めてのフレームなら
		if (ge->state == ge->over && !this->pushButton)
		{
			//フェードインアウト
			auto lo = Loading::Object::Create(true);
			lo->Set_NowTask(defGroupName);
			lo->Set_Color(0);
			//状態遷移待機状態へ
			this->pushButton = true;
		}
		//デモ状態なら
		if (ge->state == ge->demo)
		{
			//デモが32秒経ったら
			if (!this->pushButton && this->timeCnt == 60 * 32 || this->PushAnyButton())
			{
				//フェードインアウト
				auto lo = Loading::Object::Create(true);
				lo->Set_NowTask(defGroupName);
				lo->Set_Color(1);
				this->pushButton = true;
			}
			this->timeCnt++;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

	}
	//-------------------------------------------------------------------
	void  Object::Render3D_L0()
	{
		//ML::Mat4x4 matT, matS;
		//matT.Translation(ML::Vec3(825, 1, 825));
		//matS.Scaling(100);
		//DG::EffectState().param.matWorld = matS * matT;
		//DG::Mesh_Draw("bbb");
	}
	//-------------------------------------------------------------------
	//ゲーム開始処理
	void Object::Start()
	{
		ge->StopAll_G("プレイヤ", false);
		ge->StopAll_G("エネミー", false);
	}
	//
	bool Object::PushAnyButton()
	{
		auto in = DI::GPad_GetState(ge->controllerName);
		return (in.B1.down || in.B2.down || in.B3.down || in.B4.down || in.SE.down || in.ST.down);
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}