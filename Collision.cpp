#include "Collision.h"

bool Collision::Check_Collision_Cube_to_Point(Shape* c, const ML::Vec3& p)
{
	//�_���L���[�u����S�����o��
	vector<ML::Vec3> all_Points;
	c->Get_All_Points(&all_Points);
	
	for (size_t i = 0; i<all_Points.size()/2; i++)
	{
		//�Ίp���ǂ����Ŕ��肷��_�Ƀx�N�g�����q��
		ML::Vec3 a, b;

		a = all_Points[i] - p;
		b = all_Points[i+4] - p;

		//�����̊O�ς̃R�T�C���l��-�l(90<x<=180)���S�Ă̓_�ŏo��񂾂�����
		//���肳���_�̓L���[�u�̒��ɓ����Ă���
		float cos = 0.0f;
		MyMath::Vector_Cross(&cos, a, b);
		//�ЂƂł������ɍ����ĂȂ��񂾂����瓖����Ȃ����Ƃɂ���
		if (cos < 0)
		{
			return true;
		}
	}
	return false;
}


//��Ԑ}�`�Ɠ_�̂����蔻��
//���݂͕������Z���K�v�Ȃ��̂ŁA�߂�l��bool�ɂ���
bool Collision::Hit_Check(Shape* you, const ML::Vec3& me)
{
	//���肷�郁�\�b�h�𔻕�
	switch (you->Get_Type())
	{
	case Shape_Type::Shape_Cube:
		this->Check_Collision_Cube_to_Point(you, me);
		break;

	default:
		//����p���\�b�h���Ȃ��ꍇ��false�ŕԂ�
		return false;
		break;
	}
}