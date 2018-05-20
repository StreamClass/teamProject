#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

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
		//�v���C���̓��̍���
		int headHeight;
		//�v���C��̌���
		ML::Vec3 angle;
		//�v���C���̈ړ���
		ML::Vec3 moveVec;
		//�R���g���[���l�[��
		string controllerName;
	public:
		//�ǉ����\�b�h
		//�v���C���̎��_��int�^�ŕԂ�
		int Get_PointView();
		//�v���C���̍��W��ML::Vec3�^�ŕԂ�
		ML::Vec3 Get_Pos();
		//�v���C���̌�����ML::Vec3�^�ŕԂ�
		ML::Vec3 Get_Angle();
		//�M�~�b�N�𓮂���
		void Approach();
	};
}