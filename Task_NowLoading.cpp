//-------------------------------------------------------------------
//フェードイン・アウト
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_NowLoading.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_GameClear.h"
#include  "Task_GameOver.h"
#include  "Task_JecLogo.h"

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
		//時間カウンタが2秒以下なら
		if (this->timeCnt < 60 * 2)
		{
			//フェードインアウトの画像の不透明度の指定
			this->FadeInOut();
		}
		//0.5秒の時
		if (this->timeCnt == 60 * 0.5f)
		{
			//現在のタスクを指定しているとき
			if(this->nowTask != "")
				ge->KillAll_G(this->nowTask);//指定したタスクを解放
		}
		//1秒の時
		if (this->timeCnt == 60 * 1)
		{
			//次のタスクを指定していれば
			if (this->nextTask != "")
			{
				//タイトルを指定していれば
				if (this->nextTask == "タイトル")
				{
					//タイトルタスクを生成
					Title::Object::Create(true);
				}
				//ゲームを指定していれば
				else if (this->nextTask == "ゲーム")
				{
					//ゲームタスクを生成
					Game::Object::Create(true);
					ge->state = ge->game;
				}
				//ゲームオーバーを指定していれば
				else if (this->nextTask == "ゲームオーバー")
				{
					//ゲームオーバー画面を生成
					Over::Object::Create(true);
				}
				//ゲームクリアを指定していれば
				else if (this->nextTask == "ゲームクリア")
				{
					//ゲームクリア画面を生成
					Clear::Object::Create(true);
				}
				//デモ画面を指定していれば
				else if (this->nextTask == "デモ")
				{
					//ゲームタスクをデモ状態で生成
					Game::Object::Create(true);
					ge->state = ge->demo;
				}
				//ロゴ画面を指定していれば
				else if (this->nextTask == "日電ロゴ")
				{
					//ロゴタスクを生成
					JecLogo::Object::Create(true);
				}
			}
		}
		//不透明度が0未満になったら
		if (this->alpha < 0)
		{
			//タスクを解放
			this->Kill();
		}
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
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(this->alpha,this->rgb,this->rgb,this->rgb));
	}

	void  Object::Render3D_L0()
	{
	}
	//今のタスクを指定
	void Object::Set_NowTask(const string now)
	{
		//名前を保存
		this->nowTask = now;
		//保存したのが"ゲーム"なら
		if (this->nowTask == "ゲーム")
		{
			//ゲームの状態によって次のタスクを指定
			//クリアなら
			if (ge->state == ge->clear)
			{
				this->nextTask = "ゲームクリア";
			}
			//ゲームオーバーなら
			else if(ge->state == ge->over)
			{
				this->nextTask = "ゲームオーバー";
			}
			//デモなら
			else if (ge->state == ge->demo)
			{
				this->nextTask = "日電ロゴ";
			}
			//それ以外で呼び出していたら(gameかnon)
			else
			{
				this->nextTask = "タイトル";
			}
		}
	}
	//次のタスクを指定
	void Object::Set_NextTask(const string next)
	{
		this->nextTask = next;
	}
	//他のタスクから白か黒を指定(数値によっては灰色も可)
	void Object::Set_Color(float rgb)
	{
		this->rgb = rgb;
	}
	//フェードインアウト時の画像の不透明度の指定
	void Object::FadeInOut()
	{
		//タスク生成から2秒間かけて
		if (this->timeCnt < 60 * 1)
		{
			//不透明度を1に
			this->alpha = this->timeCnt / (60.0f * 0.5f);
			//不透明度の上限を指定
			if (this->alpha >= 1.0f)
			{
				this->alpha = 1.0f;
			}
		}
		//2~4秒の2秒間かけて
		else if (this->timeCnt < 60 * 2)
		{
			//不透明度を0に
			this->alpha = 1.0f - (this->timeCnt - 60) / (60 * 0.5f);
			//不透明度の上限を指定
			if (this->alpha >= 1.0f)
			{
				this->alpha = 1.0f;
			}
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