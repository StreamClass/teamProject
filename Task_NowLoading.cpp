//-------------------------------------------------------------------
//�t�F�[�h�C���E�A�E�g
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_NowLoading.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_GameClear.h"
#include  "Task_GameOver.h"

namespace  Loading
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�摜�ǂݍ���
		this->imageName = "LoadingImg";
		DG::Image_Create(this->imageName, "./data/image/Loading.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		//�摜�̉��
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
		this->render2D_Priority[1] = 0.1f;
		this->timeCnt = 0;
		this->alpha = 0.0f;
		this->color = ML::Color(0, 0, 0, 0);
		this->nowTask = "";
		this->nextTask = "";
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
		//
		if (this->timeCnt < 60 * 4)
		{
			this->FadeInOut();
		}
		//
		if (this->timeCnt == 60)
		{
			if(this->nowTask != "")
				ge->KillAll_G(this->nowTask);
		}
		//
		if (this->timeCnt == 60 * 2)
		{
			//
			if (this->nextTask != "")
			{
				//
				if (this->nextTask == "�^�C�g��")
				{
					Title::Object::Create(true);
				}
				//
				else if (this->nextTask == "�Q�[��")
				{
					Game::Object::Create(true);
				}
				//
				else if (this->nextTask == "�Q�[���I�[�o�[")
				{
					Over::Object::Create(true);
				}
				//
				else if (this->nextTask == "�Q�[���N���A")
				{
					Clear::Object::Create(true);
				}
				//
				else if (this->nextTask == "�f��")
				{

				}
				//
				else if (this->nextTask == "���d���S")
				{

				}
			}
		}
		//�s�����x��0�����ɂȂ�����
		if (this->alpha < 0)
		{
			//�^�X�N�����
			this->Kill();
		}
		//�s�����x�̏�����w��
		if (this->alpha >= 1.0f)
		{
			this->alpha = 1.0f;
		}
		//���[�f�B���O��ʂ̕s�����x�E�F���w��
		this->color = ML::Color(this->alpha, this->rgb, this->rgb, this->rgb);
		//�t���[�������J�E���g
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���[�f�B���O��ʕ`��
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 160, 90);
		DG::Image_Draw(this->res->imageName, draw, src, this->color);
	}

	void  Object::Render3D_L0()
	{
	}
	//
	void Object::Set_NowTask(const string now)
	{
		this->nowTask = now;
		if (this->nowTask == "�Q�[��")
		{
			if (ge->state == ge->clear)
			{
				this->nextTask = "�Q�[���N���A";
			}
			else if(ge->state == ge->over)
			{
				this->nextTask = "�Q�[���I�[�o�[";
			}
			else
			{
				this->nextTask = "�^�C�g��";
			}
		}
	}
	//
	void Object::Set_NextTask(const string next)
	{
		this->nextTask = next;
	}
	//���̃^�X�N����RGB�l���w��
	void Object::Set_Color(float rgb)
	{
		this->rgb = rgb;
	}
	//
	void Object::FadeInOut()
	{
		//�^�X�N��������2�b�Ԃ�����
		if (this->timeCnt < 60 * 2)
		{
			//�s�����x��1��
			this->alpha += this->timeCnt / 120.f;
		}
		//2~4�b��2�b�Ԃ�����
		else if (this->timeCnt < 60 * 4)
		{
			//�s�����x��0��
			this->alpha -= (this->timeCnt - 120) / 120.0f;
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