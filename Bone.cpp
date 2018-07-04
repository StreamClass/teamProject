#include "Bone.h"
#include "Cube.h"
#include "MyMath.h"


//ƒRƒ“ƒXƒgƒ‰ƒNƒ^

Bone::Bone(const float& tall)
	: tall(tall)
{
	//’†SŽw’è
	this->center_of_Body = ML::Vec3(0, tall / 2.0f, 0);
	//”ä—¦‚ÌŠî€‚É‚È‚éŽè‚Ì’·‚³
	float length_of_hand = (tall / 2.0f)*(4.0f / 14.0f);


	//“ª‚Ì‰Šú‰»
	ML::Vec3 center_of_head = ML::Vec3(this->center_of_Body + (this->center_of_Body*(12.0f / 14.0f)));
	Shape* head = new Cube(center_of_head, (center_of_head + ML::Vec3(-length_of_hand / 2.0f, -length_of_hand / 2.0f, -length_of_hand * (3.0f / 8.0f))), length_of_hand, length_of_hand, length_of_hand*(3.0f / 4.0f));
	//Žñ‚Ì‰Šú‰»
	ML::Vec3 center_of_neck = ML::Vec3(this->center_of_Body + (this->center_of_Body*(10.0f / 14.0f)));
	Joint* tmpneck = new Joint(center_of_neck, ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), head);
	//ã”¼g‚Ì‰Šú‰»
	ML::Vec3 center_of_upper_body = this->center_of_Body + (this->center_of_Body *(5.0f / 14.0f));
	Shape* upper_Body = new Cube(center_of_upper_body, (center_of_upper_body + ML::Vec3(-length_of_hand * (3.0f / 4.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 4.0f), length_of_hand, length_of_hand / 2.0f);
	//˜‚Ì‰Šú‰»
	ML::Vec3 center_of_waist = (this->center_of_Body + (this->center_of_Body*(1.0f / 14.0f)));
	Joint* Waist = new Joint(center_of_waist, ML::ToRadian(-45), ML::ToRadian(170), ML::ToRadian(-100), ML::ToRadian(100), ML::ToRadian(-80), ML::ToRadian(80), upper_Body);

	//—¼‘«
	//¶‘«
	ML::Vec3 left_center_of_Thigh = this->center_of_Body + ML::Vec3(-length_of_hand, -length_of_hand, 0);
	Shape* left_Thigh = new Cube(left_center_of_Thigh, (left_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//¶‚¨K‚Ì‰Šú‰»
	ML::Vec3 center_of_left_hip = this->center_of_Body + ML::Vec3(-length_of_hand, 0, 0);
	Joint* left_hip = new Joint(center_of_left_hip, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-90), ML::ToRadian(70), ML::ToRadian(-80), ML::ToRadian(40), left_Thigh);
	//•G‚©‚ç‘«Žñ‚Ü‚Å
	ML::Vec3 left_center_of_shin = left_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* left_shin = new Cube(left_center_of_shin, (left_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//•G‚Ì‰Šú‰»
	ML::Vec3 left_center_of_knee = center_of_left_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* left_knee = new Joint(left_center_of_knee, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0), left_shin);
	//‘«Œ³‚Ì‰Šú‰»
	ML::Vec3 left_center_of_foot = left_center_of_shin + ML::Vec3(0, length_of_hand*(4.0f / 3.0f), length_of_hand / 2.0f);
	Shape* left_foot = new Cube(left_center_of_foot, left_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//‘«Žñ‚Ì‰Šú‰»
	ML::Vec3 left_center_of_ankle = left_center_of_knee + ML::Vec3(0, length_of_hand*2.0f, 0);
	Joint* left_ankle = new Joint(left_center_of_ankle, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(10), left_foot);

	//‰E‘«
	ML::Vec3 right_center_of_Thigh = this->center_of_Body + ML::Vec3(length_of_hand, -length_of_hand, 0);
	Shape* right_Thigh = new Cube(right_center_of_Thigh, (right_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//¶‚¨K‚Ì‰Šú‰»
	ML::Vec3 center_of_right_hip = this->center_of_Body + ML::Vec3(length_of_hand, 0, 0);
	Joint* right_hip = new Joint(center_of_right_hip, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-70), ML::ToRadian(90), ML::ToRadian(-40), ML::ToRadian(80), right_Thigh);
	//•G‚©‚ç‘«Žñ‚Ü‚Å
	ML::Vec3 right_center_of_shin = right_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* right_shin = new Cube(right_center_of_shin, (right_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//•G‚Ì‰Šú‰»
	ML::Vec3 right_center_of_knee = center_of_right_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* right_knee = new Joint(right_center_of_knee, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0), right_shin);
	//‘«Œ³‚Ì‰Šú‰»
	ML::Vec3 right_center_of_foot = right_center_of_shin + ML::Vec3(0, length_of_hand*(4.0f / 3.0f), length_of_hand / 2.0f);
	Shape* right_foot = new Cube(right_center_of_foot, right_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//‘«Žñ‚Ì‰Šú‰»
	ML::Vec3 right_center_of_ankle = right_center_of_knee + ML::Vec3(0, length_of_hand*2.0f, 0);
	Joint* right_ankle = new Joint(right_center_of_ankle, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(-10), ML::ToRadian(0), right_foot);


	//—¼Žè
	//¶Žè
	//ã˜r‚Ì‰Šú‰»
	ML::Vec3 left_center_of_arm = center_of_neck + ML::Vec3(-length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* left_arm = new Cube(left_center_of_arm, left_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//Œ¨‚Ì‰Šú‰»
	ML::Vec3 left_center_of_sholder = center_of_neck + ML::Vec3(-length_of_hand, 0, 0);
	Joint* left_sholder = new Joint(left_center_of_sholder, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-180), ML::ToRadian(60), left_arm);
	//‘O˜r‚Ì‰Šú‰»
	ML::Vec3 left_center_of_forearm = left_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_forearm = new Cube(left_center_of_forearm, left_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//•I‚Ì‰Šú‰»
	ML::Vec3 left_center_of_elbow = left_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_elbow = new Joint(left_center_of_elbow, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0), left_forearm);
	//Žè‚Ì‰Šú‰»
	ML::Vec3 left_center_of_hand = left_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_hand = new Cube(left_center_of_hand, left_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//ŽèŽñ‚Ì‰Šú‰»
	ML::Vec3 left_center_of_wrist = left_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_wrist = new Joint(left_center_of_wrist, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90), left_hand);

	//‰EŽè
	//ã˜r‚Ì‰Šú‰»
	ML::Vec3 right_center_of_arm = center_of_neck + ML::Vec3(length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* right_arm = new Cube(right_center_of_arm, right_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//Œ¨‚Ì‰Šú‰»
	ML::Vec3 right_center_of_sholder = center_of_neck + ML::Vec3(length_of_hand, 0, 0);
	Joint* right_sholder = new Joint(right_center_of_sholder, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-60), ML::ToRadian(180), right_arm);
	//‘O˜r‚Ì‰Šú‰»
	ML::Vec3 right_center_of_forearm = right_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_forearm = new Cube(right_center_of_forearm, right_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//•I‚Ì‰Šú‰»
	ML::Vec3 right_center_of_elbow = right_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_elbow = new Joint(right_center_of_elbow, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0), right_forearm);
	//Žè‚Ì‰Šú‰»
	ML::Vec3 right_center_of_hand = right_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_hand = new Cube(right_center_of_hand, right_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//ŽèŽñ‚Ì‰Šú‰»
	ML::Vec3 right_center_of_wrist = right_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_wrist = new Joint(right_center_of_wrist, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90), right_hand);

	//ƒƒ“ƒo[‚ÉŠ„‚è“–‚Ä‚é
	this->waist = Waist;
	this->neck = tmpneck;
	this->left_Sholder = left_sholder;
	this->left_Elbow = left_elbow;
	this->left_Wrist = left_wrist;
	this->left_Hip = left_hip;
	this->left_Knee = left_knee;
	this->left_Ankle = left_ankle;
	this->right_Sholder = right_sholder;
	this->right_Elbow = right_elbow;
	this->right_Wrist = right_wrist;
	this->right_Hip = right_hip;
	this->right_Knee = right_knee;
	this->right_Ankle = right_ankle;

	//ŠÖŒW«Žw’è
	//˜‚©‚ç‚Ì‚Ç
	this->waist->Set_Next_Joint(this->neck);

	//Œ¨‚©‚çŽèŽñ
	//¶
	this->left_Sholder->Set_Next_Joint(this->left_Elbow);
	this->left_Elbow->Set_Next_Joint(this->left_Wrist);
	//‰E
	this->right_Sholder->Set_Next_Joint(this->right_Elbow);
	this->right_Elbow->Set_Next_Joint(this->right_Wrist);

	//‚¨K‚©‚ç‘«Žñ
	//¶
	this->left_Hip->Set_Next_Joint(this->left_Knee);
	this->left_Knee->Set_Next_Joint(this->left_Ankle);
	//‰E
	this->right_Hip->Set_Next_Joint(this->right_Knee);
	this->right_Knee->Set_Next_Joint(this->right_Ankle);

	//ƒ‚[ƒVƒ‡ƒ“ŠÖŒW•Ï”‰Šú‰»
	this->motions.clear();
	this->motionCnt = 0.0f;
	this->motion_Index = 0;
	this->now_Motion = "";
}

ML::Vec3 Bone::Get_Center()
{
	return this->center_of_Body;
}