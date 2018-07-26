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
		this->eImgName = "BloodImg";
		DG::Image_Create(this->eImgName, "./data/image/OverEffect.png");
		this->lImgName = "LgImg";
		DG::Image_Create(this->lImgName, "./data/image/OverLogo.png");
		//this->bgMeshName = "EndingBG";
		//DG::Mesh_CreateFromSOBFile(this->bgMeshName, "");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		//�摜�����ׂĉ��
		DG::Image_Erase(this->eImgName);
		DG::Image_Erase(this->lImgName);
		DG::Mesh_Erase(this->bgMeshName);
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
		this->endFlag = false;


		//
		this->iniFlag = true;

		//�J�����̐ݒ�
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(0.0f, 0.0f, 0.0f),				//	�^�[�Q�b�g�ʒu
			ML::Vec3(0.0f, 0.0f, -200.0f),			//	�J�����ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	�J�����̏�����x�N�g��
			ML::ToRadian(35), 15.0f, 8000.0f,	//	����p�E���싗��
			(float)ge->screenWidth / (float)ge->screenHeight);		//	��ʔ䗦		
		DG::EffectState().param.bgColor = ML::Color(0.5f, 1, 1, 1);
		//���C�e�B���O�L����
		DG::EffectState().param.lightsEnable = true;
		//
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//�����̎��
		DG::EffectState().param.light[0].direction = ML::Vec3(0, 0, 1).Normalize();//�Ǝ˕���
		DG::EffectState().param.light[0].color = ML::Color(1, 0.8f, 0.5f, 0.5f);//�F�Ƌ���

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		delete this->enBone;
		this->enBone = nullptr;

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
		auto in = DI::GPad_GetState(ge->controllerName);
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
			this->enBone->Repeat_Now_Motioin();
			this->enBone->UpDate();
			this->pos.x -= 3;
			this->enBone->Moving(this->pos);
		}
		//5�b�ォ��
		else if (this->timeCnt > 60 * 5)
		{
			//3�b�����ĕs�����x��1��
			this->al = (this->timeCnt - 60 * 5) / 60.0f * 3.0f;
		}
		//15�b��(���S�����ׂďo�Ă���7�b��)����endFlag��false�Ȃ�
		else if (this->timeCnt > 60 * 15 && this->endFlag == false)
		{
			//���[�f�B���O���Ăяo��
			auto lo = Loading::Object::Create(true);
			//���[�f�B���O��ʂ̐F�����Ɏw��
			float color = 0.0f;
			lo->Set_Color(color);
			//endFlag��true��
			this->endFlag = true;
		}
		if (this->timeCnt > 60 && this->iniFlag)
		{
			//
			ML::Vec3 pos(0, 0, 0);
			this->enBone->Moving(pos);
			float radi = ML::ToRadian(-90);
			this->enBone->Bone_RotateY_All(radi);
			this->enBone->Set_Next_Motion("Running");
			this->iniFlag = false;
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
		//�L�����N�^����ʍ�������o�čs������
		if (this->timeCnt > 60 * 8)
		{
			//�G�t�F�N�g����ʑS�̂ɕ`��
			DG::Image_Draw(this->res->eImgName, draw, src);
		}
		//���S�`��
		draw = ML::Box2D(0, 350, 1920, 300);
		src = ML::Box2D(0, 0, 1920, 300);
		//														 �s�����x
		DG::Image_Draw(this->res->lImgName, draw, src, ML::Color(this->al, 1, 1, 1));
	}

	void  Object::Render3D_L0()
	{
		this->enBone->Render();
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