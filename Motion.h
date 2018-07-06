#pragma once
#include "myLib.h"

#define JOINT_ON_HUMAN 14

namespace Motion
{
	struct Motion_Data
	{
		/*
		各関節に番号を割り当てて管理する
		0 = 腰
		1 = 首
		2 = 左肩
		3 = 左肘
		4 = 左手首
		5 = 右肩
		6 = 右肘
		7 =  右手首
		8 = 左お尻
		9 = 左膝
		10 = 左足首
		11 = 右お尻
		12 = 右膝
		13 = 右足首
		*/
		//各軸の回転データをベクトルに宣言
		ML::Vec3 joint[JOINT_ON_HUMAN];

		//名前を別々に持っていたver0.1
		/*ML::Vec3 waist_Rot;
		ML::Vec3 neck_Rot;		

		ML::Vec3 left_Sholder_Rot;
		ML::Vec3 left_Elbow_Rot;
		ML::Vec3 left_Wrist_Rot;

		ML::Vec3 right_Sholder_Rot;
		ML::Vec3 right_Elbow_Rot;
		ML::Vec3 right_Wrist_Rot;

		ML::Vec3 left_Hip_Rot;
		ML::Vec3 left_Knee_Rot;
		ML::Vec3 left_Ankle_Rot;
		
		ML::Vec3 right_Hip_Rot;
		ML::Vec3 right_Knee_Rot;
		ML::Vec3 right_Ankle_Rot;*/

		//上の回転量データまでたどり着く時間
		const int duration;

		

		//コンストラクタ
		//引数 : (各関節がたどり着く回転量 首、腰、左肩、左肘、左手首、右肩、右肘、右手首、左お尻、左膝、左足首、右お尻、右膝、右足首、たどり着くまでの時間)
		Motion_Data(const const ML::Vec3&, const const ML::Vec3&,
			const ML::Vec3&, const ML::Vec3&, const ML::Vec3&,
			const ML::Vec3&, const ML::Vec3&, const ML::Vec3&,
			const ML::Vec3&, const ML::Vec3&, const ML::Vec3&, 
			const ML::Vec3&, const ML::Vec3&, const ML::Vec3&, 
			const int&);
	};


	Motion_Data::Motion_Data(const ML::Vec3& neck, const ML::Vec3& waist,
		const ML::Vec3& l_sholder, const ML::Vec3& l_elbow, const ML::Vec3& l_wrist,
		const ML::Vec3& r_sholder, const ML::Vec3& r_elbow, const ML::Vec3& r_wrist,
		const ML::Vec3& l_hip, const ML::Vec3& l_knee, const ML::Vec3& l_ankle,
		const ML::Vec3& r_hip, const ML::Vec3& r_knee, const ML::Vec3& r_ankle,
		const int& du)
		: duration(du)
	{
		this->joint[0] = waist;
		this->joint[1] = neck;

		this->joint[2] = l_sholder;
		this->joint[3] = l_elbow;
		this->joint[4] = l_wrist;

		this->joint[5] = r_sholder;
		this->joint[6] = r_elbow;
		this->joint[7] = r_wrist;

		this->joint[8] = l_hip;
		this->joint[9] = l_knee;
		this->joint[10] = l_ankle;
		
		this->joint[11] = r_hip;
		this->joint[12] = r_knee;
		this->joint[13] = r_ankle;
	}
	//モーション生成関数
	//引数 : (モーションの名前、登録させるボーンのアドレス値、外部ファイルパス）
	//void Make_Motion(const string& motion_Name, Bone* b, const string& file_Path);
}