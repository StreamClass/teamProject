//-------------------------------------------------------------------
//�Q�[���N���A���
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameClear.h"
#include  "Task_Title.h"
#include  "Task_NowLoading.h"

namespace  Clear
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�w�i
		this->imageName[0] = "BGImg";
		DG::Image_Create(this->imageName[0], "./data/image/ClearBG.png");
		//�_00
		this->imageName[1] = "Cloud00Img";
		DG::Image_Create(this->imageName[1], "./data/image/Cloud00.png");
		//�_01
		this->imageName[2] = "Cloud01Img";
		DG::Image_Create(this->imageName[2], "./data/image/Cloud01.png");
		//�N���A�e�L�X�g
		this->imageName[3] = "TextImg";
		DG::Image_Create(this->imageName[3], "./data/image/ClearText.png");
		//�w�i���y
		this->clearBgm = "ClearBgm";
		DM::Sound_CreateStream(this->clearBgm, "./data/sound/GameClear_BGM.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 4; ++i)
		{
			DG::Image_Erase(this->imageName[i]);
		}
		DM::Sound_Erase(this->clearBgm);
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
		srand((unsigned int)time(NULL));
		this->render2D_Priority[1] = 0.5f;
		this->timeCnt = 0;
		this->cloud00pos = ML::Vec2(1920, 0);
		this->cloud01pos = ML::Vec2(960, 300);
		this->alpha = 0.0f;
		this->endflag = false;
		//bgm�Đ�
		DM::Sound_Play(this->res->clearBgm,true);
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		DM::Sound_Stop(this->res->clearBgm);

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
		auto in = DI::GPad_GetState(ge->controllerName);
		//start�{�^���������Ɖ��
		//�^�X�N��������17�b��Ƀ��[�f�B���O��ʂ��Ăяo��
		if ((this->timeCnt == 60 * 17.0f || in.ST.down) && !this->endflag)
		{
			auto lo = Loading::Object::Create(true);
			lo->Set_NowTask(defGroupName);
			lo->Set_NextTask("���d���S");
			//���[�f�B���O�̐F�𔒂Ɏw��
			lo->Set_Color(1);
			this->endflag = true;
			DM::Sound_FadeOut(this->res->clearBgm);
		}
		/*��ʂ������Ă���2�b��(�^�X�N��������4�b��)
		����3�b�����ăQ�[���N���A�̃��S��\��*/
		if (this->timeCnt > 60 * 4 && this->timeCnt < 60 * 7 )
		{
			//�^�X�N��������`��J�n�܂ł�4�b������������180�t���[�����ŕ���
			this->alpha = (this->timeCnt - 60.0f * 4.0f) / 180.0f;
		}
		//���S�̕s�����x�̉������w��
		if (this->alpha < 0.0f)
		{
			this->alpha = 0.0f;
		}
		//���S�̕s�����x�̏�����w��
		if (this->alpha >= 1.0f)
		{
			this->alpha = 1.0f;
		}
		/*�_00����ʂ̍��[����o��������Y���W��100�`400�h�b�g�̊ԂŎw�肵
		��ʂ̉E�[�Ɏw��*/
		if (this->cloud00pos.x < -400.0f)
		{
			this->cloud00pos.y = float(rand() % 300) + 100;
			this->cloud00pos.x = 1920;
		}
		/*�_01����ʂ̍��[����o��������Y���W��100�`400�h�b�g�̊ԂŎw�肵
		��ʂ̉E�[�Ɏw��*/
		if (this->cloud01pos.x < -400.0f)
		{
			this->cloud01pos.y = float(rand() % 300) + 100;
			this->cloud01pos.x = 1920;
		}
		//���t���[��3�h�b�g���Ɉړ�
		this->cloud00pos.x -= 3.0f;
		this->cloud01pos.x -= 3.0f;
		//�t���[�������J�E���g
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i�`��
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 1920, 1080);
		DG::Image_Draw(this->res->imageName[0], draw, src);
		//�_00�`��
		draw = ML::Box2D(0, 0, 400, 250);
		src = ML::Box2D(0, 0, 200, 100);
		draw.Offset(this->cloud00pos);
		DG::Image_Draw(this->res->imageName[1], draw, src);
		//�_01�`��
		draw = ML::Box2D(0, 0, 400, 250);
		draw.Offset(this->cloud01pos);
		DG::Image_Draw(this->res->imageName[2], draw, src);
		//�e�L�X�g�`��
		draw = ML::Box2D(0, 200, 1920, 300);
		src = ML::Box2D(0, 0, 1920, 300);
		DG::Image_Draw(this->res->imageName[3], draw, src,ML::Color(this->alpha,1,1,1));
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