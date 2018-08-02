//-------------------------------------------------------------------
//フェードイン・アウト
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_NowLoading.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_GameClear.h"
#include  "Task_GameOver.h"

namespace  Loading
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//画像読み込み
		this->imageName = "LoadingImg";
		DG::Image_Create(this->imageName, "./data/image/Loading.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		//画像の解放
		DG::Image_Erase(this->imageName);
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
		this->timeCnt = 0;
		this->alpha = 0.0f;
		this->color = ML::Color(0, 0, 0, 0);
		this->nowTask = "";
		this->nextTask = "";
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
		//
		if (this->timeCnt < 60 * 4)
		{
			this->FadeInOut();
		}
		//
		if (this->timeCnt == 60)
		{
			if(this->nowTask != "")
				ge->KillAll_G(this->nowTask);
		}
		//
		if (this->timeCnt == 60 * 2)
		{
			//
			if (this->nextTask != "")
			{
				//
				if (this->nextTask == "タイトル")
				{
					Title::Object::Create(true);
				}
				//
				else if (this->nextTask == "ゲーム")
				{
					Game::Object::Create(true);
				}
				//
				else if (this->nextTask == "ゲームオーバー")
				{
					Over::Object::Create(true);
				}
				//
				else if (this->nextTask == "ゲームクリア")
				{
					Clear::Object::Create(true);
				}
				//
				else if (this->nextTask == "デモ")
				{

				}
				//
				else if (this->nextTask == "日電ロゴ")
				{

				}
			}
		}
		//不透明度が0未満になったら
		if (this->alpha < 0)
		{
			//タスクを解放
			this->Kill();
		}
		//不透明度の上限を指定
		if (this->alpha >= 1.0f)
		{
			this->alpha = 1.0f;
		}
		//ローディング画面の不透明度・色を指定
		this->color = ML::Color(this->alpha, this->rgb, this->rgb, this->rgb);
		//フレーム数をカウント
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ローディング画面描画
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 160, 90);
		DG::Image_Draw(this->res->imageName, draw, src, this->color);
	}

	void  Object::Render3D_L0()
	{
	}
	//
	void Object::Set_NowTask(const string now)
	{
		this->nowTask = now;
		if (this->nowTask == "ゲーム")
		{
			if (ge->state == ge->clear)
			{
				this->nextTask = "ゲームクリア";
			}
			else if(ge->state == ge->over)
			{
				this->nextTask = "ゲームオーバー";
			}
			else
			{
				this->nextTask = "タイトル";
			}
		}
	}
	//
	void Object::Set_NextTask(const string next)
	{
		this->nextTask = next;
	}
	//他のタスクからRGB値を指定
	void Object::Set_Color(float rgb)
	{
		this->rgb = rgb;
	}
	//
	void Object::FadeInOut()
	{
		//タスク生成から2秒間かけて
		if (this->timeCnt < 60 * 2)
		{
			//不透明度を1に
			this->alpha += this->timeCnt / 120.f;
		}
		//2~4秒の2秒間かけて
		else if (this->timeCnt < 60 * 4)
		{
			//不透明度を0に
			this->alpha -= (this->timeCnt - 120) / 120.0f;
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