#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�u���[�J�[
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Breaker.h"

namespace Task_Breaker
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�u���[�J�[");	//�O���[�v��
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
		string meshName;
		string buttonMeshName;
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
		static  Object::SP  Create(bool flagGameEnginePushBack_, Breaker*, int);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(Breaker*, int);
		bool  B_Finalize();
		bool  Initialize(Breaker*, int);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������
	
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�ϐ�
		//�u���[�J�[��H
		Breaker* circuit;
		//�u���[�J�[�̌���(2 or 3)
		int angle;
		//�{�^�����W
		ML::Vec3 pos;

	public:
		//���\�b�h
		//�v���C���Ƃ̂����蔻��
		bool Hit_Check(const ML::Box3D& hit);
		//�u���[�J�[�N��
		void ActivateBreaker();
		//�����̐ݒ�
		float RotationY_Angle(int& angle);
		//�{�^���̈ړ�
		ML::Vec3 Move_Button();
		//
		bool Get_Now_State();
	};
}