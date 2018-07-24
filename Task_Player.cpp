//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h" 
#include  "Task_Aim.h"
#include  "Task_Map.h"
#include  "MapBox.h"
#include  "Task_Breaker.h"
#include  "Task_Door.h"
#include  "Task_MiniMap.h"
#include  "easing.h"



namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//���b�V���̓ǂݍ���
		this->meshName = "playerImg";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/char_Stand.sob");
		//�t�H���g�̓ǂݍ���
		DG::Font_Create("FontA", "�l�r �S�V�b�N", 10, 20);
		DM::Sound_CreateSE("standsound", "./data/sound/Stand00.wav");
		DM::Sound_CreateSE("runsound", "./data/sound/run00.wav");
		DM::Sound_CreateSE("tiredsound", "./data/sound/tired00.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
		DG::Font_Erase("FontA");
		DM::Sound_Erase("standsound");
		DM::Sound_Erase("runsound");
		DM::Sound_Erase("tiredsound");
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������)
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->controllerName = "P1";
		//�v���C���̏�����
		this->pos = ML::Vec3(0, 0, 0);
		this->headHeight = 175;
		this->headHeight_std = 175;
		this->turnSpeed = 2;
		this->adjust_TG = 175;
		this->adjust_TG_std = 175;
		this->cnt_TG = 0;
		this->add_adjust = 0.0f;
		this->cnt_SP = 1;
		this->trm_Max_std = 15.0f;
		this->trm_Min = 0.0f;
		this->trm_Max_dash = 25.0f;
		this->trm_Max_trd = 25.0f;
		this->adjust_Speed = 10.0f;
		this->adjust_Min = -400;
		this->adjust_Max = +400;
		this->heightMax = 180;
		this->hitBase = ML::Box3D(-50, 0, -50, 100, 200, 100);
		this->moveBase = ML::Box3D(-70, 0, -70, 140, 200, 140);
		this->angle = ML::Vec3(0, ML::ToRadian(-90), 0);
		this->moveVec = ML::Vec3(0, 0, 0);
		this->moveVecRec = 0.0f;
		this->speed = 10.0f;
		this->clearFlag = false;
		this->tremor = 1.0f;
		this->stamina = MAX_STAMINA;
		this->recovery_Flag = false;
		this->touch_Breaker = false;
		this->debugMode = false;

		this->a = false;
		this->b = false;
		this->c = true;

		this->plBone = new Bone(170,"Player");		


		this->breakerOnCnt = 0;

		this->tab = ge->OM.Create_Tablet();
		//���_�C�[�W���O
		easing::Set("camStdUp", easing::EXPOINOUT, this->trm_Min, this->trm_Max_std, 120);
		easing::Set("camStdDown", easing::QUADOUT, this->trm_Max_std, this->trm_Min, 120);
		//�_�b�V���p
		easing::Set("camDashUp", easing::EXPOINOUT, this->trm_Min, this->trm_Max_dash, 15);
		easing::Set("camDashDown", easing::QUADOUT, this->trm_Max_dash, this->trm_Min, 10);
		//��J���p
		easing::Set("camTrdUp", easing::EXPOINOUT, this->trm_Min, this->trm_Max_trd, 180);
		easing::Set("camTrdDown", easing::QUADOUT, this->trm_Max_trd, this->trm_Min, 180);

		//���^�X�N�̐���
		auto aim = Aiming::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�G�C��");

		//�{�[���J��
		delete this->plBone;

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
		//volume��0�̎��͎g���Ă̓_��
		//�^�u���b�g�I�t
		if (!this->tab->Is_Used_Now())
		{

			if (in.S0.down)
			{
				this->debugMode = !this->debugMode;
			}
			if (in.LStick.volume > 0) //�A�i���O�X�e�B�b�N��|���Ă��鋭��0.0~1.0f
			{
				//����r���̓X�^�~�i����
				if (in.R1.on)
				{
					this->stamina--;
				}
				ML::Mat4x4 matR;
				matR.RotationY(this->angle.y);
				this->moveVec.x = -this->speed * in.LStick.axis.y;
				this->moveVec.z = -this->speed * in.LStick.axis.x;
				//�x�N�g�������W�ϊ�������
				this->moveVec = matR.TransformCoord(this->moveVec);
				
				//���郂�[�V����
				this->plBone->Set_Next_Motion("Walking");
				this->plBone->Repeat_Now_Motioin();
			}
			else
			{
				this->moveVec = ML::Vec3(0, 0, 0);
			}

			//���_�̉�]
			this->angle.y += in.RStick.axis.x * ML::ToRadian(this->turnSpeed);

			//�{�[���S�̂�Y����]
			this->plBone->Bone_RotateY_All(this->angle.y + ML::ToRadian(90));
			
			//���x�w��
			if (in.R1.on)
			{
				if (this->recovery_Flag == false)
				{
					//cnt++;
					//if (cnt >= 10) {
					this->speed = min(DASHSPEED, this->speed + 0.5f);
					//}
					//this->speed = DASHSPEED;
				}
				else
				{
					this->speed = TIRED_SPEED;
				}
			}
			else if (in.R1.off)
			{
				if (this->recovery_Flag == false)
				{
					//					this->speed = NORMALSPEED;
					this->speed = max(NORMALSPEED, this->speed - 0.5f);
				}
				else
				{
					this->speed = TIRED_SPEED;
				}
				//�X�^�~�i��
				this->stamina += 0.3f;
			}
			//			ge->Dbg_FileOut("speed = %0.2f", this->speed);
			//�X�^�~�i�͈�
			if (this->stamina < 0)
			{
				this->stamina = 0;
			}
			else if (this->stamina > MAX_STAMINA)
			{
				this->stamina = MAX_STAMINA;
			}

			if (this->debugMode == true)
			{
				this->stamina = MAX_STAMINA;
			}
			//���J�o���[���[�h�ɐ؂�ւ�
			if (this->recovery_Flag == false && this->stamina <= 0)
			{
				this->recovery_Flag = true;
			}
			//�ʏ탂�[�h�ɐ؂�ւ�
			if (this->recovery_Flag == true && this->stamina >= MAX_STAMINA / 3)
			{
				this->recovery_Flag = false;
			}
			//�����_�̏㉺�ړ�
			if (in.RStick.U.on && this->adjust_TG < this->adjust_Max)
			{
				this->add_adjust += this->adjust_Speed;
			}
			else if (in.RStick.D.on && this->adjust_TG > this->adjust_Min)
			{
				this->add_adjust -= this->adjust_Speed;
			}
			//��ʗh��
			//�J�E���^�X�^�[�g
			this->cnt_TG++;
			//���_�h�ꑬ�x
			if (in.R1.off && !this->recovery_Flag)
			{
				if (!this->a && this->b || this->c)
				{
					if (this->b)
					{
						DM::Sound_Stop("runsound");
					}
					if (this->c)
					{
						DM::Sound_Stop("tiredsound");
					}

					DM::Sound_Play("standsound", true);
					this->a = true;
					this->b = false;
					this->c = false;
				}
				this->cnt_SP = 2;
				this->tremor = 0.5f;
			}
			//�_�b�V����
			else if (in.R1.on && in.LStick.volume > 0)
			{
				if (!this->b && this->a || this->c)
				{
					if (this->a)
					{
						DM::Sound_Stop("standsound");
					}
					if (this->c)
					{
						DM::Sound_Stop("tiredsound");
					}
					DM::Sound_Play("runsound", true);
					this->b = true;
					this->a = false;
					this->c = false;
				}
				this->cnt_SP = 14;
				this->tremor = 1.0f;
			}
			//��J��
			if(this->recovery_Flag)
			{
				if (!this->c && this->a || this->b)
				{
					if (this->a)
					{
						DM::Sound_Stop("standsound");
					}
					if (this->b)
					{
						DM::Sound_Stop("runsound");
					}
					DM::Sound_Play("tiredsound", true);
					this->c = true;
					this->a = false;
					this->b = false;
				}
				this->cnt_SP = 8;
				this->tremor = 6.0f;
			}
			//���̊�l+sin(�J�E���^*�h�ꑬ�x�̓X�s�[�h�ŕω�)*(�X�s�[�h*�h�ꕝ)
			float headY = this->headHeight_std + sin(ML::ToRadian(this->cnt_TG*this->cnt_SP))*(this->speed*this->tremor);
			float targetY = this->adjust_TG_std + sin(ML::ToRadian(this->cnt_TG*this->cnt_SP))*(this->speed*this->tremor) + this->add_adjust;
			this->headHeight = headY;
			this->adjust_TG = targetY;

			this->moveVecRec = this->moveVec.Length();
			this->Player_CheckMove(this->moveVec);

			if (in.B3.down)
			{
				this->Touch();
			}
			if (in.B2.down)
			{
				auto mm = ge->GetTask_One_G<MiniMap::Object>("�~�j�}�b�v");
				mm->Set_MiniMap_View();
			}
		}
		else
		{
			this->tab->Select_Camera();
		}
		if (in.B4.down)
		{
			this->tab->Open_or_Close_Tablet();
		}

		//�^�u���b�g�g�p�Ƃ͕ʂɃ{�[���A�j���[�V�����A�b�v�f�[�g�͐i��
		this->plBone->UpDate();

		//�f�o�b�O�p
		if (in.S1.down)
		{
			this->stamina = 0;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Vec3 bonepos = this->plBone->Get_Center();
		if(this->debugMode)
		{
			ML::Box2D draw(500, 0, 580, 300);
			string text = "X=" + to_string(this->pos.x) + "Y=" + to_string(this->pos.y) + "Z=" + to_string(this->pos.z) + "\n"
				+ "this->angle.y=" + to_string(ML::ToDegree(this->angle.y)) + "�����_�̍���" + to_string(this->adjust_TG) + "\n" +
				to_string(this->breakerOnCnt)+ "\n";
			if (this->debugMode)
			{
				text += "  Debug";
			}
			if (this->Check_Front())
			{
				text += "	true";
			}
			DG::Font_Draw("FontA", draw, text, ML::Color(1.0f, 0.0f, 0.0f, 0.0f ));
		}
	}
	//-------------------------------------------------------------------
	void  Object::Render3D_L0()
	{
		//if (this->Is_Used_Tablet() == true)
		//{
		//	ML::Mat4x4 matT;
		//	matT.Translation(this->pos);
		//	//��]�s��̐���
		//	ML::Mat4x4 matR;
		//	matR.RotationY(this->angle.y);
		//	//���f���\��
		//	DG::EffectState().param.matWorld = matR * matT;;
		//	DG::Mesh_Draw(this->res->meshName);
		//}
		this->plBone->Render();
	}
	//-------------------------------------------------------------------
	//�v���C���̍��W��ML::Vec3�^�ŕԂ�
	ML::Vec3 Object::Get_Pos()
	{
		return this->pos;
	}
	//-------------------------------------------------------------------
	//�v���C���̎��_�̍�����int�^�ŕԂ�
	int Object::Get_PointView()
	{
		return this->headHeight;
	}
	//-------------------------------------------------------------------
	//�����_�̍���(adJust_TG)��Ԃ�7
	int Object::Get_Adjust()
	{
		return this->adjust_TG;
	}
	//-------------------------------------------------------------------
	ML::Vec3 Object::Get_Angle()
	{
		return this->angle;
	}
	//-------------------------------------------------------------------
	//�v���C���̏����l�w��
	void Object::Ini_Pos(const ML::Vec3& pos)
	{
		this->pos = pos;
		this->plBone->Moving(pos+ML::Vec3(0,20,0));
	}
	//-------------------------------------------------------------------
	//�߂荞�܂Ȃ�����
	//�����F�i�v���C���̈ړ��ʁj
	void Object::Player_CheckMove(ML::Vec3& est_)
	{
		auto mp = ge->GetTask_One_G<Map::Object>("�t�B�[���h");
		//���������ix����)�ɑ΂���ړ�
		while (est_.x != 0.0f) {//�\��ړ��ʂ������Ȃ�܂ŌJ��Ԃ�
			float preX = this->pos.x;//�ړ��O�̍��W��ێ�
			float bone_Pre_X = this->plBone->Get_Center().x;
									 
			//+����
			if (est_.x >= 1.0f) {
				this->pos.x += 1.0f;
				this->plBone->Moving(ML::Vec3(1.0f, 0, 0));
				est_.x -= 1.0f;
			}//-����
			else if (est_.x <= -1.0f) {
				this->pos.x -= 1.0f;	
				this->plBone->Moving(ML::Vec3(-1.0f, 0, 0));
				est_.x += 1.0f;
			}//�����_�ȉ��̈ړ�
			else {
				this->pos.x += est_.x;
				this->plBone->Moving(ML::Vec3(est_.x, 0, 0));
				est_.x = 0.0f;
			}

			//�ڐG��������݂�
			ML::Box3D hit = this->moveBase.OffsetCopy(this->pos);
			if (true == mp->Map_CheckHit(hit)) {
				//�ڐG���Ă����̂ŁA���ɖ߂�
				this->pos.x = preX;		
				bone_Pre_X -= this->plBone->Get_Center().x;
				this->plBone->Moving(ML::Vec3(bone_Pre_X, 0, 0));

				break;	//����ȏ㎎���Ă����ʂȂ̂Ń��[�v�𔲂���
			}			
		}
		//-----------------------------------------------------------------------------
		//���������iz����)�ɑ΂���ړ�
		//���������ix����)�ɑ΂���ړ�
		while (est_.z != 0.0f) {//�\��ړ��ʂ������Ȃ�܂ŌJ��Ԃ�
			float preZ = this->pos.z;//�ړ��O�̍��W��ێ�
			float bone_Pre_Z = this->plBone->Get_Center().z;
			//+����
			if (est_.z >= 1.0f) {
				this->pos.z += 1.0f;
				this->plBone->Moving(ML::Vec3(0, 0, 1.0f));
				est_.z -= 1.0f;
			}//-����
			else if (est_.z <= -1.0f) {
				this->pos.z -= 1.0f;	
				this->plBone->Moving(ML::Vec3(0, 0, -1.0f));
				est_.z += 1.0f;
			}//�����_�ȉ��̈ړ�
			else {
				this->pos.z += est_.z;	
				this->plBone->Moving(ML::Vec3(0, 0, est_.z));
				est_.z = 0.0f;
			}

			//�ڐG��������݂�
			ML::Box3D hit = this->moveBase.OffsetCopy(this->pos);
			if (true == mp->Map_CheckHit(hit)) {
				//�ڐG���Ă����̂ŁA���ɖ߂�
				this->pos.z = preZ;
				bone_Pre_Z -= this->plBone->Get_Center().z;
				this->plBone->Moving(ML::Vec3(0, 0, bone_Pre_Z));

				break;	//����ȏ㎎���Ă����ʂȂ̂Ń��[�v�𔲂���
			}			
		}
	}
	//-------------------------------------------------------------------
	void Object::Turn_Angle()
	{
		auto in = DI::GPad_GetState("P1");
		ML::Vec3 toAngle = ge->camera[0]->target - ge->camera[0]->pos;
		toAngle=toAngle.Normalize();
		//���_�̉�]
		ML::Vec3 preAngle = this->angle;
		if (in.RStick.axis.x > 0)
		{

		}
		this->angle.y += in.RStick.axis.x * ML::ToRadian(this->turnSpeed);
		if(this->Check_Front())
		{
			this->angle = preAngle;
		}
	}
	//-------------------------------------------------------------------
	bool Object::Check_Front()
	{
		auto mp = ge->GetTask_One_G<Map::Object>("�t�B�[���h");

		ML::Vec3 toAngle = ML::Vec3(ge->camera[0]->target.x - ge->camera[0]->pos.x, 0, ge->camera[0]->target.z - ge->camera[0]->pos.z);
		toAngle = toAngle.Normalize();
		ML::Box3D front(0, 0, 0, 1, 1, 1);
		return mp->Map_CheckHit(front.OffsetCopy((this->pos + ML::Vec3(0, this->headHeight, 0)) + (toAngle * 150)));
	}
	//-------------------------------------------------------------------
	//�M�~�b�N�ւ̊���
	void Object::Touch()
	{
		auto b = ge->GetTask_Group_G<Task_Breaker::Object>("�u���[�J�[");
		auto aim = ge->GetTask_One_G<Aiming::Object>("�G�C��");
		this->touch_Breaker = false;
		for (auto it = b->begin(); it != b->end(); it++)
		{
			ML::Vec3 move = (ge->camera[0]->target - ge->camera[0]->pos);
			move = move.Normalize();
			for (int i = 0; i < 10; ++i)
			{
				ML::Vec3 setPos = (this->pos + ML::Vec3(0,this->headHeight,0)) + (move * i * 15);
				aim->Set_Pos(setPos);
				if ((*it)->Hit_Check(aim->Get_HitBase().OffsetCopy((this->pos + ML::Vec3(0, this->headHeight, 0)) + (move * i * 15))))
				{
					(*it)->ActivateBreaker();
					//�{�^�����샂�[�V�������s
					this->plBone->Set_Next_Motion("InterAction");
					this->breakerOnCnt++;
					//this->touch_Breaker = true;
					break;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//
	bool Object::Get_Touch_Breaker()
	{
		return this->touch_Breaker;
	}
	//-------------------------------------------------------------------
	//�N���A���Ă��邩����
	void Object::Check_Clear()
	{
		auto mp = ge->GetTask_One_G<Map::Object>("�t�B�[���h");
		if (mp->Goal_CheckHit(this->hitBase.OffsetCopy(this->pos)) == true)
		{
			ge->state = ge->clear;
		}
	}
	//-------------------------------------------------------------------
	//�N���A����n��
	bool Object::Get_ClearFlag()
	{
		return this->clearFlag;
	}

	//------------------------------------------------------------------------
	//�^�u���b�g�g�p����Ԃ�
	bool Object::Is_Used_Tablet()
	{
		return this->tab->Is_Used_Now();
	}
	//-----------------------------------------------------------------------
	//�����蔻��͈͂����炤�֐�
	ML::Box3D Object::Get_HitBase()
	{
		return this->hitBase;
	}
	//-----------------------------------------------------------------------
	//�ړ����x��float�ŕԂ�
	float Object::Get_MoveSpeed()
	{
		return this->moveVecRec;
	}
	//
	float Object::Get_Stamina()
	{
		return this->stamina;
	}
	//
	bool Object::Get_DebugOnOff()
	{
		return debugMode;
	}
	//
	bool Object::Is_Tired()
	{
		return this->recovery_Flag;
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