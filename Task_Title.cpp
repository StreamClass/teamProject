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
		//�w�i
		this->bgMeshName = "TitleBGImg";
		DG::Mesh_CreateFromSOBFile(this->bgMeshName, "./data/mesh/TitleBG.SOB");
		//�^�C�g�����S
		this->loImgName = "LogoImg";
		DG::Image_Create(this->loImgName, "./data/image/TitleLogo02.png");
		//Start���S
		this->sbImgName = "StartButtonImg";
		DG::Image_Create(this->sbImgName, "./data/image/StartButton.png");
		//BGM
		this->bgmName = "TitmeBGM";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/TitleBGM.wav");
		//�X�^�[�g��SE
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
		this->sTimeCnt = 0;
		this->timeCnt = 0;
		//Start�{�^���������������f
		this->pushSon = false;
		//�G�l�~�[����
		this->eneBone = new Bone(180, "Enemy");
		//���W�w��@���f
		ML::Vec3 pos(80, -160, 0);
		this->eneBone->Moving(pos);
		//�����w��@���f
		float radi = ML::ToRadian(200);
		this->eneBone->Bone_RotateY_All(radi);
		//�g�p����A�j���[�V�����̎�ޖ���vector�ɕۑ�
		this->motionName.push_back("Running");
		this->motionName.push_back("Walking");
		//�ۑ��������O�ŃA�j���[�V������ǂݍ���
		{
			//�A�j���[�V������ۑ�����e�������
			std::vector<Motion::Motion_Data> running;
			std::vector<Motion::Motion_Data> walking;
			//���O����A�j���[�V�����t�@�C�����J���ėe��ɓ����
			Motion::Make_Motion(&running, this->motionName[0]);
			Motion::Make_Motion(&walking, this->motionName[1]);
			//��ޖ��ŃA�j���[�V�������Ăяo����悤�ɂ���
			this->eneBone->Registrate_Motion(running, this->motionName[0]);
			this->eneBone->Registrate_Motion(walking, this->motionName[1]);
		}
		//�����_����0�`2���w��
		int ran = rand() % 2;
		//�Ή�����A�j���[�V�����̌Ăяo��
		this->eneBone->Set_Next_Motion(this->motionName[ran]);

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
		//�_������ݒ�
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Point;//�����̎��
		DG::EffectState().param.light[0].range = 200.0f;
		DG::EffectState().param.light[0].attenuation = (1 / 350.0f)*(1 / 350.0f);
		DG::EffectState().param.light[0].pos = ML::Vec3(-130, 90, 180);
		DG::EffectState().param.light[0].color = ML::Color(1, 1.0f, 1.0f, 1.0f);//�F�Ƌ���

		//BGM���Đ�
		DM::Sound_Play(this->res->bgmName, true);
		//���^�X�N�̐���
		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		//�G�l�~�[�̃{�[�������
		delete this->eneBone;
		//�����Ăяo����Ȃ��悤��nullptr���Q�Ƃ�����
		this->eneBone = nullptr;
		//BGM���~
		DM::Sound_Stop(this->res->bgmName);

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
		//50�t���[�����ƂɃ|�C���g���C�g�̊g��k���̐؂�ւ�
		if (this->timeCnt % 100 / 50 == 0)
		{
			//�|�C���g���C�g�̑傫����2���傫������
			DG::EffectState().param.light[0].range += 2;
		}
		else
		{
			//�|�C���g���C�g�̑傫����2������������
			DG::EffectState().param.light[0].range -= 2;
		}
		
		auto in = DI::GPad_GetState(ge->controllerName);
		//���݂̃A�j���[�V���������s�[�g����
		this->eneBone->Repeat_Now_Motioin();
		//�A�j���[�V�����X�V
		this->eneBone->UpDate();
		//���ԃJ�E���^��10�b���X�^�[�g���ĂȂ��Ƃ�
		if (this->timeCnt == 60 * 12 && !this->pushSon)
		{
			//�t�F�[�h�C���A�E�g
			auto lo = Loading::Object::Create(true);
			lo->Set_NowTask(defGroupName);
			//���̃^�X�N���f���Ɏw��
			lo->Set_NextTask("�f��");
			//��
			lo->Set_Color(1);
			DM::Sound_FadeOut(this->res->bgmName);
		}

		//�X�^�[�g�{�^������������
		if ((in.ST.down || in.B1.down || in.B2.down || in.B3.down || in.B4.down) && this->pushSon == false)
		{
			//�X�^�[�g��SE���Đ�
			DM::Sound_Play(this->res->startSEName,false);
			//�X�^�[�g��Ԃ�
			this->pushSon = true;
		}

		//3�b���
		if (this->sTimeCnt == 60 * 3)
		{
			//���[�f�B���O�Ăяo��
			auto lo = Loading::Object::Create(true);
			lo->Set_NowTask(defGroupName);
			lo->Set_NextTask("�Q�[��");
			//�F�����Ɏw��
			lo->Set_Color(0);
			DM::Sound_FadeOut(this->res->bgmName);
			//DM::Sound_Stop("TitleBGM");
		}
		//�{�^���������Ă�����
		if (this->pushSon)
		{
			//�J�E���g�J�n
			this->sTimeCnt++;
		}
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�^�C�g�����S�`��
		ML::Box2D draw(100, 200, 1300, 300);

		POINT sizelg = DG::Image_Size(this->res->loImgName);
		ML::Box2D src(0, 0, sizelg.x, sizelg.y);
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
	//-------------------------------------------------------------------
	//�u�R�`��v�P�t���[�����ɍs������
	void  Object::Render3D_L0()
	{
		//�w�i�`��
		ML::Mat4x4 matT, matS;
		matT.Translation(ML::Vec3(0, 0, 200));
		matS.Scaling(100 << 3);
		DG::EffectState().param.matWorld = matS * matT;
		DG::Mesh_Draw(this->res->bgMeshName);
		//�G�l�~�[�`��
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