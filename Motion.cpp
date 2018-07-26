#include "Motion.h"

//外部ファイルからのモーションデータ生成
Motion::Motion_Data& Motion::Create_Step_From_File(const string& fn)
{
	//外部ファイルからのモーションデータ生成

	ifstream file("./data/Motion_Data/" + fn);

	//読み込み失敗の時は無効データを返す
	if (file.fail())
	{
		return Motion::Motion_Data();
	}


	
}