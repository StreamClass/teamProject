#include "ChaseSystem.h"
#include "MyPG.h"
#include "Task_Player.h"
#include "Task_Map.h"

void ChaseSystem::PushBack_Route(const ML::Vec3& pos)
{
	//引数をvectorに組み込む
	this->player_Route.push_back(pos);
}

void ChaseSystem::SensorCheck(const ML::Box3D& hit, const ML::Vec3& plpos, const ML::Vec3& pos, const float& angle)
{
	//アングルに合わせた方向ベクトル算出
	ML::Mat4x4 matR;
	ML::Vec3 a(1, 0, 0);
	for (int i = -1; i < 2; i++)
	{
		matR.RotationY(angle + ML::ToRadian(45 * i));
		a = matR.TransformCoord(a);

		//マップとのあたり判定を持っているタスクをもらう
		auto h = ge->GetTask_One_G<Map::Object>("フィールド");
		//アングル方向にセンサー矩形発射
		for (int i = 0; i < chipX * 10; i += this->sensor.d / 2.0f)
		{
			ML::Box3D s = this->sensor.OffsetCopy(pos + (a.Normalize()*i));
			//センサーの中心に範囲1の矩形を同時に発射
			ML::Box3D c(0, 0, 0, 1, 1, 1);
			c.Offset(pos + (a.Normalize()*i));
			//マップとのあたり判定であたったら処理終了
			if (h->Map_CheckHit(c))
			{
				break;
			}
			//センサーに当たったら
			if (s.Hit(hit))
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
	if (destination > this->player_Route.size() - 1)
	{
		//通常モード返還
		this->Shift_to_Routine();
		//目的地ゼロベクトルを返す
		return ML::Vec3(0, 0, 0);
	}
	ML::Vec3 r = this->player_Route[this->destination];
	r.y = 20;
	this->destination++;
	return r;

}

void ChaseSystem::Shift_to_Routine()
{
	this->systemFlag = false;
	this->destination = 0;
	this->player_Route.clear();
}

bool ChaseSystem::Is_Chase_Mode()
{
	return this->systemFlag;
}