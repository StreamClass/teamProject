//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map.h"
#include  "Task_Player.h"


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
		DG::Image_Erase(this->imageName);
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
		//天井の数値の指定
		//今は上に移動させるだけ
		pos.y += 350.0f;
		this->ceiling = Box(chipSize, pos, hitBase);
		//ゴール	の数値指定
		chipSize = ML::Vec3(200, 300, 50);
		pos = ML::Vec3(500, 150, 10025);
		hitBase = ML::Box3D(0,0,0, 200, 300, 50);
		this->goal = Box(chipSize, pos, hitBase);
		//チップ名の初期化
		this->chipName = "box1.sob";
		//マップチップを設定
		DG::Mesh_CreateFromSOBFile(this->chipName, "./data/Mesh/" + this->chipName);
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
		DG::Mesh_Erase(this->chipName);

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
		//壁の描画
		ML::Mat4x4 matS,matT;
		int  sx, sz, ex, ez;
		sx = max(0, int(ge->camera[0]->pos.x / 100) - 18);
		ex = min(this->maxSizeX, int(ge->camera[0]->pos.x / 100) + 18);
		sz = max(0, int(ge->camera[0]->pos.z / 100) - 18);
		ez = min(this->maxSizeZ, int(ge->camera[0]->pos.z / 100) + 18);

		for (int z = ez - 1; z >= sz; --z)
		{
			for (int x = sx; x < ex; ++x)
			{
				int chipNum = this->arr[z][x].Get_Type();
				if (chipNum != Type::box) { continue; }

				matS.Scaling(this->arr[z][x].Get_Scaling());
				matT.Translation(this->arr[z][x].Get_Pos());
				DG::EffectState().param.matWorld = matS * matT;
				DG::Mesh_Draw(this->chipName);
			}
		}
		//床の描画
		ML::Mat4x4 fmatT, fmatS;
		fmatS.Scaling(this->floor.Get_Scaling());
		fmatT.Translation(this->floor.Get_Pos());
		DG::EffectState().param.matWorld = fmatS * fmatT;
		DG::Mesh_Draw(this->chipName);

		//天井の描画
		ML::Mat4x4 cmatT, cmatS;
		cmatS.Scaling(this->ceiling.Get_Scaling());
		cmatT.Translation(this->ceiling.Get_Pos());
		DG::EffectState().param.matWorld = cmatS * cmatT;
		DG::Mesh_Draw(this->chipName);
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
			//ファイル１つ分のマップサイズの読み込み
			fin >> this->sizeX >> this->sizeZ;
			//当たり判定矩形の設定
			//ファイル１つ分の読み込み
			for (int z = tmpZ + this->sizeZ -1 ; z >= this->tmpZ; --z)
			{
				for (int x = this->tmpX; x < this->tmpX + this->sizeX; ++x)
				{
					int in;
					fin >> in;
					
					ML::Vec3 pos(
						x * this->arr[z][x].Get_ChipSizeX() + this->arr[z][x].Get_ChipSizeX() / 2,
						this->arr[z][x].Get_ChipSizeY() / 2,
						z * this->arr[z][x].Get_ChipSizeZ() + this->arr[z][x].Get_ChipSizeZ() / 2
					);
					ML::Box3D hitBase(
						0,0,0,
						this->arr[z][x].Get_ChipSizeX(),
						this->arr[z][x].Get_ChipSizeY(),
						this->arr[z][x].Get_ChipSizeZ()
					);
					this->arr[z][x] = Box(pos, hitBase);
					this->arr[z][x].Type_Read(in);
					switch ((Type)in)
					{
					case Type::breaker:
						ge->OM.Create_Breaker(pos);
						break;
					case Type::player:
						ge->GetTask_One_G<Player::Object>("プレイヤ")->Ini_Pos(pos-ML::Vec3(0,150,0));
						break;
					case Type::camera_East:
					case Type::camera_North:
					case Type::camera_South:
					case Type::camera_West:
					case Type::camera_North_East:
					case Type::camera_North_West:
					case Type::camera_South_East:
					case Type::camera_South_West:
						ge->OM.Create_Camera(pos+ML::Vec3(0,100,0), (Type)in);
						break;
					case Type::corner:
						ge->OM.Push_Back_Conner(pos, num);
						++num;
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
		ge->OM.Set_Relationship();
		//ドアの生成は完全に別のところで
			for (int z =0; z < this->maxSizeZ; ++z)
			{
				for (int x = 0; x < this->sizeX; ++x)
				{
					if (this->arr[z][x].Get_Type() == Type::door)
					{					
						if (this->arr[z][x - 1].Get_Type() == Type::door)
						{
							ge->OM.Create_Door(this->arr[z][x].Get_Pos(), LR::Right);
						}
						else
						{
							ge->OM.Create_Door(this->arr[z][x].Get_Pos(), LR::Left);
						}						
					}
				}
			}
			return true;
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