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
		this->bgMeshName = "TitleBGImg";
		this->loImgName = "LogoImg";
		this->sbImgName = "StartButtonImg";
		DG::Mesh_CreateFromSOBFile(this->bgMeshName, "./data/mesh/TitleBG.SOB");
		DG::Image_Create(this->loImgName, "./data/image/TitleLogo.png");
		DG::Image_Create(this->sbImgName, "./data/image/StartButton.png");
		this->bgmName = "TitmeBGM";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/TitleBGM.wav");
		this->startSEName = "StartButtonPushBGM";
		DM::Sound_CreateSE(this->startSEName, "./data/sound/StartSE.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->bgMeshName);
		DG::Image_Erase(this->loImgName);
		DG::Image_Erase(this->sbImgName);
		DM::Sound_Erase(this->bgmName);
		DM::Sound_Erase(this->startSEName);
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
		//�{�^���p�J�E���g
		this->timeCnt = 0;
		//Start�{�^���������������f
		this->pushSon = false;
		//
		this->eneBone = new Bone(180, "Enemy");
		ML::Vec3 pos(80, -160, 0);
		this->eneBone->Moving(pos);
		float radi = ML::ToRadian(200);
		this->eneBone->Bone_RotateY_All(radi);
		int ran = rand() % 3;
		switch (ran)
		{
		case 0:
			break;
		case 1:
			this->eneBone->Set_Next_Motion("Walking");
			break;
		case 2:
			this->eneBone->Set_Next_Motion("Running");
			break;
		}

		//�J�����̐ݒ�
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(0.0f, 0.0f, 0.0f),				//	�^�[�Q�b�g�ʒu
			ML::Vec3(0.0f, 0.0f, -240.0f),			//	�J�����ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	�J�����̏�����x�N�g��
			ML::ToRadian(35), 15.0f, 8000.0f,	//	����p�E���싗��
			(float)ge->screenWidth / (float)ge->screenHeight);		//	��ʔ䗦		
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//���C�e�B���O�L����
		DG::EffectState().param.lightsEnable = true;
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.2f, 0.2f, 0.2f);
		//
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Point;//�����̎��
		DG::EffectState().param.light[0].range = 200.0f;
		DG::EffectState().param.light[0].attenuation = (1 / 350.0f)*(1 / 350.0f);
		DG::EffectState().param.light[0].pos = ML::Vec3(-100, 50, 180);
		DG::EffectState().param.light[0].color = ML::Color(1, 1.0f, 0.0f, 0.0f);//�F�Ƌ���

		//DG::EffectState().param.light[1].enable = true;
		//DG::EffectState().param.light[1].kind = DG_::Light::Directional;//�����̎��
		//DG::EffectState().param.light[1].direction = ML::Vec3(1, 0, 0).Normalize();//�Ǝ˕���
		//DG::EffectState().param.light[1].color = ML::Color(1, 0.8f, 0.2f, 0.2f);//�F�Ƌ���

		DM::Sound_Play(this->res->bgmName, true);
		//���^�X�N�̐���
		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		delete this->eneBone;
		this->eneBone = nullptr;
		DM::Sound_Stop(this->res->bgmName);

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
		auto in = DI::GPad_GetState(ge->controllerName);
		
		this->eneBone->Repeat_Now_Motioin();
		this->eneBone->UpDate();

		//�X�^�[�g�{�^������������
		if ((in.ST.down || in.B1.down || in.B2.down || in.B3.down || in.B4.down) && this->pushSon == false)
		{
			DM::Sound_Play(this->res->startSEName,false);
			this->pushSon = true;
		}
		//�f�o�b�O���p
		else if (in.B1.down && this->pushSon == true)
		{
			this->Kill();
		}

		//3�b���
		if (this->timeCnt == 60 * 3)
		{
			//���[�f�B���O�Ăяo��
			auto lo = Loading::Object::Create(true);
			//�F�����Ɏw��
			float color = 0.0f;
			lo->Set_Color(color);

			//DM::Sound_Stop("TitleBGM");
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
		//�^�C�g�����S�`��
		ML::Box2D draw(100, 200, 1300, 300);
		ML::Box2D src(0, 0, 1300, 300);
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
		
		ML::Mat4x4 matT, matS;
		matT.Translation(ML::Vec3(0, 0, 200));
		matS.Scaling(100 << 3);
		DG::EffectState().param.matWorld = matS * matT;
		DG::Mesh_Draw(this->res->bgMeshName);
		//
		this->eneBone->Render();
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