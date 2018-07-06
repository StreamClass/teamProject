#include "Joint.h"

//移動

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
	//自分とつながっている骨を回転
	this->area->Rotate(matR);
	//つながりを持っている間接なら次のやつも回転させる
	if (this->next != nullptr)
	{
		next->Rotated_by_Prev_Joint(matR);
	}
}

void Joint::Rotated_by_Prev_Joint(ML::Mat4x4* matR)
{
	//回転応じて位置を更新
	this->pos = matR->TransformCoord(this->pos);

	//その次につながっているものを回転	
	this->Rotate_Bone(matR);	
}

//getter

//関節の現在位置を返す(絶対座標)
ML::Vec3 Joint::Get_Pos()
{
	return this->pos;
}

//各回転量の限界値を返す
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

void Joint::Quartanion_Update(const ML::QT& uqt)
{
	this->rotated *= uqt;
}

void Joint::Render()
{
	//表示用ワールド行列作成
	ML::Mat4x4 matW;
	D3DXMatrixAffineTransformation(&matW, 1.0f, &this->pos, &this->rotated, &this->area->GetCenter());
	//行列適用
	DG::EffectState().param.matWorld = matW;
	//レンダリング
	DG::Mesh_Draw(this->mesh_Name);
}