//-------------------------------------------------------------------
//ゲームオーバー画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameOver.h"
#include  "Task_Title.h"
#include  "Task_NowLoading.h"

namespace  Over
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//各イメージ名指定
		//画像を読み込み
		this->eImgName = "BloodImg";
		DG::Image_Create(this->eImgName, "./data/image/OverEffect.png");
		this->lImgName = "LgImg";
		DG::Image_Create(this->lImgName, "./data/image/OverLogo.png");
		//this->bgMeshName = "EndingBG";
		//DG::Mesh_CreateFromSOBFile(this->bgMeshName, "");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		//画像をすべて解放
		DG::Image_Erase(this->eImgName);
		DG::Image_Erase(this->lImgName);
		DG::Mesh_Erase(this->bgMeshName);
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
		this->render2D_Priority[1] = 1.0f;
		this->al = 0.0f;
		this->timeCnt = 0;
		this->endCnt = 0;
		this->endFlag = false;


		//
		this->iniFlag = true;

		//カメラの設定
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(0.0f, 0.0f, 0.0f),				//	ターゲット位置
			ML::Vec3(0.0f, 0.0f, -200.0f),			//	カメラ位置
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	カメラの上方向ベクトル
			ML::ToRadian(35), 15.0f, 8000.0f,	//	視野角・視野距離
			(float)ge->screenWidth / (float)ge->screenHeight);		//	画面比率		
		DG::EffectState().param.bgColor = ML::Color(0.5f, 1, 1, 1);
		//ライティング有効化
		DG::EffectState().param.lightsEnable = true;
		//
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//光源の種類
		DG::EffectState().param.light[0].direction = ML::Vec3(0, 0, 1).Normalize();//照射方向
		DG::EffectState().param.light[0].color = ML::Color(1, 0.8f, 0.5f, 0.5f);//色と強さ

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete this->enBone;
		this->enBone = nullptr;

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			auto nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState(ge->controllerName);
		//endFlagがfalseの時にstartボタンを押すとローディングを呼び出し
		if (in.ST.down && this->endFlag == false)
		{
			auto lo = Loading::Object::Create(true);
			//ローディング画面の色を黒に指定
			float color = 0.0f;
			lo->Set_Color(color);
			this->endFlag = true;
		}
		//2秒後から
		if (this->timeCnt > 60 * 2)
		{
			this->enBone->Repeat_Now_Motioin();
			this->enBone->UpDate();
			this->pos.x -= 3;
			this->enBone->Moving(this->pos);
		}
		//5秒後から
		else if (this->timeCnt > 60 * 5)
		{
			//3秒かけて不透明度を1に
			this->al = (this->timeCnt - 60 * 5) / 60.0f * 3.0f;
		}
		//15秒後(ロゴがすべて出てから7秒後)かつendFlagがfalseなら
		else if (this->timeCnt > 60 * 15 && this->endFlag == false)
		{
			//ローディングを呼び出し
			auto lo = Loading::Object::Create(true);
			//ローディング画面の色を黒に指定
			float color = 0.0f;
			lo->Set_Color(color);
			//endFlagをtrueに
			this->endFlag = true;
		}
		if (this->timeCnt > 60 && this->iniFlag)
		{
			//
			ML::Vec3 pos(0, 0, 0);
			this->enBone->Moving(pos);
			float radi = ML::ToRadian(-90);
			this->enBone->Bone_RotateY_All(radi);
			this->enBone->Set_Next_Motion("Running");
			this->iniFlag = false;
		}

		//endFlagがtrueなら
		if (this->endFlag == true)
		{
			//カウント開始
			this->endCnt++;
		}
		//endCntが2秒分数えたら
		if (this->endCnt > 60 * 2)
		{
			//タスクを解放
			this->Kill();
		}
		//ロゴの不透明度の上限を指定
		if (this->al > 1.0f)
		{
			this->al = 1.0f;
		}
		//タスクのフレーム数をカウント
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景描画
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 1920, 1080);
		//キャラクタが画面左側から出て行ったら
		if (this->timeCnt > 60 * 8)
		{
			//エフェクトを画面全体に描画
			DG::Image_Draw(this->res->eImgName, draw, src);
		}
		//ロゴ描画
		draw = ML::Box2D(0, 350, 1920, 300);
		src = ML::Box2D(0, 0, 1920, 300);
		//														 不透明度
		DG::Image_Draw(this->res->lImgName, draw, src, ML::Color(this->al, 1, 1, 1));
	}

	void  Object::Render3D_L0()
	{
		this->enBone->Render();
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