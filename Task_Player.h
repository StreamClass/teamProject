#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Tablet.h"
#include "Bone.h"

#define NORMALSPEED 10
#define TIRED_SPEED 3
#define DASHSPEED 20
#define MAX_STAMINA 240

namespace Player
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�v���C��");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		//�I�t�X�N���[����
		string meshName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������

		//�ǉ��ϐ�
		//�v���C���̍��W
		ML::Vec3 pos;
		//�ړ����x
		float speed;
		//�G�l�~�[�Ƃ̔����`
		ML::Box3D hitBase;
		//�}�b�v�Ƃ̔����`
		ML::Box3D moveBase;
		//�v���C��̌���
		ML::Vec3 angle;
		//�v���C���̎��_(���̍���)
		int headHeight;
		//�v���C��̎��_�
		int headHeight_std;
		//���_�ړ����x
		int turnSpeed;
		//�����_�̍���
		int adjust_TG;
		//�����_�̍����
		int adjust_TG_std;
		//�����_�̑�����
		float add_adjust;
		//���_�ړ����x
		float adjust_Speed;
		//�����_�����̉���
		int adjust_Min;
		//�����_�����̏��
		int adjust_Max;
		//���_�h��J�E���^
		int cnt_TG;
		//���_�h��̑��x
		int cnt_SP;
		//��ʗh�ꉺ��
		float trm_Min;
		//��ʗh����
		float trm_Max_std;
		//�_�b�V���p���
		float trm_Max_dash;
		//��J���p���
		float trm_Max_trd;
		//���_�̗h�ꕝ
		float tremor;		
		//��ʗh����
		float heightMax;
		//�v���C���̈ړ���
		ML::Vec3 moveVec;
		//�v���C���̈ړ��ʕۑ�
		float moveVecRec;
		//�R���g���[���l�[��
		string controllerName;
		//�N���A����
		bool		clearFlag;
		//�^�u���b�g
		Tablet* tab;

		//�{�[���A�j���[�V�����p
		Bone* plBone;

		//�X�^�~�i
		float stamina;
		//�X�^�~�i�񕜃t���O
		bool recovery_Flag;

		//�f�o�b�O�p
		bool debugMode;
		int breakerOnCnt;
		bool a;
		bool b;
		bool c;
	public:
		//�ǉ����\�b�h
		//�v���C���̎��_��int�^�ŕԂ�
		int Get_PointView();
		//�����_�̍���(adJust_TG)��Ԃ�
		int Get_Adjust();		
		//�v���C���̍��W��ML::Vec3�^�ŕԂ�
		ML::Vec3 Get_Pos();
		//�v���C���̌�����ML::Vec3�^�ŕԂ�
		ML::Vec3 Get_Angle();
		//�����蔻��͈͂����炤�֐�
		ML::Box3D Get_HitBase();
		//�ړ����x��float�ŕԂ�
		float Get_MoveSpeed();
		//�v���C���̏����l�w��
		void Ini_Pos(const ML::Vec3& pos);
		//�߂荞�܂Ȃ�����
		//�����F�i�v���C���̈ړ��ʁj
		void Player_CheckMove(ML::Vec3& est_);
		//
		void Turn_Angle();
		//���ʂ��ǂɋ߂Â������Ȃ�����
		bool Check_Front();
		//�M�~�b�N�ւ̊���
		void Touch();
		//�N���A���Ă��邩����
		void Check_Clear();
		//�N���A����n��
		bool Get_ClearFlag();
		//�^�u���b�g���g�p�����m�F����֐�
		bool Is_Used_Tablet();
		//���Ă��邩��n��
		bool Is_Tired();
		//�X�^�~�i�̎c�ʂ�n��
		float Get_Stamina();
		//�f�o�b�O�p���G���[�h�̃I���I�t
		bool Get_DebugOnOff();
	};
}