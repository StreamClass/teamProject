//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Map.h"
#include  "Task_Title.h"
#include  "Task_Camera.h"
#include  "Task_Player.h"
#include  "Task_MiniMap.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//���b�V���̓ǂݍ���
		DG::Mesh_CreateFromSOBFile("ArrowMesh", "./data/mesh/arrow.sob");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase("ArrowMesh");
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

		//�J�����̐ݒ�
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(0.0f, 0.0f, 0.0f),				//	�^�[�Q�b�g�ʒu
			ML::Vec3(0.0f, 500.0f, -500.0f),			//	�J�����ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	�J�����̏�����x�N�g��
			ML::ToRadian(35), 10.0f, 4000.0f,	//	����p�E���싗��
			(float)ge->screenWidth / (float)ge->screenHeight);		//	��ʔ䗦		
		DG::EffectState().param.bgColor = ML::Color(1, 0,0,0);
		//���C�e�B���O�̐ݒ�
		//���C�e�B���O�L����
		//DG::EffectState().param.lightsEnable = true;
		////�����̋�����ݒ肷��
		//DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
		////���s�����̐ݒ�
		//DG::EffectState().param.light[0].enable = true;
		//DG::EffectState().param.light[0].kind = DG_::Light::Directional;//�����̎��
		//DG::EffectState().param.light[0].direction = ML::Vec3(1, -1, 1).Normalize();//�Ǝ˕���
		//DG::EffectState().param.light[0].color = ML::Color(1, 1, 1, 1);//�F�Ƌ���

		//���^�X�N�̐���
		auto pl = Player::Object::Create(true);
		auto cam = Camera::Object::Create(true);
		auto map = Map::Object::Create(true);
		map->Load();		
		auto mm = MiniMap::Object::Create(true);
		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�t�B�[���h");
		ge->KillAll_G("�J�����}��");
		ge->KillAll_G("�v���C��");
		ge->KillAll_G("�h�A");
		ge->KillAll_G("�u���[�J�[");
		ge->KillAll_G("�~�j�}�b�v");

		ge->OM.Finalize();

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			auto Title = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{		
		auto in = DI::GPad_GetState("P1");
		if (in.ST.down)
		{
			this->Kill();
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
		//���s�ړ��s��𐶐� //���[���h�ϊ���K�p����
		ML::Mat4x4  matT;
		matT.Translation(ML::Vec3(0, 0, 0));
		DG::EffectState().param.matWorld = matT;
		DG::Mesh_Draw("ArrowMesh");
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