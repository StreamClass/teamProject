//-------------------------------------------------------------------
//�u���[�J�[�����v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_LampBase.h"
#include  "Task_Door.h"
#include  "Task_LampGlass.h"

namespace  LampBase
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�����v�x�[�X
		this->meshName = "LampMeshName";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/LampBase.SOB");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
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
		this->pos = ML::Vec3(0, 0, 0);
		this->hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
		this->color = ML::Color(1, 1, 0, 0);

		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�����v");

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
		auto door = ge->GetTask_One_GN<Task_Door::Object>("�h�A", "�o��");
		auto lamps = ge->GetTask_Group_G<LampGlass::Object>("�����v");
		int lampNum = 0;
		for (auto it = lamps->begin(); it != lamps->end(); ++it,++lampNum)
		{
			if (lampNum >= door->How_Many_Breaker_Be_Cunnected())
			{
				break;
			}
			(*it)->Set_Color(this->color);
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}

	//-------------------------------------------------------------------
	//�u3�c�`��v�P�t���[�����ɍs������
	void  Object::Render3D_L0()
	{
		//�����v�̊�Օ`��
		ML::Mat4x4 matT, matS;
		matT.Translation(this->pos);
		matS.Scaling(100);
		DG::EffectState().param.matWorld = matS * matT;
		DG::Mesh_Draw(this->res->meshName);
	}
	//-------------------------------------------------------------------
	//�����v�̍��W�E�����蔻��̏�����
	void Object::Set_Lamp(const ML::Vec3& pos, const ML::Box3D& hitBase)
	{
		//��Ղ̏�����
		this->pos = pos + ML::Vec3(-50.0f, 0, 0);
		this->hitBase = hitBase;
		//�����v�̐���
		for (int i = 0; i < 3; ++i)
		{
			auto lamp = LampGlass::Object::Create(true);
			lamp->Set_Pos(this->pos + ML::Vec3((i - 1) * 60.0f, 0, 0));
		}
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