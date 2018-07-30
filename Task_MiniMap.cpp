//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MiniMap.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"

namespace  MiniMap
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//イメージ名の指定
		this->imageName = "MapImg";
		this->plImgName = "PlayerImg";
		this->caImgName = "CameraImg";
		this->anImgName = "Chara_Angle";
		//各画像の読み込み
		DG::Image_Create(this->imageName, "./data/image/マップ00.png");
		DG::Image_Create(this->plImgName, "./data/image/Player_MiniMap.png");
		DG::Image_Create(this->caImgName, "./data/image/Camera_MiniMap.png");
		DG::Image_Create(this->anImgName, "./data/image/Chara_Angle.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->plImgName);
		DG::Image_Erase(this->caImgName);
		DG::Image_Erase(this->anImgName);
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
		this->render2D_Priority[1] = 0.5f;
		//プレイヤ用変数の初期化
		this->plpos = ML::Vec2(0, 0);
		this->plAngle = 0;
		//カメラ用変数の初期化
		this->capos = ML::Vec2(0, 0);
		this->caAngle = 0;
		//最初から見るように設定
		this->viewFlag = true;
		//タブレットは使っていない
		this->tab_use_now = false;
		//座標参照用倍率
		this->magni = chipX / NORMALMAGNI;
		//ミニマップ表示サイズを通常状態に設定
		this->mapSize = NORMALMAPSIZE;
		//デバックモードの初期化(無効)
		this->debugMode = false;
		//エネミーの座標
		this->epos = ML::Vec2(0, 0);
		//エネミーの向き
		this->eangle = 0;
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
		//プレイヤの変数等を使えるように呼び出す
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		//タブレットの使用状況をプレイヤから受け取る
		this->tab_use_now = pl->Is_Used_Tablet();
		//デバッグモードか否かをプレイヤーから受け取る
		this->debugMode = pl->Get_DebugOnOff();
		//
		this->magni = 150.0f / NORMALMAGNI;
		this->mapSize = NORMALMAPSIZE;
		//
		if (this->tab_use_now)
		{
			this->magni = float(chipX) / TABLETMAGNI;
			this->mapSize = TABLETMAPSIZE;
		}
		//プレイヤ本体からミニマップ上の情報を参照
		this->plpos = ML::Vec2((int)pl->Get_Pos().x / this->magni, this->mapSize - (int)pl->Get_Pos().z / this->magni);
		this->plAngle = (float)pl->Get_Angle().y + ML::ToRadian(90);
		//カメラからミニマップ上の情報を参照
		this->capos = ML::Vec2((int)ge->camera[0]->pos.x / this->magni, this->mapSize - (int)ge->camera[0]->pos.z / this->magni);
		ML::Vec2 a = ML::Vec2(ge->camera[0]->target.x - ge->camera[0]->pos.x, ge->camera[0]->target.z - ge->camera[0]->pos.z);
		this->caAngle = -atan2(a.y,a.x) + ML::ToRadian(90);
		//エネミーからミニマップ上での情報を参照
		auto e = ge->GetTask_One_G<Enemy::Object>("エネミー");
		this->epos = ML::Vec2((int)e->pos.x / this->magni, this->mapSize - (int)e->pos.z / this->magni);
		this->eangle = e->angle.y + ML::ToRadian(90);

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ミニマップを表示していなかったら
		if (!this->MiniMap_View())
		{
			return;
		}
		//
		ML::Vec2 cen(16, 24);
		//
		ML::Box2D pdraw(-8 + 60, -8 + 60, 16, 16);
		ML::Box2D psrc(0, 0, 63, 63);
		//
		ML::Box2D cdraw = pdraw;
		ML::Box2D csrc(0, 0, 63, 63);
		//
		ML::Box2D edraw = pdraw;;
		ML::Box2D esrc(0, 0, 63, 63);
		//
		ML::Box2D aDraw(-16 + 60, -8 + 60, 32, 16);
		ML::Box2D aSrc(0, 0, 63, 45);
		//
		float alpha = 1;
		if (this->tab_use_now)
		{
			cen = ML::Vec2(20, 30);
			pdraw = ML::Box2D(-10 + 60, -10 + 60, 20, 20);
			cdraw = pdraw;
			edraw = pdraw;
			aDraw = ML::Box2D(-20 + 60, -10 + 60, 40, 20);
			alpha = 0.4f;
		}
		ML::Box2D draw(60, 60, this->mapSize, this->mapSize);
		ML::Box2D src(0, 0, 500, 500);
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(alpha, 1, 1, 1));
		//
		aDraw.Offset(this->capos - ML::Vec2(0, float(aDraw.h)));
		DG::Image_Rotation(this->res->anImgName, this->caAngle, cen);
		DG::Image_Draw(this->res->anImgName, aDraw, aSrc, ML::Color(0.3f, 1, 1, 1));
		//
		pdraw.Offset(this->plpos);
		DG::Image_Draw(this->res->plImgName, pdraw, psrc, ML::Color(alpha, 1, 1, 1));
		if (this->tab_use_now)
		{
			//
			cdraw.Offset(this->capos);
			DG::Image_Draw(this->res->caImgName, cdraw, csrc, ML::Color(alpha, 1, 1, 1));
		}
		//
		edraw.Offset(this->epos);
		if (this->debugMode)
		{
			DG::Image_Draw(this->res->plImgName, edraw, esrc, ML::Color(alpha, 1, 1, 0));
		}
	}
	//-------------------------------------------------------------------
	//「3Ｄ描画」１フレーム毎に行う処理
	void  Object::Render3D_L0()
	{
	}
	//ミニマップを表示するか判断
	bool Object::MiniMap_View()
	{
		return this->viewFlag;
	}
	//プレイヤからミニマップを表示するか指定
	void Object::Set_MiniMap_View()
	{
		this->viewFlag = !this->viewFlag;
	}
	//ミニマップの描画
	//画像描画範囲の設定を未実装
	void Object::MiniMap_Render()
	{
		ML::Box2D draw(60, 60, this->mapSize, this->mapSize);
		ML::Box2D src(0, 0, 50, 50);
		//描画位置の変更

		//リソースの参照位置を変更
		DG::Image_Draw(this->res->imageName, draw, src);
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