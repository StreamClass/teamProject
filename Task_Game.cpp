//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_NowLoading.h"
#include  "Task_Map.h"
#include  "Task_GameClear.h"
#include  "Task_GameOver.h"
#include  "Task_Title.h"
#include  "Task_Camera.h"
#include  "Task_Player.h"
#include  "Task_MiniMap.h"
#include  "Task_Enemy.h"

namespace  Game
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
		ge->state = ge->standby;
		this->pushButton = false;
		this->timeCnt = 0;
		this->stanbyCnt = 0;
		this->startTime = 60 * 2;

		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//���C�e�B���O�̐ݒ�
		//���C�e�B���O�L����
		DG::EffectState().param.lightsEnable = true;
		//�����̋�����ݒ肷��
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.5f, 0.5f, 0.5f);
		////���s�����̐ݒ�
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//�����̎��
		DG::EffectState().param.light[0].direction = ML::Vec3(1, 0, 1).Normalize();//�Ǝ˕���
		DG::EffectState().param.light[0].color = ML::Color(1, 0.2f,0.2f,0.2f);//�F�Ƌ���

		//���^�X�N�̐���
		auto pl = Player::Object::Create(true);
		auto cam = Camera::Object::Create(true);
		auto en = Enemy::Object::Create(true);
		auto map = Map::Object::Create(true);
		map->Load();		
		auto mm = MiniMap::Object::Create(true);

		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�t�B�[���h");
		ge->KillAll_G("�J�����}��");
		ge->KillAll_G("�v���C��");
		ge->KillAll_G("�h�A");
		ge->KillAll_G("�u���[�J�[");
		ge->KillAll_G("�~�j�}�b�v");
		ge->KillAll_G("�G�l�~�[");

		ge->OM.Finalize();

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			//auto nextTask = Clear::Object::Create(true);
			if (ge->state == ge->clear)
			{
				auto nextTask = Clear::Object::Create(true);
			}
			else if (ge->state == ge->over)
			{
				auto nextTask = Over::Object::Create(true);
			}
			else
			{
				auto nextTask = Title::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{		
		auto in = DI::GPad_GetState("P1");

		if (this->stanbyCnt == 0)
		{
			ge->StopAll_G("�G�l�~�[", true);
			this->stanbyCnt = 1;
		}
		//�X�^���o�C���̏���
		else if (this->stanbyCnt == 1)
		{
			ge->StopAll_G("�v���C��", true);
			this->stanbyCnt++;
		}
		else if(this->stanbyCnt < this->startTime)
		{
			this->stanbyCnt++;
			if (in.B1.down)
			{
				this->stanbyCnt = this->startTime;
			}
		}
		else if(this->stanbyCnt == this->startTime)
		{
			this->Start();
		}
		if (in.ST.down && this->pushButton == false)
		{
			ge->state = ge->over;
			this->pushButton = true;
			auto lo = Loading::Object::Create(true);
			float color = 0.0f;
			lo->Set_Color(color);
		}
		if (ge->state == ge->clear && this->pushButton == false)
		{
			auto lo = Loading::Object::Create(true);
			float color = 1.0f;
			lo->Set_Color(color);
			this->pushButton = true;
		}
		if (ge->state == ge->over && this->pushButton == false)
		{
			auto lo = Loading::Object::Create(true);
			float color = 0.0f;
			lo->Set_Color(color);
			this->pushButton = true;
		}
		if (this->pushButton)
		{
			this->timeCnt++;
		}
		if (this->timeCnt == 60 * 1)
		{
			this->Kill();
		}
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
	//�Q�[���J�n����
	void Object::Start()
	{
		ge->StopAll_G("�v���C��", false);
		ge->StopAll_G("�G�l�~�[", false);
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