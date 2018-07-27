//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MiniMap.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"

namespace  MiniMap
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�C���[�W���̎w��
		this->imageName = "MapImg";
		this->plImgName = "PlayerImg";
		this->caImgName = "CameraImg";
		this->anImgName = "Chara_Angle";
		//�e�摜�̓ǂݍ���
		DG::Image_Create(this->imageName, "./data/image/�}�b�v00.png");
		DG::Image_Create(this->plImgName, "./data/image/Player_MiniMap.png");
		DG::Image_Create(this->caImgName, "./data/image/Camera_MiniMap.png");
		DG::Image_Create(this->anImgName, "./data/image/Chara_Angle.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->plImgName);
		DG::Image_Erase(this->caImgName);
		DG::Image_Erase(this->anImgName);
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
		//�v���C���p�ϐ��̏�����
		this->plpos = ML::Vec2(0, 0);
		this->plAngle = 0;
		//�J�����p�ϐ��̏�����
		this->capos = ML::Vec2(0, 0);
		this->caAngle = 0;
		//�ŏ����猩��悤�ɐݒ�
		this->viewFlag = true;
		//�^�u���b�g�͎g���Ă��Ȃ�
		this->tab_use_now = false;
		//���W�Q�Ɨp�{��
		this->magni = chipX / NORMALMAGNI;
		//
		this->mapSize = NORMALMAPSIZE;
		//�f�o�b�N���[�h�̏�����(����)
		this->debugMode = false;
		//�G�l�~�[�̍��W
		this->epos = ML::Vec2(0, 0);
		//�G�l�~�[�̌���
		this->eangle = 0;
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
		//�v���C���̕ϐ������g����悤�ɌĂяo��
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		//�^�u���b�g�̎g�p�󋵂��v���C������󂯎��
		this->tab_use_now = pl->Is_Used_Tablet();
		//�f�o�b�O���[�h���ۂ����v���C���[����󂯎��
		this->debugMode = pl->Get_DebugOnOff();
		//
		this->magni = 150.0f / NORMALMAGNI;
		this->mapSize = NORMALMAPSIZE;
		//
		if (this->tab_use_now)
		{
			this->magni = float(chipX) / TABLETMAGNI;
			this->mapSize = TABLETMAPSIZE;
		}
		//�v���C���{�̂���~�j�}�b�v��̏����Q��
		this->plpos = ML::Vec2((int)pl->Get_Pos().x / this->magni, this->mapSize - (int)pl->Get_Pos().z / this->magni);
		this->plAngle = (float)pl->Get_Angle().y + ML::ToRadian(90);
		//�J��������~�j�}�b�v��̏����Q��
		this->capos = ML::Vec2((int)ge->camera[0]->pos.x / this->magni, this->mapSize - (int)ge->camera[0]->pos.z / this->magni);
		ML::Vec2 a = ML::Vec2(ge->camera[0]->target.x - ge->camera[0]->pos.x, ge->camera[0]->target.z - ge->camera[0]->pos.z);
		this->caAngle = -atan2(a.y,a.x) + ML::ToRadian(90);
		//�G�l�~�[����~�j�}�b�v��ł̏����Q��
		auto e = ge->GetTask_One_G<Enemy::Object>("�G�l�~�[");
		this->epos = ML::Vec2((int)e->pos.x / this->magni, this->mapSize - (int)e->pos.z / this->magni);
		this->eangle = e->angle.y + ML::ToRadian(90);

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�~�j�}�b�v��\�����Ă��Ȃ�������
		if (!this->MiniMap_View())
		{
			return;
		}
		//
		ML::Vec2 cen(16, 24);
		//
		ML::Box2D pdraw(-8 + 60, -8 + 60, 16, 16);
		ML::Box2D psrc(0, 0, 63, 63);
		//
		ML::Box2D cdraw = pdraw;
		ML::Box2D csrc(0, 0, 63, 63);
		//
		ML::Box2D edraw = pdraw;;
		ML::Box2D esrc(0, 0, 63, 63);
		//
		ML::Box2D aDraw(-16 + 60, -8 + 60, 32, 16);
		ML::Box2D aSrc(0, 0, 63, 45);
		//
		float alpha = 1;
		if (this->tab_use_now)
		{
			cen = ML::Vec2(20, 30);
			pdraw = ML::Box2D(-10 + 60, -10 + 60, 20, 20);
			cdraw = pdraw;
			edraw = pdraw;
			aDraw = ML::Box2D(-20 + 60, -10 + 60, 40, 20);
			alpha = 0.4f;
		}
		ML::Box2D draw(60, 60, this->mapSize, this->mapSize);
		ML::Box2D src(0, 0, 500, 500);
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(alpha, 1, 1, 1));
		//
		aDraw.Offset(this->capos - ML::Vec2(0, float(aDraw.h)));
		DG::Image_Rotation(this->res->anImgName, this->caAngle, cen);
		DG::Image_Draw(this->res->anImgName, aDraw, aSrc, ML::Color(0.3f, 1, 1, 1));
		//
		pdraw.Offset(this->plpos);
		DG::Image_Draw(this->res->plImgName, pdraw, psrc, ML::Color(alpha, 1, 1, 1));
		if (this->tab_use_now)
		{
			//
			cdraw.Offset(this->capos);
			DG::Image_Draw(this->res->caImgName, cdraw, csrc, ML::Color(alpha, 1, 1, 1));
		}
		//
		edraw.Offset(this->epos);
		if (this->debugMode)
		{
			DG::Image_Draw(this->res->plImgName, edraw, esrc, ML::Color(alpha, 1, 1, 0));
		}
	}
	//-------------------------------------------------------------------
	//�u3�c�`��v�P�t���[�����ɍs������
	void  Object::Render3D_L0()
	{
	}
	//�~�j�}�b�v��\�����邩���f
	bool Object::MiniMap_View()
	{
		return this->viewFlag;
	}
	//�v���C������~�j�}�b�v��\�����邩�w��
	void Object::Set_MiniMap_View()
	{
		this->viewFlag = !this->viewFlag;
	}
	//�~�j�}�b�v�̕`��
	//�摜�`��͈͂̐ݒ�𖢎���
	void Object::MiniMap_Render()
	{
		ML::Box2D draw(60, 60, 500, 500);
		ML::Box2D src(0, 0, 500, 500);
		//�`��ʒu�̕ύX

		//���\�[�X�̎Q�ƈʒu��ύX
		DG::Image_Draw(this->res->imageName, draw, src);
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