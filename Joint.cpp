#include "Joint.h"

//移動
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
	//自分とつながっている骨を回転
	this->area->Rotate(matR);
	this->Quartanion_Update(qtf);
	//つながりを持っている間接なら次のやつも回転させる
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
	//回転応じて位置を更新
	this->pos = matR->TransformCoord(this->pos);

	//その次につながっているものを回転	
	this->Rotate_Bone(matR, qtf);
}

//getter

//関節の現在位置を返す(絶対座標)
ML::Vec3 Joint::Get_Pos() const
{
	return this->pos;
}

//各回転量の限界値を返す
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
	//表示用ワールド行列作成
	ML::Mat4x4 matW;
	//倍率計算
	float magnification = (tall / 180.0f) * 100.0f;
	D3DXMatrixAffineTransformation(&matW, magnification, NULL, &this->rotated, &this->area->GetCenter());
	//行列適用
	DG::EffectState().param.matWorld = matW ;
	//レンダリング
	DG::Mesh_Draw(this->mesh_Name);
}