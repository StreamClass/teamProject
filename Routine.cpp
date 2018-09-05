#include "Routine.h"
#include "MapBox.h"
#include "Task_Player.h"
#include "MyPG.h"

//マップ内のコーナーの数
#define corners 28
//現在の最多選択肢数
//4又が最多
#define choices 4

//ゼロクリア
Routine::Routine()
{
	srand((unsigned int)time(NULL));
	this->pos = ML::Vec3(0.0f, 0.0f, 0.0f);
	this->moveVec = ML::Vec3(0, 0, 0);
	this->hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->now = 19;
	this->final_Phase = false;
	this->target = 0;
	for (int i = 0; i < corners; ++i)
	{
		this->cornerPos[i] = ML::Vec3(0,0,0);
	}
	for (int i = 0; i < corners; ++i)
	{
		for (int j = 0; j < choices; ++j)
		{
			this->choiceCorner[i][j]=-1;
		}
	}
}

//
int Routine::Get_Now()
{
	return this->now;
}

//近いコーナーを目指して移動する
//toVecをエネミーの移動量に代入して使用
ML::Vec3 Routine::Move(const ML::Vec3& ePos_)
{
	ML::Vec3 toVec = this->cornerPos[this->target] - ePos_;
	return toVec.Normalize();
}
//自分がコーナーに到達したかの判定
bool Routine::Judge(const ML::Box3D& eHit_, const ML::Vec3& ePos_)
{
	//判定用変数をコーナーの番号外で宣言
	this->now = this->Recieved(eHit_,ePos_);
	//judgeの値がいずれかのコーナーの番号になったとき
	if (this->now >= 0)
	{
		/*this->Choice(this->now);*/
		return true;
	}
	else
	{
		return false;
	}
}
//自分のいるコーナーから行けるコーナーを選択する
void Routine::Choice(const int& now_)
{
	this->target = -1;
	//コーナーによって選択肢の数が違うため
	//自然数が出るまで選定を繰り返す
	if (this->Is_Final_Phase() == false)
	{
		while (this->target < 0)
		{
			this->target = this->choiceCorner[this->now][rand() % choices];
		}
		//向かうべき場所が不定場所でなければ
		if (!this->priority_Pos.Is_Zero_Vec())
		{
			//そこに向かう最短ルートで行く
			this->Find_Nearest_Conner(this->priority_Pos);
		}
	}
	//最終フェーズの処理
	//選べる選択肢の中でプレイヤと一番近いところを選ぶ
	else
	{
		//プレイヤの情報をもらう
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		this->Find_Nearest_Conner(pl->Get_Pos());		
	}
}
//コーナーの持つ矩形の側がエネミーとの接触に応じて自分の番号を返す
int Routine::Recieved(const ML::Box3D& eHit_, const ML::Vec3& ePos_)
{
	ML::Box3D me = eHit_.OffsetCopy(ePos_);
	//コーナーに矩形を持たせ、接触判定を行う
	//for (int i = 0; i < corners; ++i)
	//{
	//	ML::Box3D you = ML::Box3D(this->cornerPos[i].x, this->cornerPos[i].y, this->cornerPos[i].z,
	//		1, 1, 1);
	//	if (you.Hit(me))
	//	{
	//		//接触した矩形の番号を返して抜ける
	//		return i;
	//	}
	//}
	ML::Box3D you = ML::Box3D(int(this->cornerPos[this->target].x),
		int(this->cornerPos[this->target].y),
		int(this->cornerPos[this->target].z),
		1, 1, 1);
	if (you.Hit(me))
	{
		//接触した矩形の番号を返して抜ける
		return this->target;
	}
	return -1;
}
void Routine::Set_CornerPos(const ML::Vec3& pos_,const int& num_)
{
	this->cornerPos[num_] = pos_;
}
//外部から挙動を設定したいとき行き先を入れる
void Routine::Set_Target(const int& target_)
{
	this->target = target_;
}
//目的地の検索用
//マップ読み込みの後に呼び出す
void Routine::Initialize_RelationShip(int num)
{
	int c = 0;
	if (this->RelationShip_XP(num, c))
	{
		c++;
	}
	if(this->RelationShip_XM(num, c))
	{
		c++;
	}
	if (this->RelationShip_ZP(num, c))
	{
		c++;
	}
	if (this->RelationShip_ZM(num, c))
	{
		c++;
	}
}

bool Routine::RelationShip_XP(int num_, int c)
{
	ML::Box3D me = ML::Box3D(-50, -50, -50, 100, 100, 100);	
	
	for (int xp = 0; xp < 200; xp++)
	{	
		for (int i = 0; i<corners; i++)
		{
			if (num_ == i)
			{
				continue;
			}
			if (me.OffsetCopy(this->cornerPos[num_] + ML::Vec3((float)xp * 50, 0.0f, 0.0f)).Hit(this->cornerPos[i]))
			{
				this->choiceCorner[num_][c] = i;				
				return true;
			}
		}
	}
	return false;
}

