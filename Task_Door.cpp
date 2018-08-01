//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Door.h"
#include  "MapBox.h"

namespace Task_Door
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "Door_mesh";
		this->shadowMesh = "Door_Shadow";
		//���̃��b�V��
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/door.SOB");
		DG::Mesh_CreateFromSOBFile(this->shadowMesh, "./data/mesh/SquareShadow.SOB");

		this->soundName = "DoorOpenSE";
		DM::Sound_CreateSE(this->soundName, "./data/sound/DoorOpenSE.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
		DG::Mesh_Erase(this->meshName);
		DG::Mesh_Erase(this->shadowMesh);
		DM::Sound_Erase(this->soundName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(Door* d)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->circuit = d;
		this->circuit->Set_SoundName(this->res->soundName);
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//��H�ŃI�[�v����ԂłȂ����̂݊J���锻���		
		if (!this->circuit->Get_State())
		{
			//�Ȃ����Ă���u���[�J�[���m�F���ĊJ�����ǂ������m�F
			this->circuit->Door_Open();
			//�t�F�[�Y�ڍs���邩�ǂ������m�F
			if (this->circuit->Is_Phase_Offset())
			{
				ge->OM.Game_Be_Final_Phase();
			}
		}		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{		
		ML::Mat4x4 matT, matS;
		matS.Scaling(ML::Vec3(chipX,100,chipZ));
		matT.Translation(this->circuit->Get_Pos());

		DG::EffectState().param.matWorld = matS * matT;

		DG::Mesh_Draw(this->res->meshName);

		this->Render3DShadow();
	}

	//-----------------------------------------------------------------------
	//�ǉ����\�b�h
	//�v���C���Ƃ̂����蔻��
	bool Object::Hit_Check(const ML::Box3D& hit)
	{
		//�J���Ă����ԂȂ瓖����Ȃ��������Ƃɂ��ĕԂ�
		if (this->circuit->Get_State())
		{
			return false;
		}
		return this->circuit->Player_Hit_the_Door(hit);
	}
	//�Ȃ����Ă���u���[�J�[�̐��m�F
	int Object::How_Many_Breaker_Be_Cunnected()
	{
		return this->circuit->How_Many_Breaker_Be_Cunnected();
	}
	//
	void Object::Render3DShadow()
	{
		ML::Mat4x4 matT, matS;
		matS.Scaling(ML::Vec3(150,100,40));
		matT.Translation(ML::Vec3(this->circuit->Get_Pos().x,1, this->circuit->Get_Pos().z));
		DG::EffectState().param.matWorld = matS * matT;

		DG::Mesh_Draw(this->res->shadowMesh);
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Door* d)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(d)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Door* d)
	{
		return  this->Initialize(d);
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