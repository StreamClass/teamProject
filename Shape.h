#pragma once
#include "myLib.h"

//�p����̗񋓌^
enum Shape_Type
{
	Shape_Cube,
	Shape_Sphere,
};

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
	ML::Vec3 GetCenter() const;
	
	//��]
	void Rotate_Center(ML::Mat4x4* matR);
	//����
	void Move_Center(const ML::Vec3& vec);

	//���z�֐�
	//�p����̉�]
	//���� : (��]�s��)
	virtual void Rotate(ML::Mat4x4*) = 0;
	//�p����̈ړ�
	//���� : (�ړ��x�N�g��)
	virtual void Move(const ML::Vec3&) = 0;
	//���_�ŕ�������
	//���� : (���ʂ��i�[���郔�F�N�^�[)
	virtual void Get_All_Points(std::vector<ML::Vec3>*) = 0;
	//�O�p�`�ŕ�������
	//���� : (���ʂ��i�[���郔�F�N�^�[)
	virtual void Get_All_Triangle(std::vector<Triangle>*) = 0;
	//�p����̃p�����[�^�̃R�s�[�����炤
	//���� : (�p�����[�^���ʗp�L�[�f�[�^,���ʂ��i�[����ϐ�)
	virtual void Get_Parameter(const string, ML::Vec3&) = 0;
	//��Ԑ}�`�̃^�C�v�����炤
	virtual Shape_Type Get_Type() = 0;

	//�R���X�g���N�^
	//�[���N���A
	Shape()
	{
		this->center = ML::Vec3(0, 0, 0);
	}
	//���S�_���w�肵���R���X�g���N�^
	Shape(const ML::Vec3& c)
	{
		this->center = c;
	}

	//�f�X�g���N�^
	virtual ~Shape(){}


};