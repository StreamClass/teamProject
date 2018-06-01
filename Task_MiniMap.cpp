//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MiniMap.h"
#include  "Task_Player.h"

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
		this->imageName = "MapImg";
		this->plImgName = "PlayerImg";
		this->caImgName = "CameraImg";
		DG::Image_Create(this->imageName, "./data/image/マップ00.png");
		DG::Image_Create(this->plImgName, "./data/image/MiniMap_Player.bmp");
		DG::Image_Create(this->caImgName, "./data/image/MiniMap_Camera.bmp");
		this->plpos = ML::Vec2(0, 0);
		this->capos = ML::Vec2(0, 0);
		this->viewFlag = true;
		this->tab_use_now = false;
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
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");

		this->plpos = ML::Vec2((int)pl->Get_Pos().x / 20, 500 - (int)pl->Get_Pos().z / 20);
		this->tab_use_now = pl->Is_Used_Tablet();
		if (this->tab_use_now == true)
		{
			this->capos = ML::Vec2((int)ge->camera[0]->pos.x / 20, 500 - (int)ge->camera[0]->pos.z / 20);	
		}
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
			draw = ML::Box2D(-3, -3, 5, 5);
			src = ML::Box2D(0, 0, 5, 5);
			draw.Offset(this->plpos);
			DG::Image_Draw(this->plImgName, draw, src);
			//タブレットを使用していたら
			if (this->tab_use_now == true)
			{
				draw = ML::Box2D(-3, -3, 5, 5);
				draw.Offset(this->capos);
				DG::Image_Draw(this->caImgName, draw, src);
			}
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