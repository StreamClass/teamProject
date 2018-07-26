#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[���I�[�o�[���
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Bone.h"

namespace Over
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�Q�[���I�[�o�[");	//�O���[�v��
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
		//�w�i�̃C���[�W�� 
		string bgMeshName;
		//�G�t�F�N�g�̃C���[�W��
		string eImgName;
		//�Q�[���I�[�o�[���S�̃C���[�W��
		string lImgName;
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
		//���S�̕s�����x�̎w��
		float al;
		//�t���[�����J�E���g
		int timeCnt;
		//
		Bone* enBone;
		//
		ML::Vec3 pos;
		//�G�t�F�N�g���o�ă��S���o�����Ă��邩���f�p
		bool endFlag;
		//endFlag��true�ɂȂ��Ă���̃t���[�����J�E���g
		int endCnt;
		//
		bool iniFlag;
	public:
		void Set_Bone_Ptr(Bone*);
	};
}