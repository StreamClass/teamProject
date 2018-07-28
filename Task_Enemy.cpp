//-------------------------------------------------------------------
//�G�l�~�[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MiniMap.h"
#include  "Task_Map.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"

namespace  Enemy
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
		this->rou = ge->OM.Create_Routine();

		this->pos = ML::Vec3(chipX * 5, 20, chipZ * 13);
		this->speed = 10.0f;
		this->hitBase = ML::Box3D(-100, 0, -100, 200, 200, 200);
		this->angle = ML::Vec3(0, ML::ToRadian(-90),0);
		this->chasing_Speed = 13.0f;
		this->final_Phase_Speed = 13.0f;
		this->timeCnt = 0;
		this->ebone = new Bone(180, "Enemy");
		this->ebone->Moving(this->pos);
		this->ebone->Bone_RotateY_All(this->angle.y + ML::ToRadian(-90));
		this->Init_Enemys_Animations();
		

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

		//�ړI�n
		ML::Vec3 targetPos = ML::Vec3(0, 0, 0);

		//�T�m����p��`��p��
		//ML::Box3D me = this->searchBase;
		//�v���C���̔����`��p�ӂ��ĐڐG����

		//�ړ�����v���C���̈ʒu���Q�Ƃ��Ďw��
		//this->toVec = pl->Get_Pos() - this->pos;
		//���W���ړ���܂ł̋�����20���̈ꂸ�ړ�
		//this->pos += this->toVec * 0.02f;

		//�Z���T�[�s��
		if (this->system.Is_Chase_Mode() == false)
		{
			//�ʏ탂�[�h�̎�
			//�Z���T�[�`�F�b�N
			this->system.SensorCheck(pl->Get_HitBase().OffsetCopy(pl->Get_Pos()), pl->Get_Pos(), this->pos, this->angle.y);
			this->timeCnt = 0;

			if (rou->Judge(this->hitBase,this->pos))
			{
				rou->Choice(rou->Get_Now());
			}

			targetPos = rou->Move(this->pos);

			if (rou->Is_Final_Phase() == false)
			{
				this->pos += targetPos * this->speed;
				this->ebone->Moving(targetPos * this->speed);
			}
			else
			{
				this->pos += targetPos * this->final_Phase_Speed;
				this->ebone->Moving(targetPos * this->final_Phase_Speed);
			}
			this->ebone->Set_Next_Motion(this->animations_Name[1]);
			
			this->angle.y = -atan2(targetPos.z, targetPos.x);
		}
		else
		{
			//�ǐՃ��[�h
			if (this->timeCnt == 0)
			{
				//�ړI�n�ݒ�
				this->toVec = this->system.NextRoute();
			}
			else if (this->timeCnt % 10 == 0)
			{
				//�Z���T�[�`�F�b�N
				this->system.SensorCheck(pl->Get_HitBase().OffsetCopy(pl->Get_Pos()), pl->Get_Pos(), this->pos, this->angle.y);
			}

			//���̖ړI�n�ƈ�苗���ȓ��Ȃ�V�����ړI�n��ݒ�
			if (ML::Vec3(this->toVec - this->pos).Length() <= this->chasing_Speed)
			{
				this->toVec = this->system.NextRoute();
			}

			targetPos = this->toVec - this->pos;
			//�ړ�
			this->pos += targetPos.Normalize() * this->chasing_Speed;
			this->ebone->Moving(targetPos.Normalize() * this->chasing_Speed);
			this->ebone->Set_Next_Motion(this->animations_Name[0]);
			//�������v���C�����ɂ���
			ML::Vec3 a = pl->Get_Pos() - this->pos;
			this->angle.y = -atan2(a.z, a.x);

			//�J�E���g�㏸
			this->timeCnt++;
		}
		

		//�{�[���A�b�v�f�[�g
		this->ebone->Bone_RotateY_All(this->angle.y + ML::ToRadian(90));
		this->ebone->Repeat_Now_Motioin();
		this->ebone->UpDate();

		if(!pl->Get_DebugOnOff())
			this->Player_HitCheck();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}
	void  Object::Render3D_L0()
	{
		//�G�l�~�[�̃��b�V����\��
		/*ML::Mat4x4 matS, matR, matRy, matRz, matT;
		matS.Scaling(ML::Vec3(2, 1.5f, 2));
		matRy.RotationY(this->angle.y);
		matRz.RotationZ(this->angle.z);
		matR = matRz * matRy;
		matT.Translation(this->pos);
		DG::EffectState().param.matWorld = matS * matR * matT;
		DG::EffectState().param.objectColor = ML::Color( 1, 0.6f, 0.2f, 0.2f);
		DG::Mesh_Draw(this->res->meshName);
		DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);*/
		this->ebone->Render();
	}

	//�v���C���Ƃ̂����蔻��
	void Object::Player_HitCheck()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (this->hitBase.OffsetCopy(this->pos).Hit(pl->Get_HitBase().OffsetCopy(pl->Get_Pos())))
		{
			ge->state = ge->over;
		}
	}
	//
	Bone* Object::Get_EnemyBonePtr()
	{
		return this->ebone;
	}
	//
	ML::Vec3 Object::Get_Pos()
	{
		return this->pos;
	}
	//-------------------------------------------------------------------------------
	//�A�j���[�V�����������\�b�h
	void Object::Init_Enemys_Animations()
	{
		//����A�j���[�V����
		this->animations_Name.push_back("Running");
		std::vector<Motion::Motion_Data> running;
		Motion::Make_Motion(&running, this->animations_Name[0]);
		this->ebone->Registrate_Motion(running, this->animations_Name[0]);
		//�����A�j���[�V����
		this->animations_Name.push_back("Walking");
		std::vector<Motion::Motion_Data> walking;
		Motion::Make_Motion(&walking, this->animations_Name[1]);
		this->ebone->Registrate_Motion(walking, this->animations_Name[1]);
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