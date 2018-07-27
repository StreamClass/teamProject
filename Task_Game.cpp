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
		this->bgmName = "GameBGM";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/GameBGM.wav");
		DM::Sound_Play(this->bgmName, true);
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
		ge->state = ge->standby;
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

		//
		DM::Sound_Play(this->res->bgmName, true);
		DM::Sound_Volume(this->res->bgmName, 800);

		//★タスクの生成
		auto pl = Player::Object::Create(true);
		auto cam = Camera::Object::Create(true);
		auto en = Enemy::Object::Create(true);
		this->eBone = en->Get_EnemyBonePtr();
		auto map = Map::Object::Create(true);
		map->Load_Map();
		map->Load_Objects();
		auto mm = MiniMap::Object::Create(true);

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

		DM::Sound_Stop(this->res->bgmName);

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			//auto nextTask = Clear::Object::Create(true);
			if (ge->state == ge->clear)
			{
				auto nextTask = Clear::Object::Create(true);
			}
			else if (ge->state == ge->over)
			{
				auto nextTask = Over::Object::Create(true);
				nextTask->Set_Bone_Ptr(this->eBone);
			}
			else
			{
				auto nextTask = Title::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{		
		auto in = DI::GPad_GetState(ge->controllerName);

		if (this->stanbyCnt == 0)
		{
			ge->StopAll_G("エネミー", true);
			this->stanbyCnt = 1;
		}
		//スタンバイ時の処理
		else if (this->stanbyCnt == 1)//カメラの初期位置を設定するように1フレームのみ動かす
		{
			ge->StopAll_G("プレイヤ", true);
			this->stanbyCnt++;
		}
		else if(this->stanbyCnt < this->startTime)
		{
			this->stanbyCnt++;
			if (in.B1.down)
			{
				this->stanbyCnt = this->startTime;
			}
		}
		else if(this->stanbyCnt == this->startTime)
		{
			this->Start();
		}
		if (in.ST.down && this->pushButton == false)
		{
			ge->state = ge->over;
			this->pushButton = true;
			auto lo = Loading::Object::Create(true);
			float color = 0.0f;
			lo->Set_Color(color);
		}
		if (ge->state == ge->clear && this->pushButton == false)
		{
			auto lo = Loading::Object::Create(true);
			float color = 1.0f;
			lo->Set_Color(color);
			this->pushButton = true;
		}
		if (ge->state == ge->over && this->pushButton == false)
		{
			auto lo = Loading::Object::Create(true);
			float color = 0.0f;
			lo->Set_Color(color);
			this->pushButton = true;
		}
		if (this->pushButton)
		{
			this->timeCnt++;
		}
		if (this->timeCnt == 60 * 1)
		{
			this->Kill();
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