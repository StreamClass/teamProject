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
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->render2D_Priority[1] = 0.5f;
		//イメージ名の指定
		this->imageName = "MapImg";
		this->plImgName = "PlayerImg";
		this->caImgName = "CameraImg";
		//各画像の読み込み
		DG::Image_Create(this->imageName, "./data/image/マップ00.png");
		DG::Image_Create(this->plImgName, "./data/image/Player_MiniMap.png");
		DG::Image_Create(this->caImgName, "./data/image/Camera_MiniMap.png");
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
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->plImgName);
		DG::Image_Erase(this->caImgName);

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
		//プレイヤの変数等を使えるように呼び出す
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		//プレイヤ本体からミニマップ上の情報を参照
		this->plpos = ML::Vec2((int)pl->Get_Pos().x / 20, 500 - (int)pl->Get_Pos().z / 20);
		this->plAngle = (float)pl->Get_Angle().y + ML::ToRadian(90);
		//タブレットの使用状況をプレイヤから受け取る
		this->tab_use_now = pl->Is_Used_Tablet();
		//タブレットを使っていたら
		if (this->tab_use_now == true)
		{
			//カメラからミニマップ上の情報を参照
			this->capos = ML::Vec2((int)ge->camera[0]->pos.x / 20, 500 - (int)ge->camera[0]->pos.z / 20);
			ML::Vec2 a = ML::Vec2(ge->camera[0]->target.x - ge->camera[0]->pos.x, ge->camera[0]->target.z - ge->camera[0]->pos.z);
			this->caAngle = -atan2(a.y,a.x) + ML::ToRadian(90);
		}

		//デバック用
		//エネミーからミニマップ上での情報を参照
		auto e = ge->GetTask_One_G<Enemy::Object>("エネミー");
		this->epos = ML::Vec2((int)e->pos.x / 20, 500 - (int)e->pos.z / 20);
		this->eangle = e->angle.y + ML::ToRadian(90);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ミニマップを表示していたら
		if (this->MiniMap_View() == true)
		{
			//ミニマップ
			ML::Box2D draw(0, 0, 500, 500);
			ML::Box2D src(0, 0, 500, 500);
			DG::Image_Draw(this->imageName, draw, src);
			//プレイヤ位置
			draw = ML::Box2D(-5, -7, 9, 13);
			src = ML::Box2D(0, 0, 50, 50);
			draw.Offset(this->plpos);
			DG::Image_Rotation(this->plImgName, this->plAngle, ML::Vec2(5, 10));
			DG::Image_Draw(this->plImgName, draw, src);
			//タブレットを使用していたら
			if (this->tab_use_now == true)
			{
				draw = ML::Box2D(5, -7, 9, 13);
				draw.Offset(this->capos);
				DG::Image_Rotation(this->caImgName, this->caAngle, ML::Vec2(5, 10));
				DG::Image_Draw(this->caImgName, draw, src);
			}

			//デバッグ用
			//エネミー描画
			draw = ML::Box2D(5, -7, 9, 13);
			draw.Offset(this->epos);
			DG::Image_Rotation(this->plImgName, this->eangle, ML::Vec2(5, 10));
			DG::Image_Draw(this->plImgName, draw, src,ML::Color(1,1,1,0));
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