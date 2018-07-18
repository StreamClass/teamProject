#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���G�C��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace Aiming
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G�C��");	//�O���[�v��
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
		//�G�C���p�̉摜��
		string imageName[3];
		//�ʏ펞�̑�������p�摜��
		string normalModeImg[2];
		//�Ď��J�������̑�������摜��
		string tabletModeImg[2];
		//�X�^�~�i�c�ʕ`��p�摜��
		string staminaImgName[2];
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
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�����蔻��
		ML::Box3D hitBase;
		//���ԃJ�E���^
		int timeCnt;
		//�R���g���[���[���͎���
		float moveCnt;
		//�G�C����2�������W
		ML::Vec2 aimPosC;
		ML::Vec2 aimPosT;
		ML::Vec2 aimPosB; 
		ML::Vec2 aimPosL; 
		ML::Vec2 aimPosR;
		//�����蔻��̂R�������W
		ML::Vec3 pos;
		//�G�C���̓�������
		float aimMoveSpeed;
		//�G�C���̓�����
		float aimMovetremor;
	public:
		//�e��Ԃł̑������
		void TabletModeRrender();
		void NormalModeRrender();
		//�G�C���̃A�j���[�V����
		void AimingRender();
		//�X�^�~�i�̕`��
		void StaminaRender();
		//�����蔻���n��
		ML::Box3D Get_HitBase();
		//�����蔻��̍��W�ݒ�
		void Set_Pos(ML::Vec3& pos_);
	};
}