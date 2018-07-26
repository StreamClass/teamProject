#include "Bone.h"

//後に外部ファイルからの読み込みに実装する予定
//現在外部ファイルフォーマットが決まってないので
//力技だが直値でのモーションデータを生成する(2018/07/25)


//--------------------------------------------------------------------------
//モーションプリセット生成
void Bone::Make_Interaction()
{
	//モーションデータのヴェクター生成
	std::vector<Motion::Motion_Data> interaction;

	//各モーションデータステップを生成
	//step0
	//右肘を上げる
	Motion::Motion_Data step0(ML::Vec3(0, -20, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		2
	);

	Motion::Motion_Data step1(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3((-180), 0, 0), ML::Vec3((-10), 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);

	//step1
	//肘を伸ばしながら手を上げる
	//同時に肩を前方上に
	Motion::Motion_Data step2(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-100), 0, 0), ML::Vec3((180), 0, 0), ML::Vec3((-20), 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);
	//step2, step3
	//行動を巻き戻す
	Motion::Motion_Data step3(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((100), 0, 0), ML::Vec3((-180), 0, 0), ML::Vec3((20), 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);

	Motion::Motion_Data step4(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, (0), 0), ML::Vec3((180), 0, 0), ML::Vec3((10), 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);

	Motion::Motion_Data step5(ML::Vec3(0, 20, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		2
	);

	//ヴェクター登録及びマッピング
	interaction.push_back(step0);
	interaction.push_back(step1);
	interaction.push_back(step2);
	interaction.push_back(step3);
	interaction.push_back(step4);
	interaction.push_back(step5);

	this->motions.insert({ "InterAction",interaction });
}

void Bone::Make_Running()
{
	std::vector<Motion::Motion_Data> running;

	//step0
	//肘を曲げる
	Motion::Motion_Data step0(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3((-90), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3((-90), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-45), 0, 0), ML::Vec3((45), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((30), 0, 0), ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0),
		6
	);

	Motion::Motion_Data step1(ML::Vec3(0, 10, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		4, true
	);

	//step1
	//右足、左手を出す
	Motion::Motion_Data step2(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-30), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((70), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((90), 0, 0), ML::Vec3(-45, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-100), 0, 0), ML::Vec3((60), 0, 0), ML::Vec3(0, 0, 0),
		10
	);
	//step2
	//肩整頓、右足着地
	Motion::Motion_Data step3(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-70), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-15), 0, 0), ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((25), 0, 0), ML::Vec3((-45), 0, 0), ML::Vec3(0, 0, 0),
		10
	);

	Motion::Motion_Data step4(ML::Vec3(0, -20, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		10
	);
	/*Motion::Motion_Data step5(ML::Vec3(0, -10, 0), ML::Vec3(0, 0, 0),
	ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
	ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
	ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
	ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
	2
	);*/

	//step3
	//左足、右手を出す
	Motion::Motion_Data step6(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((70), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-30), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-100), 0, 0), ML::Vec3((60), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((90), 0, 0), ML::Vec3((-45), 0, 0), ML::Vec3(0, 0, 0),
		10
	);
	//step4
	//肩整頓、左足着地
	Motion::Motion_Data step7(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-70), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((25), 0, 0), ML::Vec3((-45), 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3((-15), 0, 0), ML::Vec3((30), 0, 0), ML::Vec3(0, 0, 0),
		10
	);

	Motion::Motion_Data step8(ML::Vec3(0, 10, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		4
	);

	//ヴェクター登録及びマッピング
	running.push_back(step0);
	running.push_back(step1);
	running.push_back(step2);
	running.push_back(step3);
	running.push_back(step4);
	//running.push_back(step5);
	running.push_back(step6);
	running.push_back(step7);
	running.push_back(step8);

	this->motions.insert({ "Running",running });
}

//歩くモーション
void Bone::Make_Walking()
{
	std::vector<Motion::Motion_Data> walking;

	//step0
	//両肘を前に少し曲げる
	Motion::Motion_Data step0(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		5
	);

	//上半身の左肩が前に
	Motion::Motion_Data step1(ML::Vec3(0, 10, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0),
		5, true
	);

	//step2
	//左腕、右足が前に
	//右腕、左足が後ろに
	Motion::Motion_Data step2(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(20, 0, 0), ML::Vec3(40, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	//step3
	//腕整頓、右足着地
	Motion::Motion_Data step3(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(20, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(-30, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	//step4
	//上半身右肩前に
	Motion::Motion_Data step4(ML::Vec3(0, -20, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		10
	);

	//step5
	//右腕、左足前に
	//左腕、右足後ろに
	Motion::Motion_Data step5(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(30, 0, 0), ML::Vec3(40, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-20, 0, 0), ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	//step6
	//腕整頓
	//左足着地
	Motion::Motion_Data step6(ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(20, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(-30, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	//step7
	//上半身直立
	//両足整頓
	Motion::Motion_Data step7(ML::Vec3(0, 10, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(-10, 0, 0), ML::Vec3(-20, 0, 0), ML::Vec3(0, 0, 0),
		ML::Vec3(10, 0, 0), ML::Vec3(0, 0, 0), ML::Vec3(0, 0, 0),
		15
	);

	walking.push_back(step0);
	walking.push_back(step1);
	walking.push_back(step2);
	walking.push_back(step3);
	walking.push_back(step4);
	walking.push_back(step5);
	walking.push_back(step6);
	walking.push_back(step7);

	this->motions.insert({ "Walking", walking });
}