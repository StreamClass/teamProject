#include "Shape.h"


ML::Vec3 Shape::GetCenter()
{
	return this->center;
}

void Shape::Rotate_Center(ML::Mat4x4* matR)
{
	this->center = matR->TransformCoord(this->center);
}

void Shape::Move_Center(const ML::Vec3& vec)
{
	this->center += vec;
}