//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map.h"
#include  "Task_Player.h"

namespace  Map
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render3D_Priority[1] = 1.0f;
		this->sizeX = 0;
		this->sizeZ = 0;
		this->maxSizeX = 100;
		this->maxSizeZ = 100;
		this->tmpX = 0;
		this->tmpZ = 0;
		//�}�b�v�̃[���N���A
		//ZeroMemory(this->arr, sizeof(this->arr));
		for (int z = 0; z < this->maxSizeZ; ++z)
		{
			for (int x = 0; x < this->maxSizeX; ++x)
			{
				this->arr[z][x] = Box();
			}
		}
		//���̐��l�w��
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
		ML::Box3D hitBase(0, 0, 0, pos.x, pos.y, pos.z);
		this->floor = Box(chipSize, pos, hitBase);
		//�V��̐��l�̎w��
		//���͏�Ɉړ������邾��
		pos.y += 300.0f;
		hitBase.h = pos.y;
		this->ceiling = Box(chipSize, pos, hitBase);
		//�`�b�v���̏�����
		this->chipName = "box1.sob";
		//�}�b�v�`�b�v��ݒ�
		DG::Mesh_CreateFromSOBFile(this->chipName, "./data/Mesh/" + this->chipName);
		//�ǂݍ��ރt�@�C�����̏�����
		this->fileName = "Map00.txt";
		//�t�@�C���p�X�̏�����
		this->filePath = "";

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		DG::Mesh_Erase(this->chipName);

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{
		//�ǂ̕`��
		ML::Mat4x4 matS,matT;
		for (int z = this->maxSizeZ - 1; z >= 0; --z)
		{
			for (int x = 0; x < this->maxSizeX; ++x)
			{
				int chipNum = this->arr[z][x].Get_Type();
				if (chipNum != 1) { continue; }

				matS.Scaling(this->arr[z][x].Get_Scaling());
				matT.Translation(this->arr[z][x].Get_Pos());
				DG::EffectState().param.matWorld = matS * matT;
				DG::Mesh_Draw(this->chipName);
			}
		}
		//���̕`��
		ML::Mat4x4 fmatT, fmatS;
		fmatS.Scaling(this->floor.Get_Scaling());
		fmatT.Translation(this->floor.Get_Pos());
		DG::EffectState().param.matWorld = fmatS * fmatT;
		DG::Mesh_Draw(this->chipName);

		//�V��̕`��
		ML::Mat4x4 cmatT, cmatS;
		cmatS.Scaling(this->ceiling.Get_Scaling());
		cmatT.Translation(this->ceiling.Get_Pos());
		DG::EffectState().param.matWorld = cmatS * cmatT;
		DG::Mesh_Draw(this->chipName);
	}
	//-------------------------------------------------------------------
	//�}�b�v�̓ǂݍ���
	bool  Object::Load()
	{
		int x = 0, z = 0;
		//�ǂݍ��񂾃t�@�C������end�ȊO�̊ԉ�
		while (this->fileName != "end")
		{
			//�ǂݍ��ރt�@�C���p�X�̐ݒ�
			this->filePath = "./data/stageData/" + this->fileName;
			//�t�@�C�����J��
			ifstream fin(this->filePath);
			//�t�@�C���̓ǂݍ��݂Ɏ��s������
			if (!fin)
			{
				//�I��
				return false;
			}
			//�t�@�C���P���̃}�b�v�T�C�Y�̓ǂݍ���
			fin >> this->sizeX >> this->sizeZ;
			//�����蔻���`�̐ݒ�
			//�t�@�C���P���̓ǂݍ���
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
					switch ((Type)in)
					{
					case Type::breaker:
						ge->OM.Create_Breaker(pos);
						break;
					case Type::door:
						if (this->arr[z][x - 1].Get_Type() == Type::door)
						{
							ge->OM.Create_Door(pos, LR::Right);
						}
						else
						{
							ge->OM.Create_Door(pos, LR::Left);
						}
						break;
					case Type::player:
						ge->GetTask_One_G<Player::Object>("�v���C��")->Ini_Pos(pos-ML::Vec3(0,150,0));
						break;
					}					
				}
			}
			//X�EZ�̊�l�X�V
			x += this->sizeX;
			z += this->sizeZ;
			this->tmpX = x % this->maxSizeX;
			this->tmpZ = z / this->maxSizeZ * 20;
			//�t�@�C�����̓ǂݍ���
			fin >> this->fileName;
			//�t�@�C�������
			fin.close();
		}
		return true;
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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