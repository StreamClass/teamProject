#include "Joint.h"
#include <iostream>

//�ړ�

void Joint::Move(const ML::Vec3& vec)
{
	this->pos += vec;
	this->area->Move(vec);
}

void Joint::Set_Next_Joint(Joint* next)
{
	this->next = next;
}

void Joint::Rotate_Bone(ML::Mat4x4* matR)
{
	//�����ƂȂ����Ă��鍜����]
	this->area->Rotate(matR);
	//�Ȃ���������Ă���ԐڂȂ玟�̂����]������
	if (this->next != nullptr)
	{
		next->Rotated_by_Prev_Joint(matR);
	}
}

void Joint::Rotated_by_Prev_Joint(ML::Mat4x4* matR)
{
	//��]�����Ĉʒu���X�V
	this->pos = matR->TransformCoord(this->pos);

	//���̎��ɂȂ����Ă�����̂���]	
	this->Rotate_Bone(matR);	
}

//getter

//�֐߂̌��݈ʒu��Ԃ�(��΍��W)
ML::Vec3 Joint::Get_Pos() const
{
	return this->pos;
}

//�e��]�ʂ̌��E�l��Ԃ�
//X
float Joint::Get_Limit_X_Minus() const
{
	return this->limit_X_Minus;
}

float Joint::Get_Limit_X_Plus() const
{
	return this->limit_X_Plus;
}

//Y
float Joint::Get_Limit_Y_Minus() const
{
	return this->limit_Y_Minus;
}

float Joint::Get_Limit_Y_Plus() const
{
	return this->limit_Y_Plus;
}

//Z
float Joint::Get_Limit_Z_Minus() const
{
	return this->limit_Z_Minus;
}

float Joint::Get_Limit_Z_Plus() const
{
	return this->limit_Z_Plus;
}

ML::Vec3 Joint::Get_To_Bone() const
{
	return (this->area->GetCenter() - this->pos);
}

//-----------------------------------------------------------------------------------------
void Joint::Quartanion_Update(const ML::QT& uqt)
{
	this->rotated *= uqt;
}

void Joint::Render()
{
	//�\���p���[���h�s��쐬
	ML::Mat4x4 matW;	
	D3DXMatrixAffineTransformation(&matW, 100.0f, NULL, &this->rotated, &this->area->GetCenter());
	//�s��K�p
	DG::EffectState().param.matWorld = matW ;
	//�����_�����O
	DG::Mesh_Draw(this->mesh_Name);
}

void Joint::DEBUG()
{
	ML::Vec3 devec = this->area->GetCenter();
	cout <<this->mesh_Name +" " << "X : " <<devec.x << " Y ; " << devec.y << " Z : " << devec.z << endl;
	cout << this->mesh_Name + " " << "X : " << this->pos.x << " Y ; " << this->pos.y << " Z : " << this->pos.z << endl;
	
}