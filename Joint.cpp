#include "Joint.h"

//ˆÚ“®

void Joint::Move(const ML::Vec3& vec)
{
	this->pos += vec;
}

void Joint::Set_Next_Joint(Joint* next)
{
	this->next = next;
}

void Joint::Rotate_Bone(ML::Mat4x4* matR)
{
	//Ž©•ª‚Æ‚Â‚È‚ª‚Á‚Ä‚¢‚éœ‚ð‰ñ“]
	this->area->Rotate(matR);
	//‚Â‚È‚ª‚è‚ðŽ‚Á‚Ä‚¢‚éŠÔÚ‚È‚çŽŸ‚Ì‚â‚Â‚à‰ñ“]‚³‚¹‚é
	if (this->next != nullptr)
	{
		next->Rotated_by_Prev_Joint(matR);
	}
}

void Joint::Rotated_by_Prev_Joint(ML::Mat4x4* matR)
{
	//‰ñ“]‰ž‚¶‚ÄˆÊ’u‚ðXV
	this->pos = matR->TransformCoord(this->pos);

	//‚»‚ÌŽŸ‚É‚Â‚È‚ª‚Á‚Ä‚¢‚é‚à‚Ì‚ð‰ñ“]	
	this->Rotate_Bone(matR);	
}

//getter

//ŠÖß‚ÌŒ»ÝˆÊ’u‚ð•Ô‚·(â‘ÎÀ•W)
ML::Vec3 Joint::Get_Pos()
{
	return this->pos;
}

//Še‰ñ“]—Ê‚ÌŒÀŠE’l‚ð•Ô‚·
//X
float Joint::Get_Limit_X_Minus()
{
	return this->limit_X_Minus;
}

float Joint::Get_Limit_X_Plus()
{
	return this->limit_X_Plus;
}

//Y
float Joint::Get_Limit_Y_Minus()
{
	return this->limit_Y_Minus;
}

float Joint::Get_Limit_Y_Plus()
{
	return this->limit_Y_Plus;
}

//Z
float Joint::Get_Limit_Z_Minus()
{
	return this->limit_Z_Minus;
}

float Joint::Get_Limit_Z_Plus()
{
	return this->limit_Z_Plus;
}

ML::Vec3 Joint::Get_To_Bone()
{
	return (this->area->GetCenter() - this->pos);
}