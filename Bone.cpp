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


	// TODO 後で直す
	//頭の初期化
	ML::Vec3 center_of_head = ML::Vec3(this->center_of_Body + (this->center_of_Body*(12.0f / 14.0f)));
	Shape* head = new Cube(center_of_head, (center_of_head + ML::Vec3(-length_of_hand / 2.0f, -length_of_hand / 2.0f, -length_of_hand * (3.0f / 8.0f))), length_of_hand, length_of_hand, length_of_hand*(3.0f / 4.0f));
	//首の初期化
	ML::Vec3 center_of_neck = ML::Vec3(this->center_of_Body + (this->center_of_Body*(10.0f / 14.0f)));
	Joint* tmpneck = new Joint(center_of_neck, ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), head, owner +"/Head");
	//上半身の初期化
	ML::Vec3 center_of_upper_body = this->center_of_Body + (this->center_of_Body *(5.0f / 14.0f) + ML::Vec3(0,length_of_hand /3.0f,0));
	Shape* upper_Body = new Cube(center_of_upper_body, (center_of_upper_body + ML::Vec3(-length_of_hand * (3.0f / 4.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 4.0f), length_of_hand, length_of_hand / 2.0f);
	//腰の初期化
	ML::Vec3 center_of_waist = (this->center_of_Body + (this->center_of_Body*(1.0f / 14.0f)));
	Joint* Waist = new Joint(center_of_waist, ML::ToRadian(-45), ML::ToRadian(170), ML::ToRadian(-100), ML::ToRadian(100), ML::ToRadian(-80), ML::ToRadian(80), upper_Body, owner + "/UpperBody");

	//両足
	//左足
	ML::Vec3 left_center_of_Thigh = this->center_of_Body + ML::Vec3(-length_of_hand/2.0f, -length_of_hand*(4.0f / 5.0f), 0);
	Shape* left_Thigh = new Cube(left_center_of_Thigh, (left_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//左お尻の初期化
	ML::Vec3 center_of_left_hip = this->center_of_Body + ML::Vec3(-length_of_hand/2.0f, length_of_hand / 5.0f, 0);
	Joint* left_hip = new Joint(center_of_left_hip, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-90), ML::ToRadian(70), ML::ToRadian(-80), ML::ToRadian(40), left_Thigh, owner + "/Left_Thigh");
	//膝から足首まで
	ML::Vec3 left_center_of_shin = left_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* left_shin = new Cube(left_center_of_shin, (left_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//膝の初期化
	ML::Vec3 left_center_of_knee = center_of_left_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* left_knee = new Joint(left_center_of_knee, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0), left_shin, owner + "/Left_Shin");
	//足元の初期化
	//ML::Vec3 left_center_of_foot = left_center_of_shin + ML::Vec3(0, -length_of_hand * (4.0f / 3.0f), length_of_hand / 2.0f);
	ML::Vec3 left_center_of_foot = left_center_of_shin + ML::Vec3(0, -length_of_hand * (4.0f / 3.0f), 0);
	Shape* left_foot = new Cube(left_center_of_foot, left_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//足首の初期化
	ML::Vec3 left_center_of_ankle = left_center_of_knee + ML::Vec3(0, -length_of_hand*2.0f, 0);
	Joint* left_ankle = new Joint(left_center_of_ankle, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(10), left_foot, owner + "/Left_Foot");

	//右足
	ML::Vec3 right_center_of_Thigh = this->center_of_Body + ML::Vec3(length_of_hand/2.0f, -length_of_hand*(4.0f/5.0f), 0);
	Shape* right_Thigh = new Cube(right_center_of_Thigh, (right_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//左お尻の初期化
	ML::Vec3 center_of_right_hip = this->center_of_Body + ML::Vec3(length_of_hand/2.0f, length_of_hand/5.0f, 0);
	Joint* right_hip = new Joint(center_of_right_hip, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-70), ML::ToRadian(90), ML::ToRadian(-40), ML::ToRadian(80), right_Thigh, owner + "/Right_Thigh");
	//膝から足首まで
	ML::Vec3 right_center_of_shin = right_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* right_shin = new Cube(right_center_of_shin, (right_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//膝の初期化
	ML::Vec3 right_center_of_knee = center_of_right_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* right_knee = new Joint(right_center_of_knee, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0), right_shin, owner + "/Right_Shin");
	//足元の初期化
	//ML::Vec3 right_center_of_foot = right_center_of_shin + ML::Vec3(0, -length_of_hand*(4.0f / 3.0f), length_of_hand / 2.0f);
	ML::Vec3 right_center_of_foot = right_center_of_shin + ML::Vec3(0, -length_of_hand * (4.0f / 3.0f), 0);
	Shape* right_foot = new Cube(right_center_of_foot, right_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//足首の初期化
	ML::Vec3 right_center_of_ankle = right_center_of_knee + ML::Vec3(0, -length_of_hand*2.0f, 0);
	Joint* right_ankle = new Joint(right_center_of_ankle, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(-10), ML::ToRadian(0), right_foot, owner + "/Right_Foot");


	//両手
	//左手
	//上腕の初期化
	ML::Vec3 left_center_of_arm = center_of_neck + ML::Vec3(-length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* left_arm = new Cube(left_center_of_arm, left_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肩の初期化
	ML::Vec3 left_center_of_sholder = center_of_neck + ML::Vec3(-length_of_hand, 0, 0);
	Joint* left_sholder = new Joint(left_center_of_sholder, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-180), ML::ToRadian(60), left_arm, owner + "/Left_Arm");
	//前腕の初期化
	ML::Vec3 left_center_of_forearm = left_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_forearm = new Cube(left_center_of_forearm, left_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肘の初期化
	ML::Vec3 left_center_of_elbow = left_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_elbow = new Joint(left_center_of_elbow, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0), left_forearm, owner + "/Left_ForeArm");
	//手の初期化
	ML::Vec3 left_center_of_hand = left_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_hand = new Cube(left_center_of_hand, left_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//手首の初期化
	ML::Vec3 left_center_of_wrist = left_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_wrist = new Joint(left_center_of_wrist, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90), left_hand, owner + "/Left_Hand");

	//右手
	//上腕の初期化
	ML::Vec3 right_center_of_arm = center_of_neck + ML::Vec3(length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* right_arm = new Cube(right_center_of_arm, right_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肩の初期化
	ML::Vec3 right_center_of_sholder = center_of_neck + ML::Vec3(length_of_hand, 0, 0);
	Joint* right_sholder = new Joint(right_center_of_sholder, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-60), ML::ToRadian(180), right_arm, owner + "/Right_Arm");
	//前腕の初期化
	ML::Vec3 right_center_of_forearm = right_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_forearm = new Cube(right_center_of_forearm, right_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//肘の初期化
	ML::Vec3 right_center_of_elbow = right_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_elbow = new Joint(right_center_of_elbow, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0), right_forearm, owner + "/Right_ForeArm");
	//手の初期化
	ML::Vec3 right_center_of_hand = right_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_hand = new Cube(right_center_of_hand, right_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//手首の初期化
	ML::Vec3 right_center_of_wrist = right_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_wrist = new Joint(right_center_of_wrist, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90), right_hand, owner + "/Right_Hand");

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
	//腰から両肩
	this->joint[0]->Set_Next_Joint(this->joint[2]);
	this->joint[0]->Set_Next_Joint(this->joint[5]);

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
	this->motionCnt = 0;
	this->motion_Index = 0;
	this->now_Motion = "";
	this->next_Motion = "";
	this->repeat_Flag = false;

}


//getter
ML::Vec3 Bone::Get_Center()
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
		this->joint[0]->Rotated_by_Prev_Joint(&matR,qtY);
		/*this->joint[2]->Rotated_by_Prev_Joint(&matR, qtY);
		this->joint[5]->Rotated_by_Prev_Joint(&matR, qtY);*/
		this->joint[8]->Rotated_by_Prev_Joint(&matR, qtY);
		this->joint[11]->Rotated_by_Prev_Joint(&matR, qtY);

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
			if (this->motion_Index >= now.size() - 1 )
			{
				//連続行動フラグが立っている場合
				if (this->repeat_Flag == true && this->next_Motion == "")
				{
					//repeat Pointを探す
					int repeat_Index = 0;
					for (int n =0; n<now.size();n++)
					{
						if (now[n].repeat_Point)
						{
							repeat_Index = n;
							//repeat Pointを代入
							this->motion_Index = repeat_Index;
							//カウントをゼロにする
							this->motionCnt = 0;
						}
					}					
					//repeat Pointを探せなかったら次のモーションを探す
					if (repeat_Index == 0)
					{
						this->Next_Motion_or_None();
					}
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
		//カウントは-1に
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

//全軸に対して整頓されないバグがある(2018/07/19)
void Bone::To_Standing(bool ASAP)
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
		float sx = 0.0f, sy = 0.0f;

		ML::Vec3 bone_Vec = this->joint[i]->Get_To_Bone().Normalize();
		
		//ボーン情報と外積でsin値を取る
		MyMath::Vector_Cross(&sx, bone_Vec, standing_Vec);

		//回転軸宣言
		ML::Vec3 anker;
		MyMath::Get_Normal_to_Vector_Cross(&anker, bone_Vec, standing_Vec);
		anker = anker.Normalize();
		//もし回転軸がゼロベクトルなら次に移る
		if (anker.Is_Zero_Vec() || ML::ToDegree(asin(sx)) < 0.5f)
		{
			continue;
		}
		//関節一個ずつ戻せた後に次の関節を整頓
		if (abs(sx) <= abs(sinf(ML::ToRadian(2))))
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

void Bone::Render()
{
	//関節全体にレンダリング命令する
	for (int i = 0; i < JOINT_ON_HUMAN; i++)
	{
		this->joint[i]->Render(this->tall);
	}
}

void Bone::Registrate_Motion(const std::vector<Motion::Motion_Data>& d, const string& motion_Name)
{
	this->motions.insert({ motion_Name,d });
}