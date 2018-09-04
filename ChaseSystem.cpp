#include "ChaseSystem.h"
#include "MyPG.h"
#include "Task_Player.h"
#include "Task_Map.h"
#include "MyMath.h"

void ChaseSystem::PushBack_Route(const ML::Vec3& pos)
{
	//引数をvectorに組み込む	
	this->player_Route.push_back(pos);
}

void ChaseSystem::SensorCheck(const ML::Box3D& hit, const ML::Vec3& plpos, const ML::Vec3& pos, const float& angle)
{		
	for (int i = -5; i < 10; i++)
	{
		//アングルに合わせた方向ベクトル算出
		ML::Mat4x4 matR;
		ML::Vec3 a(1, 0, 0);
		matR.RotationY(-angle + ML::ToRadian(10 * (float)i));
		a = matR.TransformNormal(a);

		//判定の基準になる内積値を先に求めておく
		//内積値を保存する場所
		float cos = 0.0f;
		//相対ベクトル
		ML::Vec3 tovec = (plpos - pos);
		//判定中の視線ベクトルと相対ベクトルを内積する
		MyMath::Vector_Dot(&cos, a, tovec);

		//マップとのあたり判定を持っているタスクをもらう
		auto h = ge->GetTask_One_G<Map::Object>("フィールド");
		//アングル方向にセンサー矩形発射
		for (float f = 0; f <= chipX * 10; f += chipX/3.0f)
		{
			//センサーの中心に範囲1の矩形を同時に発射			
			ML::Box3D c(-3,-3,-3,6,6,6);
			c.Offset(pos + (a.Normalize()*f));
			//マップとのあたり判定であたったら処理終了
			if (h->Map_CheckHit(c))
			{
				break;
			}

			//センサーに当たったら
			if(cos > cosf(ML::ToRadian(10)) && (tovec.Length() < f))
			{
				//チェイスモードに変更し、プレイや位置をルートに登録
				this->systemFlag = true;
				this->PushBack_Route(plpos);				
				break;
			}		
		}
	}
}

ML::Vec3 ChaseSystem::NextRoute()
{
	//追跡ルートが終わったら通常モードに
	if (this->Is_Any_More_Route())
	{
		//通常モード返還
		this->Shift_to_Routine();
		//目的地ゼロベクトルを返す
		return ML::Vec3(0, 0, 0);
	}
	ML::Vec3 r = ML::Vec3(0, 0, 0);
	//不定場所が出ないまで繰り返す
	while (r.Is_Zero_Vec())
	{
		r = this->player_Route[this->destination];		
		this->destination++;
	}
	r.y = 20;
	return r;
}

void ChaseSystem::Shift_to_Routine()
{
	this->systemFlag = false;
	this->destination = 0;
	this->player_Route.clear();
}

bool ChaseSystem::Is_Chase_Mode() const
{
	return this->systemFlag;
}

bool ChaseSystem::Is_Any_More_Route() const
{
	return destination > this->player_Route.size() - 1 ? true : false;
}