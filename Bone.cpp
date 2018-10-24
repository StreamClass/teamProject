#include "Bone.h"
#include "Cube.h"
#include "MyMath.h"

//コンストラクタ

Bone::Bone(const float& tall, const string& owner)
	: tall(tall)
{
	//中心指定
	this->center_of_Body = ML::Vec3(0, tall / 2.0f, 0);
	//回転値初期化
	this->All_RotY = 0.0f;
	//正面ベクトル初期化
	this->front_Vec = ML::Vec3(0, 0, 1);
	//比率の基準になる手の長さ
	float length_of_hand = (tall / 2.0f)*(4.0f / 14.0f);


	//頭の初期化
	ML::Vec3 center_of_head = ML::Vec3(this->center_of_Body + (this->center_of_Body*(12.0f / 14.0f)));
	Shape* head = new Cube(center_of_head, (center_of_head + ML::Vec3(-length_of_hand / 2.0f, -length_of_hand / 2.0f, -length_of_hand * (3.0f / 8.0f))), length_of_hand, length_of_hand, length_of_hand*(3.0f / 4.0f));
	//首の初期化
	ML::Vec3 center_of_neck = ML::Vec3(this->center_of_Body + (this->center_of_Body*(10.0f / 14.0f)));
	Joint* tmpneck = new Joint(center_of_neck, head, owner +"/Head");
	//, ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90)
	//上半身の初期化
	ML::Vec3 center_of_upper_body = this->center_of_Body + (this->center_of_Body *(5.0f / 14.0f) + ML::Vec3(0,length_of_hand /3.0f,0));
	Shape* upper_Body = new Cube(center_of_upper_body, (center_of_upper_body + ML::Vec3(-length_of_hand * (3.0f / 4.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 4.0f), length_of_hand, length_of_hand / 2.0f);
	//腰の初期化
	ML::Vec3 center_of_waist = (this->center_of_Body + (this->center_of_Body*(1.0f / 14.0f)));
	Joint* Waist = new Joint(center_of_waist, upper_Body, owner + "/UpperBody");
	//, ML::ToRadian(-45), ML::ToRadian(170), ML::ToRadian(-100), ML::ToRadian(100), ML::ToRadian(-80), ML::ToRadian(80)

	//両足
	//左足
	ML::Vec3 left_center_of_Thigh = this->center_of_Body + ML::Vec3(-length_of_hand/2.0f, -length_of_hand*(4.0f / 5.0f), 0);
	Shape* left_Thigh = new Cube(left_center_of_Thigh, (left_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//左お尻の初期化
	ML::Vec3 center_of_left_hip = this->center_of_Body + ML::Vec3(-length_of_hand/2.0f, length_of_hand / 5.0f, 0);
	Joint* left_hip = new Joint(center_of_left_hip, left_Thigh, owner + "/Left_Thigh");
	//, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-90), ML::ToRadian(70), ML::ToRadian(-80), ML::ToRadian(40)
	//膝から足首まで
	ML::Vec3 left_center_of_shin = left_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* left_shin = new Cube(left_center_of_shin, (left_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//膝の初期化
	ML::Vec3 left_center_of_knee = center_of_left_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* left_knee = new Joint(left_center_of_knee, left_shin, owner + "/Left_Shin");
	//, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0)
	//足元の初期化
	//ML::Vec3 left_center_of_foot = left_center_of_shin + ML::Vec3(0, -length_of_hand * (4.0f / 3.0f), length_of_hand / 2.0f);
	ML::Vec3 left_center_of_foot = left_center_of_shin + ML::Vec3(0, -length_of_hand * (4.0f / 3.0f), 0);
	Shape* left_foot = new Cube(left_center_of_foot, left_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//足首の初期化
	ML::Vec3 left_center_of_ankle = left_center_of_knee + ML::Vec3(0, -length_of_hand*2.0f, 0);
	Joint* left_ankle = new Joint(left_center_of_ankle, left_foot, owner + "/Left_Foot");
	//, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(10),

	//右足
	ML::Vec3 right_center_of_Thigh = this->center_of_Body + ML::Vec3(length_of_hand/2.0f, -length_of_hand*(4.0f/5.0f), 0);
	Shape* right_Thigh = new Cube(right_center_of_Thigh, (right_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//左お尻の初期化
	ML::Vec3 center_of_right_hip = this->center_of_Body + ML::Vec3(length_of_hand/2.0f, length_of_hand/5.0f, 0);
	Joint* right_hip = new Joint(center_of_right_hip, right_Thigh, owner + "/Right_Thigh");
	//, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-70), ML::ToRadian(90), ML::ToRadian(-40), ML::ToRadian(80)
	//膝から足首まで
	ML::Vec3 right_center_of_shin = right_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* right_shin = new Cube(right_center_of_shin, (right_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//膝の初期化
	ML::Vec3 right_center_of_knee = center_of_right_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* right_knee = new Joint(right_center_of_knee, right_shin, owner + "/Right_Shin");
	//, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0)
	//足元の初期化
	//ML::Vec3 right_center_of_foot = right_center_of_shin + ML::Vec3(0, -length_of_hand*(4.0f / 3.0f), length_of_hand / 2.0f);
	ML::Vec3 right_center_of_foot = right_center_of_shin + ML::Vec3(0, -length_of_hand * (4.0f / 3.0f), 0);
	Shape* right_foot = new Cube(right_center_of_foot, right_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//足首の初期化
	ML::Vec3 right_center_of_ankle = right_center_of_knee + ML::Vec3(0, -length_of_hand*2.0f, 0);
	Joint* right_ankle = new Joint(right_center_of_ankle, right_foot, owner + "/Right_Foot");
	//, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(-10), ML::ToRadian(0)

	//両手
	//左手
	//上腕の初期化
	ML::Vec3 left_center_of_arm = center_of_neck + ML::Vec3(-length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* left_arm = new Cube(left_center_of_arm, left_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肩の初期化
	ML::Vec3 left_center_of_sholder = center_of_neck + ML::Vec3(-length_of_hand, 0, 0);
	Joint* left_sholder = new Joint(left_center_of_sholder, left_arm, owner + "/Left_Arm");
	//, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-180), ML::ToRadian(60)
	//前腕の初期化
	ML::Vec3 left_center_of_forearm = left_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_forearm = new Cube(left_center_of_forearm, left_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肘の初期化
	ML::Vec3 left_center_of_elbow = left_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_elbow = new Joint(left_center_of_elbow, left_forearm, owner + "/Left_ForeArm");
	//, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0)
	//手の初期化
	ML::Vec3 left_center_of_hand = left_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_hand = new Cube(left_center_of_hand, left_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//手首の初期化
	ML::Vec3 left_center_of_wrist = left_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_wrist = new Joint(left_center_of_wrist, left_hand, owner + "/Left_Hand");
	//, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90)

	//右手
	//上腕の初期化
	ML::Vec3 right_center_of_arm = center_of_neck + ML::Vec3(length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* right_arm = new Cube(right_center_of_arm, right_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肩の初期化
	ML::Vec3 right_center_of_sholder = center_of_neck + ML::Vec3(length_of_hand, 0, 0);
	Joint* right_sholder = new Joint(right_center_of_sholder, right_arm, owner + "/Right_Arm");
	//, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-60), ML::ToRadian(180)
	//前腕の初期化
	ML::Vec3 right_center_of_forearm = right_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_forearm = new Cube(right_center_of_forearm, right_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肘の初期化
	ML::Vec3 right_center_of_elbow = right_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_elbow = new Joint(right_center_of_elbow, right_forearm, owner + "/Right_ForeArm");
	//, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0)
	//手の初期化
	ML::Vec3 right_center_of_hand = right_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_hand = new Cube(right_center_of_hand, right_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//手首の初期化
	ML::Vec3 right_center_of_wrist = right_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_wrist = new Joint(right_center_of_wrist, right_hand, owner + "/Right_Hand");
	//, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90)

	//メンバーに割り当てる
	this->joint[Motion::Joint_Name::Waist] = Waist;
	this->joint[Motion::Joint_Name::Neck] = tmpneck;

	this->joint[Motion::Joint_Name::Left_Sholder] = left_sholder;
	this->joint[Motion::Joint_Name::Left_Elbow] = left_elbow;
	this->joint[Motion::Joint_Name::Left_Wrist] = left_wrist;

	this->joint[Motion::Joint_Name::Right_Sholder] = right_sholder;
	this->joint[Motion::Joint_Name::Right_Elbow] = right_elbow;
	this->joint[Motion::Joint_Name::Right_Wrist] = right_wrist;

	this->joint[Motion::Joint_Name::Left_Hip] = left_hip;
	this->joint[Motion::Joint_Name::Left_Knee] = left_knee;
	this->joint[Motion::Joint_Name::Left_Ankle] = left_ankle;

	this->joint[Motion::Joint_Name::Right_Hip] = right_hip;
	this->joint[Motion::Joint_Name::Right_Knee] = right_knee;
	this->joint[Motion::Joint_Name::Right_Ankle] = right_ankle;
	
	
	//関係性指定
	//腰からのど
	this->joint[Motion::Joint_Name::Waist]->Set_Next_Joint(this->joint[Motion::Joint_Name::Neck]);
	//腰から両肩
	this->joint[Motion::Joint_Name::Waist]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Sholder]);
	this->joint[Motion::Joint_Name::Waist]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Sholder]);

	//肩から手首
	//左
	this->joint[Motion::Joint_Name::Left_Sholder]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Elbow]);
	this->joint[Motion::Joint_Name::Left_Elbow]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Wrist]);
	//右
	this->joint[Motion::Joint_Name::Right_Sholder]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Elbow]);
	this->joint[Motion::Joint_Name::Right_Elbow]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Wrist]);

	//お尻から足首
	//左
	this->joint[Motion::Joint_Name::Left_Hip]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Knee]);
	this->joint[Motion::Joint_Name::Left_Knee]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Ankle]);
	//右
	this->joint[Motion::Joint_Name::Right_Hip]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Knee]);
	this->joint[Motion::Joint_Name::Right_Knee]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Ankle]);

	//モーション関係変数初期化
	this->motions.clear();
	this->motionCnt = 0;
	this->motion_Index = 0;
	this->repeat_Index = 0;
	this->now_Motion = "";
	this->next_Motion = "";
	this->repeat_Flag = false;

}


