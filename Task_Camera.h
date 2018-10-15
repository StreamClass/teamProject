#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�J�����}��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace Camera
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�J�����}��");	//�O���[�v��
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
		string tablet_Img_Name;
		string display_Noise_Img_Name;
		string hearts_Sound_Name;
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

		//�J�����̍��W
		ML::Vec3 pos;
		//�J�����}���̌���
		ML::Vec3 angle;
		//�����_
		ML::Vec3 dist;
		//�m�C�Y�̓����p�̃J�E���g
		int noise_Cnt;

		//test flag
		bool test_flag;

	public:
		//�ǉ����\�b�h
		//
		void Noise_Reset();
		//�S���̑傫���𒲐�
		void Change_Volume_Hearts_Sound();
	};
}