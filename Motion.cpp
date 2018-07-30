#include "Motion.h"

//外部ファイルからのモーションデータ生成
Motion::Motion_Data Motion::Create_Step_From_File(const string& fn)
{
	//外部ファイルからのモーションデータ生成
	//ファイルオープン
	ifstream file("./data/Motion_Data/" + fn);

	//読み込み失敗の時は無効データを返す
	if (file.fail())
	{
		return Motion::Motion_Data();
	}

	//格納する場所宣言
	ML::Vec3 md[JOINT_ON_HUMAN];
	int du;
	bool flag;

	while (!file.eof())
	{
		//フォーマットは順番通りに作ってるけど
		//もしものためにif文で確認する
		string key_Data;
		
		file >> key_Data;
		//腰
		if (key_Data.at(0) == 'W' || key_Data.at(0) == 'w')
		{
			file >> md[0].x >> md[0].y >> md[0].z;
		}
		//頭
		else if (key_Data.at(0) == 'N' || key_Data.at(0) == 'n')
		{
			file >> md[1].x >> md[1].y >> md[1].z;
		}
		//左半身
		else if (key_Data.at(0) == 'L' || key_Data.at(0) == 'l')
		{
			//左肩
			if (key_Data.at(1) == 'S' || key_Data.at(1) == 's')
			{
				file >> md[2].x >> md[2].y >> md[2].z;
			}
			//左肘
			if (key_Data.at(1) == 'E' || key_Data.at(1) == 'e')
			{
				file >> md[3].x >> md[3].y >> md[3].z;
			}
			//左手首
			if (key_Data.at(1) == 'W' || key_Data.at(1) == 'w')
			{
				file >> md[4].x >> md[4].y >> md[4].z;
			}
			//左お尻
			if (key_Data.at(1) == 'H' || key_Data.at(1) == 'h')
			{
				file >> md[8].x >> md[8].y >> md[8].z;
			}
			//左膝
			if (key_Data.at(1) == 'K' || key_Data.at(1) == 'k')
			{
				file >> md[9].x >> md[9].y >> md[9].z;
			}
			// 左足首
			if (key_Data.at(1) == 'A' || key_Data.at(1) == 'a')
			{
				file >> md[10].x >> md[10].y >> md[10].z;
			}
		}
		//右半身
		else if (key_Data.at(0) == 'R' || key_Data.at(0) == 'r')
		{
			//右肩
			if (key_Data.at(1) == 'S' || key_Data.at(1) == 's')
			{
				file >> md[5].x >> md[5].y >> md[5].z;
			}
			//右肘
			if (key_Data.at(1) == 'E' || key_Data.at(1) == 'e')
			{
				file >> md[6].x >> md[6].y >> md[6].z;
			}
			// 右手首
			if (key_Data.at(1) == 'W' || key_Data.at(1) == 'w')
			{
				file >> md[7].x >> md[7].y >> md[7].z;
			}
			// 右お尻
			if (key_Data.at(1) == 'H' || key_Data.at(1) == 'h')
			{
				file >> md[11].x >> md[11].y >> md[11].z;
			}
			// 右膝
			if (key_Data.at(1) == 'K' || key_Data.at(1) == 'k')
			{
				file >> md[12].x >> md[12].y >> md[12].z;
			}
			// 右足首
			if (key_Data.at(1) == 'A' || key_Data.at(1) == 'a')
			{
				file >> md[13].x >> md[13].y >> md[13].z;
			}
		}
		//フレーム数
		else if (key_Data.at(0) == 'D' || key_Data.at(0) == 'd')
		{
			file >> du;
		}
		//repeat flag
		else if (key_Data.at(0) == 'T' || key_Data.at(0) == 't')
		{
			flag = true;
		}
		else if(key_Data.at(0) == 'F' || key_Data.at(0) == 'f')
		{
			flag = false;
		}
	}

	//ファイルクローズ
	file.close();

	//戻り値を作成して返す
	return Motion_Data(md[0], md[1],
		md[2], md[3], md[4],
		md[5], md[6], md[7],
		md[8], md[9], md[10],
		md[11], md[12], md[13],
		du, flag);
}

void Motion::Make_Motion(std::vector<Motion_Data>* result, const string& motion_Name)
{
	//何番目かを確認するインデックス
	int i = 0;
	while(true)
	{	
		//外部ファイルからの読み込み
		Motion::Motion_Data step = Motion::Create_Step_From_File(motion_Name + "/Step" + to_string(i) + ".txt");
		//無効データならループを抜ける
		if (step.Is_Zero_Data())
		{
			break;
		}
		else
		{
			//無効データではない時データ登録
			result->push_back(step);
		}
		//インデックス上昇
		i++;
	}


}