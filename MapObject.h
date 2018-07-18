#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�}�b�v�I�u�W�F�N�g�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class MapObj
{
private:
	//���W
	ML::Vec3 pos_;
	//�����̃N�H�[�^�j�I��
	ML::QT angle_;
	//�����蔻��
	ML::Box3D hitBase_;
	//�T�C�Y(��l��100)
	int scale_;
	//���b�V����
	string meshName_;
public:
	//�������R���X�g���N�^
	MapObj();
	//�f�[�^�ݒ�p�R�s�[�R���X�g���N�^
	//�����F( ML::Vec3�^ ���W ,ML::QT�^ ���� ,ML::Box3D�^ �����蔻�� ,int�^ �X�P�[�����O ,string�^ ���b�V���� )
	MapObj(ML::Vec3&, ML::QT&, ML::Box3D&, int&, string&);
	//�ێ��摜����p�f�X�g���N�^
	~MapObj();

	//���\�b�h---------------------------------------------
	//���W��Ԃ�
	ML::Vec3 Get_Pos();
	//�����蔻���Ԃ�
	ML::Box3D Get_HitBase();
	//�`�揈��
	void Render3D();
};