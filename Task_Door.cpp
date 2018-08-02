//-------------------------------------------------------------------
//ドアタスク
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Door.h"
#include  "MapBox.h"

namespace Task_Door
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "Door_mesh";
		this->shadowMesh = "Door_Shadow";
		//仮のメッシュ
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/door.SOB");
		DG::Mesh_CreateFromSOBFile(this->shadowMesh, "./data/mesh/SquareShadow.SOB");

		this->soundName = "DoorOpenSE";
		DM::Sound_CreateSE(this->soundName, "./data/sound/DoorOpenSE.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{		
		DG::Mesh_Erase(this->meshName);
		DG::Mesh_Erase(this->shadowMesh);
		DM::Sound_Erase(this->soundName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(Door* d)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->circuit = d;
		this->circuit->Set_SoundName(this->res->soundName);
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//回路でオープン状態でない時のみ開ける判定と		
		if (!this->circuit->Get_State())
		{
			//つながっているブレーカーを確認して開くかどうかを確認
			this->circuit->Door_Open();
			//フェーズ移行するかどうかを確認
			if (this->circuit->Is_Phase_Offset())
			{
				ge->OM.Game_Be_Final_Phase();
			}
		}		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{		
		ML::Mat4x4 matT, matS;
		matS.Scaling(ML::Vec3(chipX,100,chipZ));
		matT.Translation(this->circuit->Get_Pos());

		DG::EffectState().param.matWorld = matS * matT;

		DG::Mesh_Draw(this->res->meshName);

		this->Render3DShadow();
	}

	//-----------------------------------------------------------------------
	//追加メソッド
	//プレイヤとのあたり判定
	bool Object::Hit_Check(const ML::Box3D& hit)
	{
		//開いている状態なら当たらなかったことにして返す
		if (this->circuit->Get_State())
		{
			return false;
		}
		return this->circuit->Player_Hit_the_Door(hit);
	}
	//つながっているブレーカーの数確認
	int Object::How_Many_Breaker_Be_Cunnected()
	{
		return this->circuit->How_Many_Breaker_Be_Cunnected();
	}
	//
	void Object::Render3DShadow()
	{
		ML::Mat4x4 matT, matS;
		matS.Scaling(ML::Vec3(150,100,40));
		matT.Translation(ML::Vec3(this->circuit->Get_Pos().x,1, this->circuit->Get_Pos().z));
		DG::EffectState().param.matWorld = matS * matT;

		DG::Mesh_Draw(this->res->shadowMesh);
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Door* d)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize(d)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Door* d)
	{
		return  this->Initialize(d);
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