//-------------------------------------------------------------------
//エネミー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MiniMap.h"
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
		this->chasing_BG = "chasing_BG";
		DG::Image_Create(this->chasing_BG, "./data/image/chasingbg.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->chasing_BG);
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
		this->render2D_Priority[1] = 0.1f;
		this->rou = ge->OM.Create_Routine();
		this->demo = false;
		this->pos = ML::Vec3(chipX * 5, 20, chipZ * 13);
		this->speed = 10.0f;
		this->hitBase = ML::Box3D(-100, 0, -100, 200, 200, 200);
		this->angle = ML::Vec3(0, ML::ToRadian(-90),0);
		this->chasing_Speed = 14.0f;
		this->final_Phase_Speed = 13.0f;
		this->timeCnt = 0;
		this->ebone = new Bone(180, "Enemy");
		this->ebone->Moving(this->pos);
		this->ebone->Bone_RotateY_All(this->angle.y + ML::ToRadian(-90));
		this->Init_Enemys_Animations();
		this->priority_Position = ML::Vec3(0, 0, 0);

		//デモコーナー仮初期化
		/*for (int i = 0; i < 9; ++i)
		{
			this->demoCorner[i] = i;
		}*/
		//デモ用初期化
		this->demoCorner[0] = 0;
		this->demoCorner[1] = 1;
		this->demoCorner[2] = 7;
		this->demoCorner[3] = 12;
		this->demoCorner[4] = 11;
		this->demoCorner[5] = 6;
		this->demoCorner[6] = 2;
		this->demoCorner[7] = 1;
		this->demoCorner[8] = 1;
		//デモ用現在地の初期化
		this->demoNowCorner = 0;
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete this->ebone;
		this->ebone = nullptr;

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
		//デモ用処理
		if (ge->state == MyPG::MyGameEngine::State::demo)
		{
			auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
			//目的地
			ML::Vec3 targetPos = ML::Vec3(0, 0, 0);
			//コーナーに達した時
			if (rou->Judge(this->hitBase, this->pos))
			{
				//配列にあらかじめ用意したコーナーに移動させる
				rou->Set_Target(this->demoCorner[this->demoNowCorner]);
				//要素番号を進める
				this->demoNowCorner++;
			}
			//最後のカメラの前にたどり着くまでは
			//今の行き先へ移動
			if (this->demoNowCorner < 7)
			{
				targetPos = rou->Move(this->pos);

				//移動先の座標情報に移動量をかけて移動
				this->pos += targetPos * this->speed;
				//ボーンの移動、アニメ処理
				this->ebone->Moving(targetPos * this->speed);
				this->ebone->Set_Next_Motion(this->animations_Name[1]);
				this->angle.y = -atan2(targetPos.z, targetPos.x);
				//ボーンアップデート
				this->ebone->Bone_RotateY_All(this->angle.y + ML::ToRadian(90));
				this->ebone->Repeat_Now_Motioin();
				this->ebone->UpDate();

				if (!pl->Get_DebugOnOff())
					this->Player_HitCheck();
			}
		}
		else
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

				if (rou->Judge(this->hitBase, this->pos))
				{
					rou->Choice(rou->Get_Now());
				}

				targetPos = rou->Move(this->pos);

				if (rou->Is_Final_Phase() == false)
				{
					this->pos += targetPos * this->speed;
					this->ebone->Moving(targetPos * this->speed);
					//一定以上近いなら向かう場所を不定にする
					ML::Vec3 dist = this->pos - this->priority_Position;
					if (dist.Length() <= chipX*10.0f)
					{
						this->priority_Position = ML::Vec3(0, 0, 0);
						this->Set_Destination(ML::Vec3(0, 0, 0));
					}
				}
				else
				{
					this->pos += targetPos * this->final_Phase_Speed;
					this->ebone->Moving(targetPos * this->final_Phase_Speed);
				}
				this->ebone->Set_Next_Motion(this->animations_Name[1]);

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
				else if (this->timeCnt % 3 == 0)
				{
					//センサーチェック
					this->system.SensorCheck(pl->Get_HitBase().OffsetCopy(pl->Get_Pos()), pl->Get_Pos(), this->pos, this->angle.y);
				}

				//今の目的地と一定距離以内なら新しい目的地を設定
				if (ML::Vec3(this->toVec - this->pos).Length() <= this->chasing_Speed)
				{
					this->toVec = this->system.NextRoute();
				}
				//ルーチンワークに戻るときの動作
				if (this->toVec.Is_Zero_Vec())
				{
					//センサーチェック
					this->system.SensorCheck(pl->Get_HitBase().OffsetCopy(pl->Get_Pos()), pl->Get_Pos(), this->pos, this->angle.y);
				}
				targetPos = this->toVec - this->pos;
				//移動
				this->pos += targetPos.Normalize() * this->chasing_Speed;
				this->ebone->Moving(targetPos.Normalize() * this->chasing_Speed);
				this->ebone->Set_Next_Motion(this->animations_Name[0]);
				//向きをプレイヤ側にする
				ML::Vec3 a = pl->Get_Pos() - this->pos;
				this->angle.y = -atan2(a.z, a.x);

				//カウント上昇
				this->timeCnt++;
			}


			//ボーンアップデート
			this->ebone->Bone_RotateY_All(this->angle.y + ML::ToRadian(90));
			this->ebone->Repeat_Now_Motioin();
			this->ebone->UpDate();

			if (!pl->Get_DebugOnOff())
				this->Player_HitCheck();
		}
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//チェイスモードの時はプレイヤに知らせる
		if (this->system.Is_Chase_Mode())
		{
			ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
			ML::Box2D src;
			//イメージ全体サイズをもらう
			POINT imgsize = DG::Image_Size(this->res->chasing_BG);
			src = ML::Box2D(0, 0, imgsize.x, imgsize.y);
			//描画
			DG::Image_Draw(this->res->chasing_BG, draw, src,ML::Color(0.4f,1,1,1));
		}
	}
	void  Object::Render3D_L0()
	{
		//エネミーのメッシュを表示		
		this->ebone->Render();
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
	//
	//Bone* Object::Get_EnemyBonePtr()
	//{
	//	return this->ebone;
	//}
	//
	ML::Vec3 Object::Get_Pos()
	{
		return this->pos;
	}
	//-------------------------------------------------------------------------------
	//アニメーション生成メソッド
	void Object::Init_Enemys_Animations()
	{
		//走るアニメーション
		this->animations_Name.push_back("Running");
		std::vector<Motion::Motion_Data> running;
		Motion::Make_Motion(&running, this->animations_Name[0]);
		this->ebone->Registrate_Motion(running, this->animations_Name[0]);
		//歩くアニメーション
		this->animations_Name.push_back("Walking");
		std::vector<Motion::Motion_Data> walking;
		Motion::Make_Motion(&walking, this->animations_Name[1]);
		this->ebone->Registrate_Motion(walking, this->animations_Name[1]);
	}
	//--------------------------------------------------------------------------------
	//向かわせる場所を設定する処理
	void Object::Set_Destination(const ML::Vec3& des)
	{
		//ルーチンシステムとエネミ－本人の目的地を設定
		this->rou->Set_Priority_Position(des);
		this->priority_Position = des;
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