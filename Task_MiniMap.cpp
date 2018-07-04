//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MiniMap.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"
#include  "MapBox.h"

namespace  MiniMap
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
		//�C���[�W���̎w��
		this->imageName = "MapImg";
		this->plImgName = "PlayerImg";
		this->caImgName = "CameraImg";
		//�e�摜�̓ǂݍ���
		DG::Image_Create(this->imageName, "./data/image/�}�b�v00.png");
		DG::Image_Create(this->plImgName, "./data/image/Player_MiniMap.png");
		DG::Image_Create(this->caImgName, "./data/image/Camera_MiniMap.png");
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
		this->magni = chipX / 5;

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
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->plImgName);
		DG::Image_Erase(this->caImgName);

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�v���C���̕ϐ������g����悤�ɌĂяo��
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		//�v���C���{�̂���~�j�}�b�v��̏����Q��
		this->plpos = ML::Vec2((int)pl->Get_Pos().x / this->magni, 500 - (int)pl->Get_Pos().z / this->magni);
		this->plAngle = (float)pl->Get_Angle().y + ML::ToRadian(90);
		//�^�u���b�g�̎g�p�󋵂��v���C������󂯎��
		this->tab_use_now = pl->Is_Used_Tablet();
		//�^�u���b�g���g���Ă�����
		if (this->tab_use_now == true)
		{
			//�J��������~�j�}�b�v��̏����Q��
			this->capos = ML::Vec2((int)ge->camera[0]->pos.x / this->magni, 500 - (int)ge->camera[0]->pos.z / this->magni);
			ML::Vec2 a = ML::Vec2(ge->camera[0]->target.x - ge->camera[0]->pos.x, ge->camera[0]->target.z - ge->camera[0]->pos.z);
			this->caAngle = -atan2(a.y,a.x) + ML::ToRadian(90);
		}

		//�f�o�b�N�p
		//�G�l�~�[����~�j�}�b�v��ł̏����Q��
		auto e = ge->GetTask_One_G<Enemy::Object>("�G�l�~�[");
		this->epos = ML::Vec2((int)e->pos.x / this->magni, 500 - (int)e->pos.z / this->magni);
		this->eangle = e->angle.y + ML::ToRadian(90);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�~�j�}�b�v��\�����Ă�����
		if (this->MiniMap_View() == true)
		{
			//�~�j�}�b�v
			ML::Box2D draw(60,60, 500, 500);
			ML::Box2D src(0, 0, 500, 500);
			//draw.Offset(this->plpos);
			//src.Offset(this->plpos);
			//src.w += this->plpos.x;
			//src.h += this->plpos.y;
			//if (src.w > 500)
			//{
			//	src.w = 500;
			//}
			//if (src.h > 500)
			//{
			//	src.h = 500;
			//}
			DG::Image_Draw(this->imageName, draw, src);
			//�v���C���ʒu
			draw = ML::Box2D(-5 + 60, -7 + 60, 9, 13);
			src = ML::Box2D(0, 0, 50, 50);
			draw.Offset(this->plpos);
			DG::Image_Rotation(this->plImgName, this->plAngle, ML::Vec2(5, 10));
			DG::Image_Draw(this->plImgName, draw, src);
			//�^�u���b�g���g�p���Ă�����
			if (this->tab_use_now == true)
			{
				draw = ML::Box2D(-5 + 60 , -7 + 60, 9, 13);
				draw.Offset(this->capos);
				DG::Image_Rotation(this->caImgName, this->caAngle, ML::Vec2(5, 10));
				DG::Image_Draw(this->caImgName, draw, src);
			}

			//�f�o�b�O�p
			//�G�l�~�[�`��
			draw = ML::Box2D(-5 + 60, -7 + 60, 9, 13);
			draw.Offset(this->epos);
			DG::Image_Rotation(this->plImgName, this->eangle, ML::Vec2(5, 10));
			DG::Image_Draw(this->plImgName, draw, src,ML::Color(1,1,1,0));
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