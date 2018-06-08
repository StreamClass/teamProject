//-------------------------------------------------------------------
//�J�����}��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Camera.h"
#include  "Task_Player.h"

namespace  Camera
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
		this->angle = ML::Vec3(0, 0, 0);
		this->dist = ML::Vec3(100, 0, 0);
		
		//�J�����̐ݒ�
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(0.0f, 0.0f, 0.0f),				//	�^�[�Q�b�g�ʒu
			ML::Vec3(0.0f, 500.0f, -500.0f),			//	�J�����ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	�J�����̏�����x�N�g��
			ML::ToRadian(35), 10.0f, 8000.0f,	//	����p�E���싗��
			(float)ge->screenWidth / (float)ge->screenHeight);		//	��ʔ䗦		
		
		//�t�H�O(��)�̐ݒ�
		DG::EffectState().param.fogEnable = true;
		DG::EffectState().param.fogColor = ML::Color(1, 0, 0, 0);
		DG::EffectState().param.fogFore = 1800.0f;
		DG::EffectState().param.fogMode = true;
		DG::EffectState().param.fogNear = 1300.0f;

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
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");//[180517-����]�J�����}���̌������v���C���Ɠ�������

		//�^�u���b�g���g�p���Ȃ�X�V���~�߂�
		if (pl->Is_Used_Tablet())
		{
			return;
		}
		//�����_�̋���
		//�J�����}���̍��W�Ƀv���C�����W����
		this->pos = pl->Get_Pos();
		//�J�����}���̃A���O���Ƀv���C���̃A���O������
		this->angle = pl->Get_Angle();
		//�����_
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);
		ML::Vec3 vec = ML::Vec3(800, pl->Get_Adjust(), 0);
		vec = matR.TransformCoord(vec);

		ge->camera[0]->target = this->pos + vec;
		ge->camera[0]->pos = this->pos + ML::Vec3(0, pl->Get_PointView(), 0);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
	}
	//-------------------------------------------------------------------
	void  Object::Render3D_L0()
	{
		
	}
	//-------------------------------------------------------------------
	
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