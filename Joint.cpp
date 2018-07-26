#include "Joint.h"

//�ړ�
void Joint::Move(const ML::Vec3& vec)
{
	this->pos += vec;
	this->area->Move(vec);
}

void Joint::Set_Next_Joint(Joint* next)
{
	this->next.push_back(next);
}

void Joint::Rotate_Bone(ML::Mat4x4* matR, const ML::QT& qtf)
{
	//�����ƂȂ����Ă��鍜����]
	this->area->Rotate(matR);
	this->Quartanion_Update(qtf);
	//�Ȃ���������Ă���ԐڂȂ玟�̂����]������
	if (this->next.size() !=0)
	{
		for (auto& n : this->next)
		{
			n->Rotated_by_Prev_Joint(matR, qtf);
		}
	}
}

void Joint::Rotated_by_Prev_Joint(ML::Mat4x4* matR, const ML::QT& qtf)
{
	//��]�����Ĉʒu���X�V
	this->pos = matR->TransformCoord(this->pos);

	//���̎��ɂȂ����Ă�����̂���]	
	this->Rotate_Bone(matR, qtf);
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

void Joint::Render(const float& tall)
{
	//�\���p���[���h�s��쐬
	ML::Mat4x4 matW;
	//�{���v�Z
	float magnification = (tall / 180.0f) * 100.0f;
	D3DXMatrixAffineTransformation(&matW, magnification, NULL, &this->rotated, &this->area->GetCenter());
	//�s��K�p
	DG::EffectState().param.matWorld = matW ;
	//�����_�����O
	DG::Mesh_Draw(this->mesh_Name);
}