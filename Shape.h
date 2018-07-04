#pragma once
#include "myLib.h"

//�O�p�`�̊�{���
//���_�Ɩ@���x�N�g��
struct Triangle
{
	ML::Vec3 a, b, c;
	ML::Vec3 normal;
};

//�����蔻��̌�ɕԂ��\����
struct After_Collision
{
	bool collision_Flag;
	ML::Vec3 normal;

	After_Collision()
	{
		this->collision_Flag = false;
		this->normal = ML::Vec3(0, 0, 0);
	}
};

//3�����}�`��{�`

class Shape
{
private:
	//���S�_
	ML::Vec3 center;

public:
	//���S�_�̃R�s�[�����炤
	ML::Vec3 GetCenter();
	//��]
	void Rotate_Center(ML::Mat4x4* matR);
	//����
	void Move_Center(const ML::Vec3& vec);

	//���z�֐�
	//�p����̉�]
	virtual void Rotate(ML::Mat4x4*) = 0;
	//�p����̈ړ�
	virtual void Move(const ML::Vec3&) = 0;
	//���_�ŕ�������
	virtual void Get_All_Points(std::vector<ML::Vec3>*) = 0;
	//�O�p�`�ŕ�������
	virtual void Get_All_Triangle(std::vector<Triangle>*) = 0;

	//�R���X�g���N�^
	//�[���N���A
	Shape()
	{
		this->center = ML::Vec3(0, 0, 0);
	}
	
	Shape(const ML::Vec3& c)
	{
		this->center = c;
	}

	//�f�X�g���N�^
	virtual ~Shape(){}


};