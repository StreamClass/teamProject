#pragma once
#pragma warning (disable : 4996)
#include "GameEngine_Ver3_7.h"

class Tablet
{
private:
	//�J�����̈ʒu
	std::vector<ML::Vec3> camera_Pos;
	//�压�_�̈ʒu
	std::vector<ML::Vec3> target_Pos;
	//���ݑI�������J����
	int Select;
	//�^�u���b�g���g�p���Ȃ̂����m�F����t���O
	bool be_used_now;	


	//�I��͈͂𒴂��Ȃ��悤�ɂ��鏈��
	void Is_Select_Range_Over();
	//�J�����ύX
	void Change_Camera();
	//�f���̎��̃J�����̓���
	bool Update_On_Demo();

public:

	//�J���������̎��̈ʒu�o�^
	//���� : (�o�^����J�����̈ʒu�A�o�^����J�����̎压�_)
	void PushBack_Camera(const ML::Vec3&, const ML::Vec3&);
	//�g�p�t���O�𔽓]������֐�
	void Open_or_Close_Tablet();
	//���͔���
	void Select_Camera(const unsigned int& );

	//�g�p����Ă��邩��Ԃ��֐�
	bool Is_Used_Now() const;

	//�R���X�g���N�^�E�f�X�g���N�^
	//�Z���N���A
	Tablet();

	~Tablet() {}
};