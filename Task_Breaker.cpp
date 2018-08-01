//-------------------------------------------------------------------
//�u���[�J�[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Breaker.h"

namespace Task_Breaker
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "Breaker_Mesh";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/BreakerBase.SOB");
		this->buttonMeshName = "ButtonMesh";
		DG::Mesh_CreateFromSOBFile(this->buttonMeshName, "./data/mesh/BreakerButton.SOB");
		this->soundName = "ButtonPushSE";
		//DM::Sound_CreateSE(this->soundName, "./data/sound/");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
		DG::Mesh_Erase(this->meshName);
		DG::Mesh_Erase(this->buttonMeshName);
		//DM::Sound_Erase(this->soundName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(Breaker* b, int angle)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->circuit = b;

		this->angle = angle;
		this->pos = this->circuit->Get_Pos();
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
		//�u���[�J�[�x�[�X
		ML::Mat4x4 matT, matS, matR;
		matS.Scaling(100);
		matT.Translation(this->circuit->Get_Pos());
		matR.RotationY(this->RotationY_Angle(this->angle));
		DG::EffectState().param.matWorld = matS * matR * matT;
		DG::Mesh_Draw(this->res->meshName);

		//�{�^��
		ML::Mat4x4 bmatT;
		bmatT.Translation(this->Move_Button());
		DG::EffectState().param.matWorld = matS * matR * bmatT;
		DG::Mesh_Draw(this->res->buttonMeshName);
	}
	//-----------------------------------------------------------------------
	//�v���C���Ƃ̂����蔻��
	bool Object::Hit_Check(const ML::Box3D& hit)
	{
		return this->circuit->Player_Touch_Breaker(hit);
	}
	//-----------------------------------------------------------------------
	//�u���[�J�[�N��
	void Object::ActivateBreaker()
	{
		this->circuit->Activate_Breaker();
		//DM::Sound_Play(this->res->soundName,false);
	}
	//-----------------------------------------------------------------------
	//�����̐ݒ�
	float Object::RotationY_Angle(int& angle)
	{
		float angle_;
		if (this->angle == 2)
		{
			angle_= ML::ToRadian(0);
		}
		else if (this->angle == 3)
		{
			angle_ = ML::ToRadian(180);
		}
		return angle_;
	}
	//-----------------------------------------------------------------------
	//�{�^���̈ړ�
	ML::Vec3 Object::Move_Button()
	{
		if (this->circuit->Get_Now_State())
		{
			if (this->angle == 2)
			{
				this->pos.z += 1.0f;
				if (this->pos.z > this->circuit->Get_Pos().z + 15.0f)
				{
					this->pos.z = this->circuit->Get_Pos().z + 15.0f;
				}
			}
			else
			{
				this->pos.z -= 1.0f;
				if (this->pos.z < this->circuit->Get_Pos().z - 15.0f)
				{
					this->pos.z = this->circuit->Get_Pos().z - 15.0f;
				}
			}
			return this->pos;
		}
		return this->pos;
	}
	//
	bool Object::Get_Now_State()
	{
		return this->circuit->Get_Now_State();
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Breaker* b, int angle)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(b, angle)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Breaker* b, int angle)
	{
		return  this->Initialize(b, angle);
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