#pragma once
#include "Shape.h"
#include "MyMath.h"

//�p�x�������Ă����Ԑ}�`�̂����蔻��p�N���X

class Collision
{
private:
	//360�x�𔻒肷�邽�߂̃��W�A���l
	const float judge_360;
	//�L���[�u�Ɠ_1�̂����蔻��
	bool Check_Collision_Cube_to_Point(const Shape*, const ML::Vec3&);

public:
};