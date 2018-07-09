#include "Bone.h"
#include "Cube.h"
#include "MyMath.h"

//�R���X�g���N�^

Bone::Bone(const float& tall)
	: tall(tall)
{
	//���S�w��
	this->center_of_Body = ML::Vec3(0, tall / 2.0f, 0);
	//��]�l������
	this->All_RotY = 0.0f;
	//�䗦�̊�ɂȂ��̒���
	float length_of_hand = (tall / 2.0f)*(4.0f / 14.0f);


	// TODO ��Œ���
	//���̏�����
	ML::Vec3 center_of_head = ML::Vec3(this->center_of_Body + (this->center_of_Body*(12.0f / 14.0f)));
	Shape* head = new Cube(center_of_head, (center_of_head + ML::Vec3(-length_of_hand / 2.0f, -length_of_hand / 2.0f, -length_of_hand * (3.0f / 8.0f))), length_of_hand, length_of_hand, length_of_hand*(3.0f / 4.0f));
	//��̏�����
	ML::Vec3 center_of_neck = ML::Vec3(this->center_of_Body + (this->center_of_Body*(10.0f / 14.0f)));
	Joint* tmpneck = new Joint(center_of_neck, ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), head,"Head");
	//�㔼�g�̏�����
	ML::Vec3 center_of_upper_body = this->center_of_Body + (this->center_of_Body *(5.0f / 14.0f));
	Shape* upper_Body = new Cube(center_of_upper_body, (center_of_upper_body + ML::Vec3(-length_of_hand * (3.0f / 4.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 4.0f), length_of_hand, length_of_hand / 2.0f);
	//���̏�����
	ML::Vec3 center_of_waist = (this->center_of_Body + (this->center_of_Body*(1.0f / 14.0f)));
	Joint* Waist = new Joint(center_of_waist, ML::ToRadian(-45), ML::ToRadian(170), ML::ToRadian(-100), ML::ToRadian(100), ML::ToRadian(-80), ML::ToRadian(80), upper_Body,"UpperBody");

	//����
	//����
	ML::Vec3 left_center_of_Thigh = this->center_of_Body + ML::Vec3(-length_of_hand/2.0f, -length_of_hand, 0);
	Shape* left_Thigh = new Cube(left_center_of_Thigh, (left_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//�����K�̏�����
	ML::Vec3 center_of_left_hip = this->center_of_Body + ML::Vec3(-length_of_hand/2.0f, 0, 0);
	Joint* left_hip = new Joint(center_of_left_hip, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-90), ML::ToRadian(70), ML::ToRadian(-80), ML::ToRadian(40), left_Thigh,"Left_Thigh");
	//�G���瑫��܂�
	ML::Vec3 left_center_of_shin = left_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* left_shin = new Cube(left_center_of_shin, (left_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//�G�̏�����
	ML::Vec3 left_center_of_knee = center_of_left_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* left_knee = new Joint(left_center_of_knee, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0), left_shin,"Left_Shin");
	//�����̏�����
	ML::Vec3 left_center_of_foot = left_center_of_shin + ML::Vec3(0, -length_of_hand*(4.0f / 3.0f), length_of_hand / 2.0f);
	Shape* left_foot = new Cube(left_center_of_foot, left_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//����̏�����
	ML::Vec3 left_center_of_ankle = left_center_of_knee + ML::Vec3(0, length_of_hand*2.0f, 0);
	Joint* left_ankle = new Joint(left_center_of_ankle, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(10), left_foot,"Left_Foot");

	//�E��
	ML::Vec3 right_center_of_Thigh = this->center_of_Body + ML::Vec3(length_of_hand/2.0f, -length_of_hand, 0);
	Shape* right_Thigh = new Cube(right_center_of_Thigh, (right_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//�����K�̏�����
	ML::Vec3 center_of_right_hip = this->center_of_Body + ML::Vec3(length_of_hand/2.0f, 0, 0);
	Joint* right_hip = new Joint(center_of_right_hip, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-70), ML::ToRadian(90), ML::ToRadian(-40), ML::ToRadian(80), right_Thigh,"Right_Thigh");
	//�G���瑫��܂�
	ML::Vec3 right_center_of_shin = right_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* right_shin = new Cube(right_center_of_shin, (right_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//�G�̏�����
	ML::Vec3 right_center_of_knee = center_of_right_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* right_knee = new Joint(right_center_of_knee, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0), right_shin,"Right_Shin");
	//�����̏�����
	ML::Vec3 right_center_of_foot = right_center_of_shin + ML::Vec3(0, -length_of_hand*(4.0f / 3.0f), length_of_hand / 2.0f);
	Shape* right_foot = new Cube(right_center_of_foot, right_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//����̏�����
	ML::Vec3 right_center_of_ankle = right_center_of_knee + ML::Vec3(0, length_of_hand*2.0f, 0);
	Joint* right_ankle = new Joint(right_center_of_ankle, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(-10), ML::ToRadian(0), right_foot,"Right_Foot");


	//����
	//����
	//��r�̏�����
	ML::Vec3 left_center_of_arm = center_of_neck + ML::Vec3(-length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* left_arm = new Cube(left_center_of_arm, left_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//���̏�����
	ML::Vec3 left_center_of_sholder = center_of_neck + ML::Vec3(-length_of_hand, 0, 0);
	Joint* left_sholder = new Joint(left_center_of_sholder, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-180), ML::ToRadian(60), left_arm,"Left_Arm");
	//�O�r�̏�����
	ML::Vec3 left_center_of_forearm = left_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_forearm = new Cube(left_center_of_forearm, left_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//�I�̏�����
	ML::Vec3 left_center_of_elbow = left_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_elbow = new Joint(left_center_of_elbow, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0), left_forearm,"Left_ForeArm");
	//��̏�����
	ML::Vec3 left_center_of_hand = left_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_hand = new Cube(left_center_of_hand, left_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//���̏�����
	ML::Vec3 left_center_of_wrist = left_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_wrist = new Joint(left_center_of_wrist, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90), left_hand,"Left_Hand");

	//�E��
	//��r�̏�����
	ML::Vec3 right_center_of_arm = center_of_neck + ML::Vec3(length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* right_arm = new Cube(right_center_of_arm, right_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//���̏�����
	ML::Vec3 right_center_of_sholder = center_of_neck + ML::Vec3(length_of_hand, 0, 0);
	Joint* right_sholder = new Joint(right_center_of_sholder, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-60), ML::ToRadian(180), right_arm,"Right_Arm");
	//�O�r�̏�����
	ML::Vec3 right_center_of_forearm = right_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_forearm = new Cube(right_center_of_forearm, right_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//�I�̏�����
	ML::Vec3 right_center_of_elbow = right_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_elbow = new Joint(right_center_of_elbow, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0), right_forearm,"Right_ForeArm");
	//��̏�����
	ML::Vec3 right_center_of_hand = right_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_hand = new Cube(right_center_of_hand, right_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//���̏�����
	ML::Vec3 right_center_of_wrist = right_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_wrist = new Joint(right_center_of_wrist, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90), right_hand,"Right_Hand");

	//�����o�[�Ɋ��蓖�Ă�
	this->joint[0] = Waist;
	this->joint[1] = tmpneck;

	this->joint[2] = left_sholder;
	this->joint[3] = left_elbow;
	this->joint[4] = left_wrist;

	this->joint[5] = right_sholder;
	this->joint[6] = right_elbow;
	this->joint[7] = right_wrist;

	this->joint[8] = left_hip;
	this->joint[9] = left_knee;
	this->joint[10] = left_ankle;

	this->joint[11] = right_hip;
	this->joint[12] = right_knee;
	this->joint[13] = right_ankle;

	
	
	
	//�֌W���w��
	//������̂�
	this->joint[0]->Set_Next_Joint(this->joint[1]);

	//��������
	//��
	this->joint[2]->Set_Next_Joint(this->joint[3]);
	this->joint[3]->Set_Next_Joint(this->joint[4]);
	//�E
	this->joint[5]->Set_Next_Joint(this->joint[6]);
	this->joint[6]->Set_Next_Joint(this->joint[7]);

	//���K���瑫��
	//��
	this->joint[8]->Set_Next_Joint(this->joint[9]);
	this->joint[9]->Set_Next_Joint(this->joint[10]);
	//�E
	this->joint[11]->Set_Next_Joint(this->joint[12]);
	this->joint[12]->Set_Next_Joint(this->joint[13]);

	//���[�V�����֌W�ϐ�������
	this->motions.clear();
	this->motionCnt = 0.0f;
	this->motion_Index = 0;
	this->now_Motion = "";
	this->next_Motion = "";
	this->repeat_Flag = false;
}


//getter
ML::Vec3 Bone::Get_Center()
{
	return this->center_of_Body;
}

void Bone::Bone_RotateY_All(const float& radian)
{
	//�S�̉�]�l�Ƃ̍���������]����
	float def = this->All_RotY - radian;
	if (def != 0.0f)
	{
		//�A�t�B���ϊ��ŉ�]�s��쐬
		ML::Mat4x4 matR;
		ML::QT qtY = ML::QT(ML::Vec3(0, 1, 0), def);
		//this->All_Qt *= ML::QT(ML::Vec3(0, 1, 0), radian);
		D3DXMatrixAffineTransformation(&matR, 1.0f, &this->center_of_Body, &qtY, NULL);

		//�֐ߑS�̂���]������
		this->joint[0]->Rotated_by_Prev_Joint(&matR);
		this->joint[2]->Rotated_by_Prev_Joint(&matR);
		this->joint[5]->Rotated_by_Prev_Joint(&matR);
		this->joint[8]->Rotated_by_Prev_Joint(&matR);
		this->joint[11]->Rotated_by_Prev_Joint(&matR);

		//�S�֐߂̃N�H�[�^�j�I�����X�V
		for (int i = 0; i < JOINT_ON_HUMAN; i++)
		{
			this->joint[i]->Quartanion_Update(qtY);
		}
	}
	
	this->All_RotY = radian;
	
}

void Bone::Moving(const ML::Vec3& vec)
{
	//�S�̒��S�y�ъe�{�[���̍��W���ړ�������
	this->center_of_Body += vec;

	for (int i = 0; i < JOINT_ON_HUMAN; i++)
	{
		this->joint[i]->Move(vec);
	}
}

void Bone::UpDate()
{
	//���݃��[�V��������łȂ��ꍇ
	if (this->now_Motion != "")
	{
		//�����Ă��郂�[�V����������
		auto& now = this->motions.find(this->now_Motion)->second;

		//���݃J�E���g���������ԓ��Ȃ�
		if (this->motionCnt <= now[this->motion_Index].duration)
		{
			//�f�[�^�ɏ]����]���s��
			ML::Mat4x4 matR;
			//X���N�H�[�^�j�I���AY���N�H�[�^�j�I���AZ���N�H�[�^�j�I���A�S�̂���������N�H�[�^�j�I��
			ML::QT qtx, qty, qtz, qtA;
			
			for (int i = 0; i < JOINT_ON_HUMAN; i++)
			{				
				//�N�H�[�^�j�I���쐬
				qtx = ML::QT(ML::Vec3(1, 0, 0), (now[this->motion_Index].joint[i].x) / now[this->motion_Index].duration);
				qty = ML::QT(ML::Vec3(0, 1, 0), (now[this->motion_Index].joint[i].y) / now[this->motion_Index].duration);
				qtz = ML::QT(ML::Vec3(0, 0, 1), (now[this->motion_Index].joint[i].z) / now[this->motion_Index].duration);
				//�N�H�[�^�j�I������
				qtA = qtx * qty * qtz;

				//�֐߂̃N�H�[�^�j�I���X�V
				this->joint[i]->Quartanion_Update(qtA);
				//��]�s��쐬
				matR.Identity();
				D3DXMatrixAffineTransformation(&matR, 1.0f, &this->joint[i]->Get_Pos(), &qtA, NULL);
				//�e�֐߂���]
				this->joint[i]->Rotate_Bone(&matR);
			}
		}
		//�J�E���g���x�����Ԃ𒴂����ꍇ
		else
		{
			//����I�����
			if (this->motion_Index == now.size() - 1 )
			{
				//�A���s���t���O�������Ă���ꍇ
				if (this->repeat_Flag == true)
				{
					//�C���f�N�X�ƃJ�E���g���[���ɂ���
					this->motion_Index = 0;
					this->motionCnt = -1.0f;
				}
				//�A���s���t���O�������Ă��Ȃ��ꍇ
				else
				{
					//�\�񃂁[�V����������ꍇ���݃��[�V�����ɒu��������
					if (this->next_Motion != "")
					{
						this->now_Motion = this->next_Motion;
						//�\��͋�ɂ���
						this->next_Motion = "";
						//���̃��[�V����������O�ɃX�^���f�B���O�ɖ߂���
						//��������̃��\�b�h�Ɏd�l�ύX�����邩������Ȃ�(2018/07/09)
						this->To_Standing();
					}
					else
					{
						//���[�V��������ɂ���
						this->now_Motion == "";
						this->motion_Index = 0;
					}
				}
			}
			//����������ꍇ
			else
			{
				this->motion_Index++;
				this->motionCnt = -1.0f;
			}
		}

	}
	//��̎��ɃX�^���f�B���O�ɖ߂���
	else
	{
		//��������̃��\�b�h�Ɏd�l�ύX�����邩������Ȃ�(2018/07/09)
		this->To_Standing();
	}


	//���[�V�������󂩂ɂȂ��Ɋւ�炸�J�E���g�͐i��
	this->motionCnt += 1.0f;
	//�A���s���t���O�͖���false�ɖ߂�(���肵�Ă����~�܂�悤��)
	this->repeat_Flag = false;
}

void Bone::Set_Next_Motion(const string& next)
{
	//�O�̂��߂ɓo�^����Ă���̂����m�F
	if (this->motions.count(next) >= 1)
	{
		//���̃��[�V������\��
		this->next_Motion = next;
	}
}

void Bone::Repeat_Now_Motioin()
{
	this->repeat_Flag = true;
}

void Bone::To_Standing()
{
	for (int i = 0; i < JOINT_ON_HUMAN; i++)
	{
		ML::Vec3 standing_Vec;
		//�X�^���f�B���O�̎��Ɋ֐߂��獜�܂Ńx�N�g����錾
		if (i < 2)
		{
			//Y+(���Ǝ�)
			standing_Vec = ML::Vec3(0, 1, 0);
		}
		else
		{
			//Y-(���̑�)
			standing_Vec = ML::Vec3(0, -1, 0);
		}
		float s = 0.0f;

		ML::Vec3 bone_Vec = this->joint[i]->Get_To_Bone();
		//�{�[�����ƊO�ς�sin�l�����
		MyMath::Vector_Cross(&s, bone_Vec, standing_Vec);

		//�֐߈���߂�����Ɏ��̊֐߂𐮓�
		if (abs(s) <= sinf(ML::ToRadian(2)))
		{
			continue;
		}
		else
		{
			//��]���錾
			ML::Vec3 anker;
			MyMath::Get_Normal_to_Vector_Cross(&anker, bone_Vec, standing_Vec);

			//�c���]�ʂ̔�������]�������Ԃ�
			//�N�H�[�^�j�I���錾
			ML::QT remain = ML::QT(anker, -s / 2.0f);
			//��]�s��錾
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, 1.0f, &this->joint[i]->Get_Pos(), &remain, NULL);
			//��]
			this->joint[i]->Rotate_Bone(&matR);

			return;
		}
	}
}

void Bone::Render()
{
	//�֐ߑS�̂Ƀ����_�����O���߂���
	for (int i = 0; i < JOINT_ON_HUMAN; i++)
	{
		this->joint[i]->Render();
	}
}