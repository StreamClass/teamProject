//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map.h"

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
		this->sizeX = 0;
		this->sizeZ = 0;
		this->maxSizeX = 100;
		this->maxSizeZ = 100;
		this->tmpX = 0;
		this->tmpZ = 0;
		//マップのゼロクリア
		//ZeroMemory(this->arr, sizeof(this->arr));
		for (int z = 0; z < 100; ++z)
		{
			for (int x = 0; x < 100; ++x)
			{
				this->arr[z][x] = Box();
			}
		}
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
		ML::Mat4x4 fmatS;
		fmatS.Scaling(ML::Vec3(50.0f, 0.1f, 50.0f));
		ML::Mat4x4 fmatT;
		fmatT.Translation(ML::Vec3(2500, -5, 2500));
		DG::EffectState().param.matWorld = fmatS * fmatT;
		DG::Mesh_Draw(this->chipName);

		ML::Mat4x4 matS;
		ML::Mat4x4 matT;
		for (int z = this->maxSizeZ - 1; z >= 0; --z)
		{
			for (int x = 0; x < this->maxSizeX; ++x)
			{
				int chipNum = this->arr[z][x].Get_Type();
				if (chipNum != 1) { continue; }

				matS.Scaling(
				ML::Vec3(
					this->arr[z][x].Get_ChipSizeX() / 100.0f,
					this->arr[z][x].Get_ChipSizeY() / 100.0f,
					this->arr[z][x].Get_ChipSizeZ() / 100.0f
				));
				matT.Translation(this->arr[z][x].Get_Pos());
				DG::EffectState().param.matWorld = matS * matT;
				DG::Mesh_Draw(this->chipName);
			}
		}
	}
	//-------------------------------------------------------------------
	//マップの読み込み
	bool  Object::Load()
	{
		int x = 0, z = 0;
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
						x * this->arr[z][x].Get_ChipSizeX() + this->arr[z][x].Get_ChipSizeX() / 2,
						this->arr[z][x].Get_ChipSizeY() / 2,
						z * this->arr[z][x].Get_ChipSizeZ() + this->arr[z][x].Get_ChipSizeZ() / 2
					);
					this->arr[z][x] = Box(pos, hitBase);
					this->arr[z][x].Type_Read(in);
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