//getter
ML::Vec3 Bone::Get_Center() const
{
	return this->center_of_Body;
}

void Bone::Bone_RotateY_All(const float& radian)
{
	//全体回転値との差分だけ回転する
	float def = this->All_RotY - radian;
	if (def != 0.0f)
	{
		//アフィン変換で回転行列作成
		ML::Mat4x4 matR;		
		ML::QT qtY = ML::QT(ML::Vec3(0, 1, 0), -def);
		//this->All_Qt *= ML::QT(ML::Vec3(0, 1, 0), radian);
		D3DXMatrixAffineTransformation(&matR, 1.0f, &this->center_of_Body, &qtY, NULL);		
		

		//正面ベクトル回転
		this->front_Vec = matR.TransformNormal(this->front_Vec);
		//関節全体を回転させる
		this->joint[Motion::Joint_Name::Waist]->Rotated_by_Prev_Joint(&matR,qtY);
		/*this->joint[2]->Rotated_by_Prev_Joint(&matR, qtY);
		this->joint[5]->Rotated_by_Prev_Joint(&matR, qtY);*/
		this->joint[Motion::Joint_Name::Left_Hip]->Rotated_by_Prev_Joint(&matR, qtY);
		this->joint[Motion::Joint_Name::Right_Hip]->Rotated_by_Prev_Joint(&matR, qtY);

	}
	
	this->All_RotY = radian;
	
}

