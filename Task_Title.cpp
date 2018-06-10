//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_NowLoading.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->bgImgName = "TitleImg";
		DG::Image_Create(this->bgImgName, "./data/image/TitleBG01.bmp");
		this->loImgName = "LogoImg";
		DG::Image_Create(this->loImgName, "./data/image/TitleLogo.png");
		this->sbImgName = "StartButtonImg";
		DG::Image_Create(this->sbImgName, "./data/image/StartButton.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->bgImgName);
		DG::Image_Erase(this->loImgName);
		DG::Image_Erase(this->sbImgName);
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
		//�{�^���p�J�E���g
		this->timeCnt = 0;
		//Start�{�^���������������f
		this->pushSon = false;

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
			auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		//�X�^�[�g�{�^������������
		if (in.ST.down)
		{
			this->pushSon = true;
		}
		//3�b���
		if (this->timeCnt == 60 * 3)
		{
			//���[�f�B���O�Ăяo��
			auto lo = Loading::Object::Create(true);
			//�F�����Ɏw��
			float color = 0.0f;
			lo->Set_Color(color);
		}
		//5�b���
		if (this->timeCnt == 60 * 5)
		{
			//����
			this->Kill();
		}
		//�{�^���������Ă�����
		if (this->pushSon)
		{
			//�J�E���g�J�n
			this->timeCnt++;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//TitleBG�`��
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 1920, 1080);
		DG::Image_Draw(this->res->bgImgName, draw, src);
		//�^�C�g�����S�`��
		draw = ML::Box2D(100, 200, 1300, 300);
		src = ML::Box2D(0, 0, 1300, 300);
		DG::Image_Draw(this->res->loImgName, draw, src);
		//��Ԃ𔻒f����
		if (this->LogoAnim())
		{
			//�{�^����`��
			draw = ML::Box2D(400, 700, 345, 80);
			src = ML::Box2D(0, 0, 332, 63);
			DG::Image_Draw(this->res->sbImgName, draw, src);
		}
	}
	//
	void  Object::Render3D_L0()
	{
		
	}

	//���\�b�h
	//StartButton���S�̃A�j���[�V��������
	bool Object::LogoAnim()
	{
		//�{�^����������Ă��Ȃ�������
		if (!this->pushSon)
		{
			//��ɕ`��
			return true;
		}
		//������Ă�����
		if (this->pushSon)
		{
			//20�t���[���Ԋu�ŕ`��
			if ((this->timeCnt / 20) % 2 == 0)
			{
				return true;
			}
		}
		//����ȊO�͌����Ȃ��悤�ɂ���
		return false;
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