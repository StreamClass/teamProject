//-------------------------------------------------------------------
//ブレーカー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Breaker.h"
#include  "Task_Enemy.h"

namespace Task_Breaker
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "Breaker_Mesh";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/BreakerBase.SOB");
		this->buttonMeshName = "ButtonMesh";
		DG::Mesh_CreateFromSOBFile(this->buttonMeshName, "./data/mesh/BreakerButton.SOB");
		this->soundName = "ButtonPushSE";
		DM::Sound_CreateSE(this->soundName, "./data/sound/pushButton.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{		
		DG::Mesh_Erase(this->meshName);
		DG::Mesh_Erase(this->buttonMeshName);
		DM::Sound_Erase(this->soundName);
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
		this->pushedButton = false;
		this->pos = this->circuit->Get_Pos();
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
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		//ブレーカーベース
		ML::Mat4x4 matT, matS, matR;
		matS.Scaling(100);
		matT.Translation(this->circuit->Get_Pos());
		matR.RotationY(this->RotationY_Angle(this->angle));
		DG::EffectState().param.matWorld = matS * matR * matT;
		DG::Mesh_Draw(this->res->meshName);

		//ボタン
		ML::Mat4x4 bmatT;
		bmatT.Translation(this->Move_Button());
		DG::EffectState().param.matWorld = matS * matR * bmatT;
		DG::Mesh_Draw(this->res->buttonMeshName);
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
		//DM::Sound_Play(this->res->soundName,false);

		//起動したらエネミーの目的地を設定するようにする
		this->Now_Be_Pushed();
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
	//-----------------------------------------------------------------------
	//ボタンの移動
	ML::Vec3 Object::Move_Button()
	{
		if (this->circuit->Get_Now_State())
		{
			if (this->angle == 2)
			{
				this->pos.z += 1.0f;
				if (this->pos.z > this->circuit->Get_Pos().z + 15.0f)
				{
					this->pos.z = this->circuit->Get_Pos().z + 15.0f;
					if (!this->pushedButton)
					{
						DM::Sound_Play(this->res->soundName, false);
						this->pushedButton = true;
					}
				}
			}
			else
			{
				this->pos.z -= 1.0f;
				if (this->pos.z < this->circuit->Get_Pos().z - 15.0f)
				{
					this->pos.z = this->circuit->Get_Pos().z - 15.0f;
					if (!this->pushedButton)
					{
						DM::Sound_Play(this->res->soundName, false);
						this->pushedButton = true;
					}

				}
			}
		}
		return this->pos;
	}
	//
	bool Object::Get_Now_State()
	{
		return this->circuit->Get_Now_State();
	}
	//---------------------------------------------------------------------
	//プレイヤがボタンを押したことをエネミーに教える処理
	void Object::Now_Be_Pushed()
	{
		auto en = ge->GetTask_One_G<Enemy::Object>("エネミー");
		en->Set_Destination(this->pos);
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