#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�~�j�}�b�v�\��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "MapBox.h"

#define NORMALMAGNI 5.0f
#define NORMALMAPSIZE 500
#define TABLETMAGNI 8.0f
#define TABLETMAPSIZE 800

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
		//�C���[�W��
		string imageName;
		string plImgName;
		string caImgName;
		string anImgName;
		string cursor_Image_Name;
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

		//�~�j�}�b�v�̃v���C���p�ϐ�
		//���W
		ML::Vec2 plpos;
		//����
		float plAngle;
		//�~�j�}�b�v�̃J�����p�ϐ�
		//���W
		ML::Vec2 capos;
		//�g�p����Ă��Ȃ��J�����̍��W
		//std::vector<ML::Vec2> stanbyCamera;
		std::map<ML::Vec2,unsigned int>stanbyCamera;
		//����
		float caAngle;
		//�~�j�}�b�v��\�����邩
		bool viewFlag;
		//�^�u���b�g���g�p���Ă��邩
		bool tab_use_now;
		//���W�Q�Ɨp�{��
		float magni;
		//�}�b�v�̃T�C�Y
		int mapSize;
		//�f�o�b�N���[�h�̗L��
		bool debugMode;

		//�^�u���b�g���[�h�ŃJ�����I��p�J�[�\��
		//���W
		ML::Vec2 cursor_Pos;
		//�����蔻��p��`
		ML::Box2D cursor_Box;
	public:

		//���\�b�h
		//�~�j�}�b�v��\�����邩���f
		bool MiniMap_View();
		//�v���C������~�j�}�b�v���o�����w��
		void Set_MiniMap_View();
		//�~�j�}�b�v�`��p
		void MiniMap_Render();
		//�Ď��J�������~�j�}�b�v�\���p�ɍ��W���擾
		void Set_StanbyCameraPos(const ML::Vec3&, unsigned int&);

		//�J�[�\���ړ�
		int Cursor_Move();		

		//�f�o�b�O���p�ϐ�
		//�G�l�~�[�̍��W
		ML::Vec2 epos;
		//�G�l�~�[�̌���
		float eangle;

	};
}