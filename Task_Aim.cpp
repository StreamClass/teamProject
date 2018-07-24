//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Aim.h"
#include  "Task_Player.h"
#include  "Task_Map.h"

namespace  Aiming
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//エイムの画像
		this->imageName[0] = "AimCenterImg";
		DG::Image_Create(this->imageName[0], "./data/image/AimCenter.png");
		this->imageName[1] = "AimUDImg";
		DG::Image_Create(this->imageName[1], "./data/image/aimTB.png");
		this->imageName[2] = "AimLRImg";
		DG::Image_Create(this->imageName[2], "./data/image/aimLR.png");
		//通常時の操作説明
		this->normalModeImg[0] = "Nomal01Img";
		DG::Image_Create(this->normalModeImg[0], "./data/image/NormalImg00.png");
		this->normalModeImg[1] = "Nomal02Img";
		DG::Image_Create(this->normalModeImg[1], "./data/image/NormalImg01.png");
		//タブレット時の操作説明
		this->tabletModeImg[0] = "Tablet01Img";
		DG::Image_Create(this->tabletModeImg[0], "./data/image/TabletImg01.png");
		this->tabletModeImg[1] = "Tablet02Img";
		DG::Image_Create(this->tabletModeImg[1], "./data/image/TabletImg00.png");
		//スタミナの画像
		this->staminaImgName[0] = "StaminaMax";
		DG::Image_Create(this->staminaImgName[0], "./data/image/StaminaMax.png");
		this->staminaImgName[1] = "StaminaGage";
		DG::Image_Create(this->staminaImgName[1], "./data/image/StaminaGage.png");

		this->pushButtonImg = "AButton";
		DG::Image_Create(this->pushButtonImg, "./data/image/PushButton.png");
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
			DG::Image_Erase(this->normalModeImg[i]);
			DG::Image_Erase(this->tabletModeImg[i]);
			DG::Image_Erase(this->staminaImgName[i]);
		}
		DG::Image_Erase(this->pushButtonImg);
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
		this->hitBase = ML::Box3D(0, 0, 0, 1, 1, 1);
		this->aimPosC = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2);
		this->aimPosT = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2 - 15);
		this->aimPosB = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2 + 15);
		this->aimPosL = ML::Vec2(ge->screen2DWidth / 2 - 15, ge->screen2DHeight / 2);
		this->aimPosR = ML::Vec2(ge->screen2DWidth / 2 + 15, ge->screen2DHeight / 2);
		this->timeCnt = 0;
		this->pos = ML::Vec3(0, 0, 0);
		this->aimMoveSpeed = 0;
		this->aimMovetremor = 5.0f;
		//DG::Mesh_CreateFromSOBFile("t", "./data/mesh/box1.sob");
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		//DG::Mesh_Erase("t");

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
			this->aimMoveSpeed = 5.0f;
			this->aimMovetremor = pl->Get_MoveSpeed() + 5.0f;
			if (pl->Is_Tired())
			{
				this->aimMoveSpeed = (MAX_STAMINA / 3 - pl->Get_Stamina()) / 10.0f;
				this->aimMovetremor =  (pl->Get_MoveSpeed() + 5.0f) * 5.0f;
			}
			this->moveCnt += this->aimMoveSpeed;
			this->aimPosT.y = -sin(ML::ToRadian(this->moveCnt)) * this->aimMovetremor + (ge->screen2DHeight / 2.0f - (15 + this->aimMovetremor));
			this->aimPosB.y =  sin(ML::ToRadian(this->moveCnt)) * this->aimMovetremor + (ge->screen2DHeight / 2.0f + (15 + this->aimMovetremor));
			this->aimPosL.x = -sin(ML::ToRadian(this->moveCnt)) * this->aimMovetremor + (ge->screen2DWidth / 2.0f - (15 + this->aimMovetremor));
			this->aimPosR.x =  sin(ML::ToRadian(this->moveCnt)) * this->aimMovetremor + (ge->screen2DWidth / 2.0f + (15 + this->aimMovetremor));
			if (pl->Get_MoveSpeed() >= -1.0f && pl->Get_MoveSpeed() <= 1.0f && pl->Is_Tired() == false)
			{
				this->moveCnt = 0;
			}
		}
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (pl->Is_Used_Tablet() == true)
		{
			this->TabletModeRrender();
			return;
		}
		this->NormalModeRrender();
		this->AimingRender();
		this->StaminaRender();
		if (pl->Get_Touch_Breaker())
		{
			ML::Box2D draw(ge->screen2DWidth / 2 + 100, ge->screen2DHeight / 2 + 100, 100, 100);
			ML::Box2D src(0, 0, 200, 200);
			DG::Image_Draw(this->res->pushButtonImg, draw, src);
		}
	}
	//-------------------------------------------------------------------
	//「3Ｄ描画」１フレーム毎に行う処理
	void  Object::Render3D_L0()
	{
		//auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		//ML::Mat4x4 matT, matS;
		//matT.Translation(this->pos + ML::Vec3(pl->Get_Pos().x,0,pl->Get_Pos().z));
		//matS.Scaling(ML::Vec3(this->hitBase.w / 100, this->hitBase.h / 100, this->hitBase.d / 100));
		//DG::EffectState().param.matWorld = matS * matT;
		//DG::Mesh_Draw("t");
	}
	//-------------------------------------------------------------------
	//通常時の操作説明
	void Object::NormalModeRrender()
	{
		ML::Box2D draw(0, 1020, 1920, 60);
		ML::Box2D src(0, 0, 1920, 60);
		DG::Image_Draw(this->res->normalModeImg[(this->timeCnt / 20) % 2], draw, src);
	}
	//-------------------------------------------------------------------
	//タブレット時の操作説明
	void Object::TabletModeRrender()
	{
		ML::Box2D draw(0, 1020, 1920, 60);
		ML::Box2D src(0, 0, 1920, 60);
		DG::Image_Draw(this->res->tabletModeImg[(this->timeCnt / 20) % 2], draw, src);
	}
	//-------------------------------------------------------------------
	//エイムのアニメーション
	void Object::AimingRender()
	{
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
	//-------------------------------------------------------------------
	//スタミナの描画
	void Object::StaminaRender()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		
		ML::Box2D draw(ge->screen2DWidth - 100, ge->screen2DHeight - 300, 50, 240);
		ML::Box2D src(0, 0, 200, 500);
		DG::Image_Draw(this->res->staminaImgName[0], draw, src);

		draw = ML::Box2D(ge->screen2DWidth - 100, ge->screen2DHeight - 300 + (MAX_STAMINA -  pl->Get_Stamina()), 50, pl->Get_Stamina());
		DG::Image_Draw(this->res->staminaImgName[1], draw, src);
	}
	//-------------------------------------------------------------------
	//ブレーカーとの接触判定
	ML::Box3D Object::Get_HitBase()
	{
		return this->hitBase;
	}
	//-------------------------------------------------------------------
	//あたリ判定の３次元座標設定
	void Object::Set_Pos(ML::Vec3& pos_)
	{
		this->pos = pos_;
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