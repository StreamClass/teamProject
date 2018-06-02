#include "Routine.h"

//マップ内のコーナーの数
#define corners 24
//現在の最多選択肢数
//4又が最多
#define choices 4

//ゼロクリア
Routine::Routine()
{
	this->pos = ML::Vec3(0.0f, 0.0f, 0.0f);
	this->moveVec = ML::Vec3(0, 0, 0);
	this->hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->now = 0;
	this->target = 0;
	for (int i = 0; i < corners; ++i)
	{
		this->cornerPos[i] = ML::Vec3(0,0,0);
	}
	for (int i = 0; i < corners; ++i)
	{
		for (int j = 0; j < choices; ++j)
		{
			this->choiceCorner[i][j]=0;
		}
	}
}
//近いコーナーを目指して移動する
void Routine::Move()
{
	ML::Vec3 toVec = this->pos - this->cornerPos[this->Choice(this->now)];
	this->moveVec = toVec;
}
//自分がコーナーに到達したかの判定
bool Routine::Judge()
{
	//判定用変数をコーナーの番号外で宣言
	this->now = this->Recieved();
	//judgeの値がいずれかのコーナーの番号になったとき
	if (this->now >= 0)
	{
		this->Choice(this->now);
		return true;
	}
	else
	{
		return false;
	}
}
//自分のいるコーナーから行けるコーナーを選択する
int Routine::Choice(const int& now_)
{
	int c = -1;
	//コーナーによって選択肢の数が違うため
	//自然数が出るまで選定を繰り返す
	while (c < 0)
	{
		c = this->choiceCorner[this->now][rand() % choices];
	}
	//行先を決定し、返す
	return c;
}
//コーナーの持つ矩形の側がエネミーとの接触に応じて自分の番号を返す
int Routine::Recieved()
{
	ML::Box3D me = this->hitBase.OffsetCopy(this->pos);
	//コーナーに矩形を持たせ、接触判定を行う
	for (int i = 0; i < corners; ++i)
	{
		ML::Box3D you = ML::Box3D(this->cornerPos[i].x, this->cornerPos[i].y, this->cornerPos[i].z,
			1, 1, 1);
		if (you.Hit(me))
		{
			//接触した矩形の番号を返して抜ける
			return i;
			break;
		}
	}
}
