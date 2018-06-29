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
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
		DG::Font_Erase("FontA");
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
		this->angle = ML::Vec3(0, ML::ToRadian(-90), 0);
		this->moveVec = ML::Vec3(0, 0, 0);
		this->moveVecRec = 0.0f;
		this->speed = 10.0f;
		this->clearFlag = false;
		this->tremor = 4.0f;
		this->stamina = MAX_STAMINA;
		this->recovery_Flag = false;

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
		auto in = DI::GPad_GetState("P1");
		//volume��0�̎��͎g���Ă̓_��
		//�^�u���b�g�I�t
		if (!this->tab->Is_Used_Now())
		{
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
				//���_�����W�ϊ�������
				this->moveVec = matR.TransformCoord(this->moveVec);
			}
			else
			{
				this->moveVec = ML::Vec3(0, 0, 0);
			}
			this->angle.y += in.RStick.axis.x * ML::ToRadian(2);
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
			//sin()�̒��_�����ȏ�Ȃ甽�f���Ȃ�(�����z���Ă�������~�߂�)
			if (this->speed != DASHSPEED)
			{
				float headY = this->headHeight_std + sin(ML::ToRadian(this->cnt_TG))*this->speed;
				float targetY = this->adjust_TG_std + sin(ML::ToRadian(this->cnt_TG))*this->speed + this->add_adjust;

				if (!(headY > this->heightMax || targetY > this->heightMax))
				{
					this->headHeight = headY;
					this->adjust_TG = targetY;
				}
			}
			else
			{
				this->headHeight = this->headHeight_std + sin(ML::ToRadian(this->cnt_TG*this->speed))*this->speed;
				this->adjust_TG = this->adjust_TG_std + sin(ML::ToRadian(this->cnt_TG*this->speed))*this->speed + this->add_adjust;
			}
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
			//�f�o�b�O�p
			//if (in.L1.on)
			//{
			//	this->pos.y += 8;
			//}
			//if (in.R1.on)
			//{
			//	this->pos.y -= 8;
			//}
		}
		else
		{
			this->tab->Select_Camera();
		}

		if (in.B4.down)
		{
			this->tab->Open_or_Close_Tablet();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//ML::Box2D draw(500, 0, 580, 300);
		//string text = "X=" + to_string(this->pos.x) + "Y=" + to_string(this->pos.y) + "Z=" + to_string(this->pos.z) + "\n"
		//	+ "this->angle.y=" + to_string(ML::ToDegree(this->angle.y)) + "�����_�̍���" + to_string(this->adjust_TG);
		//DG::Font_Draw("FontA", draw, text, ML::Color(1.0f, 0.0f, 0.0f, 0.0f ));
	}
	//-------------------------------------------------------------------
	void  Object::Render3D_L0()
	{
		if (this->Is_Used_Tablet() == true)
		{
			ML::Mat4x4 matT;
			matT.Translation(this->pos);
			//��]�s��̐���
			ML::Mat4x4 matR;
			matR.RotationY(this->angle.y);
			//���f���\��
			DG::EffectState().param.matWorld = matR * matT;;
			DG::Mesh_Draw(this->res->meshName);
		}
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
	}
	//-------------------------------------------------------------------
	//�}�b�v�Ƃ̐ڐG����
	//�����F�i�}�b�v�̋�`, �v���C���̋�`, �}�b�v�̃`�b�v�T�C�Y�j
	bool Object::Map_CheckHit(const ML::Box3D& pHit)
	{
		auto mp = ge->GetTask_One_G<Map::Object>("�t�B�[���h");
		//�ǂݍ��񂾋�`�̍ő�A�ŏ����_�̍��W
		struct Box3D_2Point
		{
			int fx, fy, fz;//�l�����������̓_
			int bx, by, bz;//�l���傫�����̓_
		};
		//�v���C���̔���p���_��ݒ�
		Box3D_2Point r =
		{
			pHit.x,			pHit.y,			pHit.z,
			pHit.x + pHit.w,pHit.y + pHit.h,pHit.z + pHit.d
		};
		//�}�b�v�̔���p���_��ݒ�
		Box3D_2Point m =
		{
			mp->arr[0][0].Get_HitBase().x,
			mp->arr[0][0].Get_HitBase().y,
			mp->arr[0][0].Get_HitBase().z,
			mp->arr[0][0].Get_HitBase().x + mp->arr[0][0].Get_HitBase().w * mp->maxSizeX,
			mp->arr[0][0].Get_HitBase().y + mp->arr[0][0].Get_HitBase().h,
			mp->arr[0][0].Get_HitBase().z + mp->arr[0][0].Get_HitBase().d * mp->maxSizeZ
		};

		//�L�����N�^�̋�`���}�b�v�͈͓��Ɋۂߍ���
		if (r.fx < m.fx) { r.fx = m.fx; }
		if (r.fz < m.fz) { r.fz = m.fz; }
		if (r.bx > m.bx) { r.bx = m.bx; }
		if (r.bz > m.bz) { r.bz = m.bz; }

		//�L�����N�^���}�b�v�͈͊O�ɂ����S�ɏo�Ă����画��I��
		if (r.bx <= r.fx) { return false; }
		if (r.bz <= r.fz) { return false; }
		//���[�v�͈͂����
		int sx, sz, ex, ez;
		sx = r.fx / (int)mp->arr[0][0].Get_ChipSizeX();
		sz = r.fz / (int)mp->arr[0][0].Get_ChipSizeZ();
		ex = (r.bx - 1) / (int)mp->arr[0][0].Get_ChipSizeX();
		ez = (r.bz - 1) / (int)mp->arr[0][0].Get_ChipSizeZ();
		//�ڐG����J�n
		for (int z = sz; z <= ez; ++z) {
			for (int x = sx; x <= ex; ++x) {
				if (mp->arr[z][x].Get_Type() == Type::box) {
					return true;
				}
				this->Check_Clear();
			}
		}
		//auto d = ge->GetTask_Group_GN<Task_Door::Object>("�h�A","NoName");
		//for (auto it = d->begin(); it != d->end(); it++)
		//{
		//	if ((*it)->Hit_Check(pHit))
		//	{
		//		return true;
		//	}
		//}
		return false;//�ڐG������̂����o����Ȃ�����
	}
	//-------------------------------------------------------------------
	//�߂荞�܂Ȃ�����
	//�����F�i�v���C���̈ړ��ʁj
	void Object::Player_CheckMove(ML::Vec3& est_)
	{

		//���������ix����)�ɑ΂���ړ�
		while (est_.x != 0.0f) {//�\��ړ��ʂ������Ȃ�܂ŌJ��Ԃ�
			float preX = this->pos.x;//�ړ��O�̍��W��ێ�

			//1cm�������͂���ȉ��̎c�蕪�ړ�������
			if (est_.x >= 1.0f) {
				this->pos.x += 1.0f;		est_.x -= 1.0f;
			}//+����
			else if (est_.x <= -1.0f) {
				this->pos.x -= 1.0f;		est_.x += 1.0f;
			}//-����
			else {
				this->pos.x += est_.x;		est_.x = 0.0f;
			}

			//�ڐG��������݂�
			ML::Box3D hit = this->hitBase.OffsetCopy(this->pos);
			//hit.Offset((int)this->pos.x, (int)this->pos.y, (int)this->pos.z);
			if (true == this->Map_CheckHit(hit)) {
				this->pos.x = preX;		//�ڐG���Ă����̂ŁA���ɖ߂�
				break;	//����ȏ㎎���Ă����ʂȂ̂Ń��[�v�𔲂���
			}			
		}
		//-----------------------------------------------------------------------------
		//���������iz����)�ɑ΂���ړ�
		//���������ix����)�ɑ΂���ړ�
		while (est_.z != 0.0f) {//�\��ړ��ʂ������Ȃ�܂ŌJ��Ԃ�
			float preZ = this->pos.z;//�ړ��O�̍��W��ێ�

									 //1cm�������͂���ȉ��̎c�蕪�ړ�������
			if (est_.z >= 1.0f) {
				this->pos.z += 1.0f;		est_.z -= 1.0f;
			}//+����
			else if (est_.z <= -1.0f) {
				this->pos.z -= 1.0f;		est_.z += 1.0f;
			}//-����
			else {
				this->pos.z += est_.z;		est_.z = 0.0f;
			}

			//�ڐG��������݂�
			ML::Box3D hit = this->hitBase.OffsetCopy(this->pos);
			//hit.Offset((int)this->pos.x, (int)this->pos.y, (int)this->pos.z);
			if (true == this->Map_CheckHit(hit)) {
				this->pos.z = preZ;		//�ڐG���Ă����̂ŁA���ɖ߂�
				break;	//����ȏ㎎���Ă����ʂȂ̂Ń��[�v�𔲂���
			}			
		}
	}
	//-------------------------------------------------------------------
	//�M�~�b�N�ւ̊���
	void Object::Touch()
	{
		auto b = ge->GetTask_Group_G<Task_Breaker::Object>("�u���[�J�[");
		for (auto it = b->begin(); it != b->end(); it++)
		{
			if ((*it)->Hit_Check(this->hitBase.OffsetCopy(this->pos)))
			{
				(*it)->ActivateBreaker();
				break;
			}
		}
	}
	//-------------------------------------------------------------------
	//�N���A���Ă��邩����
	void Object::Check_Clear()
	{
		auto m = ge->GetTask_Group_G<Map::Object>("�t�B�[���h");
		for (auto it = m->begin(); it != m->end(); ++it)
		{
			if ((*it)->goal.Map_Hit_Check(this->hitBase.OffsetCopy(this->pos)))
			{
				//this->clearFlag = true;
				ge->state = ge->clear;
			}
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