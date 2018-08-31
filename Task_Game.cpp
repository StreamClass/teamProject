//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_NowLoading.h"
#include  "Task_Map.h"
#include  "Task_GameClear.h"
#include  "Task_GameOver.h"
#include  "Task_Title.h"
#include  "Task_Camera.h"
#include  "Task_Player.h"
#include  "Task_MiniMap.h"
#include  "Task_Enemy.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//BGM�w��
		this->bgmName = "GameBGM";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/GameBGM.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DM::Sound_Erase(this->bgmName);
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
		this->pushButton = false;
		this->timeCnt = 0;
		this->stanbyCnt = 0;
		this->startTime = 60 * 2;

		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//���C�e�B���O�̐ݒ�
		//���C�e�B���O�L����
		DG::EffectState().param.lightsEnable = true;
		//�����̋�����ݒ肷��
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.5f, 0.5f, 0.5f);
		////���s�����̐ݒ�
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//�����̎��
		DG::EffectState().param.light[0].direction = ML::Vec3(1, 0, 1).Normalize();//�Ǝ˕���
		DG::EffectState().param.light[0].color = ML::Color(1, 0.2f,0.2f,0.2f);//�F�Ƌ���

		//BGM�Đ�
		DM::Sound_Play(this->res->bgmName, true);
		//�傫���������߉��ʂ�����������(800 / 1000)
		DM::Sound_Volume(this->res->bgmName, 800);

		//���^�X�N�̐���
		auto pl = Player::Object::Create(true);
		auto cam = Camera::Object::Create(true);
		auto en = Enemy::Object::Create(true);
		auto map = Map::Object::Create(true);
		auto mm = MiniMap::Object::Create(true);
		/*
		�~�j�}�b�v�ŊĎ��J�����̍��W��ۑ����邽�߂�
		�~�j�}�b�v�𐶐��������ƂɃ}�b�v���t�@�C������ǂݍ���
		(�I�u�W�F�N�g�̓~�j�}�b�v�̑O�ł���)
		*/
		map->Load_Map();
		map->Load_Objects();

		//DG::Mesh_CreateFromSOBFile("bbb", "./data/mesh/.SOB");
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//DG::Mesh_Erase("bbb");
		//���f�[�^���^�X�N���
		ge->KillAll_G("�t�B�[���h");
		ge->KillAll_G("�J�����}��");
		ge->KillAll_G("�v���C��");
		ge->KillAll_G("�h�A");
		ge->KillAll_G("�u���[�J�[");
		ge->KillAll_G("�~�j�}�b�v");
		ge->KillAll_G("�G�l�~�[");
		ge->KillAll_G("�����v���");

		ge->OM.Finalize();
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
		auto in = DI::GPad_GetState(ge->controllerName);
		//�ŏ��̃t���[���Ȃ�
		if (this->stanbyCnt == 0)
		{
			//�G�l�~�[���~
			ge->StopAll_G("�G�l�~�[", true);
			this->stanbyCnt++;
		}
		//�X�^���o�C���̏���
		else if (this->stanbyCnt == 1)//�J�����̏����ʒu��ݒ肷��悤��1�t���[���̂ݓ�����
		{
			ge->StopAll_G("�v���C��", true);
			this->stanbyCnt++;
		}
		//�X�^�[�g����^�C�~���O�ɂȂ�܂�
		else if(this->stanbyCnt < this->startTime)
		{
			this->stanbyCnt++;
			//�f�o�b�O�p
			//B1(Xor��)����������
			if (in.B1.down)
			{
				//�X�^�[�g����^�C�~���O�܂Ŕ�΂�
				this->stanbyCnt = this->startTime - 1;
			}
		}
		//�X�^�[�g����^�C�~���O�Ȃ�
		else if(this->stanbyCnt == this->startTime)
		{
			//�Q�[�����J�n
			this->Start();
		}
		//�X�^�[�g�ƃZ���N�g��������
		if (in.ST.down && in.SE.down)
		{
			//�Q�[���I�[�o�[��
			ge->state = ge->over;
		}
		//�N���A��Ԃŏ��߂Ẵt���[���Ȃ�
		if (ge->state == ge->clear && !this->pushButton)
		{
			//�t�F�[�h�C���A�E�g
			auto lo = Loading::Object::Create(true);
			lo->Set_NowTask(defGroupName);
			lo->Set_Color(1);
			//��ԑJ�ڑҋ@��Ԃ�
			this->pushButton = true;
			DM::Sound_FadeOut(this->res->bgmName);
		}
		//�Q�[���I�[�o�[��Ԃŏ��߂Ẵt���[���Ȃ�
		if (ge->state == ge->over && !this->pushButton)
		{
			//�t�F�[�h�C���A�E�g
			auto lo = Loading::Object::Create(true);
			lo->Set_NowTask(defGroupName);
			lo->Set_Color(0);
			//��ԑJ�ڑҋ@��Ԃ�
			this->pushButton = true;
			DM::Sound_FadeOut(this->res->bgmName);
		}
		//�f����ԂȂ�
		if (ge->state == ge->demo)
		{
			//�f����32�b�o������
			if (!this->pushButton && (this->timeCnt == 60 * 32 || this->PushAnyButton()))
			{
				//�t�F�[�h�C���A�E�g
				auto lo = Loading::Object::Create(true);
				lo->Set_NowTask(defGroupName);
				lo->Set_Color(1);
				this->pushButton = true;
				DM::Sound_FadeOut(this->res->bgmName);
			}
			this->timeCnt++;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}
	//-------------------------------------------------------------------
	void  Object::Render3D_L0()
	{
		//ML::Mat4x4 matT, matS;
		//matT.Translation(ML::Vec3(825, 1, 825));
		//matS.Scaling(100);
		//DG::EffectState().param.matWorld = matS * matT;
		//DG::Mesh_Draw("bbb");
	}
	//-------------------------------------------------------------------
	//�Q�[���J�n����
	void Object::Start()
	{
		ge->StopAll_G("�v���C��", false);
		ge->StopAll_G("�G�l�~�[", false);
	}
	//������̃{�^����������true��Ԃ�
	bool Object::PushAnyButton()
	{
		auto in = DI::GPad_GetState(ge->controllerName);
		return (in.B1.down || in.B2.down || in.B3.down || in.B4.down || in.SE.down || in.ST.down);
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