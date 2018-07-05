#include "Breaker.h"
#include "MapBox.h"

Breaker::Breaker()
{
	this->pos = ML::Vec3(0, 0, 0);
	this->active_Base = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->active_Falg = false;
}

Breaker::Breaker(ML::Vec3 pos)
{
	this->pos = pos;
	this->active_Base = ML::Box3D(-chipX / 2,-chipY / 2,-chipZ / 2,chipX,chipY,chipZ);
	this->active_Falg = false;
}

bool Breaker::Player_Touch_Breaker(const ML::Box3D& hit)
{
	return hit.Hit(this->active_Base.OffsetCopy(this->pos));
}

void Breaker::Activate_Breaker()
{
	this->active_Falg = true;
}

bool Breaker::Get_Now_State()
{
	return this->active_Falg;
}

ML::Vec3 Breaker::Get_Pos()
{
	return this->pos;
}