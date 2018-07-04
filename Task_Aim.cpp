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
		this->hitBase = ML::Box3D(-150, 100, -150, 300, 100, 300);
		this->aimPosC = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2);
		this->aimPosT = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2 - 15);
		this->aimPosB = ML::Vec2(ge->screen2DWidth / 2, ge->screen2DHeight / 2 + 15);
		this->aimPosL = ML::Vec2(ge->screen2DWidth / 2 - 15, ge->screen2DHeight / 2);
		this->aimPosR = ML::Vec2(ge->screen2DWidth / 2 + 15, ge->screen2DHeight / 2);
		this->aimMoveMax = 20.0f;
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
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (pl->Is_Used_Tablet() == false)
		{
			this->aimPosT.y = -sin(ML::ToRadian(this->moveCnt) * 2.0f) * (pl->Get_MoveSpeed() + 5.0f) + (ge->screen2DHeight / 2.0f - (15 + (pl->Get_MoveSpeed() + 5.0f)));
			this->aimPosB.y =  sin(ML::ToRadian(this->moveCnt) * 2.0f) * (pl->Get_MoveSpeed() + 5.0f) + (ge->screen2DHeight / 2.0f + (15 + (pl->Get_MoveSpeed() + 5.0f)));
			this->aimPosL.x = -sin(ML::ToRadian(this->moveCnt) * 2.0f) * (pl->Get_MoveSpeed() + 5.0f) + (ge->screen2DWidth / 2.0f - (15 + (pl->Get_MoveSpeed() + 5.0f)));
			this->aimPosR.x =  sin(ML::ToRadian(this->moveCnt) * 2.0f) * (pl->Get_MoveSpeed() + 5.0f) + (ge->screen2DWidth / 2.0f + (15 + (pl->Get_MoveSpeed() + 5.0f)));
			this->moveCnt++;
			if (pl->Get_MoveSpeed() >= -1.0f && pl->Get_MoveSpeed() <= 1.0f )
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
			this->TabletMode();
			return;
		}
		this->NormalMode();
		this->AimingRender();
	}
	//-------------------------------------------------------------------
	//�u3�c�`��v�P�t���[�����ɍs������
	void  Object::Render3D_L0()
	{
	}

	//-------------------------------------------------------------------
	//�ʏ펞�̑������
	void Object::NormalMode()
	{
		ML::Box2D draw(0, 1020, 1920, 60);
		ML::Box2D src(0, 0, 1920, 60);
		DG::Image_Draw(this->res->normalModeImg[(this->timeCnt / 20) % 2], draw, src);
	}
	//-------------------------------------------------------------------
	//�^�u���b�g���̑������
	void Object::TabletMode()
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
	//�u���[�J�[�Ƃ̐ڐG����
	ML::Box3D Object::Get_HitBase()
	{
		//auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		//switch (ML::ToDegree(pl->Get_Angle().y) / 90 % 4)
		//{
		//}
		return this->hitBase;
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