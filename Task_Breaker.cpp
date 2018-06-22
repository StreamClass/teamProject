//-------------------------------------------------------------------
//ブレーカー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Breaker.h"

namespace Task_Breaker
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "Breaker_Mesh";
		//仮のメッシュ
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/BreakerBase.SOB");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{		
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(Breaker* b, int angle)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->circuit = b;

		this->angle = angle;
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
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT, matS, matR;
		matS.Scaling(100);
		matT.Translation(this->circuit->Get_Pos());
		matR.RotationY(this->RotationY_Angle(this->angle));

		DG::EffectState().param.matWorld = matS * matR * matT;

		DG::Mesh_Draw(this->res->meshName);
	}
	//-----------------------------------------------------------------------
	//プレイヤとのあたり判定
	bool Object::Hit_Check(const ML::Box3D& hit)
	{
		return this->circuit->Player_Touch_Breaker(hit);
	}
	//-----------------------------------------------------------------------
	//ブレーカー起動
	void Object::ActivateBreaker()
	{
		this->circuit->Activate_Breaker();
	}
	//-----------------------------------------------------------------------
	//向きの設定
	float Object::RotationY_Angle(int& angle)
	{
		float angle_;
		if (this->angle == 2)
		{
			angle_= ML::ToRadian(0);
		}
		else if (this->angle == 3)
		{
			angle_ = ML::ToRadian(180);
		}
		return angle_;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Breaker* b, int angle)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize(b, angle)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Breaker* b, int angle)
	{
		return  this->Initialize(b, angle);
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