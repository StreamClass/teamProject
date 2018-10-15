//-------------------------------------------------------------------
//�t�F�[�h�C���E�A�E�g
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_NowLoading.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_GameClear.h"
#include  "Task_GameOver.h"
#include  "Task_JecLogo.h"

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
		//���ԃJ�E���^��2�b�ȉ��Ȃ�
		if (this->timeCnt < 60 * 2)
		{
			//�t�F�[�h�C���A�E�g�̉摜�̕s�����x�̎w��
			this->FadeInOut();
		}
		//0.5�b�̎�
		if (this->timeCnt == 60 * 0.5f)
		{
			//���݂̃^�X�N���w�肵�Ă���Ƃ�
			if(this->nowTask != "")
				ge->KillAll_G(this->nowTask);//�w�肵���^�X�N�����
		}
		//1�b�̎�
		if (this->timeCnt == 60 * 1)
		{
			//���̃^�X�N���w�肵�Ă����
			if (this->nextTask != "")
			{
				//�^�C�g�����w�肵�Ă����
				if (this->nextTask == "�^�C�g��")
				{
					//�^�C�g���^�X�N�𐶐�
					Title::Object::Create(true);
				}
				//�Q�[�����w�肵�Ă����
				else if (this->nextTask == "�Q�[��")
				{
					//�Q�[���^�X�N�𐶐�
					Game::Object::Create(true);
					ge->state = ge->game;
				}
				//�Q�[���I�[�o�[���w�肵�Ă����
				else if (this->nextTask == "�Q�[���I�[�o�[")
				{
					//�Q�[���I�[�o�[��ʂ𐶐�
					Over::Object::Create(true);
				}
				//�Q�[���N���A���w�肵�Ă����
				else if (this->nextTask == "�Q�[���N���A")
				{
					//�Q�[���N���A��ʂ𐶐�
					Clear::Object::Create(true);
				}
				//�f����ʂ��w�肵�Ă����
				else if (this->nextTask == "�f��")
				{
					//�Q�[���^�X�N���f����ԂŐ���
					Game::Object::Create(true);
					ge->state = ge->demo;
				}
				//���S��ʂ��w�肵�Ă����
				else if (this->nextTask == "���d���S")
				{
					//���S�^�X�N�𐶐�
					JecLogo::Object::Create(true);
				}
			}
		}
		//�s�����x��0�����ɂȂ�����
		if (this->alpha < 0)
		{
			//�^�X�N�����
			this->Kill();
		}
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
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(this->alpha,this->rgb,this->rgb,this->rgb));
	}

	void  Object::Render3D_L0()
	{
	}
	//���̃^�X�N���w��
	void Object::Set_NowTask(const string now)
	{
		//���O��ۑ�
		this->nowTask = now;
		//�ۑ������̂�"�Q�[��"�Ȃ�
		if (this->nowTask == "�Q�[��")
		{
			//�Q�[���̏�Ԃɂ���Ď��̃^�X�N���w��
			//�N���A�Ȃ�
			if (ge->state == ge->clear)
			{
				this->nextTask = "�Q�[���N���A";
			}
			//�Q�[���I�[�o�[�Ȃ�
			else if(ge->state == ge->over)
			{
				this->nextTask = "�Q�[���I�[�o�[";
			}
			//�f���Ȃ�
			else if (ge->state == ge->demo)
			{
				this->nextTask = "���d���S";
			}
			//����ȊO�ŌĂяo���Ă�����(game��non)
			else
			{
				this->nextTask = "�^�C�g��";
			}
		}
	}
	//���̃^�X�N���w��
	void Object::Set_NextTask(const string next)
	{
		this->nextTask = next;
	}
	//���̃^�X�N���甒�������w��(���l�ɂ���Ă͊D�F����)
	void Object::Set_Color(float rgb)
	{
		this->rgb = rgb;
	}
	//�t�F�[�h�C���A�E�g���̉摜�̕s�����x�̎w��
	void Object::FadeInOut()
	{
		//�^�X�N��������1�b�Ԃ�����
		if (this->timeCnt < 60 * 1)
		{
			//�s�����x��1��
			this->alpha = this->timeCnt / (60.0f * 0.5f);
			//�s�����x�̏�����w��
			if (this->alpha >= 1.0f)
			{
				this->alpha = 1.0f;
			}
		}
		//1~2�b��1�b�Ԃ�����
		else if (this->timeCnt < 60 * 2)
		{
			//�s�����x��0��
			this->alpha = 1.0f - (this->timeCnt - 60) / (60 * 0.5f);
			//�s�����x�̏�����w��
			if (this->alpha >= 1.0f)
			{
				this->alpha = 1.0f;
			}
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