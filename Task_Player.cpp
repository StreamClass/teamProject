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

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//フォントの読み込み
		DG::Font_Create("FontA", "ＭＳ ゴシック", 10, 20);
		//メッシュの読み込み
		DM::Sound_CreateSE("standsound", "./data/sound/Stand00.wav");
		DM::Sound_CreateSE("runsound", "./data/sound/run00.wav");
		DM::Sound_CreateSE("tiredsound", "./data/sound/tired00.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
		DG::Font_Erase("FontA");
		DM::Sound_Erase("standsound");
		DM::Sound_Erase("runsound");
		DM::Sound_Erase("tiredsound");
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
		this->headHeight = 175.0f;
		this->headHeight_std = 175.0f;
		this->adjust_TG = 175.0f;
		this->adjust_TG_std = 175;
		this->cnt_TG = 0;
		this->add_adjust = 0.0f;
		this->cnt_SP = 1;
		this->trm_Max_std = 15.0f;
		this->trm_Min = 0.0f;
		this->trm_Max_dash = 25.0f;
		this->trm_Max_trd = 25.0f;
		this->adjust_Speed = 6.5f;
		this->adjust_Min = -400;
		this->adjust_Max = +400;
		this->heightMax = 180;
		this->hitBase = ML::Box3D(-50, 0, -50, 100, 200, 100);
		this->moveBase = ML::Box3D(-70, 0, -70, 140, 200, 140);
		this->angle = ML::Vec3(0, ML::ToRadian(-90), 0);
		this->moveVec = ML::Vec3(0, 0, 0);
		this->moveVecRec = 0.0f;
		this->speed = 10.0f;
		this->clearFlag = false;
		this->tremor = 1.0f;
		this->stamina = MAX_STAMINA;
		this->recovery_Flag = false;
		this->debugMode = false;
		this->motion = neutral;

		this->plBone = new Bone(170,"Player");
		this->Init_Players_Animations();

		this->breakerOnCnt = 0;

		//サウンド用フラグの初期化
		this->neutralSoundFlag = true;
		this->dashSoundFlag = false;
		this->tiredSoundFlag = false;

		this->tab = ge->OM.Create_Tablet();
		
		//通常時音声を再生
		DM::Sound_Play("standsound",true);
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

		//ボーン開放
		delete this->plBone;

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
		auto in = DI::GPad_GetState(ge->controllerName);		
		//状態管理
		switch (this->motion)
		{
		case neutral:
			if (in.R1.on && this->stamina >0 && in.LStick.volume > 0) { this->motion = dash; }
			if (this->stamina <= 0) { this->motion = tired; }
			break;
		case dash:
			if (in.R1.off&&this->stamina > 0) { this->motion = neutral; }
			if (in.R1.on && !(in.LStick.volume > 0.1f)) { this->motion = neutral; }
			if (this->stamina <= 0) { this->motion = tired; }
			break;
		case tired:
			if (this->stamina >= MAX_STAMINA / 3 && in.R1.off) { this->motion = neutral; }
			if (this->stamina >= MAX_STAMINA / 3 && in.R1.on && in.LStick.volume > 0) { this->motion = dash; }
			break;
		}
		//タブレットオフ
		if (!this->tab->Is_Used_Now())
		{

			if (in.S0.down)
			{
				this->debugMode = !this->debugMode;
			}
			if (in.LStick.volume >= 0.1f) //アナログスティックを倒している強さ0.0~1.0f
			{
				ML::Mat4x4 matR;
				matR.RotationY(this->angle.y);
				this->moveVec.x = -this->speed * in.LStick.axis.y;
				this->moveVec.z = -this->speed * in.LStick.axis.x;				
				//ベクトルを座標変換させる
				this->moveVec = matR.TransformCoord(this->moveVec);				
				//走るモーション
				this->plBone->Set_Next_Motion(this->animations_Name[1]);
				this->plBone->Repeat_Now_Motioin();
			}
			else
			{
				this->moveVec = ML::Vec3(0, 0, 0);
			}
			//ボーン全体をY軸回転
			this->plBone->Bone_RotateY_All(this->angle.y + ML::ToRadian(90));
			//現在のスピードをテキストに保存
			/*ge->Dbg_FileOut("speed = %0.2f", this->speed);*/
			//スタミナ範囲
			if (this->stamina < 0)
			{
				this->stamina = 0;
			}
			else if (this->stamina > MAX_STAMINA)
			{
				this->stamina = MAX_STAMINA;
			}

			if (this->debugMode == true)
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
			//視点の回転
			if (in.RStick.volume >= 0.1f)
			{
				this->angle.y += in.RStick.axis.x * ML::ToRadian(TURNSPEED);
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
			//画面揺れ
			//カウンタスタート
			this->cnt_TG++;
			//頭の基準値+sin(カウンタ*揺れ速度はスピードで変化)*(スピード*揺れ幅)
			float headY = this->headHeight_std +
				sin(ML::ToRadian(float(this->cnt_TG*this->cnt_SP)))*(this->speed*this->tremor);
			float targetY = this->adjust_TG_std +
				sin(ML::ToRadian(float(this->cnt_TG*this->cnt_SP)))*(this->speed*this->tremor) + this->add_adjust;
			//頭の高さを更新
			this->headHeight = headY;
			//注視点の高さを更新
			this->adjust_TG = targetY;
			//視点揺れ速度を選択
			//サウンドを選択
			switch (this->motion)
			{
			case neutral:
				//速度を変更
				this->speed = max(NORMALSPEED, this->speed -= 0.5f);
				//不要なサウンドをストップ
				if (this->dashSoundFlag)
				{
					DM::Sound_Stop("runsound");
				}
				if (this->tiredSoundFlag)
				{ 
					DM::Sound_Stop("tiredsound");
				}
				//必要なサウンドをプレイ
				if (!this->neutralSoundFlag) 
				{
					DM::Sound_Play("standsound", true); 
				}
				//視点揺れの速度を指定
				this->cnt_SP = 2;
				//揺れ幅を指定
				this->tremor = 0.5f;
				//スタミナを回復
				this->stamina += 0.5f;
				//各フラグを反転
				this->neutralSoundFlag = true;
				this->dashSoundFlag = false;
				this->tiredSoundFlag = false;
				break;
			case dash:
				//速度を変更
				this->speed = min(DASHSPEED, this->speed += 0.5f);
				//不要なサウンドをストップ
				if (this->neutralSoundFlag)
				{
					DM::Sound_Stop("standsound"); 
				}
				if (this->tiredSoundFlag) 
				{
					DM::Sound_Stop("tiredsound");
				}
				//必要なサウンドをプレイ
				if (!this->dashSoundFlag)
				{
					DM::Sound_Play("runsound", true); 
				}
				//視点揺れの速度を指定
				this->cnt_SP = 14;
				//揺れ幅を指定
				this->tremor = 0.8f;
				//dash時はスタミナ減少
				this->stamina--;
				//各フラグを反転
				this->neutralSoundFlag = false;
				this->dashSoundFlag = true;
				this->tiredSoundFlag = false;
				break;
			case tired:
				//速度を変更
				this->speed = TIRED_SPEED;
				//不要なサウンドをストップ
				if (this->dashSoundFlag)
				{ 
					DM::Sound_Stop("runsound"); 
				}
				if (this->neutralSoundFlag)
				{
					DM::Sound_Stop("standsound");
				}
				//必要なサウンドをプレイ
				if (!this->tiredSoundFlag) 
				{
					DM::Sound_Play("tiredsound", true); 
				}
				//視点揺れの速度を指定
				this->cnt_SP = 8;
				//揺れ幅を指定
				this->tremor = 6.0f;
				//スタミナを回復
				this->stamina += 0.3f;
				//各フラグを反転
				this->neutralSoundFlag = false;
				this->dashSoundFlag = false;
				this->tiredSoundFlag = true;
				break;
			}

			this->moveVecRec = this->moveVec.Length();
			this->Player_CheckMove(this->moveVec);

			if (in.B3.down)
			{
				this->Touch();
			}
			if (in.SE.down)
			{
				auto mm = ge->GetTask_One_G<MiniMap::Object>("ミニマップ");
				mm->Set_MiniMap_View();
			}
		}
		//タブレットモード
		else
		{
			//ミニマップタスクでカメラ選択
			auto mini = ge->GetTask_One_G<MiniMap::Object>("ミニマップ");
			//正しい値が返された時だけカメラを変える
			int index = mini->Cursor_Move();
			if (index >= 0)
			{
				this->tab->Select_Camera((unsigned int)index);
			}
		}
		//デモ画面以外の時Yボタンでタブレットオンオフ
		if (in.B4.down && ge->state != ge->demo)
		{
			this->tab->Open_or_Close_Tablet();
		}

		//タブレット使用とは別にボーンアニメーションアップデートは進む
		this->plBone->UpDate();

		//デバッグ用
		if (in.S1.down)
		{
			this->stamina = 0;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Vec3 bonepos = this->plBone->Get_Center();
		if(this->debugMode)
		{
			ML::Box2D draw(500, 0, 580, 300);
			string text = "X=" + to_string(this->pos.x) + "Y=" + to_string(this->pos.y) + "Z=" + to_string(this->pos.z) + "\n"
				+ "this->angle.y=" + to_string(ML::ToDegree(this->angle.y)) + "注視点の高さ" + to_string(this->adjust_TG) + "\n" +
				to_string(this->breakerOnCnt)+ "\n"+
				"moiton="+to_string(this->motion);
			if (this->debugMode)
			{
				text += "  Debug";
			}
			if (this->Check_Front())
			{
				text += "	true";
			}
			DG::Font_Draw("FontA", draw, text, ML::Color(1.0f, 0.0f, 0.0f, 0.0f ));
		}
	}
	//-------------------------------------------------------------------
	void  Object::Render3D_L0()
	{
		this->plBone->Render();
	}
	//-------------------------------------------------------------------
	//プレイヤの座標をML::Vec3型で返す
	ML::Vec3 Object::Get_Pos()
	{
		return this->pos;
	}
	//-------------------------------------------------------------------
	//プレイヤの視点の高さをint型で返す
	float Object::Get_PointView()
	{
		return this->headHeight;
	}
	//-------------------------------------------------------------------
	//注視点の高さ(adJust_TG)を返す7
	float Object::Get_Adjust()
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
		this->plBone->Moving(pos+ML::Vec3(0,20,0));
	}
	//-------------------------------------------------------------------
	//めり込まない処理
	//引数：（プレイヤの移動量）
	void Object::Player_CheckMove(ML::Vec3& est_)
	{
		auto mp = ge->GetTask_One_G<Map::Object>("フィールド");
		//水平方向（x平面)に対する移動
		while (est_.x != 0.0f) {//予定移動量が無くなるまで繰り返す
			float preX = this->pos.x;//移動前の座標を保持
			float bone_Pre_X = this->plBone->Get_Center().x;
									 
			//+方向
			if (est_.x >= 1.0f) {
				this->pos.x += 1.0f;
				this->plBone->Moving(ML::Vec3(1.0f, 0, 0));
				est_.x -= 1.0f;
			}//-方向
			else if (est_.x <= -1.0f) {
				this->pos.x -= 1.0f;	
				this->plBone->Moving(ML::Vec3(-1.0f, 0, 0));
				est_.x += 1.0f;
			}//小数点以下の移動
			else {
				this->pos.x += est_.x;
				this->plBone->Moving(ML::Vec3(est_.x, 0, 0));
				est_.x = 0.0f;
			}

			//接触判定を試みる
			ML::Box3D hit = this->moveBase.OffsetCopy(this->pos);
			if (true == mp->Map_CheckHit(hit)) {
				//接触していたので、元に戻す
				this->pos.x = preX;		
				bone_Pre_X -= this->plBone->Get_Center().x;
				this->plBone->Moving(ML::Vec3(bone_Pre_X, 0, 0));

				break;	//これ以上試しても無駄なのでループを抜ける
			}			
		}
		//-----------------------------------------------------------------------------
		//水平方向（z平面)に対する移動
		//水平方向（x平面)に対する移動
		while (est_.z != 0.0f) {//予定移動量が無くなるまで繰り返す
			float preZ = this->pos.z;//移動前の座標を保持
			float bone_Pre_Z = this->plBone->Get_Center().z;
			//+方向
			if (est_.z >= 1.0f) {
				this->pos.z += 1.0f;
				this->plBone->Moving(ML::Vec3(0, 0, 1.0f));
				est_.z -= 1.0f;
			}//-方向
			else if (est_.z <= -1.0f) {
				this->pos.z -= 1.0f;	
				this->plBone->Moving(ML::Vec3(0, 0, -1.0f));
				est_.z += 1.0f;
			}//小数点以下の移動
			else {
				this->pos.z += est_.z;	
				this->plBone->Moving(ML::Vec3(0, 0, est_.z));
				est_.z = 0.0f;
			}

			//接触判定を試みる
			ML::Box3D hit = this->moveBase.OffsetCopy(this->pos);
			if (true == mp->Map_CheckHit(hit)) {
				//接触していたので、元に戻す
				this->pos.z = preZ;
				bone_Pre_Z -= this->plBone->Get_Center().z;
				this->plBone->Moving(ML::Vec3(0, 0, bone_Pre_Z));

				break;	//これ以上試しても無駄なのでループを抜ける
			}			
		}
	}
	//-------------------------------------------------------------------
	void Object::Turn_Angle()
	{
		auto in = DI::GPad_GetState("P1");
		ML::Vec3 toAngle = ge->camera[0]->target - ge->camera[0]->pos;
		toAngle=toAngle.Normalize();
		//視点の回転
		ML::Vec3 preAngle = this->angle;
		if (in.RStick.axis.x > 0)
		{

		}
		this->angle.y += in.RStick.axis.x * ML::ToRadian(TURNSPEED);
		if(this->Check_Front())
		{
			this->angle = preAngle;
		}
	}
	//-------------------------------------------------------------------
	bool Object::Check_Front()
	{
		auto mp = ge->GetTask_One_G<Map::Object>("フィールド");

		ML::Vec3 toAngle = ML::Vec3(ge->camera[0]->target.x - ge->camera[0]->pos.x, 0, ge->camera[0]->target.z - ge->camera[0]->pos.z);
		toAngle = toAngle.Normalize();
		ML::Box3D front(0, 0, 0, 1, 1, 1);
		return mp->Map_CheckHit(front.OffsetCopy((this->pos + ML::Vec3(0, this->headHeight, 0)) + (toAngle * 150)));
	}
	//-------------------------------------------------------------------
	//ギミックへの干渉
	void Object::Touch()
	{
		auto b = ge->GetTask_Group_G<Task_Breaker::Object>("ブレーカー");
		auto aim = ge->GetTask_One_G<Aiming::Object>("エイム");
		for (auto it = b->begin(); it != b->end(); it++)
		{
			if (!(*it)->Get_Now_State())
			{
				ML::Vec3 move = (ge->camera[0]->target - ge->camera[0]->pos);
				move = move.Normalize();
				for (int i = 0; i < 10; ++i)
				{
					if ((*it)->Hit_Check(aim->Get_HitBase().OffsetCopy((this->pos + ML::Vec3(0.0f, this->headHeight, 0.0f)) + (move * (float)i * 15.0f))))
					{
						(*it)->ActivateBreaker();
						//ボタン操作モーション実行
						this->plBone->Set_Next_Motion(this->animations_Name[0]);
						this->breakerOnCnt++;
						break;
					}
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//
	bool Object::Touch_AimToBreaker()
	{
		auto b = ge->GetTask_Group_G<Task_Breaker::Object>("ブレーカー");
		auto aim = ge->GetTask_One_G<Aiming::Object>("エイム");
		for (auto it = b->begin(); it != b->end(); it++)
		{
			if (!(*it)->Get_Now_State())
			{
				ML::Vec3 move = (ge->camera[0]->target - ge->camera[0]->pos);
				move = move.Normalize();
				for (int i = 0; i < 10; ++i)
				{
					if ((*it)->Hit_Check(aim->Get_HitBase().OffsetCopy((this->pos + ML::Vec3(0, this->headHeight, 0)) + (move * (float)i * 15.0f))))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	//-------------------------------------------------------------------
	//クリアしているか判定
	void Object::Check_Clear()
	{
		auto mp = ge->GetTask_One_G<Map::Object>("フィールド");
		if (mp->Goal_CheckHit(this->hitBase.OffsetCopy(this->pos)) == true)
		{
			ge->state = ge->clear;
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
	//-----------------------------------------------------------------------
	//移動速度をfloatで返す
	float Object::Get_MoveSpeed()
	{
		return this->moveVecRec;
	}
	//
	float Object::Get_Stamina()
	{
		return this->stamina;
	}
	//
	bool Object::Get_DebugOnOff()
	{
		return debugMode;
	}
	//
	bool Object::Is_Tired()
	{
		return this->recovery_Flag;
	}

	//-----------------------------------------------------------------------------
	//初期化の時アニメーション登録するメソッド
	void Object::Init_Players_Animations()
	{
		//ボタン押しアニメーション
		this->animations_Name.push_back("Interaction");
		std::vector<Motion::Motion_Data> interaction;
		Motion::Make_Motion(&interaction, this->animations_Name[0]);
		this->plBone->Registrate_Motion(interaction, this->animations_Name[0]);
		//走るアニメーション
		this->animations_Name.push_back("Running");
		std::vector<Motion::Motion_Data> running;
		Motion::Make_Motion(&running, this->animations_Name[1]);
		this->plBone->Registrate_Motion(running, this->animations_Name[1]);
		//歩くアニメーション
		this->animations_Name.push_back("Walking");
		std::vector<Motion::Motion_Data> walking;
		Motion::Make_Motion(&walking, this->animations_Name[2]);
		this->plBone->Registrate_Motion(walking, this->animations_Name[2]);
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