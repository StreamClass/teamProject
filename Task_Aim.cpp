//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Aim.h"
#include  "Task_Player.h"

namespace  Aiming
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName[0] = "AimCenterImg";
		DG::Image_Create(this->imageName[0], "./data/image/AimCenter.png");
		this->imageName[1] = "AimUDImg";
		DG::Image_Create(this->imageName[1], "./data/image/aimTB.png");
		this->imageName[2] = "AimLRImg";
		DG::Image_Create(this->imageName[2], "./data/image/aimLR.png");

		this->controrlImg[0] = "NomalImg";
		DG::Image_Create(this->controrlImg[0], "./data/image/NormalImg.png");
		this->controrlImg[1] = "TabletImg";
		DG::Image_Create(this->controrlImg[1], "./data/image/TabletImg.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 3; ++i)
		{
			DG::Image_Erase(this->imageName[i]);
		}
		for (int i = 0; i < 2; ++i)
		{
			DG::Image_Erase(this->controrlImg[i]);
		}
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
		this->render2D_Priority[1] = 0.2f;
		this->hitBase = ML::Box3D(0, 0, 0, 100, 10, 10);
		this->aimPosC = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2);
		this->aimPosT = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2 - 15);
		this->aimPosB = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2 + 15);
		this->aimPosL = ML::Vec2(ge->screen2DWidth / 2 - 15, ge->screen2DHeight / 2);
		this->aimPosR = ML::Vec2(ge->screen2DWidth / 2 + 15, ge->screen2DHeight / 2);
		this->aimMoveMax = 20.0f;
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
		if (pl->Is_Used_Tablet() == false)
		{
			this->aimPosT.y = -sin(ML::ToRadian(this->timeCnt)) * (pl->Get_MoveSpeed() + 5.0f) + (ge->screen2DHeight / 2.0f - (15 + (pl->Get_MoveSpeed() + 5.0f)));
			this->aimPosB.y =  sin(ML::ToRadian(this->timeCnt)) * (pl->Get_MoveSpeed() + 5.0f) + (ge->screen2DHeight / 2.0f + (15 + (pl->Get_MoveSpeed() + 5.0f)));
			this->aimPosL.x = -sin(ML::ToRadian(this->timeCnt)) * (pl->Get_MoveSpeed() + 5.0f) + (ge->screen2DWidth / 2.0f - (15 + (pl->Get_MoveSpeed() + 5.0f)));
			this->aimPosR.x =  sin(ML::ToRadian(this->timeCnt)) * (pl->Get_MoveSpeed() + 5.0f) + (ge->screen2DWidth / 2.0f + (15 + (pl->Get_MoveSpeed() + 5.0f)));
			this->timeCnt++;
			if (pl->Get_MoveSpeed() >= -1.0f && pl->Get_MoveSpeed() <= 1.0f )
			{
				this->timeCnt = 0;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (pl->Is_Used_Tablet() == true)
		{
			this->TabletMode();
			return;
		}
		this->NormalMode();
		//注視点
		ML::Box2D draw(-5, -5, 9, 9);
		ML::Box2D src(0, 0, 9, 9);
		draw.Offset(this->aimPosC);
		DG::Image_Draw(this->res->imageName[0], draw, src);
		//エイム上
		draw = ML::Box2D(-3, -10, 5, 19);
		src = ML::Box2D(0, 0, 9, 39);
		draw.Offset(this->aimPosT);
		DG::Image_Draw(this->res->imageName[1], draw, src);
		//エイム下
		draw = ML::Box2D(-3, -10, 5, 19);
		draw.Offset(this->aimPosB);
		DG::Image_Draw(this->res->imageName[1], draw, src);
		//エイム右
		draw = ML::Box2D(-10, -3, 19, 5);
		src = ML::Box2D(0, 0, 39, 9);
		draw.Offset(this->aimPosR);
		DG::Image_Draw(this->res->imageName[2], draw, src);
		//エイム左
		draw = ML::Box2D(-10, -3, 19, 5);
		draw.Offset(this->aimPosL);
		DG::Image_Draw(this->res->imageName[2], draw, src);
	}

	void  Object::Render3D_L0()
	{
	}

	void Object::NormalMode()
	{
		ML::Box2D draw(0, 1020, 1920, 60);
		ML::Box2D src(0, 0, 1920, 60);
		DG::Image_Draw(this->res->controrlImg[0], draw, src);
	}
	void Object::TabletMode() 
	{
		ML::Box2D draw(0, 1020, 1920, 60);
		ML::Box2D src(0, 0, 1920, 60);
		DG::Image_Draw(this->res->controrlImg[1], draw, src);
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