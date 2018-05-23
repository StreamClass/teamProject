//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h" 

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
		this->angle = ML::Vec3(0, 0, 0);
		this->moveVec = ML::Vec3(0, 0, 0);

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
		auto in = DI::GPad_GetState("P1");
		//volume��0�̎��͎g���Ă̓_��
		if (in.LStick.volume > 0) //�A�i���O�X�e�B�b�N��|���Ă��鋭��0.0~1.0f
		{
			ML::Mat4x4 matR;
			matR.RotationY(this->angle.y);
			this->moveVec.x = -10 * in.LStick.axis.y;
			this->moveVec.z = -10 * in.LStick.axis.x;
			//���_�����W�ϊ�������
			this->moveVec = matR.TransformCoord(this->moveVec);
		}
		else
		{
			this->moveVec = ML::Vec3(0, 0, 0);
		}
		this->angle.y += in.RStick.axis.x * ML::ToRadian(5);

		this->pos += this->moveVec;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 300, 100);
		string text = "X=" + to_string(this->pos.x) + "Y=" + to_string(this->pos.y) + "Z=" + to_string(this->pos.z) + "\n"
			+ "this->angle.y=" + to_string(ML::ToDegree(this->angle.y));
		DG::Font_Draw("FontA", draw, text, ML::Color(1.0f, 0.0f, 0.0f, 0.0f ));
	}
	//-------------------------------------------------------------------
	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT;
		matT.Translation(this->pos);
		//��]�s��̐���
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);

		//���f���\��
		//ML::Mat4x4 matW = matR * matT;
		//DG::EffectState().param.matWorld = matW;
		//DG::Mesh_Draw(this->res->meshName);
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
	ML::Vec3 Object::Get_Angle()
	{
		return this->angle;
	}
	//-------------------------------------------------------------------
	//�M�~�b�N�𓮂����֐�
	void Object::Approach()
	{

	}
	//-------------------------------------------------------------------
	//
	void Object::Ini_Pos(const ML::Vec3& pos)
	{
		this->pos = pos;
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