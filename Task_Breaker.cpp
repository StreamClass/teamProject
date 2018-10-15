//-------------------------------------------------------------------
//�u���[�J�[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Breaker.h"
#include  "Task_Enemy.h"

namespace Task_Breaker
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//���b�V���E�����̓ǂݍ���
		this->meshName = "Breaker_Mesh";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/BreakerBase.SOB");
		this->buttonMeshName = "ButtonMesh";
		DG::Mesh_CreateFromSOBFile(this->buttonMeshName, "./data/mesh/BreakerButton.SOB");
		this->soundName = "ButtonPushSE";
		DM::Sound_CreateSE(this->soundName, "./data/sound/pushButton.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
		DG::Mesh_Erase(this->meshName);
		DG::Mesh_Erase(this->buttonMeshName);
		DM::Sound_Erase(this->soundName);
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
		this->pushedButton = false;
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

		//�N��������G�l�~�[�̖ړI�n��ݒ肷��悤�ɂ���
		this->Now_Be_Pushed();
	}
	//-----------------------------------------------------------------------
	//�����̐ݒ�
	float Object::RotationY_Angle(int& angle)
	{
		float angle_;
		//���ʂ�-Z�Ȃ�
		if (this->angle == 2)
		{
			angle_= ML::ToRadian(0);
		}
		//���ʂ�+Z�Ȃ�
		else if (this->angle == 3)
		{
			//180�x��]������
			angle_ = ML::ToRadian(180);
		}
		return angle_;
	}
	//-----------------------------------------------------------------------
	//�{�^���̈ړ�
	ML::Vec3 Object::Move_Button()
	{
		//�u���[�J�[�̏�Ԃ��画�f
		if (this->circuit->Get_Now_State())
		{
			//���ʂ�-Z�Ȃ�
			if (this->angle == 2)
			{
				//�{�^����+Z������1���ړ�
				this->pos.z += 1.0f;
				//�g�[�^����15�ړ�������
				if (this->pos.z > this->circuit->Get_Pos().z + 15.0f)
				{
					//�������W����15�ړ������ʒu�ɌŒ�
					this->pos.z = this->circuit->Get_Pos().z + 15.0f;
					//���߂ĉ����ꂽ�Ȃ�
					if (!this->pushedButton)
					{
						//�v�b�V������炷
						DM::Sound_Play(this->res->soundName, false);
						this->pushedButton = true;
					}
				}
			}
			//���ʂ�+Z�Ȃ�
			else
			{
				//�{�^����-Z������1���ړ�
				this->pos.z -= 1.0f;
				//�g�[�^����15�ړ�������
				if (this->pos.z < this->circuit->Get_Pos().z - 15.0f)
				{
					//�������W����15�ړ������ʒu�ɌŒ�
					this->pos.z = this->circuit->Get_Pos().z - 15.0f;
					//���߂ĉ����ꂽ�Ȃ�
					if (!this->pushedButton)
					{
						//�v�b�V������炷
						DM::Sound_Play(this->res->soundName, false);
						this->pushedButton = true;
					}

				}
			}
		}
		return this->pos;
	}
	//���̃u���[�J�[�̏�Ԃ����炤
	bool Object::Get_Now_State()
	{
		return this->circuit->Get_Now_State();
	}
	//---------------------------------------------------------------------
	//�v���C�����{�^�������������Ƃ��G�l�~�[�ɋ����鏈��
	void Object::Now_Be_Pushed()
	{
		auto en = ge->GetTask_One_G<Enemy::Object>("�G�l�~�[");
		en->Set_Destination(this->pos);
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