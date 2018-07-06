//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Aim.h"
#include  "Task_Player.h"
#include  "Task_Map.h"

namespace  Aiming
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�G�C���̉摜
		this->imageName[0] = "AimCenterImg";
		DG::Image_Create(this->imageName[0], "./data/image/AimCenter.png");
		this->imageName[1] = "AimUDImg";
		DG::Image_Create(this->imageName[1], "./data/image/aimTB.png");
		this->imageName[2] = "AimLRImg";
		DG::Image_Create(this->imageName[2], "./data/image/aimLR.png");
		//�ʏ펞�̑������
		this->normalModeImg[0] = "Nomal01Img";
		DG::Image_Create(this->normalModeImg[0], "./data/image/NormalImg00.png");
		this->normalModeImg[1] = "Nomal02Img";
		DG::Image_Create(this->normalModeImg[1], "./data/image/NormalImg01.png");
		//�^�u���b�g���̑������
		this->tabletModeImg[0] = "Tablet01Img";
		DG::Image_Create(this->tabletModeImg[0], "./data/image/TabletImg01.png");
		this->tabletModeImg[1] = "Tablet02Img";
		DG::Image_Create(this->tabletModeImg[1], "./data/image/TabletImg00.png");
		//�X�^�~�i�̉摜
		this->staminaImgName[0] = "StaminaMax";
		DG::Image_Create(this->staminaImgName[0], "./data/image/StaminaMax.png");
		this->staminaImgName[1] = "StaminaGage";
		DG::Image_Create(this->staminaImgName[1], "./data/image/StaminaGage.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 3; ++i)
		{
			DG::Image_Erase(this->imageName[i]);
		}
		for (int i = 0; i < 2; ++i)
		{
			DG::Image_Erase(this->normalModeImg[i]);
			DG::Image_Erase(this->tabletModeImg[i]);
			DG::Image_Erase(this->staminaImgName[i]);
		}
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
		this->render2D_Priority[1] = 0.2f;
		this->hitBase = ML::Box3D(0, 0, 0, 1, 1, 1);
		this->aimPosC = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2);
		this->aimPosT = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2 - 15);
		this->aimPosB = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2 + 15);
		this->aimPosL = ML::Vec2(ge->screen2DWidth / 2 - 15, ge->screen2DHeight / 2);
		this->aimPosR = ML::Vec2(ge->screen2DWidth / 2 + 15, ge->screen2DHeight / 2);
		this->aimMoveMax = 20.0f;
		this->timeCnt = 0;
		this->pos = ML::Vec3(0, 0, 0);
		this->aimMoveSpeed = 0;
		this->aimMovetremor = 5.0f;
		DG::Mesh_CreateFromSOBFile("t", "./data/mesh/box1.sob");
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		DG::Mesh_Erase("t");

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
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (pl->Is_Used_Tablet() == false)
		{
			this->aimMoveSpeed = 5.0f;
			this->aimMovetremor = pl->Get_MoveSpeed() + 5.0f;
			if (pl->Is_Tired())
			{
				this->aimMoveSpeed = 8.0f/*(MAX_STAMINA / 3 - pl->Get_Stamina()) / 10.0f*/;
				this->aimMovetremor =  (pl->Get_MoveSpeed() + 5.0f) * 5.0f;
			}
			this->aimPosT.y = -sin(ML::ToRadian(this->moveCnt * this->aimMoveSpeed)) * this->aimMovetremor + (ge->screen2DHeight / 2.0f - (15 + this->aimMovetremor));
			this->aimPosB.y = sin(ML::ToRadian(this->moveCnt * this->aimMoveSpeed)) * this->aimMovetremor + (ge->screen2DHeight / 2.0f + (15 + this->aimMovetremor));
			this->aimPosL.x = -sin(ML::ToRadian(this->moveCnt * this->aimMoveSpeed)) * this->aimMovetremor + (ge->screen2DWidth / 2.0f - (15 + this->aimMovetremor));
			this->aimPosR.x = sin(ML::ToRadian(this->moveCnt * this->aimMoveSpeed)) * this->aimMovetremor + (ge->screen2DWidth / 2.0f + (15 + this->aimMovetremor));
			if (pl->Get_MoveSpeed() >= -1.0f && pl->Get_MoveSpeed() <= 1.0f && pl->Is_Tired() == false)
			{
				this->moveCnt = 0;
			}
		}
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (pl->Is_Used_Tablet() == true)
		{
			this->TabletModeRrender();
			return;
		}
		this->NormalModeRrender();
		this->AimingRender();
		this->StaminaRender();
	}
	//-------------------------------------------------------------------
	//�u3�c�`��v�P�t���[�����ɍs������
	void  Object::Render3D_L0()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		ML::Mat4x4 matT, matS;
		matT.Translation(this->pos + ML::Vec3(pl->Get_Pos().x,0,pl->Get_Pos().z));
		matS.Scaling(ML::Vec3(this->hitBase.w / 100, this->hitBase.h / 100, this->hitBase.d / 100));
		DG::EffectState().param.matWorld = matS * matT;
		DG::Mesh_Draw("t");
	}
	//-------------------------------------------------------------------
	//�ʏ펞�̑������
	void Object::NormalModeRrender()
	{
		ML::Box2D draw(0, 1020, 1920, 60);
		ML::Box2D src(0, 0, 1920, 60);
		DG::Image_Draw(this->res->normalModeImg[(this->timeCnt / 20) % 2], draw, src);
	}
	//-------------------------------------------------------------------
	//�^�u���b�g���̑������
	void Object::TabletModeRrender()
	{
		ML::Box2D draw(0, 1020, 1920, 60);
		ML::Box2D src(0, 0, 1920, 60);
		DG::Image_Draw(this->res->tabletModeImg[(this->timeCnt / 20) % 2], draw, src);
	}
	//-------------------------------------------------------------------
	//�G�C���̃A�j���[�V����
	void Object::AimingRender()
	{
		//�����_
		ML::Box2D draw(-5, -5, 9, 9);
		ML::Box2D src(0, 0, 9, 9);
		draw.Offset(this->aimPosC);
		DG::Image_Draw(this->res->imageName[0], draw, src);
		//�G�C����
		draw = ML::Box2D(-3, -10, 5, 19);
		src = ML::Box2D(0, 0, 9, 39);
		draw.Offset(this->aimPosT);
		DG::Image_Draw(this->res->imageName[1], draw, src);
		//�G�C����
		draw = ML::Box2D(-3, -10, 5, 19);
		draw.Offset(this->aimPosB);
		DG::Image_Draw(this->res->imageName[1], draw, src);
		//�G�C���E
		draw = ML::Box2D(-10, -3, 19, 5);
		src = ML::Box2D(0, 0, 39, 9);
		draw.Offset(this->aimPosR);
		DG::Image_Draw(this->res->imageName[2], draw, src);
		//�G�C����
		draw = ML::Box2D(-10, -3, 19, 5);
		draw.Offset(this->aimPosL);
		DG::Image_Draw(this->res->imageName[2], draw, src);
	}
	//-------------------------------------------------------------------
	//�X�^�~�i�̕`��
	void Object::StaminaRender()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		
		ML::Box2D draw(ge->screen2DWidth - 100, ge->screen2DHeight - 300, 50, 240);
		ML::Box2D src(0, 0, 200, 500);
		DG::Image_Draw(this->res->staminaImgName[0], draw, src);

		draw = ML::Box2D(ge->screen2DWidth - 100, ge->screen2DHeight - 300 + (MAX_STAMINA -  pl->Get_Stamina()), 50, pl->Get_Stamina());
		DG::Image_Draw(this->res->staminaImgName[1], draw, src);
	}
	//-------------------------------------------------------------------
	//�u���[�J�[�Ƃ̐ڐG����
	ML::Box3D Object::Get_HitBase()
	{
		return this->hitBase;
	}
	//
	void Object::Set_Pos(ML::Vec3& pos_)
	{
		this->pos = pos_;
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