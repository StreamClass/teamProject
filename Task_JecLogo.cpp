//-------------------------------------------------------------------
//���{�d�q���S
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_JecLogo.h"
#include  "Task_NowLoading.h"

namespace JecLogo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//���d���S
		this->logoImgName = "JecLogoImg";
		DG::Image_Create(this->logoImgName, "./data/image/JecLogo.png");
		//�w�i
		this->bgImgName = "BGImg";
		DG::Image_Create(this->bgImgName, "./data/image/Loading.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->logoImgName);
		DG::Image_Erase(this->bgImgName);
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
		this->render2D_Priority[1] = 0.5f;
		this->timeCnt = 0;
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
		//3�b�o������
		if (this->timeCnt == 60 * 3)
		{
			//�t�F�[�h�C���A�E�g
			auto lo = Loading::Object::Create(true);
			lo->Set_NowTask(defGroupName);
			lo->Set_NextTask("�^�C�g��");
			lo->Set_Color(1);
		}
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i�`��
		ML::Box2D draw(0,0,ge->screen2DWidth,ge->screen2DHeight);
		ML::Box2D src(0, 0, 160, 90);
		DG::Image_Draw(this->res->bgImgName, draw, src);
		//���S�`��
		draw = ML::Box2D(ge->screen2DWidth / 2 - 800, ge->screen2DHeight  / 2 - 300, 1600, 600);
		src = ML::Box2D(0, 0, 4251, 731);
		DG::Image_Draw(this->res->logoImgName, draw, src,ML::Color((float)(this->timeCnt / 60.0f),1,1,1));
	}

	void  Object::Render3D_L0()
	{
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