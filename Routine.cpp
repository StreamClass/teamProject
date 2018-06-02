#include "Routine.h"

//�}�b�v���̃R�[�i�[�̐�
#define corners 24
//���݂̍ő��I������
//4�����ő�
#define choices 4

//�[���N���A
Routine::Routine()
{
	this->pos = ML::Vec3(0.0f, 0.0f, 0.0f);
	this->moveVec = ML::Vec3(0, 0, 0);
	this->hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
	this->now = 0;
	this->target = 0;
	for (int i = 0; i < corners; ++i)
	{
		this->cornerPos[i] = ML::Vec3(0,0,0);
	}
	for (int i = 0; i < corners; ++i)
	{
		for (int j = 0; j < choices; ++j)
		{
			this->choiceCorner[i][j]=0;
		}
	}
}
//�߂��R�[�i�[��ڎw���Ĉړ�����
void Routine::Move()
{
	ML::Vec3 toVec = this->pos - this->cornerPos[this->Choice(this->now)];
	this->moveVec = toVec;
}
//�������R�[�i�[�ɓ��B�������̔���
bool Routine::Judge()
{
	//����p�ϐ����R�[�i�[�̔ԍ��O�Ő錾
	this->now = this->Recieved();
	//judge�̒l�������ꂩ�̃R�[�i�[�̔ԍ��ɂȂ����Ƃ�
	if (this->now >= 0)
	{
		this->Choice(this->now);
		return true;
	}
	else
	{
		return false;
	}
}
//�����̂���R�[�i�[����s����R�[�i�[��I������
int Routine::Choice(const int& now_)
{
	int c = -1;
	//�R�[�i�[�ɂ���đI�����̐����Ⴄ����
	//���R�����o��܂őI����J��Ԃ�
	while (c < 0)
	{
		c = this->choiceCorner[this->now][rand() % choices];
	}
	//�s������肵�A�Ԃ�
	return c;
}
//�R�[�i�[�̎���`�̑����G�l�~�[�Ƃ̐ڐG�ɉ����Ď����̔ԍ���Ԃ�
int Routine::Recieved()
{
	ML::Box3D me = this->hitBase.OffsetCopy(this->pos);
	//�R�[�i�[�ɋ�`���������A�ڐG������s��
	for (int i = 0; i < corners; ++i)
	{
		ML::Box3D you = ML::Box3D(this->cornerPos[i].x, this->cornerPos[i].y, this->cornerPos[i].z,
			1, 1, 1);
		if (you.Hit(me))
		{
			//�ڐG������`�̔ԍ���Ԃ��Ĕ�����
			return i;
			break;
		}
	}
}