bool Routine::RelationShip_XM(int num_, int c)
{
	ML::Box3D me = ML::Box3D(-50, -50, -50, 100, 100, 100);
	for (int xm = 0; xm < 200; xm++)	
	{
		
		for (int i = 0; i<corners; i++)
		{
			if (num_ == i)
			{
				continue;
			}
			if (me.OffsetCopy(this->cornerPos[num_] + ML::Vec3((float)xm * -50, 0.0f, 0.0f)).Hit(this->cornerPos[i]))
			{
				this->choiceCorner[num_][c] = i;				
				return true;
			}
		}
	}
	return false;
}

bool Routine::RelationShip_ZP(int num_, int c)
{
	ML::Box3D me = ML::Box3D(-50, -50, -50, 100, 100, 100);
	
	for (int zp = 0; zp < 200; zp++)	
	{
		
		for (int i = 0; i<corners; i++)
		{	
			if (num_ == i)
			{
				continue;
			}
			if (me.OffsetCopy(this->cornerPos[num_] + ML::Vec3(0.0f, 0.0f, (float)zp * 50)).Hit(this->cornerPos[i]))
			{
				this->choiceCorner[num_][c] = i;				
				return true;
			}
		}
	}
	return false;
}

bool Routine::RelationShip_ZM(int num_, int c)
{
	ML::Box3D me = ML::Box3D(-50, -50, -50, 100, 100, 100);
	for (int zm = 0; zm < 200; zm++)
	{
		
		for (int i = 0; i<corners; i++)
		{
			if (num_ == i)
			{
				continue;
			}

			if (me.OffsetCopy(this->cornerPos[num_] + ML::Vec3(0.0f, 0.0f, (float)zm * -50)).Hit(this->cornerPos[i]))
			{
				this->choiceCorner[num_][c] = i;				
				return true;
			}
		}
	}
	return false;
}

//void Routine::Initialize_Corner();
//{
//	this->choiceCorner[0][0] = 1;
//	this->choiceCorner[0][1] = 9;
//
//	this->choiceCorner[1][0] = 2;
//	this->choiceCorner[1][1] = 7;
//
//	this->choiceCorner[2][0] = 1;
//	this->choiceCorner[2][1] = 3;
//
//	this->choiceCorner[3][0] = 2;
//	this->choiceCorner[3][1] = 4;
//	this->choiceCorner[3][2] = 8;
//
//	this->choiceCorner[4][0] = 3;
//	this->choiceCorner[4][1] = 5;
//
//	this->choiceCorner[5][0] = 4;
//	this->choiceCorner[5][1] = 18;
//
//	this->choiceCorner[6][0] = 7;
//	this->choiceCorner[6][1] = 11;
//
//	this->choiceCorner[7][0] = 1;
//	this->choiceCorner[7][1] = 6;
//	this->choiceCorner[7][2] = 8;
//	this->choiceCorner[7][3] = 12;
//
//	this->choiceCorner[8][0] = 3;
//	this->choiceCorner[8][1] = 7;
//	this->choiceCorner[8][2] = 17;
//
//	this->choiceCorner[9][0] = 0;
//	this->choiceCorner[9][1] = 10;
//	this->choiceCorner[9][2] = 19;
//
//	this->choiceCorner[10][0] = 9;
//	this->choiceCorner[10][1] = 11;
//	this->choiceCorner[10][2] = 14;
//
//	this->choiceCorner[11][0] = 6;
//	this->choiceCorner[11][1] = 10;
//	this->choiceCorner[11][2] = 12;
//	this->choiceCorner[11][3] = 15;
//
//	this->choiceCorner[12][0] = 7;
//	this->choiceCorner[12][1] = 11;
//	this->choiceCorner[12][2] = 13;
//
//	this->choiceCorner[13][0] = 7;
//}


//最終フェーズに移行
void Routine::Set_Final_Phase()
{
	this->final_Phase = true;
}

//ファイナルフェーズかを確認する
bool Routine::Is_Final_Phase()
{
	return this->final_Phase;
}

//引数の場所と一番近いコーナーを探す
void Routine::Find_Nearest_Conner(const ML::Vec3& find)
{
	int m = 0;
	ML::Vec3 min = find - this->cornerPos[this->choiceCorner[this->now][m]];

	for (int i = 1; i < choices; i++)
	{
		int check = this->choiceCorner[this->now][i];
		//-1は不定アクセスだから処理せず次に
		if (check == -1)
		{
			continue;
		}

		ML::Vec3 compare = find - this->cornerPos[check];
		if (min.Length() > compare.Length())
		{
			min = compare;
			m = i;
		}
	}
	//次の場所を設定
	this->target = this->choiceCorner[this->now][m];	
}

//向かうべき場所を設定
void Routine::Set_Priority_Position(const ML::Vec3& pp)
{
	this->priority_Pos = pp;
}