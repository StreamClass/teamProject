#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�R�c�}�b�v
//-------------------------------------------------------------------
#include "MyPG.h"
#include "MapBox.h"
#include "MapObject.h"

namespace Map
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�t�B�[���h");	//�O���[�v��
	const  string  defName("�}�b�v");	//�^�X�N��
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
		//�C���[�W��
		string imageName;
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
		int			sizeX, sizeZ;		//�}�b�vtxt���͈̔̓T�C�Y
		int			maxSizeX, maxSizeZ;	//�}�b�v��X�EZ�̍ő�T�C�Y
		int			tmpX, tmpZ;			//�}�b�v��ݒ肷��Ƃ��̊�l
		Box			arr[100][100];		//�}�b�v�S�̂�arr[z][x]
		Box			floor, ceiling;		//�V��Ə�
		Box			goal;				//�S�[��
		string		fileName;			//�ǂݎ��}�b�v�̖��O
		string		filePath;			//�}�b�v�t�@�C���ǂݎ�莞�̃p�X

		std::vector<MapObj*> mapObjects;//�}�b�v�I�u�W�F�N�g�p�x�N�^�[

	public:
		//���\�b�h------------------------------------------------------
		//�}�b�v�̓ǂݍ���
		bool  Load_Map();
		//�I�u�W�F�N�g�̓ǂݍ���
		bool  Load_Objects();
		//�����蔻��
		//����(�}�b�v�Ƃ̔�����s������̓����蔻��͈�)
		bool Map_CheckHit(const ML::Box3D& pHit);
		//�S�[���ɐڐG���Ă��邩��Ԃ�
		bool Goal_CheckHit(const ML::Box3D& pHit);
	};
}