//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h" 

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//メッシュの読み込み
		this->meshName = "playerImg";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/char_Stand.sob");
		//フォントの読み込み
		DG::Font_Create("FontA", "ＭＳ ゴシック", 10, 20);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
		DG::Font_Erase("FontA");
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化)
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->controllerName = "P1";
		//プレイヤの初期化
		this->pos = ML::Vec3(-100, 0, 0);
		this->headHeight = 175;
		this->angle = ML::Vec3(0, 0, 0);
		this->moveVec = ML::Vec3(0, 0, 0);

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
		auto in = DI::GPad_GetState("P1");
		//volumeが0の時は使ってはダメ
		if (in.LStick.volume > 0) //アナログスティックを倒している強さ0.0~1.0f
		{
			ML::Mat4x4 matR;
			matR.RotationY(this->angle.y);
			this->moveVec.x = -10 * in.LStick.axis.y;
			this->moveVec.z = -10 * in.LStick.axis.x;
			//頂点を座標変換させる
			this->moveVec = matR.TransformCoord(this->moveVec);
		}
		else
		{
			this->moveVec = ML::Vec3(0, 0, 0);
		}
		this->angle.y += in.RStick.axis.x * ML::ToRadian(5);

		this->pos += this->moveVec;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 300, 100);
		string text = "X=" + to_string(this->pos.x) + "Y=" + to_string(this->pos.y) + "Z=" + to_string(this->pos.z) + "\n"
			+ "this->angle.y=" + to_string(ML::ToDegree(this->angle.y));
		DG::Font_Draw("FontA", draw, text, ML::Color(1.0f, 0.0f, 0.0f, 0.0f ));
	}
	//-------------------------------------------------------------------
	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT;
		matT.Translation(this->pos);
		//回転行列の生成
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);

		//モデル表示
		//ML::Mat4x4 matW = matR * matT;
		//DG::EffectState().param.matWorld = matW;
		//DG::Mesh_Draw(this->res->meshName);
	}
	//-------------------------------------------------------------------
	//プレイヤの座標をML::Vec3型で返す
	ML::Vec3 Object::Get_Pos()
	{
		return this->pos;
	}
	//-------------------------------------------------------------------
	//プレイヤの視点の高さをint型で返す
	int Object::Get_PointView()
	{
		return this->headHeight;
	}
	//-------------------------------------------------------------------
	ML::Vec3 Object::Get_Angle()
	{
		return this->angle;
	}
	//-------------------------------------------------------------------
	//ギミックを動かす関数
	void Object::Approach()
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