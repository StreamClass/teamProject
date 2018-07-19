//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_NowLoading.h"

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
		this->task01_ = "";
		this->task02_ = "";
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
		//if (this->timeCnt == 0)
		//{
		//	this->Stop_Task();
		//}
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
		//if (this->timeCnt == 60 * 4)
		//{
		//	this->Start_Task();
		//}
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
	void Object::Stop_Task()
	{
		if (this->task01_ == "" || this->task02_ == "")
		{
			return;
		}
		ge->StopAll_G(this->task01_);
		ge->StopAll_G(this->task02_);
	}
	//
	void Object::Start_Task()
	{
		ge->StopAll_G(this->task02_, false);
	}
	//
	void Object::Set_TaskName(const string& task01 = "", const string& task02 = "")
	{
		this->task01_ = task01;
		this->task02_ = task02;
	}
	//���̃^�X�N����RGB�l���w��
	void Object::Set_Color(float& rgb)
	{
		this->rgb = rgb;
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