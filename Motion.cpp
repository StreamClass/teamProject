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
		//もらったデータを全部小文字にする
		Motion::Replace_to_Key(key_Data);
		//キーデータと比較していれる場所を判別
		if (Motion::motion_Data_Key.count(key_Data) != 0)
		{
			auto& index = Motion::motion_Data_Key.find(key_Data);
			file >> md[(*index).second].x >> md[(*index).second].y >> md[(*index).second].z;
		}
		else if (key_Data.at(0) == 't')
		{
			flag = true;
		}
		else if (key_Data.at(0) == 'f')
		{
			flag = false;
		}
		else if (key_Data.at(0) == 'd')
		{
			file >> du;
		}
		//どちらにも合わない場合フォーマットに合わないのでゼロデータを返す
		else
		{
			file.close();
			return Motion::Motion_Data();
		}
	}

	//ファイルクローズ
	file.close();

	//戻り値を作成して返す
	return Motion_Data(md[Joint_Name::Waist], md[Joint_Name::Neck],
		md[Joint_Name::Left_Sholder], md[Joint_Name::Left_Elbow], md[Joint_Name::Left_Wrist],
		md[Joint_Name::Right_Sholder], md[Joint_Name::Right_Elbow], md[Joint_Name::Right_Wrist],
		md[Joint_Name::Left_Hip], md[Joint_Name::Left_Knee], md[Joint_Name::Left_Ankle],
		md[Joint_Name::Right_Hip], md[Joint_Name::Right_Knee], md[Joint_Name::Right_Ankle],
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

void Motion::Replace_to_Key(string& s)
{
	for (unsigned int i = 0; i < s.length(); i++)
	{
		//小文字に置き換える
		if (s.at(i) >= 'A' && s.at(i) <= 'Z')
		{
			s.at(i) += 32;
		}
		//：を発見したら、そこまでのデータをキーデータとする
		if (s.at(i) == ':')
		{
			s.resize(i);
		}
	}
}