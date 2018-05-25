//-------------------------------------------------------------------
//エネミー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"

namespace  Enemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "enemyMesh";
		DG::Mesh_CreateFromSOBFile("enemyMesh", "./data/mesh/char_Stand.SOB");
		//-------------------------------------------------------------------
		this->searchMesh = "enemySearch";
		DG::Mesh_CreateFromSOBFile("enemySearch", "./data/mesh/box1.sob");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
		DG::Mesh_Erase(this->searchMesh);
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
		//仮の初期座標
		//ゴール前
		this->pos = ML::Vec3(500, 0, 9500);
		this->searchBase = ML::Box3D(-250, -100, -250, 500, 200, 500);

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
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");

		//探知判定用矩形を用意
		ML::Box3D me = this->searchBase;
		//プレイヤの判定矩形を用意して接触判定

		this->toVec = pl->Get_Pos() - this->pos;

		this->pos += this->toVec*0.05f;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

	}
	void  Object::Render3D_L0()
	{
		//エネミーのメッシュを表示
		ML::Mat4x4 matS,matT;
		matS.Scaling(ML::Vec3(2, 1.5f, 2));
		matT.Translation(this->pos);
		DG::EffectState().param.matWorld = matS * matT;
		DG::Mesh_Draw(this->res->meshName);
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