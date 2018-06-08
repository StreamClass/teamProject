#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace MiniMap
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�~�j�}�b�v");	//�O���[�v��
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
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����

		ML::Vec2 epos;
		float eangle;
		//�C���[�W��
		string imageName;
		string plImgName;
		string caImgName;
		//�~�j�}�b�v�̃v���C���p�ϐ�
		//���W
		ML::Vec2 plpos;
		//����
		float plAngle;
		//�~�j�}�b�v�̃J�����p�ϐ�
		//���W
		ML::Vec2 capos;
		//����
		float caAngle;
		//�~�j�}�b�v��\�����邩
		bool viewFlag;
		//�^�u���b�g���g�p���Ă��邩
		bool tab_use_now;

		//�~�j�}�b�v��\�����邩���f
		bool MiniMap_View();
		//�v���C������~�j�}�b�v���o�����w��
		void Set_MiniMap_View();
	};
}