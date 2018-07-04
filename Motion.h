#pragma once
#include "myLib.h"

namespace Motion
{
	struct Motion_Data
	{
		//各軸の回転データをベクトルに宣言
		ML::Vec3 neck_Rot;
		ML::Vec3 waist_Rot;

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
		ML::Vec3 right_Ankle_Rot;

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
		this->neck_Rot = neck;
		this->waist_Rot = waist;

		this->left_Sholder_Rot = l_sholder;
		this->left_Elbow_Rot = l_elbow;
		this->left_Wrist_Rot = l_wrist;

		this->right_Sholder_Rot = r_sholder;
		this->right_Elbow_Rot = r_elbow;
		this->right_Wrist_Rot = r_wrist;

		this->left_Hip_Rot = l_hip;
		this->left_Knee_Rot = l_knee;
		this->left_Ankle_Rot = l_ankle;
		
		this->right_Hip_Rot = r_hip;
		this->right_Knee_Rot = r_knee;
		this->right_Ankle_Rot = r_ankle;
	}
}