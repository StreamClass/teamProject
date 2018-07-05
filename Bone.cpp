#include "Bone.h"
#include "Cube.h"
#include "MyMath.h"

//コンストラクタ

Bone::Bone(const float& tall)
	: tall(tall)
{
	//中心指定
	this->center_of_Body = ML::Vec3(0, tall / 2.0f, 0);
	//比率の基準になる手の長さ
	float length_of_hand = (tall / 2.0f)*(4.0f / 14.0f);


	//頭の初期化
	ML::Vec3 center_of_head = ML::Vec3(this->center_of_Body + (this->center_of_Body*(12.0f / 14.0f)));
	Shape* head = new Cube(center_of_head, (center_of_head + ML::Vec3(-length_of_hand / 2.0f, -length_of_hand / 2.0f, -length_of_hand * (3.0f / 8.0f))), length_of_hand, length_of_hand, length_of_hand*(3.0f / 4.0f));
	//首の初期化
	ML::Vec3 center_of_neck = ML::Vec3(this->center_of_Body + (this->center_of_Body*(10.0f / 14.0f)));
	Joint* tmpneck = new Joint(center_of_neck, ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), head);
	//上半身の初期化
	ML::Vec3 center_of_upper_body = this->center_of_Body + (this->center_of_Body *(5.0f / 14.0f));
	Shape* upper_Body = new Cube(center_of_upper_body, (center_of_upper_body + ML::Vec3(-length_of_hand * (3.0f / 4.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 4.0f), length_of_hand, length_of_hand / 2.0f);
	//腰の初期化
	ML::Vec3 center_of_waist = (this->center_of_Body + (this->center_of_Body*(1.0f / 14.0f)));
	Joint* Waist = new Joint(center_of_waist, ML::ToRadian(-45), ML::ToRadian(170), ML::ToRadian(-100), ML::ToRadian(100), ML::ToRadian(-80), ML::ToRadian(80), upper_Body);

	//両足
	//左足
	ML::Vec3 left_center_of_Thigh = this->center_of_Body + ML::Vec3(-length_of_hand, -length_of_hand, 0);
	Shape* left_Thigh = new Cube(left_center_of_Thigh, (left_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//左お尻の初期化
	ML::Vec3 center_of_left_hip = this->center_of_Body + ML::Vec3(-length_of_hand, 0, 0);
	Joint* left_hip = new Joint(center_of_left_hip, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-90), ML::ToRadian(70), ML::ToRadian(-80), ML::ToRadian(40), left_Thigh);
	//膝から足首まで
	ML::Vec3 left_center_of_shin = left_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* left_shin = new Cube(left_center_of_shin, (left_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//膝の初期化
	ML::Vec3 left_center_of_knee = center_of_left_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* left_knee = new Joint(left_center_of_knee, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0), left_shin);
	//足元の初期化
	ML::Vec3 left_center_of_foot = left_center_of_shin + ML::Vec3(0, length_of_hand*(4.0f / 3.0f), length_of_hand / 2.0f);
	Shape* left_foot = new Cube(left_center_of_foot, left_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//足首の初期化
	ML::Vec3 left_center_of_ankle = left_center_of_knee + ML::Vec3(0, length_of_hand*2.0f, 0);
	Joint* left_ankle = new Joint(left_center_of_ankle, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(10), left_foot);

	//右足
	ML::Vec3 right_center_of_Thigh = this->center_of_Body + ML::Vec3(length_of_hand, -length_of_hand, 0);
	Shape* right_Thigh = new Cube(right_center_of_Thigh, (right_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//左お尻の初期化
	ML::Vec3 center_of_right_hip = this->center_of_Body + ML::Vec3(length_of_hand, 0, 0);
	Joint* right_hip = new Joint(center_of_right_hip, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-70), ML::ToRadian(90), ML::ToRadian(-40), ML::ToRadian(80), right_Thigh);
	//膝から足首まで
	ML::Vec3 right_center_of_shin = right_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* right_shin = new Cube(right_center_of_shin, (right_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//膝の初期化
	ML::Vec3 right_center_of_knee = center_of_right_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* right_knee = new Joint(right_center_of_knee, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0), right_shin);
	//足元の初期化
	ML::Vec3 right_center_of_foot = right_center_of_shin + ML::Vec3(0, length_of_hand*(4.0f / 3.0f), length_of_hand / 2.0f);
	Shape* right_foot = new Cube(right_center_of_foot, right_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//足首の初期化
	ML::Vec3 right_center_of_ankle = right_center_of_knee + ML::Vec3(0, length_of_hand*2.0f, 0);
	Joint* right_ankle = new Joint(right_center_of_ankle, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(-10), ML::ToRadian(0), right_foot);


	//両手
	//左手
	//上腕の初期化
	ML::Vec3 left_center_of_arm = center_of_neck + ML::Vec3(-length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* left_arm = new Cube(left_center_of_arm, left_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肩の初期化
	ML::Vec3 left_center_of_sholder = center_of_neck + ML::Vec3(-length_of_hand, 0, 0);
	Joint* left_sholder = new Joint(left_center_of_sholder, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-180), ML::ToRadian(60), left_arm);
	//前腕の初期化
	ML::Vec3 left_center_of_forearm = left_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_forearm = new Cube(left_center_of_forearm, left_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肘の初期化
	ML::Vec3 left_center_of_elbow = left_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_elbow = new Joint(left_center_of_elbow, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0), left_forearm);
	//手の初期化
	ML::Vec3 left_center_of_hand = left_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_hand = new Cube(left_center_of_hand, left_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//手首の初期化
	ML::Vec3 left_center_of_wrist = left_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_wrist = new Joint(left_center_of_wrist, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90), left_hand);

	//右手
	//上腕の初期化
	ML::Vec3 right_center_of_arm = center_of_neck + ML::Vec3(length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* right_arm = new Cube(right_center_of_arm, right_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肩の初期化
	ML::Vec3 right_center_of_sholder = center_of_neck + ML::Vec3(length_of_hand, 0, 0);
	Joint* right_sholder = new Joint(right_center_of_sholder, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-60), ML::ToRadian(180), right_arm);
	//前腕の初期化
	ML::Vec3 right_center_of_forearm = right_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_forearm = new Cube(right_center_of_forearm, right_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肘の初期化
	ML::Vec3 right_center_of_elbow = right_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_elbow = new Joint(right_center_of_elbow, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0), right_forearm);
	//手の初期化
	ML::Vec3 right_center_of_hand = right_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_hand = new Cube(right_center_of_hand, right_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//手首の初期化
	ML::Vec3 right_center_of_wrist = right_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_wrist = new Joint(right_center_of_wrist, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90), right_hand);

	//メンバーに割り当てる
	this->joint[0] = Waist;
	this->joint[1] = tmpneck;

	this->joint[2] = left_sholder;
	this->joint[3] = left_elbow;
	this->joint[4] = left_wrist;

	this->joint[5] = right_sholder;
	this->joint[6] = right_elbow;
	this->joint[7] = right_wrist;

	this->joint[8] = left_hip;
	this->joint[9] = left_knee;
	this->joint[10] = left_ankle;

	this->joint[11] = right_hip;
	this->joint[12] = right_knee;
	this->joint[13] = right_ankle;

	
	
	
	//関係性指定
	//腰からのど
	this->joint[0]->Set_Next_Joint(this->joint[1]);

	//肩から手首
	//左
	this->joint[2]->Set_Next_Joint(this->joint[3]);
	this->joint[3]->Set_Next_Joint(this->joint[4]);
	//右
	this->joint[5]->Set_Next_Joint(this->joint[6]);
	this->joint[6]->Set_Next_Joint(this->joint[7]);

	//お尻から足首
	//左
	this->joint[8]->Set_Next_Joint(this->joint[9]);
	this->joint[9]->Set_Next_Joint(this->joint[10]);
	//右
	this->joint[11]->Set_Next_Joint(this->joint[12]);
	this->joint[12]->Set_Next_Joint(this->joint[13]);

	//モーション関係変数初期化
	this->motions.clear();
	this->motionCnt = 0.0f;
	this->motion_Index = 0;
	this->now_Motion = "";
	this->repeat_Flag = false;
}


//getter
ML::Vec3 Bone::Get_Center()
{
	return this->center_of_Body;
}

void Bone::Bone_RotateY_All(const float& radian)
{
	//アフィン変換で回転行列作成
	ML::Mat4x4 matR;
	ML::QT qtY = ML::QT(ML::Vec3(0, 1, 0), radian);
	D3DXMatrixAffineTransformation(&matR, 1.0f, &this->center_of_Body, &qtY, NULL);

	//関節全体を回転させる
	this->joint[0]->Rotated_by_Prev_Joint(&matR);
	this->joint[2]->Rotated_by_Prev_Joint(&matR);
	this->joint[5]->Rotated_by_Prev_Joint(&matR);
	this->joint[8]->Rotated_by_Prev_Joint(&matR);
	this->joint[11]->Rotated_by_Prev_Joint(&matR);

}

void Bone::UpDate()
{
	//現在モーションが空でない場合
	if (this->now_Motion != "")
	{
		//持っているモーションを検索
		auto& now = this->motions.find(this->now_Motion)->second;

		//現在カウントが持続時間内なら
		if (this->motionCnt <= now[this->motion_Index].duration)
		{
			//データに従い回転を行う
			ML::Mat4x4 matR;
			//X軸クォータニオン、Y軸クォータニオン、Z軸クォータニオン、全体を合成するクォータニオン
			ML::QT qtx, qty, qtz, qtA;
			
			for (int i = 0; i < JOINT_ON_HUMAN; i++)
			{
				//クォータニオン作成
				qtx = ML::QT(ML::Vec3(1, 0, 0), (now[this->motion_Index].joint[i].x) / now[this->motion_Index].duration);
				qty = ML::QT(ML::Vec3(0, 1, 0), (now[this->motion_Index].joint[i].y) / now[this->motion_Index].duration);
				qtz = ML::QT(ML::Vec3(0, 0, 1), (now[this->motion_Index].joint[i].z) / now[this->motion_Index].duration);
				//クォータニオン合成
				qtA = qtx * qty * qtz;

				//回転行列作成
				matR.Identity();
				D3DXMatrixAffineTransformation(&matR, 1.0f, &this->joint[i]->Get_Pos(), &qtA, NULL);
				//各関節を回転
				this->joint[i]->Rotate_Bone(&matR);
			}
		}
		//カウントが遅速時間を超えた場合
		else
		{
			//一回り終わった
			if (this->motion_Index == now.size() - 1 )
			{
				//連続行動フラグが立っている場合
				if (this->repeat_Flag == true)
				{
					//インデクスとカウントをゼロにする
					this->motion_Index = 0;
					this->motionCnt = -1.0f;
				}
				//連続行動フラグが立っていない場合
				else
				{
					//モーションを空にする
					this->now_Motion == "";
					this->motion_Index = 0;
				}
			}
			//つずきがある場合
			else
			{
				this->motion_Index++;
				this->motionCnt = -1.0f;
			}
		}

	}
	//空の時にスタンディングに戻せる
	else
	{
		//未実装
	}
	//モーションが空かになかに関わらずカウントは進む
	this->motionCnt += 1.0f;
	//連続行動フラグは毎回falseに戻す(一回りしてすぐ止まるように)
	this->repeat_Flag = false;
}

void Bone::Repeat_Now_Motioin()
{
	this->repeat_Flag = true;
}