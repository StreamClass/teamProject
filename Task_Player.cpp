//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h" 
#include  "Task_Aim.h"
#include  "Task_Map.h"
#include  "MapBox.h"
#include  "Task_Breaker.h"
#include  "Task_Door.h"
#include  "Task_MiniMap.h"
#include  "easing.h"


#define NORMALSPEED 10
#define TIRED_SPEED 3
#define DASHSPEED 20
#define MAX_STAMINA 240

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
		this->pos = ML::Vec3(0, 0, 0);
		this->headHeight = 175;
		this->headHeight_std = 175;
		this->adjust_TG = 175;
		this->adjust_TG_std = 175;
		this->cnt_TG = 0;
		this->add_adjust = 0.0f;
		this->cnt_SP = 1;
		this->trm_Max_std = 15.0f;
		this->trm_Min = 0.0f;
		this->trm_Max_dash = 25.0f;
		this->trm_Max_trd = 25.0f;
		this->adjust_Speed = 10.0f;
		this->adjust_Min = -400;
		this->adjust_Max = +400;
		this->hitBase = ML::Box3D(-50, 0, -50, 100, 200, 100);
		this->angle = ML::Vec3(0, ML::ToRadian(-90), 0);
		this->moveVec = ML::Vec3(0, 0, 0);
		this->speed = 10.0f;
		this->clearFlag = false;
		this->tremor = 4.0f;
		this->stamina = MAX_STAMINA;
		this->recovery_Flag = false;

		this->tab = ge->OM.Create_Tablet();
		//視点イージング
		easing::Set("camStdUp", easing::EXPOINOUT, this->trm_Min, this->trm_Max_std, 120);
		easing::Set("camStdDown", easing::QUADOUT, this->trm_Max_std, this->trm_Min, 120);
		//ダッシュ用
		easing::Set("camDashUp", easing::EXPOINOUT, this->trm_Min, this->trm_Max_dash, 15);
		easing::Set("camDashDown", easing::QUADOUT, this->trm_Max_dash, this->trm_Min, 10);
		//疲労時用
		easing::Set("camTrdUp", easing::EXPOINOUT, this->trm_Min, this->trm_Max_trd, 180);
		easing::Set("camTrdDown", easing::QUADOUT, this->trm_Max_trd, this->trm_Min, 180);

		//★タスクの生成
		auto aim = Aiming::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("エイム");

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
		//タブレットオフ
		if (!this->tab->Is_Used_Now())
		{
			if (in.LStick.volume > 0) //アナログスティックを倒している強さ0.0~1.0f
			{
				//走る途中はスタミナ減少
				if (in.R1.on)
				{
					this->stamina--;
				}
				ML::Mat4x4 matR;
				matR.RotationY(this->angle.y);
				this->moveVec.x = -this->speed * in.LStick.axis.y;
				this->moveVec.z = -this->speed * in.LStick.axis.x;
				//頂点を座標変換させる
				this->moveVec = matR.TransformCoord(this->moveVec);
			}
			else
			{
				this->moveVec = ML::Vec3(0, 0, 0);
			}
			this->angle.y += in.RStick.axis.x * ML::ToRadian(2);
			//速度指定
			if (in.R1.on)
			{
				if (this->recovery_Flag == false)
				{
					this->speed = DASHSPEED;
				}
				else
				{
					this->speed = TIRED_SPEED;
				}
			}
			else if (in.R1.off)
			{
				if (this->recovery_Flag == false)
				{
					this->speed = NORMALSPEED;
				}
				else
				{
					this->speed = TIRED_SPEED;
				}
				//スタミナ回復
				this->stamina += 0.3f;
			}
			
			//スタミナ範囲
			if (this->stamina < 0)
			{
				this->stamina = 0;
			}
			else if (this->stamina > MAX_STAMINA)
			{
				this->stamina = MAX_STAMINA;
			}

			//リカバリーモードに切り替え
			if (this->recovery_Flag == false && this->stamina <= 0)
			{
				this->recovery_Flag = true;
			}
			//通常モードに切り替え
			if (this->recovery_Flag == true && this->stamina >= MAX_STAMINA / 3)
			{
				this->recovery_Flag = false;
			}
			//注視点の上下移動
			if (in.RStick.U.on && this->adjust_TG < this->adjust_Max)
			{
				this->add_adjust += this->adjust_Speed;
			}
			else if (in.RStick.D.on && this->adjust_TG > this->adjust_Min)
			{
				this->add_adjust -= this->adjust_Speed;
			}
			//画面揺れ用カウンタスタート
			//走る速度で画面揺れの速度が変化する
			this->cnt_TG += this->cnt_SP;
			//イージング
			easing::UpDate();
			//通常の速度の画面揺れ
			if (this->speed == NORMALSPEED)
			{
				if (this->cnt_TG < 120)
				{
					easing::Start("camStdUp");
					easing::Reset("camStdDown");
					this->headHeight = this->headHeight_std + easing::GetPos("camStdUp");
					this->adjust_TG = this->adjust_TG_std + this->add_adjust + easing::GetPos("camStdUp");
				}
				else if (this->cnt_TG > 120)
				{
					easing::Start("camStdDown");
					easing::Reset("camStdUp");
					this->headHeight = this->headHeight_std + easing::GetPos("camStdDown");
					this->adjust_TG = this->adjust_TG_std + this->add_adjust + easing::GetPos("camStdDown");
				}
				//カウンタのリセット
				if (this->cnt_TG > 240)
				{
					this->cnt_TG = 0;
				}
			}
			//ダッシュ中の画面揺れ
			else if (this->speed == DASHSPEED)
			{
				if (this->cnt_TG < 15)
				{
					easing::Start("camDashUp");
					easing::Reset("camDashDown");
					this->headHeight = this->headHeight_std + easing::GetPos("camDashUp");
					this->adjust_TG = this->adjust_TG_std + this->add_adjust + easing::GetPos("camDashUp");
				}
				else if (this->cnt_TG > 15)
				{
					easing::Start("camDashDown");
					easing::Reset("camDashUp");
					this->headHeight = this->headHeight_std + easing::GetPos("camDashDown");
					this->adjust_TG = this->adjust_TG_std + this->add_adjust + easing::GetPos("camDashDown");
				}
				//カウンタのリセット
				if (this->cnt_TG > 25)
				{
					this->cnt_TG = 0;
				}
			}
			//疲労時の画面揺れ
			else if (this->speed == TIRED_SPEED)
			{
				if (this->cnt_TG < 180)
				{
					easing::Start("camTrdUp");
					easing::Reset("camTrdDown");
					this->headHeight = this->headHeight_std + easing::GetPos("camTrdUp");
					this->adjust_TG = this->adjust_TG_std + this->add_adjust + easing::GetPos("camTrdUp");
				}
				else if (this->cnt_TG > 180)
				{
					easing::Start("camTrdDown");
					easing::Reset("camTrdUp");
					this->headHeight = this->headHeight_std + easing::GetPos("camTrdDown");
					this->adjust_TG = this->adjust_TG_std + this->add_adjust + easing::GetPos("camTrdDown");
				}
				//カウンタのリセット
				if (this->cnt_TG > 360)
				{
					this->cnt_TG = 0;
				}
			}

			this->Player_CheckMove(this->moveVec);

			if (in.B3.down)
			{
				this->Touch();
			}
			if (in.B2.down)
			{
				auto mm = ge->GetTask_One_G<MiniMap::Object>("ミニマップ");
				mm->Set_MiniMap_View();
			}
			//デバッグ用
			//if (in.L1.on)
			//{
			//	this->pos.y += 8;
			//}
			//if (in.R1.on)
			//{
			//	this->pos.y -= 8;
			//}
		}
		else
		{
			this->tab->Select_Camera();
		}

		if (in.B4.down)
		{
			this->tab->Open_or_Close_Tablet();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(500, 0, 580, 300);
		string text = "X=" + to_string(this->pos.x) + "Y=" + to_string(this->pos.y) + "Z=" + to_string(this->pos.z) + "\n"
			+ "this->angle.y=" + to_string(ML::ToDegree(this->angle.y)) + "注視点の高さ" + to_string(this->adjust_TG);
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
	//注視点の高さ(adJust_TG)を返す
	int Object::Get_Adjust()
	{
		return this->adjust_TG;
	}
	//-------------------------------------------------------------------
	ML::Vec3 Object::Get_Angle()
	{
		return this->angle;
	}
	//-------------------------------------------------------------------
	//プレイヤの初期値指定
	void Object::Ini_Pos(const ML::Vec3& pos)
	{
		this->pos = pos;
	}
	//-------------------------------------------------------------------
	//マップとの接触判定
	//引数：（マップの矩形, プレイヤの矩形, マップのチップサイズ）
	bool Object::Map_CheckHit(const ML::Box3D& pHit)
	{
		auto mp = ge->GetTask_One_G<Map::Object>("フィールド");
		//読み込んだ矩形の最大、最小頂点の座標
		struct Box3D_2Point
		{
			int fx, fy, fz;//値が小さい側の点
			int bx, by, bz;//値が大きい側の点
		};
		//プレイヤの判定用頂点を設定
		Box3D_2Point r =
		{
			pHit.x,			pHit.y,			pHit.z,
			pHit.x + pHit.w,pHit.y + pHit.h,pHit.z + pHit.d
		};
		//マップの判定用頂点を設定
		Box3D_2Point m =
		{
			mp->arr[0][0].Get_HitBase().x,
			mp->arr[0][0].Get_HitBase().y,
			mp->arr[0][0].Get_HitBase().z,
			mp->arr[0][0].Get_HitBase().x + mp->arr[0][0].Get_HitBase().w * mp->maxSizeX,
			mp->arr[0][0].Get_HitBase().y + mp->arr[0][0].Get_HitBase().h,
			mp->arr[0][0].Get_HitBase().z + mp->arr[0][0].Get_HitBase().d * mp->maxSizeZ
		};

		//キャラクタの矩形をマップ範囲内に丸め込む
		if (r.fx < m.fx) { r.fx = m.fx; }
		if (r.fz < m.fz) { r.fz = m.fz; }
		if (r.bx > m.bx) { r.bx = m.bx; }
		if (r.bz > m.bz) { r.bz = m.bz; }

		//キャラクタがマップ範囲外にっ完全に出ていたら判定終了
		if (r.bx <= r.fx) { return false; }
		if (r.bz <= r.fz) { return false; }
		//ループ範囲を特定
		int sx, sz, ex, ez;
		sx = r.fx / (int)mp->arr[0][0].Get_ChipSizeX();
		sz = r.fz / (int)mp->arr[0][0].Get_ChipSizeZ();
		ex = (r.bx - 1) / (int)mp->arr[0][0].Get_ChipSizeX();
		ez = (r.bz - 1) / (int)mp->arr[0][0].Get_ChipSizeZ();
		//接触判定開始
		for (int z = sz; z <= ez; ++z) {
			for (int x = sx; x <= ex; ++x) {
				if (mp->arr[z][x].Get_Type() == Type::box) {
					return true;
				}
				this->Check_Clear();
			}
		}
		auto d = ge->GetTask_Group_GN<Task_Door::Object>("ドア","NoName");
		for (auto it = d->begin(); it != d->end(); it++)
		{
			if ((*it)->Hit_Check(pHit))
			{
				return true;
			}
		}
		return false;//接触するものが検出されなかった
	}
	//-------------------------------------------------------------------
	//めり込まない処理
	//引数：（プレイヤの移動量）
	void Object::Player_CheckMove(ML::Vec3& est_)
	{

		//水平方向（x平面)に対する移動
		while (est_.x != 0.0f) {//予定移動量が無くなるまで繰り返す
			float preX = this->pos.x;//移動前の座標を保持

			//1cmもしくはそれ以下の残り分移動させる
			if (est_.x >= 1.0f) {
				this->pos.x += 1.0f;		est_.x -= 1.0f;
			}//+方向
			else if (est_.x <= -1.0f) {
				this->pos.x -= 1.0f;		est_.x += 1.0f;
			}//-方向
			else {
				this->pos.x += est_.x;		est_.x = 0.0f;
			}

			//接触判定を試みる
			ML::Box3D hit = this->hitBase.OffsetCopy(this->pos);
			//hit.Offset((int)this->pos.x, (int)this->pos.y, (int)this->pos.z);
			if (true == this->Map_CheckHit(hit)) {
				this->pos.x = preX;		//接触していたので、元に戻す
				break;	//これ以上試しても無駄なのでループを抜ける
			}			
		}
		//-----------------------------------------------------------------------------
		//水平方向（z平面)に対する移動
		//水平方向（x平面)に対する移動
		while (est_.z != 0.0f) {//予定移動量が無くなるまで繰り返す
			float preZ = this->pos.z;//移動前の座標を保持

									 //1cmもしくはそれ以下の残り分移動させる
			if (est_.z >= 1.0f) {
				this->pos.z += 1.0f;		est_.z -= 1.0f;
			}//+方向
			else if (est_.z <= -1.0f) {
				this->pos.z -= 1.0f;		est_.z += 1.0f;
			}//-方向
			else {
				this->pos.z += est_.z;		est_.z = 0.0f;
			}

			//接触判定を試みる
			ML::Box3D hit = this->hitBase.OffsetCopy(this->pos);
			//hit.Offset((int)this->pos.x, (int)this->pos.y, (int)this->pos.z);
			if (true == this->Map_CheckHit(hit)) {
				this->pos.z = preZ;		//接触していたので、元に戻す
				break;	//これ以上試しても無駄なのでループを抜ける
			}			
		}
	}
	//-------------------------------------------------------------------
	//ギミックへの干渉
	void Object::Touch()
	{
		auto b = ge->GetTask_Group_G<Task_Breaker::Object>("ブレーカー");
		for (auto it = b->begin(); it != b->end(); it++)
		{
			if ((*it)->Hit_Check(this->hitBase.OffsetCopy(this->pos)))
			{
				(*it)->ActivateBreaker();
				break;
			}
		}
	}
	//-------------------------------------------------------------------
	//クリアしているか判定
	void Object::Check_Clear()
	{
		auto m = ge->GetTask_Group_G<Map::Object>("フィールド");
		for (auto it = m->begin(); it != m->end(); ++it)
		{
			if ((*it)->goal.Map_Hit_Check(this->hitBase.OffsetCopy(this->pos)))
			{
				//this->clearFlag = true;
				ge->state = ge->clear;
			}
		}
	}
	//-------------------------------------------------------------------
	//クリア情報を渡す
	bool Object::Get_ClearFlag()
	{
		return this->clearFlag;
	}

	//------------------------------------------------------------------------
	//タブレット使用中を返す
	bool Object::Is_Used_Tablet()
	{
		return this->tab->Is_Used_Now();
	}
	//-----------------------------------------------------------------------
	//あたり判定範囲をもらう関数
	ML::Box3D Object::Get_HitBase()
	{
		return this->hitBase;
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