#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�G�l�~�[
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "ChaseSystem.h"
#include "Routine.h"
#include "Bone.h"

namespace Enemy
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G�l�~�[");	//�O���[�v��
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
		string searchMesh, chasing_BG;
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
	public:
		//�ύX������������������������������������������������������
		//�ǉ��ϐ�
		//�f���t���O
		bool demo;
		//�f���p�ړ���R�[�i�[�z��
		int demoCorner[9];
		//�f���p���̃R�[�i�[��ێ�
		int demoNowCorner;
		//���g�̍��W
		ML::Vec3 pos;
		//�p�j���̈ړ����x
		float speed;
		//
		ML::Box3D hitBase;
		//����
		ML::Vec3 angle;
		//�ǐ՗p�ړ���
		ML::Vec3 toVec;
		//���x
		float chasing_Speed;
		float final_Phase_Speed;
		//�T�m�p��`
		//ML::Box3D searchBase;

		//�{�[���A�j���p�{�[��
		Bone* ebone;
		//�A�j���[�V�����̖��O��ۑ�����x�N�^�[
		std::vector<string> animations_Name;
		//�A�j���[�V�����������\�b�h
		void Init_Enemys_Animations();

		//���[�`�[�����[�N
		Routine* rou;

		//�ǐՃV�X�e��
		ChaseSystem system;
		int timeCnt;
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//
		void Player_HitCheck();
		//
		//Bone* Get_EnemyBonePtr();
		//
		ML::Vec3 Get_Pos();
	};
}