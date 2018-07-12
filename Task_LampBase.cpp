//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_LampBase.h"
#include  "Task_Door.h"
#include  "Task_LampGlass.h"

namespace  LampBase
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//ランプベース
		this->meshName = "LampMeshName";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/LampBase.SOB");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
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
		this->pos = ML::Vec3(0, 0, 0);
		this->hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
		this->color = ML::Color(1, 1, 0, 0);

		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("ランプ");

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto door = ge->GetTask_One_GN<Task_Door::Object>("ドア", "出口");
		auto lamps = ge->GetTask_Group_G<LampGlass::Object>("ランプ");
		int lampNum = 0;
		for (auto it = lamps->begin(); it != lamps->end(); ++it,++lampNum)
		{
			if (lampNum >= door->How_Many_Breaker_Be_Cunnected())
			{
				break;
			}
			(*it)->Set_Color(this->color);
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}

	//-------------------------------------------------------------------
	//「3Ｄ描画」１フレーム毎に行う処理
	void  Object::Render3D_L0()
	{
		//ランプの基盤描画
		ML::Mat4x4 matT, matS;
		matT.Translation(this->pos);
		matS.Scaling(100);
		DG::EffectState().param.matWorld = matS * matT;
		DG::Mesh_Draw(this->res->meshName);
	}
	//-------------------------------------------------------------------
	//ランプの座標・当たり判定の初期化
	void Object::Set_Lamp(const ML::Vec3& pos, const ML::Box3D& hitBase)
	{
		//基盤の初期化
		this->pos = pos + ML::Vec3(-50.0f, 0, 0);
		this->hitBase = hitBase;
		//ランプの生成
		for (int i = 0; i < 3; ++i)
		{
			auto lamp = LampGlass::Object::Create(true);
			lamp->Set_Pos(this->pos + ML::Vec3((i - 1) * 60.0f, 0, 0));
		}
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