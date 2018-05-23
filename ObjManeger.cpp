#include "ObjManeger.h"
#include "Task_Breaker.h"
#include "Task_Door.h"

ObjManeger::ObjManeger()
{
	this->door_Connencted_Breaker.clear();
	this->first_Door_Connencted_Breaker.clear();
}

void ObjManeger::Init_First_Doors_Breaker(ML::Vec3 pos)
{
	Breaker* b = new Breaker(pos);
	this->first_Door_Connencted_Breaker.push_back(b);

	//�u���[�J�[�^�X�N����
	Task_Breaker::Object::Create(true, b);
}

void ObjManeger::Init_Doors_Breaker(ML::Vec3 pos)
{
	Breaker* b = new Breaker(pos);
	this->door_Connencted_Breaker.push_back(b);

	//�u���[�J�[�^�X�N����
	Task_Breaker::Object::Create(true, b);
}

void ObjManeger::Init_First_Door(ML::Vec3 pos, LR a)
{
	Door* d = new Door(pos, this->first_Door_Connencted_Breaker, a);

	this->door.push_back(d);

	//�h�A�^�X�N����
	Task_Door::Object::Create(true, d);
}

void ObjManeger::Init_Door(ML::Vec3 pos, LR a)
{
	//������Ńh�A���ǉ����ꂽ��ς���\������(2018/05/18)
	Door* d = new Door(pos, this->door_Connencted_Breaker, a);

	this->door.push_back(d);

	//�h�A�^�X�N����
	Task_Door::Object::Create(true, d);
}
//--------------------------------------------------------------------------------------------
//���ۂɌĂяo����֐�
void ObjManeger::Create_Breaker(ML::Vec3 pos)
{
	if (this->first_Door_Connencted_Breaker.size() == 0)
	{
		this->Init_First_Doors_Breaker(pos);
	}
	else
	{
		this->Init_Doors_Breaker(pos);
	}
}

void ObjManeger::Create_Door(ML::Vec3 pos, LR a)
{
	if (this->door.size() <= 1)
	{
		this->Init_First_Door(pos, a);
	}
	else
	{
		this->Init_Door(pos, a);
	}
}

void ObjManeger::Finalize()
{
	//�q�[�v����̉������
	for (auto b : this->first_Door_Connencted_Breaker)
	{
		delete b;
	}

	for (auto b : this->door_Connencted_Breaker)
	{
		delete b;
	}

	for (auto d : this->door)
	{
		delete d;
	}

	//���F�N�^�[�N���A
	this->door_Connencted_Breaker.clear();
	this->first_Door_Connencted_Breaker.clear();
	this->door.clear();
}