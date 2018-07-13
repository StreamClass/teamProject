//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map.h"
#include  "Task_Player.h"
#include  "Task_Door.h"

namespace  Map
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		this->render3D_Priority[1] = 1.0f;
		//テキスト1つあたりのX・Z座標のサイズ
		this->sizeX = 0;
		this->sizeZ = 0;
		//マップのサイズ
		this->maxSizeX = 100;
		this->maxSizeZ = 100;
		//読み込み時にｘ・ｚの基準値として使用
		this->tmpX = 0;
		this->tmpZ = 0;
		//マップをすべて初期化
		for (int z = 0; z < this->maxSizeZ; ++z)
		{
			for (int x = 0; x < this->maxSizeX; ++x)
			{
				this->arr[z][x] = Box();
			}
		}
		//床の数値指定
		ML::Vec3 chipSize(
			this->maxSizeX * this->arr[0][0].Get_ChipSizeX(),
			50.0f,
			this->maxSizeZ * this->arr[0][0].Get_ChipSizeZ()
		);
		ML::Vec3 pos(
			chipSize.x / 2,
			-25.0f,
			chipSize.z / 2
		);
		ML::Box3D hitBase(0, 0, 0, chipSize.x, 50.0f, chipSize.z);
		this->floor = Box(chipSize, pos, hitBase);
		this->floor.Set_MeshName("MapBox");
		//天井の数値の指定
		//今は上に移動させるだけ
		pos.y += 350.0f;
		this->ceiling = Box(chipSize, pos, hitBase);
		this->ceiling.Set_MeshName("MapBox");
		//ゴール	の数値指定
		chipSize = ML::Vec3(chipX * 2, chipY, chipZ / 3);
		pos = ML::Vec3(chipX * 3 + chipSize.x / 2, chipY / 2, chipZ * 100 + chipSize.z / 2);
		hitBase = ML::Box3D(0,0,0, chipSize.x, chipSize.y, chipSize.z);
		this->goal = Box(chipSize, pos, hitBase);
		//チップ名の初期化
		//読み込むファイル名の初期化
		this->fileName = "Map00.txt";
		//ファイルパスの初期化
		this->filePath = "";

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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{
		DG::EffectState().param.light[0].enable = false;
		//壁の描画
		ML::Mat4x4 matS,matT;
		//描画する範囲をカメラを中心に前後左右18マス分に指定
		int  sx, sz, ex, ez;
		sx = max(0, int(ge->camera[0]->pos.x / chipX) - 15);
		ex = min(this->maxSizeX, int(ge->camera[0]->pos.x / chipX) + 15);
		sz = max(0, int(ge->camera[0]->pos.z / chipZ) - 15);
		ez = min(this->maxSizeZ, int(ge->camera[0]->pos.z / chipZ) + 15);

		//指定した範囲の壁のみを描画
		for (int z = ez - 1; z >= sz; --z)
		{
			for (int x = sx; x < ex; ++x)
			{
				this->arr[z][x].Render3D();
			}
		}
		
		//床の描画
		this->floor.Render3D();

		//天井の描画
		this->ceiling.Render3D();
		DG::EffectState().param.light[0].enable = true;

		//ゴール位置確認用
		//ML::Mat4x4 gT, gS;
		//gS.Scaling(this->goal.Get_Scaling());
		//gT.Translation(this->goal.Get_Pos());
		//DG::EffectState().param.matWorld = gS * gT;
		//DG::Mesh_Draw(this->chipName);
	}
	//-------------------------------------------------------------------
	//マップの読み込み
	bool  Object::Load()
	{
		int x = 0, z = 0;
		int num = 0;
		//読み込んだファイル名がend以外の間回す
		while (this->fileName != "end")
		{
			//読み込むファイルパスの設定
			this->filePath = "./data/stageData/" + this->fileName;
			//ファイルを開く
			ifstream fin(this->filePath);
			//ファイルの読み込みに失敗したら
			if (!fin)
			{
				//終了
				return false;
			}

			//最初の読み込みならチップ名を読み込む
			if (this->fileName == "Map00.txt")
			{
				string meshName = "";
				for (int m = 0; m < 4; ++m)
				{
					fin >> meshName;
					DG::Mesh_CreateFromSOBFile(meshName, "./data/mesh/" + meshName + ".SOB");
				}
			}
			//ファイル１つ分のマップサイズの読み込み
			fin >> this->sizeX >> this->sizeZ;
			//当たり判定矩形の設定
			//ファイル１つ分の読み込み
			for (int z = tmpZ + this->sizeZ -1 ; z >= this->tmpZ; --z)
			{
				for (int x = this->tmpX; x < this->tmpX + this->sizeX; ++x)
				{
					//タイプを読み込み
					int in;
					fin >> in;
					
					//座標の指定
					ML::Vec3 pos(
						x * chipX + chipX / 2,
						chipY / 2,
						z * chipZ + chipZ / 2
					);
					//あたり判定矩形のサイズを指定
					ML::Box3D hitBase(
						0,0,0,
						chipX,
						chipY,
						chipZ
					);
					//Boxのコンストラクタで座標と矩形を設定
					this->arr[z][x] = Box(pos, hitBase);
					//マスのタイプを設定
					this->arr[z][x].Type_Read(in);
					//オブジェクトのサイズ変更
					float s = 0;
					ML::Vec3 sca = ML::Vec3(0, 0, 0);
					//タイプによって処理を分ける
					switch ((Type)in)
					{
					//壁なら
					case Type::box:
						this->arr[z][x].Set_MeshName("MapBox");
						break;
					//ブレイカーなら
					case Type::breakerN:
					case Type::breakerS:
						//オブジェクトマネージャでブレイカーを生成
						ge->OM.Create_Breaker(pos, (Type)in);
						break;

					//プレイヤなら
					case Type::player:
						//プレイヤの初期座標を設定
						//															座標
						ge->GetTask_One_G<Player::Object>("プレイヤ")->Ini_Pos(pos - ML::Vec3(0, 150, 0));
						break;

					//各監視カメラなら
					case Type::camera_East:
					case Type::camera_North:
					case Type::camera_South:
					case Type::camera_West:
					case Type::camera_North_East:
					case Type::camera_North_West:
					case Type::camera_South_East:
					case Type::camera_South_West:
						//オブジェクトマネージャで監視カメラを生成
						//						座標					  ,	タイプ
						ge->OM.Create_Camera(pos + ML::Vec3(0, 100, 0), (Type)in);
						break;

					//曲がり角なら
					case Type::corner:
						//エネミー用に高さを足元の高さに設定
						pos.y = 50.0f;
						//オブジェクトマネージャでコーナーを生成
						//						座標,番号
						ge->OM.Push_Back_Conner(pos, num);
						//番号を1増やす
						++num;
						break;
					case Type::lamp:
						ge->OM.Create_Lamp(pos, hitBase);
						break;
					case Type::table:
						s = 150.0f;
						this->arr[z][x].Set_PosY(0);
						this->arr[z][x].Set_Size(s);
						this->arr[z][x].Set_MeshName("Table");
						break;
					case Type::chair:
						s = 150.0f;
						this->arr[z][x].Set_PosY(0);
						this->arr[z][x].Set_Size(s);
						//this->arr[z][x].Set_MeshName("chair");
						break;
					case Type::bed:
						sca = ML::Vec3(100, 125, 100);
						this->arr[z][x].Set_PosY(60.0f);
						this->arr[z][x].Set_Size(sca);
						this->arr[z][x].Set_MeshName("bed");
						break;
					case Type::toilet:
						s= 150.0f;
						this->arr[z][x].Set_PosY(0);
						this->arr[z][x].Set_Size(s);
						//this->arr[z][x].Set_MeshName("toilet");
						break;
					case Type::torch:
						s= 150.0f;
						this->arr[z][x].Set_PosY(0);
						this->arr[z][x].Set_Size(s);
						//this->arr[z][x].Set_MeshName("torch");
						break;
					case Type::bookshelf:
						s = 150.0f;
						this->arr[z][x].Set_PosY(0);
						this->arr[z][x].Set_Size(s);
						//this->arr[z][x].Set_MeshName("bookshelf");
						break;
					case Type::bfloor:
						s = 300.0f;
						this->arr[z][x].Set_PosY(0.01f);
						this->arr[z][x].Set_Size(s);
						this->arr[z][x].Set_MeshName("BloodFloor");
						break;
					}
				}
			}

			//X・Zの基準値更新
			x += this->sizeX;
			z += this->sizeZ;
			this->tmpX = x % this->maxSizeX;
			this->tmpZ = z / this->maxSizeZ * 20;
			//ファイル名の読み込み
			fin >> this->fileName;
			//ファイルを閉じる
			fin.close();
		}
		//コーナー同士の関係性を構築
		ge->OM.Set_Relationship();

		//ドアの生成は完全に別のところで
		for (int z =0; z < this->maxSizeZ; ++z)
		{
			for (int x = 0; x < this->sizeX; ++x)
			{
				//タイプがドアなら
				if (this->arr[z][x].Get_Type() == Type::door)
				{				
					//左にもドアがあれば
					if (this->arr[z][x - 1].Get_Type() == Type::door)
					{
						//右側としてドアを生成
						ge->OM.Create_Door(this->arr[z][x].Get_Pos(), LR::Right);
					}
					//なければ
					else
					{
						//左側としてドアを生成
						ge->OM.Create_Door(this->arr[z][x].Get_Pos(), LR::Left);
					}						
				}
			}
		}
		return true;
	}
	//-------------------------------------------------------------------
	//マップとの接触判定
	//引数：（プレイヤの矩形）
	bool Object::Map_CheckHit(const ML::Box3D& pHit)
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
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
			0,
			0,
			0,
			chipX * this->maxSizeX,
			chipY,
			chipZ * this->maxSizeZ
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
		sx = r.fx / chipX;
		sz = r.fz / chipZ;
		ex = (r.bx - 1) / chipX;
		ez = (r.bz - 1) / chipZ;
		//接触判定開始
		for (int z = sz; z <= ez; ++z) {
			for (int x = sx; x <= ex; ++x) {
				if (this->arr[z][x].Get_Type() == Type::box) {
					return true;
				}
				pl->Check_Clear();
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
	//ゴールとの接触判定
	//引数：（プレイヤの矩形）
	bool Object::Goal_CheckHit(const ML::Box3D& pHit)
	{
		ML::Box3D goalHit = this->goal.Get_HitBase().OffsetCopy(this->goal.Get_Pos());
		return goalHit.Hit(pHit);
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