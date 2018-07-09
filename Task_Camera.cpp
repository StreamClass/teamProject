//-------------------------------------------------------------------
//カメラマン
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Camera.h"
#include  "Task_Player.h"
#include  "easing.h"

namespace  Camera
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->display_Noise_Img_Name = "noise";
		this->tablet_Img_Name = "tablet";
		DG::Image_Create(this->tablet_Img_Name, "./data/image/tablet.png");
		DG::Image_Create(this->display_Noise_Img_Name, "./data/image/disp_noise.jpg");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{		
		DG::Image_Erase(this->display_Noise_Img_Name);
		DG::Image_Erase(this->tablet_Img_Name);
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
		this->render2D_Priority[1] = 0.3f;
		this->angle = ML::Vec3(0, 0, 0);
		this->dist = ML::Vec3(100, 0, 0);
		
		//カメラの設定
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(0.0f, 0.0f, 0.0f),				//	ターゲット位置
			ML::Vec3(0.0f, 500.0f, -500.0f),			//	カメラ位置
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	カメラの上方向ベクトル
			ML::ToRadian(35), 10.0f, 8000.0f,	//	視野角・視野距離
			(float)ge->screenWidth / (float)ge->screenHeight);		//	画面比率		
		
		//フォグ(霧)の設定
		DG::EffectState().param.fogEnable = true;
		DG::EffectState().param.fogColor = ML::Color(1, 0, 0, 0);
		DG::EffectState().param.fogFore = 1800.0f;
		DG::EffectState().param.fogMode = true;
		DG::EffectState().param.fogNear = 1500.0f;

		//タブレット使用中のeasingをセット
		easing::Set("disp_Noise_Alpha", easing::CUBICOUT, 1.0f, 0.03f, 30);
		easing::Start("disp_Noise_Alpha");
		
		//ノイズのスクロールカウント
		this->noise_Cnt = 0;

		this->test_flag = false;

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
		easing::UpDate();
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");//[180517-持丸]カメラマンの向きをプレイヤと同期する
		this->noise_Cnt += 10;
		if (this->noise_Cnt > (int)ge->screenWidth)
		{
			this->noise_Cnt = 0;
		}

		//タブレットが使用中なら更新を止める
		if (pl->Is_Used_Tablet())
		{
			return;
		}
		
		if (DI::GPad_GetState("P1").SE.down)
		{
			this->test_flag ? this->test_flag = false : this->test_flag = true;
		}
		//注視点の距離
		//カメラマンの座標にプレイヤ座標を代入
		if (this->test_flag == false)
		{
			this->pos = pl->Get_Pos();
		}
		else
		{
			this->pos = pl->Get_Pos() - ML::Vec3(0, 0, 500);
		}
		//カメラマンのアングルにプレイヤのアングルを代入
		this->angle = pl->Get_Angle();
		//注視点
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);
		ML::Vec3 vec = ML::Vec3(800, pl->Get_Adjust(), 0);
		vec = matR.TransformCoord(vec);

		ge->camera[0]->target = this->pos + vec;
		ge->camera[0]->pos = this->pos + ML::Vec3(0, pl->Get_PointView(), 0);		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");//[180517-持丸]カメラマンの向きをプレイヤと同期する

		//タブレットが使用中ならノイズとタブレット枠を描画する
		if (pl->Is_Used_Tablet())
		{
			//ノイズ描画
			ML::Box2D noise_Draw0(0, 0, ge->screenWidth, ge->screenHeight);
			ML::Box2D noise_Draw1(ge->screenWidth, 0, ge->screenWidth, ge->screenHeight);
			POINT ns = DG::Image_Size(this->res->display_Noise_Img_Name);
			ML::Box2D noise_Src(0, 0, ns.x, ns.y);
			//ノイズの無限スクロール
			noise_Draw0.Offset(-this->noise_Cnt, 0);
			noise_Draw1.Offset(-this->noise_Cnt, 0);

			DG::Image_Draw(this->res->display_Noise_Img_Name, noise_Draw0, noise_Src, ML::Color(easing::GetPos("disp_Noise_Alpha"), 1, 1, 1));
			DG::Image_Draw(this->res->display_Noise_Img_Name, noise_Draw1, noise_Src, ML::Color(easing::GetPos("disp_Noise_Alpha"), 1, 1, 1));
			//タブレット枠描画
			ML::Box2D tablet_Draw(0, 0, ge->screenWidth,ge->screenHeight);
			POINT tb = DG::Image_Size(this->res->tablet_Img_Name);
			ML::Box2D tablet_Src(0, 0, tb.x, tb.y);

			DG::Image_Draw(this->res->tablet_Img_Name, tablet_Draw, tablet_Src);			
		}

		return;
	}
	//-------------------------------------------------------------------
	void  Object::Render3D_L0()
	{
		
	}
	//-------------------------------------------------------------------
	//ノイズeasingリセット(タブレットクラスで呼ぶ処理)
	void Object::Noise_Reset()
	{
		easing::Reset("disp_Noise_Alpha");
		easing::Start("disp_Noise_Alpha");
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