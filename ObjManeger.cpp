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

	//ブレーカータスク生成
	Task_Breaker::Object::Create(true, b);
}

void ObjManeger::Init_Doors_Breaker(ML::Vec3 pos)
{
	Breaker* b = new Breaker(pos);
	this->door_Connencted_Breaker.push_back(b);

	//ブレーカータスク生成
	Task_Breaker::Object::Create(true, b);
}

void ObjManeger::Init_First_Door(ML::Vec3 pos, LR a)
{
	Door* d = new Door(pos, this->first_Door_Connencted_Breaker, a);

	this->door.push_back(d);

	//ドアタスク生成
	Task_Door::Object::Create(true, d);
}

void ObjManeger::Init_Door(ML::Vec3 pos, LR a)
{
	//もし後でドアが追加されたら変える可能性あり(2018/05/18)
	Door* d = new Door(pos, this->door_Connencted_Breaker, a);

	//ドアタスク生成
	Task_Door::Object::Create(true, d);
}

void ObjManeger::Finalize()
{
	//ヒープからの解放処理
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

	//ヴェクタークリア
	this->door_Connencted_Breaker.clear();
	this->first_Door_Connencted_Breaker.clear();
	this->door.clear();
}