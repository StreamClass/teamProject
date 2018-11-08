#pragma once
#include "myLib.h"
#include "Joint.h"

#define JOINT_ON_HUMAN 14

namespace Motion
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
	enum Joint_Name
	{
		Waist			= 0,
		Neck			= 1,
		Left_Sholder	= 2,
		Left_Elbow		=3,
		Left_Wrist		=4,
		Right_Sholder	=5,
		Right_Elbow		=6,
		Right_Wrist		=7,
		Left_Hip		=8,
		Left_Knee		= 9,
		Left_Ankle		= 10,
		Right_Hip		= 11,
		Right_Knee		= 12,
		Right_Ankle		= 13,
	};
	struct Motion_Data
	{		
		//各軸の回転データをベクトルに宣言
		ML::Vec3 joint[JOINT_ON_HUMAN];
		

		//上の回転量データまでたどり着く時間
		const int duration;
		//repeat flagが立っている時帰ってくる場所確認用
		const bool repeat_Point;

		//コンストラクタ(反復行動フラグは0番につけないこと)
		/*引数 : (各関節がたどり着く回転量 首、腰、
		左肩、左肘、左手首、
		右肩、右肘、右手首、
		左お尻、左膝、左足首、
		右お尻、右膝、右足首、
		たどり着くまでの時間、反復行動を探すフラグ)*/
		Motion_Data(const ML::Vec3& waist, const ML::Vec3& neck,
			const ML::Vec3& l_sholder, const ML::Vec3& l_elbow, const ML::Vec3& l_wrist,
			const ML::Vec3& r_sholder, const ML::Vec3& r_elbow, const ML::Vec3& r_wrist,
			const ML::Vec3& l_hip, const ML::Vec3& l_knee, const ML::Vec3& l_ankle,
			const ML::Vec3& r_hip, const ML::Vec3& r_knee, const ML::Vec3& r_ankle,
			const int& du, bool r = false)
			: duration(du),
			repeat_Point(r)
		{
			this->joint[Joint_Name::Waist] = waist;
			this->joint[Joint_Name::Neck] = neck;

			this->joint[Joint_Name::Left_Sholder] = l_sholder;
			this->joint[Joint_Name::Left_Elbow] = l_elbow;
			this->joint[Joint_Name::Left_Wrist] = l_wrist;

			this->joint[Joint_Name::Right_Sholder] = r_sholder;
			this->joint[Joint_Name::Right_Elbow] = r_elbow;
			this->joint[Joint_Name::Right_Wrist] = r_wrist;

			this->joint[Joint_Name::Left_Hip] = l_hip;
			this->joint[Joint_Name::Left_Knee] = l_knee;
			this->joint[Joint_Name::Left_Ankle] = l_ankle;

			this->joint[Joint_Name::Right_Hip] = r_hip;
			this->joint[Joint_Name::Right_Knee] = r_knee;
			this->joint[Joint_Name::Right_Ankle] = r_ankle;
		}

		//ゼロクリア
		Motion_Data()
			: duration(0)
			, repeat_Point(false)
		{
			for (int i = 0; i < JOINT_ON_HUMAN; i++)
			{
				this->joint[i] = ML::Vec3(0, 0, 0);
			}
		}

		//回転行列をもらってその分回転する
		void Rotation_Matrix(ML::Mat4x4* matR)
		{
			for (int i =0; i<JOINT_ON_HUMAN; i++)
			{
				this->joint[i] = matR->TransformNormal(this->joint[i]);
			}
		}

		//Y軸回転に対する足し算
		Motion_Data& operator +(const float& val)
		{
			for (auto& j : this->joint)
			{
				if (!j.Is_Zero_Vec())
				{
					j.y += val;
				}
			}
			return *this;
		}

		Motion_Data& operator +=(const float& val)
		{
			return *this + val;
		}
		
		//無効データかを確認
		bool Is_Zero_Data()
		{
			//ゼロフレーム行動なら
			//無効データと認識する
			return this->duration == 0;
		}
	}; 
	const std::map<string,Joint_Name> motion_Data_Key = 
	{
		{"waist",	Waist },
		{ "neck",	Neck },
		{ "lsholder",Left_Sholder },
		{ "lelbow",	Left_Elbow },
		{ "lwrist",	Left_Wrist },
		{ "rsholder",Right_Sholder },
		{ "relbow",	Right_Elbow },
		{ "rwrist",	Right_Wrist },
		{ "lhip",	Left_Hip },
		{ "lknee",	Left_Knee },
		{ "lankle",	Left_Ankle },
		{ "rhip",	Right_Hip },
		{ "rknee",	Right_Knee },
		{ "rankle",	Right_Ankle }
	};
	//外部ファイルからのモーションデータ生成
	//引数 : (ファイル名 + .txt)
	Motion_Data Create_Step_From_File(const string&);

	void Make_Motion(std::vector<Motion_Data>* result, const string& motion_Name);

	void Replace_to_Key(string& s);
}