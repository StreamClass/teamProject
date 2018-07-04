#include "Cube.h"
#include "MyMath.h"

#define VERTEX_ON_CUBE 8
#define TRIANGLE_ON_CUBE 12

//Getter

ML::Vec3 Cube::Get_LeftBottom()
{
	return this->leftBottom;
}

ML::Vec3 Cube::Get_LengthX()
{
	return this->lengthX;
}

ML::Vec3 Cube::Get_LengthY()
{
	return this->lengthY;
}

ML::Vec3 Cube::Get_LengthZ()
{
	return this->lengthZ;
}

//移動・回転
void Cube::Rotate(ML::Mat4x4* matR)
{
	//中心点回転
	this->Rotate_Center(matR);
	//始点と長さを回転
	this->leftBottom = matR->TransformCoord(this->leftBottom);
	this->lengthX = matR->TransformCoord(this->lengthX);
	this->lengthY = matR->TransformCoord(this->lengthY);
	this->lengthZ = matR->TransformCoord(this->lengthZ);
}

void Cube::Move(const ML::Vec3& vec)
{
	this->Move_Center(vec);
	this->leftBottom += vec;
}

//分割メソッド
//vectorをクリアするかは未定(2018/06/28)
void Cube::Get_All_Points(std::vector<ML::Vec3>* vertex)
{
	//vertex->resize(VERTEX_ON_CUBE);
	vertex->push_back(this->leftBottom);//0
	vertex->push_back(this->leftBottom + this->lengthZ);//1
	vertex->push_back(this->leftBottom + this->lengthX + this->lengthZ);//2
	vertex->push_back(this->leftBottom + this->lengthX);//3
	vertex->push_back(this->leftBottom + this->lengthY);//4
	vertex->push_back(this->leftBottom + this->lengthY + this->lengthZ);//5
	vertex->push_back(this->leftBottom + this->lengthX + this->lengthY + this->lengthZ);//6
	vertex->push_back(this->leftBottom + this->lengthY + this->lengthX);//7
}

void Cube::Get_All_Triangle(std::vector<Triangle>* tri)
{
	//t->resize(TRIANGLE_ON_CUBE);

	std::vector<ML::Vec3>vertex_Index;
	this->Get_All_Points(&vertex_Index);

	//6面体には12個の三角形がある
	Triangle t[TRIANGLE_ON_CUBE] = {};

	//三角形の情報入力
	t[0] =
	{
		vertex_Index[0],
		vertex_Index[3],
		vertex_Index[4],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[0].normal, (t[0].c - t[0].a), (t[0].b - t[0].a));
	t[1] =
	{
		vertex_Index[3],
		vertex_Index[4],
		vertex_Index[7],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[1].normal, (t[1].b - t[1].a), (t[1].c - t[1].a));
	t[2] =
	{
		vertex_Index[1],
		vertex_Index[0],
		vertex_Index[5],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[2].normal, (t[2].c - t[2].a), (t[2].b - t[2].a));
	t[3] =
	{
		vertex_Index[0],
		vertex_Index[5],
		vertex_Index[4],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[3].normal, (t[3].b - t[3].a), (t[3].c - t[3].a));
	t[4] =
	{
		vertex_Index[1],
		vertex_Index[5],
		vertex_Index[2],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[4].normal, (t[4].c - t[4].a), (t[4].b - t[4].a));
	t[5] =
	{
		vertex_Index[5],
		vertex_Index[2],
		vertex_Index[6],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[5].normal, (t[5].b - t[5].a), (t[5].c - t[5].a));
	t[6] =
	{
		vertex_Index[3],
		vertex_Index[7],
		vertex_Index[2],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[6].normal, (t[6].b - t[6].a), (t[4].c - t[6].a));
	t[7] =
	{
		vertex_Index[7],
		vertex_Index[2],
		vertex_Index[6],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[7].normal, (t[7].c - t[7].a), (t[7].b - t[7].a));
	t[8] =
	{
		vertex_Index[4],
		vertex_Index[7],
		vertex_Index[5],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[8].normal, (t[8].c - t[8].a), (t[8].b - t[8].a));
	t[9] =
	{
		vertex_Index[7],
		vertex_Index[5],
		vertex_Index[6],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[9].normal, (t[9].b - t[9].a), (t[9].c - t[9].a));
	t[10] =
	{
		vertex_Index[0],
		vertex_Index[3],
		vertex_Index[1],

	};
	MyMath::Get_Normal_to_Vector_Cross(&t[10].normal, (t[10].b - t[10].a), (t[10].c - t[10].a));
	t[11] =
	{
		vertex_Index[3],
		vertex_Index[1],
		vertex_Index[2],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[11].normal, (t[11].c - t[11].a), (t[11].b - t[11].a));
	//vectorに登録
	for (int i = 0; i < TRIANGLE_ON_CUBE; i++)
	{
		t[i].normal = t[i].normal.Normalize();
		tri->push_back(t[i]);
	}
}