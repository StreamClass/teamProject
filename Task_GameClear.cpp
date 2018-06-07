//-------------------------------------------------------------------
//^CgζΚ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameClear.h"
#include  "Task_Title.h"
#include  "Task_NowLoading.h"

namespace  Clear
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		//wi
		this->imageName[0] = "BGImg";
		DG::Image_Create(this->imageName[0], "./data/image/ClearBG.png");
		//_00
		this->imageName[1] = "Cloud00Img";
		DG::Image_Create(this->imageName[1], "./data/image/Cloud00.png");
		//_01
		this->imageName[2] = "Cloud01Img";
		DG::Image_Create(this->imageName[2], "./data/image/Cloud01.png");
		//NAeLXg
		this->imageName[3] = "TextImg";
		DG::Image_Create(this->imageName[3], "./data/image/ClearText.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 4; ++i)
		{
			DG::Image_Erase(this->imageName[i]);
		}
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		srand((unsigned int)time(NULL));
		this->render2D_Priority[1] = 0.5f;
		this->timeCnt = 0;
		this->cloud00pos = ML::Vec2(1920, 0);
		this->cloud01pos = ML::Vec2(960, 300);
		this->alpha = 0.0f;
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//ψ«p¬^XNΜΆ¬
			auto nectTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		//ΑΕ
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
		if (this->cloud00pos.x < -400.0f)
		{
			this->cloud00pos.y = rand() % 300 + 100;
			this->cloud00pos.x = 1920;
		}
		if (this->cloud01pos.x < -400.0f)
		{
			this->cloud01pos.y = rand() % 300 + 100;
			this->cloud01pos.x = 1920;
		}
		this->cloud00pos.x -= 3.0f;
		this->cloud01pos.x -= 3.0f;
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		//wi`ζ
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 1920, 1080);
		DG::Image_Draw(this->res->imageName[0], draw, src);
		//_00`ζ
		draw = ML::Box2D(0, 0, 400, 250);
		src = ML::Box2D(0, 0, 200, 100);
		draw.Offset(this->cloud00pos);
		DG::Image_Draw(this->res->imageName[1], draw, src);
		//_01`ζ
		draw = ML::Box2D(0, 0, 400, 250);
		draw.Offset(this->cloud01pos);
		DG::Image_Draw(this->res->imageName[2], draw, src);
		//eLXg`ζ
		draw = ML::Box2D(0, 200, 1920, 300);
		src = ML::Box2D(0, 0, 1920, 300);
		DG::Image_Draw(this->res->imageName[3], draw, src,ML::Color(this->alpha,1,1,1));
	}

	void  Object::Render3D_L0()
	{
	}
	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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