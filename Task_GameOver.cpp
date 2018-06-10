//-------------------------------------------------------------------
//�Q�[���I�[�o�[���
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameOver.h"
#include  "Task_Title.h"
#include  "Task_NowLoading.h"

namespace  Over
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�e�C���[�W���w��
		//�摜��ǂݍ���
		this->bImgName = "BGImg";
		DG::Image_Create(this->bImgName, "./data/image/OverBG.png");
		this->cImgName = "CharaImg";
		DG::Image_Create(this->cImgName, "./data/image/OverChara.png");
		this->eImgName = "BloodImg";
		DG::Image_Create(this->eImgName, "./data/image/OverEffect.png");
		this->lImgName = "LgImg";
		DG::Image_Create(this->lImgName, "./data/image/OverLogo.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		//�摜�����ׂĉ��
		DG::Image_Erase(this->bImgName);
		DG::Image_Erase(this->cImgName);
		DG::Image_Erase(this->eImgName);
		DG::Image_Erase(this->lImgName);
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
		this->render2D_Priority[1] = 1.0f;
		this->al = 0.0f;
		this->timeCnt = 0;
		this->endCnt = 0;
		this->cPos = ML::Vec2(ge->screen2DWidth - 500, ge->screen2DHeight - 400);
		this->endFlag = false;
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
			auto nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		//endFlag��false�̎���start�{�^���������ƃ��[�f�B���O���Ăяo��
		if (in.ST.down && this->endFlag == false)
		{
			auto lo = Loading::Object::Create(true);
			//���[�f�B���O��ʂ̐F�����Ɏw��
			float color = 0.0f;
			lo->Set_Color(color);
			this->endFlag = true;
		}
		//2�b�ォ��
		if (this->timeCnt > 60 * 2)
		{
			//�L�����N�^������6�h�b�g���ړ�
			this->cPos.x -= (this->timeCnt - 120) * 6;
		}
		//5�b�ォ��
		if (this->timeCnt > 60 * 5)
		{
			//3�b�����ĕs�����x��1��
			this->al = (this->timeCnt - 60 * 5) / 60.0f * 3.0f;
		}
		//15�b��(���S�����ׂďo�Ă���7�b��)����endFlag��false�Ȃ�
		if (this->timeCnt > 60 * 15 && this->endFlag == false)
		{
			//���[�f�B���O���Ăяo��
			auto lo = Loading::Object::Create(true);
			//���[�f�B���O��ʂ̐F�����Ɏw��
			float color = 0.0f;
			lo->Set_Color(color);
			//endFlag��true��
			this->endFlag = true;
		}
		//endFlag��true�Ȃ�
		if (this->endFlag == true)
		{
			//�J�E���g�J�n
			this->endCnt++;
		}
		//endCnt��2�b����������
		if (this->endCnt > 60 * 2)
		{
			//�^�X�N�����
			this->Kill();
		}
		//���S�̕s�����x�̏�����w��
		if (this->al > 1.0f)
		{
			this->al = 1.0f;
		}
		//�^�X�N�̃t���[�������J�E���g
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i�`��
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D src(0, 0, 1920, 1080);
		DG::Image_Draw(this->res->bImgName, draw, src);
		//�L�����N�^����ʍ�������o�čs������
		if (this->cPos.x < -300)
		{
			//�G�t�F�N�g����ʑS�̂ɕ`��
			DG::Image_Draw(this->res->eImgName, draw, src);
		}

		//�L�����N�^�`��
		draw = ML::Box2D(0, 0, 300, 400);
		src = ML::Box2D(0, 0, 300, 400);
		draw.Offset(this->cPos);
		DG::Image_Draw(this->res->cImgName,draw,src);
		
		//���S�`��
		draw = ML::Box2D(0, 350, 1920, 300);
		src = ML::Box2D(0, 0, 1920, 300);
		//														 �s�����x
		DG::Image_Draw(this->res->lImgName, draw, src, ML::Color(this->al, 1, 1, 1));
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