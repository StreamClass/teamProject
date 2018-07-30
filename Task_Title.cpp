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
		this->bgMeshName = "TitleBGImg";
		this->loImgName = "LogoImg";
		this->sbImgName = "StartButtonImg";
		DG::Mesh_CreateFromSOBFile(this->bgMeshName, "./data/mesh/TitleBG.SOB");
		DG::Image_Create(this->loImgName, "./data/image/TitleLogo.png");
		DG::Image_Create(this->sbImgName, "./data/image/StartButton.png");
		this->bgmName = "TitmeBGM";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/TitleBGM.wav");
		this->startSEName = "StartButtonPushBGM";
		DM::Sound_CreateSE(this->startSEName, "./data/sound/StartSE.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->bgMeshName);
		DG::Image_Erase(this->loImgName);
		DG::Image_Erase(this->sbImgName);
		DM::Sound_Erase(this->bgmName);
		DM::Sound_Erase(this->startSEName);
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
		//ボタン用カウント
		this->timeCnt = 0;
		//Startボタンを押したか判断
		this->pushSon = false;
		//
		this->eneBone = new Bone(180, "Enemy");
		ML::Vec3 pos(80, -160, 0);
		this->eneBone->Moving(pos);
		float radi = ML::ToRadian(200);
		this->eneBone->Bone_RotateY_All(radi);
		int ran = rand() % 3;
		switch (ran)
		{
		case 0:
			break;
		case 1:
			this->eneBone->Set_Next_Motion("Walking");
			break;
		case 2:
			this->eneBone->Set_Next_Motion("Running");
			break;
		}

		//カメラの設定
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(0.0f, 0.0f, 0.0f),				//	ターゲット位置
			ML::Vec3(0.0f, 0.0f, -240.0f),			//	カメラ位置
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	カメラの上方向ベクトル
			ML::ToRadian(35), 15.0f, 8000.0f,	//	視野角・視野距離
			(float)ge->screenWidth / (float)ge->screenHeight);		//	画面比率		
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//ライティング有効化
		DG::EffectState().param.lightsEnable = true;
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.2f, 0.2f, 0.2f);
		//
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Point;//光源の種類
		DG::EffectState().param.light[0].range = 200.0f;
		DG::EffectState().param.light[0].attenuation = (1 / 350.0f)*(1 / 350.0f);
		DG::EffectState().param.light[0].pos = ML::Vec3(-100, 50, 180);
		DG::EffectState().param.light[0].color = ML::Color(1, 1.0f, 0.0f, 0.0f);//色と強さ

		//DG::EffectState().param.light[1].enable = true;
		//DG::EffectState().param.light[1].kind = DG_::Light::Directional;//光源の種類
		//DG::EffectState().param.light[1].direction = ML::Vec3(1, 0, 0).Normalize();//照射方向
		//DG::EffectState().param.light[1].color = ML::Color(1, 0.8f, 0.2f, 0.2f);//色と強さ

		DM::Sound_Play(this->res->bgmName, true);
		//★タスクの生成
		
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete this->eneBone;
		this->eneBone = nullptr;
		DM::Sound_Stop(this->res->bgmName);

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
		auto in = DI::GPad_GetState(ge->controllerName);
		
		this->eneBone->Repeat_Now_Motioin();
		this->eneBone->UpDate();

		//スタートボタンを押したら
		if ((in.ST.down || in.B1.down || in.B2.down || in.B3.down || in.B4.down) && this->pushSon == false)
		{
			DM::Sound_Play(this->res->startSEName,false);
			this->pushSon = true;
		}
		//デバッグ時用
		//else if (in.B1.down && this->pushSon == true)
		//{
		//	this->Kill();
		//}

		//3秒後に
		if (this->timeCnt == 60 * 3)
		{
			//ローディング呼び出し
			auto lo = Loading::Object::Create(true);
			//色を黒に指定
			float color = 0.0f;
			lo->Set_Color(color);

			//DM::Sound_Stop("TitleBGM");
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
		//タイトルロゴ描画
		ML::Box2D draw(100, 200, 1300, 300);
		ML::Box2D src(0, 0, 1300, 300);
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
		
		ML::Mat4x4 matT, matS;
		matT.Translation(ML::Vec3(0, 0, 200));
		matS.Scaling(100 << 3);
		DG::EffectState().param.matWorld = matS * matT;
		DG::Mesh_Draw(this->res->bgMeshName);
		//
		this->eneBone->Render();
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