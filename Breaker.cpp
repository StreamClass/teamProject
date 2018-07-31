#include "Breaker.h"
#include "MyPG.h"
//ゼロクリア
Breaker::Breaker()
{
	this->pos = ML::Vec3(0, 0, 0);
	this->active_Base = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->active_Falg = false;
}
//場所指定生成
Breaker::Breaker(ML::Vec3 pos)
{
	this->pos = pos;
	this->active_Base = ML::Box3D(-chipX / 2,-chipY / 2,-chipZ / 2,chipX,chipY,chipZ);
	this->active_Falg = false;
}
//引数(プレイヤエイムの矩形)と当たっているかを返すメソッド
bool Breaker::Player_Touch_Breaker(const ML::Box3D& hit)
{
	return hit.Hit(this->active_Base.OffsetCopy(this->pos));
}
//ブレーカー起動
void Breaker::Activate_Breaker()
{
	this->active_Falg = true;
}
//現在状態を返す
bool Breaker::Get_Now_State()
{
	return this->active_Falg;
}
//ブレーカーの位置を返す
ML::Vec3 Breaker::Get_Pos()
{
	return this->pos;
}