//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameClear.h"
#include  "Task_Title.h"
#include  "Task_NowLoading.h"

namespace  Clear
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//背景
		this->imageName[0] = "BGImg";
		DG::Image_Create(this->imageName[0], "./data/image/ClearBG.png");
		//雲00
		this->imageName[1] = "Cloud00Img";
		DG::Image_Create(this->imageName[1], "./data/image/Cloud00.png");
		//雲01
		this->imageName[2] = "Cloud01Img";
		DG::Image_Create(this->imageName[2], "./data/image/Cloud01.png");
		//クリアテキスト
		this->imageName[3] = "TextImg";
		DG::Image_Create(this->imageName[3], "./data/image/ClearText.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 4; ++i)
		{
			DG::Image_Erase(this->imageName[i]);
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
		srand((unsigned int)time(NULL));
		this->render2D_Priority[1] = 0.5f;
		this->timeCnt = 0;
		this->cloud00pos = ML::Vec2(1920, 0);
		this->cloud01pos = ML::Vec2(960, 300);
		this->alpha = 0.0f;
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
			auto nectTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		//消滅
		if (in.ST.down)
		{
			this->Kill();
		}
		if (this->timeCnt == 60 * 17.0f)
		{
			auto lo = Loading::Object::Create(true);
			float color = 1.0f;
			lo->Set_Color(color);
		}
		if (this->timeCnt > 60 * 19.0f)
		{
			this->Kill();
		}
		if (this->timeCnt > 60 * 4 && this->timeCnt < 60 * 7 )
		{
			this->alpha = (this->timeCnt - 60.0f * 4.0f) / 180.0f;
		}
		if (this->alpha < 0.0f)
		{
			this->alpha = 0.0f;
		}
		if (this->alpha >= 1.0f)
		{
			this->alpha = 1.0f;
		}
		if (this->cloud00pos.x < -300.0f)
		{
			this->cloud00pos.y = rand() % 300 + 100;
			this->cloud00pos.x = 1920;
		}
		if (this->cloud01pos.x < -300.0f)
		{
			this->cloud01pos.y = rand() % 300 + 100;
			this->cloud01pos.x = 1920;
		}
		this->cloud00pos.x -= 3.0f;
		this->cloud01pos.x -= 3.0f;
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景描画
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 1920, 1080);
		DG::Image_Draw(this->res->imageName[0], draw, src);
		//雲00描画
		draw = ML::Box2D(0, 0, 300, 150);
		src = ML::Box2D(0, 0, 200, 100);
		draw.Offset(this->cloud00pos);
		DG::Image_Draw(this->res->imageName[1], draw, src);
		//雲01描画
		draw = ML::Box2D(0, 0, 300, 150);
		draw.Offset(this->cloud01pos);
		DG::Image_Draw(this->res->imageName[2], draw, src);
		//テキスト描画
		draw = ML::Box2D(0, 200, 1920, 300);
		src = ML::Box2D(0, 0, 1920, 300);
		DG::Image_Draw(this->res->imageName[3], draw, src,ML::Color(this->alpha,1,1,1));
	}

	void  Object::Render3D_L0()
	{
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