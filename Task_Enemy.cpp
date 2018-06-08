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
		this->rou = ge->OM.Create_Routine();

		this->pos = ML::Vec3(500, 50, 9500);
		this->speed = 10.0f;
		this->hitBase = ML::Box3D(-100, 0, -100, 200, 200, 200);
		this->angle = ML::Vec3(0, ML::ToRadian(90), ML::ToRadian(-10));
		this->chasing_Speed = 16;
		this->timeCnt = 0;

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

		//目的地
		ML::Vec3 targetPos = ML::Vec3(0, 0, 0);

		//探知判定用矩形を用意
		//ML::Box3D me = this->searchBase;
		//プレイヤの判定矩形を用意して接触判定

		//移動先をプレイヤの位置を参照して指定
		//this->toVec = pl->Get_Pos() - this->pos;
		//座標を移動先までの距離を20分の一ずつ移動
		//this->pos += this->toVec * 0.02f;

		//センサー行動
		if (this->system.Is_Chase_Mode() == false)
		{
			//通常モードの時
			//センサーチェック
			this->system.SensorCheck(pl->Get_HitBase().OffsetCopy(pl->Get_Pos()), pl->Get_Pos(), this->pos, this->angle.y);
			this->timeCnt = 0;

			if (rou->Judge(this->hitBase,this->pos))
			{
				rou->Choice(rou->Get_Now());
			}

			targetPos = rou->Move(this->pos);

			this->pos += targetPos * this->speed;

			
			this->angle.y = -atan2(targetPos.z, targetPos.x);
		}
		else
		{
			//追跡モード
			if (this->timeCnt == 0)
			{
				//目的地設定
				this->toVec = this->system.NextRoute();
			}
			else if (this->timeCnt % 30 == 0)
			{
				//センサーチェック
				this->system.SensorCheck(pl->Get_HitBase().OffsetCopy(pl->Get_Pos()), pl->Get_Pos(), this->pos, this->angle.y);
			}

			//今の目的地と一定距離以内なら新しい目的地を設定
			if (ML::Vec3(this->toVec - this->pos).Length() <= this->chasing_Speed)
			{
				this->toVec = this->system.NextRoute();
			}

			targetPos = this->toVec - this->pos;
			//移動
			this->pos += targetPos.Normalize() * this->chasing_Speed;

			//向きをプレイヤ側にする
			ML::Vec3 a = pl->Get_Pos() - this->pos;
			this->angle.y = -atan2(a.z, a.x);

			//カウント上昇
			this->timeCnt++;
		}		
		if (this->toVec != ML::Vec3(0, 0, 0))
		{
			//this->angle.y = atan2(this->toVec.z, this->toVec.x);
		}
		this->Player_HitCheck();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

	}
	void  Object::Render3D_L0()
	{
		//エネミーのメッシュを表示
		ML::Mat4x4 matS, matR, matRy, matRz, matT;
		matS.Scaling(ML::Vec3(2, 1.5f, 2));
		matRy.RotationY(this->angle.y);
		matRz.RotationZ(this->angle.z);
		matR = matRz * matRy;
		matT.Translation(this->pos);
		DG::EffectState().param.matWorld = matS * matR * matT;
		DG::EffectState().param.objectColor = ML::Color( 1, 0.6f, 0.2f, 0.2f);
		DG::Mesh_Draw(this->res->meshName);
		DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
	}

	//プレイヤとのあたり判定
	void Object::Player_HitCheck()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (this->hitBase.OffsetCopy(this->pos).Hit(pl->Get_HitBase().OffsetCopy(pl->Get_Pos())))
		{
			ge->state = ge->over;
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