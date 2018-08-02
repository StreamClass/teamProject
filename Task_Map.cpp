//-------------------------------------------------------------------
//�R�c�}�b�v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map.h"
#include  "Task_Player.h"
#include  "Task_Door.h"

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
		//�e�L�X�g1�������X�EZ���W�̃T�C�Y
		this->sizeX = 0;
		this->sizeZ = 0;
		//�}�b�v�̃T�C�Y
		this->maxSizeX = 100;
		this->maxSizeZ = 100;
		//�ǂݍ��ݎ��ɂ��E���̊�l�Ƃ��Ďg�p
		this->tmpX = 0;
		this->tmpZ = 0;
		//�}�b�v�����ׂď�����
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
		ML::Box3D hitBase(0, 0, 0, int(chipSize.x), 50, int(chipSize.z));
		this->floor = Box(chipSize, pos, hitBase);
		this->floor.Set_MeshName("MapBox");
		//�V��̐��l�̎w��
		//���͏�Ɉړ������邾��
		pos.y += 350.0f;
		this->ceiling = Box(chipSize, pos, hitBase);
		this->ceiling.Set_MeshName("MapBox");
		//�S�[��	�̐��l�w��
		chipSize = ML::Vec3(chipX * 2, chipY, chipZ / 3);
		pos = ML::Vec3(chipX * 3 + chipSize.x / 2, chipY / 2, chipZ * 100 + chipSize.z / 2);
		hitBase = ML::Box3D(0,0,0, int(chipSize.x), int(chipSize.y), int(chipSize.z));
		this->goal = Box(chipSize, pos, hitBase);
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
		for (MapObj* mapobj : this->mapObjects)
		{
			delete mapobj;
		}

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
		//���C�e�B���O�𖳌���
		//�}�b�v�ɂ̓��C�e�B���O��K�����Ȃ�
		DG::EffectState().param.light[0].enable = false;
		//�ǂ̕`��
		ML::Mat4x4 matS,matT;
		//�`�悷��͈͂��J�����𒆐S�ɑO�㍶�E18�}�X���Ɏw��
		int  sx, sz, ex, ez;
		sx = max(0, int(ge->camera[0]->pos.x / chipX) - RENDERRENGE);
		ex = min(this->maxSizeX, int(ge->camera[0]->pos.x / chipX) + RENDERRENGE);
		sz = max(0, int(ge->camera[0]->pos.z / chipZ) - RENDERRENGE);
		ez = min(this->maxSizeZ, int(ge->camera[0]->pos.z / chipZ) + RENDERRENGE);

		//�w�肵���͈͂̕ǂ݂̂�`��
		for (int z = ez - 1; z >= sz; --z)
		{
			for (int x = sx; x < ex; ++x)
			{
				this->arr[z][x].Render3D();
			}
		}
		
		//���̕`��
		this->floor.Render3D();

		//�V��̕`��
		this->ceiling.Render3D();

		//�I�u�W�F�N�g
		for (auto* obj : this->mapObjects)
		{
			//�`�悷��͈͂��w��
			ML::Vec3 len(obj->Get_Pos().x - ge->camera[0]->pos.x,0, obj->Get_Pos().z - ge->camera[0]->pos.z);
			if (len.Length() <=  RENDERRENGE * chipX)
			{
				//�͈͓��̃I�u�W�F�N�g��`��
				obj->Render3D();
			}
		}

		//���C�g�O�Ԃ̃��C�e�B���O��L����
		DG::EffectState().param.light[0].enable = true;

		//�S�[���ʒu�m�F�p
		//ML::Mat4x4 gT, gS;
		//gS.Scaling(this->goal.Get_Scaling());
		//gT.Translation(this->goal.Get_Pos());
		//DG::EffectState().param.matWorld = gS * gT;
		//DG::Mesh_Draw(this->chipName);
	}
	//-------------------------------------------------------------------
	//�}�b�v�̓ǂݍ���
	bool  Object::Load_Map()
	{
		int x = 0, z = 0;
		int num = 0;
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

			//�ŏ��̓ǂݍ��݂Ȃ�`�b�v����ǂݍ���
			if (this->fileName == "Map00.txt")
			{
				int meshNum;
				fin >> meshNum;
				string meshName = "";
				//���ݒǉ����̃I�u�W�F�N�g�̐�������
				for (int m = 0; m < meshNum; ++m)
				{
					//���b�V������ǂݍ���
					fin >> meshName;
					//�摜��p��
					DG::Mesh_CreateFromSOBFile(meshName, "./data/mesh/" + meshName + ".SOB");
				}
			}
			//�t�@�C���P���̃}�b�v�T�C�Y�̓ǂݍ���
			fin >> this->sizeX >> this->sizeZ;
			//�t�@�C���P���̓ǂݍ���
			for (int z = tmpZ + this->sizeZ -1 ; z >= this->tmpZ; --z)
			{
				for (int x = this->tmpX; x < this->tmpX + this->sizeX; ++x)
				{
					//�^�C�v��ǂݍ���
					int in;
					fin >> in;
					
					//���W�̎w��
					ML::Vec3 pos(
						float(x * chipX + chipX / 2.0f),
						chipY / 2.0f,
						float(z * chipZ + chipZ / 2.0f)
					);
					//�����蔻���`�̃T�C�Y���w��
					ML::Box3D hitBase(
						0,0,0,
						chipX,
						chipY,
						chipZ
					);
					//Box�̃R���X�g���N�^�ō��W�Ƌ�`��ݒ�
					this->arr[z][x] = Box(pos, hitBase);
					//�}�X�̃^�C�v��ݒ�
					this->arr[z][x].Type_Read(in);
					//�^�C�v�ɂ���ď����𕪂���
					switch ((Type)in)
					{
					//�ǂȂ�
					case Type::box:
						this->arr[z][x].Set_MeshName("MapBox");
						break;

					//�u���C�J�[�Ȃ�
					case Type::breakerN:
					case Type::breakerS:
						//�I�u�W�F�N�g�}�l�[�W���Ńu���C�J�[�𐶐�
						ge->OM.Create_Breaker(pos, (Type)in);
						break;

					//�v���C���Ȃ�
					case Type::player:
						//�v���C���̏������W��ݒ�
						//															���W
						ge->GetTask_One_G<Player::Object>("�v���C��")->Ini_Pos(pos - ML::Vec3(0, 150, 0));
						break;

					//�e�Ď��J�����Ȃ�
					case Type::camera_East:
					case Type::camera_North:
					case Type::camera_South:
					case Type::camera_West:
					case Type::camera_North_East:
					case Type::camera_North_West:
					case Type::camera_South_East:
					case Type::camera_South_West:
						//�I�u�W�F�N�g�}�l�[�W���ŊĎ��J�����𐶐�
						//						���W					  ,	�^�C�v
						ge->OM.Create_Camera(pos + ML::Vec3(0, 100, 0), (Type)in);
						break;

					//�Ȃ���p�Ȃ�
					case Type::corner:
						//�G�l�~�[�p�ɍ����𑫌��̍����ɐݒ�
						pos.y = 50.0f;
						//�I�u�W�F�N�g�}�l�[�W���ŃR�[�i�[�𐶐�
						//						���W,�ԍ�
						ge->OM.Push_Back_Conner(pos, num);
						//�ԍ���1���₷
						++num;
						break;
					//�u���[�J�[�����v�Ȃ�
					case Type::lamp:
						//���W�Ƃ����蔻����w�肷��
						ge->OM.Create_Lamp(pos, hitBase);
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
		//�R�[�i�[���m�̊֌W�����\�z
		ge->OM.Set_Relationship();

		//�h�A�̐����͊��S�ɕʂ̂Ƃ����
		for (int z =0; z < this->maxSizeZ; ++z)
		{
			for (int x = 0; x < this->sizeX; ++x)
			{
				//�^�C�v���h�A�Ȃ�
				if (this->arr[z][x].Get_Type() == Type::door)
				{				
					//���ɂ��h�A�������
					if (this->arr[z][x - 1].Get_Type() == Type::door)
					{
						//�E���Ƃ��ăh�A�𐶐�
						ge->OM.Create_Door(this->arr[z][x].Get_Pos(), LR::Right);
					}
					//�Ȃ����
					else
					{
						//�����Ƃ��ăh�A�𐶐�
						ge->OM.Create_Door(this->arr[z][x].Get_Pos(), LR::Left);
					}						
				}
			}
		}
		return true;
	}
	//-------------------------------------------------------------------
	//�I�u�W�F�N�g�̓ǂݍ���
	bool Object::Load_Objects()
	{
		ifstream fin("./data/StageData/MapObjects.txt");
		if (!fin)//���s������
		{
			//�I��
			return false;
		}

		//�ǂݍ��ޗp�̃f�[�^�̓��ꕨ��p��
		ML::Vec3 p(0, 0, 0);
		ML::Vec3 a(0, 0, 0);
		int s = 0;
		ML::Box3D h(0, 0, 0, 0, 0, 0);
		string m = "";

		while (true)//break�܂�
		{
			fin >> m;//���b�V�����̓ǂݍ���
			if (m == "end")//�ǂݍ��񂾕�����end�Ȃ�
			{
				//while���甲���o��
				break;
			}
			//���WXYZ ����XYZ�@�����蔻��WHD �T�C�Y �̏��ɓǂݍ���
			//�e�L�X�g�ɔ���������ƃo�O�ƂȂ�̂�
			//������Ɛ���10������
			fin >> p.x >> p.y >> p.z >> a.x >> a.y >> a.z
				>> h.w >> h.h >> h.d >> s;

			//
			h.x = -h.w / 2; h.y = -h.h / 2; h.z = -h.d / 2;
			//����(�x��)����N�H�[�^�j�I���ɕϊ�
			ML::QT qtx = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(a.x));
			ML::QT qty = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(a.y));
			ML::QT qtz = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(a.z));
			//��L������
			ML::QT qtA = qtx * qty * qtz;

			//�R�s�[�R���X�g���N�^�Œl��ݒ肵�A�x�N�^�[�ɒǉ�
			mapObjects.push_back(new MapObj(p, qtA, h, s, m));
		}
		//����ɏI��
		return true;
	}
	//-------------------------------------------------------------------
	//�}�b�v�Ƃ̐ڐG����
	//�����F�i�v���C���̋�`�j
	bool Object::Map_CheckHit(const ML::Box3D& pHit)
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		//�ǂݍ��񂾋�`�̍ő�A�ŏ����_�̍��W
		struct Box3D_2Point
		{
			int fx, fy, fz;//�l�����������̓_
			int bx, by, bz;//�l���傫�����̓_
		};
		//�v���C���̔���p���_��ݒ�
		Box3D_2Point r =
		{
			pHit.x,			pHit.y,			pHit.z,
			pHit.x + pHit.w,pHit.y + pHit.h,pHit.z + pHit.d
		};
		//�}�b�v�̔���p���_��ݒ�
		Box3D_2Point m =
		{
			0,0,0,
			chipX * this->maxSizeX,
			chipY,
			chipZ * this->maxSizeZ
		};

		//�L�����N�^�̋�`���}�b�v�͈͓��Ɋۂߍ���
		if (r.fx < m.fx) { r.fx = m.fx; }
		if (r.fz < m.fz) { r.fz = m.fz; }
		if (r.bx > m.bx) { r.bx = m.bx; }
		if (r.bz > m.bz) { r.bz = m.bz; }

		//�L�����N�^���}�b�v�͈͊O�ɂ����S�ɏo�Ă����画��I��
		if (r.bx <= r.fx) { return false; }
		if (r.bz <= r.fz) { return false; }
		//���[�v�͈͂����
		int sx, sz, ex, ez;
		sx = r.fx / chipX;
		sz = r.fz / chipZ;
		ex = (r.bx - 1) / chipX;
		ez = (r.bz - 1) / chipZ;
		//�ڐG����J�n
		for (int z = sz; z <= ez; ++z) {
			for (int x = sx; x <= ex; ++x) {
				if (this->arr[z][x].Get_Type() == Type::box) {
					return true;
				}
				pl->Check_Clear();
			}
		}
		auto d = ge->GetTask_Group_GN<Task_Door::Object>("�h�A","NoName");
		for (auto it = d->begin(); it != d->end(); it++)
		{
			if ((*it)->Hit_Check(pHit))
			{
				return true;
			}
		}
		for (auto obj : this->mapObjects)
		{
			if (obj->HitCheck(pHit))
			{
				return true;
			}
		}
		return false;//�ڐG������̂����o����Ȃ�����
	}
	//-------------------------------------------------------------------
	//�S�[���Ƃ̐ڐG����
	//�����F�i�v���C���̋�`�j
	bool Object::Goal_CheckHit(const ML::Box3D& pHit)
	{
		ML::Box3D goalHit = this->goal.Get_HitBase().OffsetCopy(this->goal.Get_Pos());
		return goalHit.Hit(pHit);
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