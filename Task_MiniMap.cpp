//-------------------------------------------------------------------
//ミニマップ表示
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MiniMap.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"

namespace  MiniMap
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//イメージ名の指定
		this->imageName = "MapImg";
		this->plImgName = "PlayerImg";
		this->caImgName = "CameraImg";
		this->anImgName = "Chara_Angle";
		this->cursor_Image_Name = "Cursor";
		//各画像の読み込み
		DG::Image_Create(this->imageName, "./data/image/マップ00.png");
		DG::Image_Create(this->plImgName, "./data/image/Player_MiniMap.png");
		DG::Image_Create(this->caImgName, "./data/image/Camera_MiniMap.png");
		DG::Image_Create(this->anImgName, "./data/image/Chara_Angle.png");
		DG::Image_Create(this->cursor_Image_Name, "./data/image/camera_Cursor.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->plImgName);
		DG::Image_Erase(this->caImgName);
		DG::Image_Erase(this->anImgName);
		DG::Image_Erase(this->cursor_Image_Name);
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
		//プレイヤ用変数の初期化
		this->plpos = ML::Vec2(0, 0);
		this->plAngle = 0;
		//カメラ用変数の初期化
		this->capos = ML::Vec2(0, 0);
		this->caAngle = 0;
		//最初から見るように設定
		this->viewFlag = true;
		//タブレットは使っていない
		this->tab_use_now = false;
		//座標参照用倍率
		this->magni = chipX / NORMALMAGNI;
		//ミニマップ表示サイズを通常状態に設定
		this->mapSize = NORMALMAPSIZE;
		//デバックモードの初期化(無効)
		this->debugMode = false;
		//エネミーの座標
		this->epos = ML::Vec2(0, 0);
		//エネミーの向き
		this->eangle = 0;
		//カーソル初期座標
		this->cursor_Pos = ML::Vec2(30, 700);
		//カーソル及びカメラアイコン矩形
		this->cursor_Box = ML::Box2D(-32+60, -32+60, 64, 64);

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
		//プレイヤの変数等を使えるように呼び出す
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		//タブレットの使用状況をプレイヤから受け取る
		this->tab_use_now = pl->Is_Used_Tablet();
		//デバッグモードか否かをプレイヤーから受け取る
		this->debugMode = pl->Get_DebugOnOff();
		//通常状態の表示倍率とミニマップ表示サイズを指定
		this->magni = float(chipX) / NORMALMAGNI;
		this->mapSize = NORMALMAPSIZE;
		//タブレットを使用しているなら
		if (this->tab_use_now)
		{
			//タブレット使用状態での表示倍率とミニマップ表示サイズを指定
			this->magni = float(chipX) / TABLETMAGNI;
			this->mapSize = TABLETMAPSIZE;
		}
		//プレイヤ本体からミニマップ上の情報を参照
		this->plpos = ML::Vec2((int)pl->Get_Pos().x / this->magni, this->mapSize - (int)pl->Get_Pos().z / this->magni);
		this->plAngle = (float)pl->Get_Angle().y + ML::ToRadian(90);
		//カメラからミニマップ上の情報を参照
		this->capos = ML::Vec2((int)ge->camera[0]->pos.x / this->magni, this->mapSize - (int)ge->camera[0]->pos.z / this->magni);
		ML::Vec2 a = ML::Vec2(ge->camera[0]->target.x - ge->camera[0]->pos.x, ge->camera[0]->target.z - ge->camera[0]->pos.z);
		this->caAngle = -atan2(a.y,a.x) + ML::ToRadian(90);
		//エネミーからミニマップ上での情報を参照
		auto e = ge->GetTask_One_G<Enemy::Object>("エネミー");
		this->epos = ML::Vec2((int)e->pos.x / this->magni, this->mapSize - (int)e->pos.z / this->magni);
		this->eangle = e->angle.y + ML::ToRadian(90);

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ミニマップを表示していなかったら
		if (!this->MiniMap_View())
		{
			return;
		}
		//リソースの参照のサイズを指定
		ML::Box2D mapSrc(0, 0, 500, 500);
		ML::Box2D src(0, 0, 63, 63);
		ML::Box2D aSrc(0, 0, 63, 45);
		//タブレットを使用していない
		//(通常状態)
		if (!this->tab_use_now)
		{
			//回転中心
			ML::Vec2 cen(16, 24);
			//プレイヤの表示座標等を指定
			ML::Box2D pdraw(-8 + 60, -8 + 60, 16, 16);
			//カメラ用
			ML::Box2D cdraw = pdraw;
			//エネミー用(基本は表示しない)
			ML::Box2D edraw = pdraw;;
			//向き用
			ML::Box2D aDraw(-16 + 60, -8 + 60, 32, 16);
			//不透明度を1に
			float alpha = 1;
			//ミニマップを描画
			ML::Box2D draw(60, 60, this->mapSize, this->mapSize);
			DG::Image_Draw(this->res->imageName, draw, mapSrc, ML::Color(alpha, 1, 1, 1));
			//向きを描画
			aDraw.Offset(this->capos - ML::Vec2(0, float(aDraw.h)));
			//中心軸とカメラの向きによって回転　
			DG::Image_Rotation(this->res->anImgName, this->caAngle, cen);
			DG::Image_Draw(this->res->anImgName, aDraw, aSrc, ML::Color(0.3f, 1, 1, 1));
			//描画位置をプレイヤの座標を参照して描画
			pdraw.Offset(this->plpos);
			DG::Image_Draw(this->res->plImgName, pdraw, src, ML::Color(alpha, 1, 1, 1));
			//描画位置をエネミーの座標を参照
			edraw.Offset(this->epos);
			//デバッグモードなら
			if (this->debugMode)
			{
				//エネミーを描画
				DG::Image_Draw(this->res->plImgName, edraw, src, ML::Color(alpha, 1, 1, 0));
			}
		}
		//タブレット使用時
		else
		{
			//回転中心
			ML::Vec2 cen(20, 30);
			//プレイヤの表示座標等を指定
			ML::Box2D pdraw(-10 + 60, -10 + 60, 20, 20);
			//カメラ用
			ML::Box2D cdraw = pdraw;
			//エネミー用(デモなら使用する(かも))
			ML::Box2D edraw = pdraw;
			//向き用
			ML::Box2D aDraw = ML::Box2D(-20 + 60, -10 + 60, 40, 20);
			//不透明度をほぼ半透明に
			float alpha = 0.4f;
			//ミニマップを描画
			ML::Box2D mapDraw(60, 60, this->mapSize, this->mapSize);
			DG::Image_Draw(this->res->imageName, mapDraw, mapSrc, ML::Color(alpha, 1, 1, 1));
			//監視カメラの座標を参照して描画
			for (auto& scpos : this->stanbyCamera)
			{
				ML::Box2D draw(-10 + 60, -10 + 60, 20, 20);
				draw.Offset(scpos.first);
				DG::Image_Draw(this->res->caImgName, draw, src, ML::Color(alpha, 1, 0.5f, 0.5f));
			}
			if (ge->state != ge->demo)
			{
				//カメラの向きと座標を参照して描画
				aDraw.Offset(this->capos - ML::Vec2(0, float(aDraw.h)));
				DG::Image_Rotation(this->res->anImgName, this->caAngle, cen);
				DG::Image_Draw(this->res->anImgName, aDraw, aSrc, ML::Color(0.3f, 1, 1, 1));
			}
			//プレイヤの座標を参照して描画
			pdraw.Offset(this->plpos);
			DG::Image_Draw(this->res->plImgName, pdraw, src, ML::Color(alpha, 1, 1, 1));
			//使用中の現在のカメラを座標を参照して描画
			cdraw.Offset(this->capos);
			DG::Image_Draw(this->res->caImgName, cdraw, src, ML::Color(alpha, 1, 1, 1));
			//エネミーの座標を参照
			edraw.Offset(this->epos);
			//デバッグモードもしくはデモなら
			if (this->debugMode || ge->state == ge->demo)
			{
				//エネミーを描画
				DG::Image_Draw(this->res->plImgName, edraw, src, ML::Color(alpha, 1, 1, 0));
			}
			//カメラ選択カーソル描画
			POINT cursor_image_size = DG::Image_Size(this->res->cursor_Image_Name);
			ML::Box2D src_Cursor = ML::Box2D(0, 0, cursor_image_size.x, cursor_image_size.y);
			ML::Box2D draw_Cursor = this->cursor_Box.OffsetCopy(this->cursor_Pos);

			DG::Image_Draw(this->res->cursor_Image_Name, draw_Cursor, src_Cursor);
		}
	}
	//-------------------------------------------------------------------
	//「3Ｄ描画」１フレーム毎に行う処理
	void  Object::Render3D_L0()
	{
	}
	//ミニマップを表示するか判断
	bool Object::MiniMap_View()
	{
		return this->viewFlag;
	}
	//プレイヤからミニマップを表示するか指定
	void Object::Set_MiniMap_View()
	{
		this->viewFlag = !this->viewFlag;
	}
	//ミニマップの描画
	//画像描画範囲の設定を未実装
	void Object::MiniMap_Render()
	{
		ML::Box2D draw(60, 60, this->mapSize, this->mapSize);
		ML::Box2D src(0, 0, 50, 50);
		//描画位置の変更

		//リソースの参照位置を変更
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//
	void Object::Set_StanbyCameraPos(const ML::Vec3& pos, unsigned int& camera_Num)
	{
		if (pos.x < 0 || pos.x > chipX * 100 || pos.z < 0 || pos.z > chipZ * 100)
		{
			return;
		}
		this->stanbyCamera.insert(pair<ML::Vec2,unsigned int>( ML::Vec2(pos.x / (float(chipX) / TABLETMAGNI), TABLETMAPSIZE - pos.z / (float(chipX) / TABLETMAGNI)),camera_Num ));
	}

	//-------------------------------------------------------------------------------------------
	//カーソル移動
	int Object::Cursor_Move()
	{
		auto in1 = DI::GPad_GetState(ge->controllerName);
		//カーソルの移動速度
		const float cursor_Speed = 8.0f;		

		//カーソルの移動
		this->cursor_Pos += in1.LStick.axis * cursor_Speed;

		//カーソルとのあたり判定
		for (auto& standby : this->stanbyCamera)
		{
			//あたったら
			if (this->cursor_Box.OffsetCopy(standby.first).Hit(this->cursor_Box.OffsetCopy(this->cursor_Pos)))
			{
				//そのまま値を返す
				return standby.second;
			}
		}
		//あたらなかったら-値を返す
		return -1;
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