//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_NowLoading.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->bgImgName = "TitleImg";
		DG::Image_Create(this->bgImgName, "./data/image/TitleBG01.bmp");
		this->loImgName = "LogoImg";
		DG::Image_Create(this->loImgName, "./data/image/TitleLogo.png");
		this->sbImgName = "StartButtonImg";
		DG::Image_Create(this->sbImgName, "./data/image/StartButton.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->bgImgName);
		DG::Image_Erase(this->loImgName);
		DG::Image_Erase(this->sbImgName);
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
		//ボタン用カウント
		this->timeCnt = 0;
		//Startボタンを押したか判断
		this->pushSon = false;

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
			auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		//スタートボタンを押したら
		if (in.ST.down && this->pushSon == false)
		{
			this->pushSon = true;
		}

		else if (in.B1.down && this->pushSon == true)
		{
			this->Kill();
		}
		//3秒後に
		if (this->timeCnt == 60 * 3)
		{
			//ローディング呼び出し
			auto lo = Loading::Object::Create(true);
			//色を黒に指定
			float color = 0.0f;
			lo->Set_Color(color);
		}
		//5秒後に
		if (this->timeCnt == 60 * 5)
		{
			//消滅
			this->Kill();
		}
		//ボタンを押していたら
		if (this->pushSon)
		{
			//カウント開始
			this->timeCnt++;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//TitleBG描画
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 1920, 1080);
		DG::Image_Draw(this->res->bgImgName, draw, src);
		//タイトルロゴ描画
		draw = ML::Box2D(100, 200, 1300, 300);
		src = ML::Box2D(0, 0, 1300, 300);
		DG::Image_Draw(this->res->loImgName, draw, src);
		//状態を判断して
		if (this->LogoAnim())
		{
			//ボタンを描画
			draw = ML::Box2D(400, 700, 345, 80);
			src = ML::Box2D(0, 0, 332, 63);
			DG::Image_Draw(this->res->sbImgName, draw, src);
		}
	}
	//
	void  Object::Render3D_L0()
	{
		
	}

	//メソッド
	//StartButtonロゴのアニメーション処理
	bool Object::LogoAnim()
	{
		//ボタンが押されていなかったら
		if (!this->pushSon)
		{
			//常に描画
			return true;
		}
		//押されていたら
		if (this->pushSon)
		{
			//20フレーム間隔で描画
			if ((this->timeCnt / 20) % 2 == 0)
			{
				return true;
			}
		}
		//それ以外は見えないようにする
		return false;
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