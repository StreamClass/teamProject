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
	//©•ª‚Æ‚Â‚È‚ª‚Á‚Ä‚¢‚éœ‚ğ‰ñ“]
	this->area->Rotate(matR);
	//‚Â‚È‚ª‚è‚ğ‚Á‚Ä‚¢‚éŠÔÚ‚È‚çŸ‚Ì‚â‚Â‚à‰ñ“]‚³‚¹‚é
	if (this->next != nullptr)
	{
		next->Rotated_by_Prev_Joint(matR);
	}
}

void Joint::Rotated_by_Prev_Joint(ML::Mat4x4* matR)
{
	//‰ñ“]‰‚¶‚ÄˆÊ’u‚ğXV
	this->pos = matR->TransformCoord(this->pos);

	//‚»‚ÌŸ‚É‚Â‚È‚ª‚Á‚Ä‚¢‚é‚à‚Ì‚ğ‰ñ“]	
	this->Rotate_Bone(matR);	
}

//getter

//ŠÖß‚ÌŒ»İˆÊ’u‚ğ•Ô‚·(â‘ÎÀ•W)
ML::Vec3 Joint::Get_Pos()
{
	return this->pos;
}

//Še‰ñ“]—Ê‚ÌŒÀŠE’l‚ğ•Ô‚·
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