void Bone::Moving(const ML::Vec3& vec)
{
	//全体中心及び各ボーンの座標を移動させる
	this->center_of_Body += vec;

	for (int i = 0; i < JOINT_ON_HUMAN; i++)
	{
		this->joint[i]->Move(vec);
	}
}

void Bone::UpDate()
{
	//現在モーションが空でない場合
	if (this->now_Motion != "")
	{
		//持っているモーションを検索
		auto& now = this->motions.find(this->now_Motion)->second;		

		//連続行動ポイントならそのインデックスを保存
		if (now[this->motion_Index].repeat_Point)
		{
			this->repeat_Index = this->motion_Index;
		}

		//現在カウントが持続時間内なら
		if (this->motionCnt <= now[this->motion_Index].duration)
		{
			//データに従い回転を行う
			ML::Mat4x4 matR;
			//X軸クォータニオン、Y軸クォータニオン、Z軸クォータニオン、全体を合成するクォータニオン
			ML::QT qtx, qty, qtz, qtA;
			//モーションデータで回転を行う回転軸
			ML::Vec3 ankerX, ankerY, ankerZ;
			ankerX = ML::Vec3(1, 0, 0);
			ankerY = ML::Vec3(0, 1, 0);
			ankerZ = ML::Vec3(0, 0, 1);
			//全体Y軸回転に対応するアンカー回転
			ML::Mat4x4 matY;
			matY.RotationY(this->All_RotY);
			ankerX = matY.TransformCoord(ankerX);
			ankerY = matY.TransformCoord(ankerY);
			ankerZ = matY.TransformCoord(ankerZ);
			//モーション開始
			for (int i = 0; i < JOINT_ON_HUMAN; i++)
			{
				//無効のデータなら次へ
				if (now[this->motion_Index].joint[i].Is_Zero_Vec())
				{
					continue;
				}				

				//クォータニオン作成
				qtx = ML::QT(ankerX, ML::ToRadian((now[this->motion_Index].joint[i].x) / now[this->motion_Index].duration));
				qty = ML::QT(ankerY, ML::ToRadian((now[this->motion_Index].joint[i].y) / now[this->motion_Index].duration));
				qtz = ML::QT(ankerZ, ML::ToRadian((now[this->motion_Index].joint[i].z) / now[this->motion_Index].duration));
				//クォータニオン合成
				qtA = qtx * qty * qtz;
				
				//回転行列作成
				matR.Identity();
				D3DXMatrixAffineTransformation(&matR, 1.0f, &this->joint[i]->Get_Pos(), &qtA, NULL);
				//各関節を回転
				this->joint[i]->Rotate_Bone(&matR, qtA);
			}
		}
		//カウントが遅速時間を超えた場合
		else
		{
			//一回り終わった
			if (this->motion_Index >= now.size() - 1)
			{
				//連続行動をする場合
				if (this->Is_Ok_to_Repeat())
				{
					//現在インデックスを探しておいたインデックスに上書きする
					this->motion_Index = this->repeat_Index;
					this->motionCnt = 0;
				}
				else
				{
					//次のモーションか空か確認する
					this->Next_Motion_or_None();
				}
			}
			//つずきがある場合
			else
			{
				this->motion_Index++;
				this->motionCnt = 0;
			}			
		}

	}
	//空の時
	else
	{
		//連続行動インデックスを初期値にする
		this->repeat_Index = 0;
		//次のモーションを探す
		if (!this->Next_Motion_or_None())
		{
			//次のモーションが空の場合はゆっくりスタンディングへ
			this->To_Standing(false);
		}
	}


	//モーションが空かになかに関わらずカウントは進む
	this->motionCnt++;
	//連続行動フラグは毎回falseに戻す(一回りしてすぐ止まるように)
	this->repeat_Flag = false;
}

