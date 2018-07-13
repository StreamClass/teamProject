#pragma once
#include "Shape.h"

//3������`

class Cube : public Shape
{
private:
	//�n�_
	ML::Vec3 leftBottom;
	//�e�ӂ̒���
	ML::Vec3 lengthX;
	ML::Vec3 lengthY;
	ML::Vec3 lengthZ;


public:
	//�Q�b�^�[
	//�p�����[�^��Ԃ����\�b�h
	//�����̖��O�Ŗ߂�����̂����߂�
	//���� : (��������L�[�A���[����R���e�i)
	void Get_Parameter(const string key, ML::Vec3& cont) override;

	//�ړ��E��]
	//���� : (��]�s��̃A�h���X�l)
	void Rotate(ML::Mat4x4* matR) override;
	//���� : (�ړ��x�N�g��)
	void Move(const ML::Vec3& vec) override;
	//���� : (���ʂ�ۑ�����ML::Vec3�^��vector�̃A�h���X�l)
	void Get_All_Points(std::vector<ML::Vec3>* result) override;
	//���� : (���ʂ�ۑ�����Triangle�^��vector�̃A�h���X�l)
	void Get_All_Triangle(std::vector<Triangle>* resultr) override;	
	

	//�R���X�g���N�^
	//�[���N���A
	Cube():Shape()
	{
		this->leftBottom = ML::Vec3(0, 0, 0);
		this->lengthX = ML::Vec3(0, 0, 0);
		this->lengthY = ML::Vec3(0, 0, 0);
		this->lengthZ = ML::Vec3(0, 0, 0);
	}
	//���� : (���S�_�A���[�����̓_�AX�̒����AY�̒����AZ�̒���)
	Cube(const ML::Vec3& c, const ML::Vec3& lb, const float& x, const float& y, const float&z)
		: Shape(c)
	{
		this->leftBottom = lb;
		this->lengthX = ML::Vec3(x,0,0);
		this->lengthY = ML::Vec3(0,y,0);
		this->lengthZ = ML::Vec3(0,0,z);
	}
	//�������C�u�����[�ł������ł���
	Cube(const ML::Box3D& b)
		: Shape((ML::Vec3(b.x, b.y, b.z) + ML::Vec3(b.w / 2.0f, b.h / 2.0f, b.d / 2.0f)))
	{
		this->leftBottom = ML::Vec3(b.x, b.y, b.z);
		this->lengthX = ML::Vec3(b.x, 0, 0);
		this->lengthY = ML::Vec3(0, b.y, 0);
		this->lengthZ = ML::Vec3(0, 0, b.z);
	}

	//�f�X�g���N�^
	~Cube(){}
};