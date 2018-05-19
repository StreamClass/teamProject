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

	//�h�A�^�X�N����
	Task_Door::Object::Create(true, d);
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