bool Bone::Next_Motion_or_None()
{
	//予約モーションがある場合現在モーションに置き換える
	if (this->next_Motion != "")
	{
		this->now_Motion = this->next_Motion;
		//予約は空にする
		this->next_Motion = "";
		//カウントは0に
		this->motionCnt = 0;
		this->motion_Index = 0;
		//次のモーションが入る前にスタンディングに戻せる		
		this->To_Standing(true);
		return true;
	}
	else
	{
		//モーションを空にする
		this->now_Motion = "";
		this->motion_Index = 0;
		return false;
	}
}

void Bone::Set_Next_Motion(const string& next)
{
	//同じモーションを繰り返す条件は
	//Repeat_Now_Motion()を通じてのみにする(2018/07/10)

	//念のために登録されてるものかを確認
	if (this->motions.count(next) >= 1)
	{
		//今のモーションと違うモーションなら
		if (this->now_Motion != next)
		{
			//次のモーションを予約
			this->next_Motion = next;
		}
	}
}

void Bone::Repeat_Now_Motioin()
{
	this->repeat_Flag = true;
}

bool Bone::Is_Ok_to_Repeat()
{
	return (this->repeat_Flag == true && this->next_Motion == "" && repeat_Index != 0) ? true : false;
}

//全軸に対して整頓されないバグがある(2018/07/19)
void Bone::To_Standing(bool ASAP) const
{
	for (int i = 0; i < JOINT_ON_HUMAN; i++)
	{
		ML::Vec3 standing_Vec;
		//スタンディングの時に関節から骨までベクトルを宣言
		if (i < 2)
		{
			//Y+(腰と首)
			standing_Vec = ML::Vec3(0, 1, 0);
		}
		else
		{
			//Y-(その他)
			standing_Vec = ML::Vec3(0, -1, 0);
		}
		float sx = 0.0f;

		ML::Vec3 bone_Vec = this->joint[i]->Get_To_Bone().Normalize();
		
		//ボーン情報と外積でsin値を取る
		MyMath::Vector_Cross(&sx, bone_Vec, standing_Vec);

		//回転軸宣言
		ML::Vec3 anker;
		MyMath::Get_Normal_to_Vector_Cross(&anker, bone_Vec, standing_Vec);	
		//もし回転軸がゼロベクトルなら次に移る
		if (anker.Is_Zero_Vec())
		{
			continue;
		}

		//anker = anker.Normalize();
		
		//関節一個ずつ戻せた後に次の関節を整頓
		if (abs(sx) <= abs(sinf(ML::ToRadian(1))))
		{
			continue;
		}
		else if (abs(sx) <= abs(sinf(ML::ToRadian(3))))
		{
			//直立になる回転を代入					
			//クォータニオン宣言
			ML::QT remainX = ML::QT(anker, asin(sx));
			//回転行列宣言
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, 1.0f, &this->joint[i]->Get_Pos(), &remainX, NULL);
			//回転
			this->joint[i]->Rotate_Bone(&matR, remainX);
			continue;
		}
		else
		{			
			//残り回転量の半分ずつ回転を巻き返す
			//フラグが立っていれば1フレームで済ませる
			float progress = 0.0f;
			ASAP ? progress = 1.0f : progress = 2.0f;
			//クォータニオン宣言
			ML::QT remainX = ML::QT(anker, asin(sx) / progress);
			//回転行列宣言
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, 1.0f, &this->joint[i]->Get_Pos(), &remainX, NULL);
			//回転
			this->joint[i]->Rotate_Bone(&matR, remainX);			
			if (!ASAP)
			{
				return;
			}
		}
	}
}

void Bone::Render() const
{
	//関節全体にレンダリング命令する
	for (int i = 0; i < JOINT_ON_HUMAN; i++)
	{
		this->joint[i]->Render(this->tall);
	}
}

void Bone::Registrate_Motion(const std::vector<Motion::Motion_Data>& d, const string& motion_Name)
{
	//無効のモーションデータかを確認した上に登録する
	if (d.size() == 0)
	{
		return;
	}
	this->motions.insert({ motion_Name,d });
}