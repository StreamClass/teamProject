#include "ObjManeger.h"
#include "Task_Breaker.h"
#include "Task_Door.h"

ObjManeger::ObjManeger()
{
	this->door_Connencted_Breaker.clear();
	this->first_Door_Connencted_Breaker.clear();
	this->door.clear();	
	this->tab = nullptr;
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

	this->door.push_back(d);

	//ドアタスク生成
	Task_Door::Object::Create(true, d);
}
//--------------------------------------------------------------------------------------------
//実際に呼び出せる関数
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
	delete this->tab;

	//ヴェクタークリア
	this->door_Connencted_Breaker.clear();
	this->first_Door_Connencted_Breaker.clear();
	this->door.clear();
}

void ObjManeger::Create_Camera(ML::Vec3 pos, Type angle)
{
	//引数でもらった角度を計算して
	//カメラ位置をタブレットクラスに更新
	ML::Vec3 target(0, 0, 0);
	switch (angle)
	{		
		//東
	case Type::camera_East:
		target = pos + ML::Vec3(700, -200, 0);
		this->tab->PushBack_Camera(pos, target);
		break;
		//西
	case Type::camera_West:
		target = pos + ML::Vec3(-700, -200, 0);
		this->tab->PushBack_Camera(pos, target);
		break;
		//南
	case Type::camera_South:
		target = pos + ML::Vec3(0, -200, -700);
		this->tab->PushBack_Camera(pos, target);
		break;
		//北
	case Type::camera_North:
		target = pos + ML::Vec3(0, -200, 700);
		this->tab->PushBack_Camera(pos, target);
		break;
		//北東
	case Type::camera_North_East:
		target = pos + ML::Vec3(350, -200, 350);
		this->tab->PushBack_Camera(pos, target);
		break;
		//北西
	case Type::camera_North_West:
		target = pos + ML::Vec3(-350, -200, 350);
		this->tab->PushBack_Camera(pos, target);
		break;
		//南東
	case Type::camera_South_East:
		target = pos + ML::Vec3(350, -200, -350);
		this->tab->PushBack_Camera(pos, target);
		break;
		//南西
	case Type::camera_South_West:
		target = pos + ML::Vec3(-350, -200, -350);
		this->tab->PushBack_Camera(pos, target);
		break;
	}

	//タスク生成
	//未実装
}

Tablet* ObjManeger::Create_Tablet()
{
	this->tab = new Tablet();
	return this->tab;
}