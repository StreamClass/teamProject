#pragma once
#include "GameEngine_Ver3_7.h"
#include <fstream>
#include <sstream>
#include "ObjManeger.h"
#include "Dm2008_Ver1_3.h"

//�Q�[�����S��Ŏg�p����\���̂Ȃǂ��`����
//-----------------------------------------------------------------------------------------------
//�ǂ̃T�C�Y�w��
#define chipX 150
#define chipY 300
#define chipZ 150
//�`��͈�
#define RENDERRENGE 14
//�v���C���p
//���_���x
#define TURNSPEED 2

//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//�J������{�`
	class Camera
	{
		Camera( ){ }
		Camera(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j			
	public:
	//	�r���[���i�����֘A�j
		ML::Vec3 target;			//	��ʑ̂̈ʒu
		ML::Vec3 pos;			//	�J�����̈ʒu
		ML::Vec3 up;				//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
	//	�ˉe���i����͈͊֘A�j
		float fov;					//	����p
		float nearPlane;			//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
		float forePlane;			//	��N���b�v���ʁi��������͉f��Ȃ��j
		float aspect;				//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j
	//	�s����
		ML::Mat4x4  matView, matProj;
		~Camera( );
		typedef shared_ptr<Camera> SP;
	//	�J�����𐶐�����
		static SP Create(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j	
	//	�J�����̐ݒ�
		void UpDate( );
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine( );
	//�Q�[���G���W���ɒǉ��������̂̏������ƊJ��
		bool Initialize(HWND wnd_, IDXGIAdapter* adp);
		~MyGameEngine( );
	//�Q�[���G���W���ɒǉ��������̂̃X�e�b�v����
		void UpDate( );

	//3DPG1�Ή��ɂ��ǉ�
		//2D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set2DRenderState(DWORD l_);
		//3D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set3DRenderState(DWORD l_);



	//�Q�[���G���W���ɒǉ����������͉̂��ɉ�����
	//----------------------------------------------
		MyPG::Camera::SP		camera[4];		//	�J����
		D3D10_VIEWPORT			vp3D_L0;
		D3D10_VIEWPORT			vp3D_L1;
		D3D10_VIEWPORT			vp3D_L2;
		D3D10_VIEWPORT			vp3D_L3;
		D3D10_VIEWPORT			vp3D_BF;
		D3D10_VIEWPORT			vp3D_AF;
		
		ObjManeger OM;
		//shared_ptr<BEffect::SP> effmaneger;

		enum State
		{
			non = -1,
			game = 0,
			clear = 1,
			over = 2,
			demo = 3,
		};
		State state;
		void Dbg_ShowFps();

		double FpsTime = (timeGetTime() + 1.0);
		double FpsData = 0.0;
		int FpsCnt = 1;

		void Dbg_DisplayToMessage(int x, int y, const char * str, ...);
		void Dbg_TilteToMessage(const char * str, ...);
		void Dbg_BoxToMessage(const char *str, ...);
		void Dbg_FileOut(const char *str, ...);
		
		//XBox:P1 DualShock:P2
		string controllerName = "P1";
	//----------------------------------------------
	};
}
extern MyPG::MyGameEngine* ge;


