#include "Bone.h"
#include "Cube.h"
#include "MyMath.h"

//�R���X�g���N�^

Bone::Bone(const float& tall, const string& owner)
	: tall(tall)
{
	//���S�w��
	this->center_of_Body = ML::Vec3(0, tall / 2.0f, 0);
	//��]�l������
	this->All_RotY = 0.0f;
	//���ʃx�N�g��������
	this->front_Vec = ML::Vec3(0, 0, 1);
	//�䗦�̊�ɂȂ��̒���
	float length_of_hand = (tall / 2.0f)*(4.0f / 14.0f);


	//���̏�����
	ML::Vec3 center_of_head = ML::Vec3(this->center_of_Body + (this->center_of_Body*(12.0f / 14.0f)));
	Shape* head = new Cube(center_of_head, (center_of_head + ML::Vec3(-length_of_hand / 2.0f, -length_of_hand / 2.0f, -length_of_hand * (3.0f / 8.0f))), length_of_hand, length_of_hand, length_of_hand*(3.0f / 4.0f));
	//��̏�����
	ML::Vec3 center_of_neck = ML::Vec3(this->center_of_Body + (this->center_of_Body*(10.0f / 14.0f)));
	Joint* tmpneck = new Joint(center_of_neck, head, owner +"/Head");
	//, ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90), ML::ToRadian(-90), ML::ToRadian(90)
	//�㔼�g�̏�����
	ML::Vec3 center_of_upper_body = this->center_of_Body + (this->center_of_Body *(5.0f / 14.0f) + ML::Vec3(0,length_of_hand /3.0f,0));
	Shape* upper_Body = new Cube(center_of_upper_body, (center_of_upper_body + ML::Vec3(-length_of_hand * (3.0f / 4.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 4.0f), length_of_hand, length_of_hand / 2.0f);
	//���̏�����
	ML::Vec3 center_of_waist = (this->center_of_Body + (this->center_of_Body*(1.0f / 14.0f)));
	Joint* Waist = new Joint(center_of_waist, upper_Body, owner + "/UpperBody");
	//, ML::ToRadian(-45), ML::ToRadian(170), ML::ToRadian(-100), ML::ToRadian(100), ML::ToRadian(-80), ML::ToRadian(80)

	//����
	//����
	ML::Vec3 left_center_of_Thigh = this->center_of_Body + ML::Vec3(-length_of_hand/2.0f, -length_of_hand*(4.0f / 5.0f), 0);
	Shape* left_Thigh = new Cube(left_center_of_Thigh, (left_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//�����K�̏�����
	ML::Vec3 center_of_left_hip = this->center_of_Body + ML::Vec3(-length_of_hand/2.0f, length_of_hand / 5.0f, 0);
	Joint* left_hip = new Joint(center_of_left_hip, left_Thigh, owner + "/Left_Thigh");
	//, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-90), ML::ToRadian(70), ML::ToRadian(-80), ML::ToRadian(40)
	//�G���瑫��܂�
	ML::Vec3 left_center_of_shin = left_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* left_shin = new Cube(left_center_of_shin, (left_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//�G�̏�����
	ML::Vec3 left_center_of_knee = center_of_left_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* left_knee = new Joint(left_center_of_knee, left_shin, owner + "/Left_Shin");
	//, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0)
	//�����̏�����
	//ML::Vec3 left_center_of_foot = left_center_of_shin + ML::Vec3(0, -length_of_hand * (4.0f / 3.0f), length_of_hand / 2.0f);
	ML::Vec3 left_center_of_foot = left_center_of_shin + ML::Vec3(0, -length_of_hand * (4.0f / 3.0f), 0);
	Shape* left_foot = new Cube(left_center_of_foot, left_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//����̏�����
	ML::Vec3 left_center_of_ankle = left_center_of_knee + ML::Vec3(0, -length_of_hand*2.0f, 0);
	Joint* left_ankle = new Joint(left_center_of_ankle, left_foot, owner + "/Left_Foot");
	//, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(10),

	//�E��
	ML::Vec3 right_center_of_Thigh = this->center_of_Body + ML::Vec3(length_of_hand/2.0f, -length_of_hand*(4.0f/5.0f), 0);
	Shape* right_Thigh = new Cube(right_center_of_Thigh, (right_center_of_Thigh + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand * (3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//�����K�̏�����
	ML::Vec3 center_of_right_hip = this->center_of_Body + ML::Vec3(length_of_hand/2.0f, length_of_hand/5.0f, 0);
	Joint* right_hip = new Joint(center_of_right_hip, right_Thigh, owner + "/Right_Thigh");
	//, ML::ToRadian(-70), ML::ToRadian(160), ML::ToRadian(-70), ML::ToRadian(90), ML::ToRadian(-40), ML::ToRadian(80)
	//�G���瑫��܂�
	ML::Vec3 right_center_of_shin = right_center_of_Thigh + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Shape* right_shin = new Cube(right_center_of_shin, (right_center_of_shin + ML::Vec3(-length_of_hand * (3.0f / 16.0f), -length_of_hand, -length_of_hand / 2.0f)), length_of_hand*(3.0f / 8.0f), length_of_hand*2.0f, length_of_hand / 2.0f);
	//�G�̏�����
	ML::Vec3 right_center_of_knee = center_of_right_hip + ML::Vec3(0, -length_of_hand * 2.0f, 0);
	Joint* right_knee = new Joint(right_center_of_knee, right_shin, owner + "/Right_Shin");
	//, ML::ToRadian(-180), ML::ToRadian(0), ML::ToRadian(-15), ML::ToRadian(15), ML::ToRadian(0), ML::ToRadian(0)
	//�����̏�����
	//ML::Vec3 right_center_of_foot = right_center_of_shin + ML::Vec3(0, -length_of_hand*(4.0f / 3.0f), length_of_hand / 2.0f);
	ML::Vec3 right_center_of_foot = right_center_of_shin + ML::Vec3(0, -length_of_hand * (4.0f / 3.0f), 0);
	Shape* right_foot = new Cube(right_center_of_foot, right_center_of_foot + ML::Vec3(-length_of_hand / 6.0f, 0, -length_of_hand / 2.0f), length_of_hand / 3.0f, length_of_hand*(2.0f / 3.0f), length_of_hand);
	//����̏�����
	ML::Vec3 right_center_of_ankle = right_center_of_knee + ML::Vec3(0, -length_of_hand*2.0f, 0);
	Joint* right_ankle = new Joint(right_center_of_ankle, right_foot, owner + "/Right_Foot");
	//, ML::ToRadian(-90), ML::ToRadian(45), ML::ToRadian(-5), ML::ToRadian(5), ML::ToRadian(-10), ML::ToRadian(0)

	//����
	//����
	//��r�̏�����
	ML::Vec3 left_center_of_arm = center_of_neck + ML::Vec3(-length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* left_arm = new Cube(left_center_of_arm, left_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//���̏�����
	ML::Vec3 left_center_of_sholder = center_of_neck + ML::Vec3(-length_of_hand, 0, 0);
	Joint* left_sholder = new Joint(left_center_of_sholder, left_arm, owner + "/Left_Arm");
	//, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-180), ML::ToRadian(60)
	//�O�r�̏�����
	ML::Vec3 left_center_of_forearm = left_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_forearm = new Cube(left_center_of_forearm, left_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//�I�̏�����
	ML::Vec3 left_center_of_elbow = left_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_elbow = new Joint(left_center_of_elbow, left_forearm, owner + "/Left_ForeArm");
	//, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0)
	//��̏�����
	ML::Vec3 left_center_of_hand = left_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* left_hand = new Cube(left_center_of_hand, left_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//���̏�����
	ML::Vec3 left_center_of_wrist = left_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* left_wrist = new Joint(left_center_of_wrist, left_hand, owner + "/Left_Hand");
	//, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90)

	//�E��
	//��r�̏�����
	ML::Vec3 right_center_of_arm = center_of_neck + ML::Vec3(length_of_hand, -length_of_hand / 2.0f, 0);
	Shape* right_arm = new Cube(right_center_of_arm, right_center_of_arm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//���̏�����
	ML::Vec3 right_center_of_sholder = center_of_neck + ML::Vec3(length_of_hand, 0, 0);
	Joint* right_sholder = new Joint(right_center_of_sholder, right_arm, owner + "/Right_Arm");
	//, ML::ToRadian(-80), ML::ToRadian(180), ML::ToRadian(-30), ML::ToRadian(30), ML::ToRadian(-60), ML::ToRadian(180)
	//�O�r�̏�����
	ML::Vec3 right_center_of_forearm = right_center_of_arm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_forearm = new Cube(right_center_of_forearm, right_center_of_forearm + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//�I�̏�����
	ML::Vec3 right_center_of_elbow = right_center_of_sholder + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_elbow = new Joint(right_center_of_elbow, right_forearm, owner + "/Right_ForeArm");
	//, ML::ToRadian(0), ML::ToRadian(170), ML::ToRadian(-110), ML::ToRadian(110), ML::ToRadian(0), ML::ToRadian(0)
	//��̏�����
	ML::Vec3 right_center_of_hand = right_center_of_forearm + ML::Vec3(0, -length_of_hand, 0);
	Shape* right_hand = new Cube(right_center_of_hand, right_center_of_hand + ML::Vec3(-length_of_hand / 6.0f, -length_of_hand / 2.0f, -length_of_hand / 6.0f), length_of_hand / 3.0f, length_of_hand, length_of_hand / 3.0f);
	//���̏�����
	ML::Vec3 right_center_of_wrist = right_center_of_elbow + ML::Vec3(0, -length_of_hand, 0);
	Joint* right_wrist = new Joint(right_center_of_wrist, right_hand, owner + "/Right_Hand");
	//, ML::ToRadian(-30), ML::ToRadian(5), ML::ToRadian(0), ML::ToRadian(0), ML::ToRadian(-90), ML::ToRadian(90)

	//�����o�[�Ɋ��蓖�Ă�
	this->joint[Motion::Joint_Name::Waist] = Waist;
	this->joint[Motion::Joint_Name::Neck] = tmpneck;

	this->joint[Motion::Joint_Name::Left_Sholder] = left_sholder;
	this->joint[Motion::Joint_Name::Left_Elbow] = left_elbow;
	this->joint[Motion::Joint_Name::Left_Wrist] = left_wrist;

	this->joint[Motion::Joint_Name::Right_Sholder] = right_sholder;
	this->joint[Motion::Joint_Name::Right_Elbow] = right_elbow;
	this->joint[Motion::Joint_Name::Right_Wrist] = right_wrist;

	this->joint[Motion::Joint_Name::Left_Hip] = left_hip;
	this->joint[Motion::Joint_Name::Left_Knee] = left_knee;
	this->joint[Motion::Joint_Name::Left_Ankle] = left_ankle;

	this->joint[Motion::Joint_Name::Right_Hip] = right_hip;
	this->joint[Motion::Joint_Name::Right_Knee] = right_knee;
	this->joint[Motion::Joint_Name::Right_Ankle] = right_ankle;
	
	
	//�֌W���w��
	//������̂�
	this->joint[Motion::Joint_Name::Waist]->Set_Next_Joint(this->joint[Motion::Joint_Name::Neck]);
	//�����痼��
	this->joint[Motion::Joint_Name::Waist]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Sholder]);
	this->joint[Motion::Joint_Name::Waist]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Sholder]);

	//��������
	//��
	this->joint[Motion::Joint_Name::Left_Sholder]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Elbow]);
	this->joint[Motion::Joint_Name::Left_Elbow]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Wrist]);
	//�E
	this->joint[Motion::Joint_Name::Right_Sholder]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Elbow]);
	this->joint[Motion::Joint_Name::Right_Elbow]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Wrist]);

	//���K���瑫��
	//��
	this->joint[Motion::Joint_Name::Left_Hip]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Knee]);
	this->joint[Motion::Joint_Name::Left_Knee]->Set_Next_Joint(this->joint[Motion::Joint_Name::Left_Ankle]);
	//�E
	this->joint[Motion::Joint_Name::Right_Hip]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Knee]);
	this->joint[Motion::Joint_Name::Right_Knee]->Set_Next_Joint(this->joint[Motion::Joint_Name::Right_Ankle]);

	//���[�V�����֌W�ϐ�������
	this->motions.clear();
	this->motionCnt = 0;
	this->motion_Index = 0;
	this->repeat_Index = 0;
	this->now_Motion = "";
	this->next_Motion = "";
	this->repeat_Flag = false;

}


//getter
ML::Vec3 Bone::Get_Center() const
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
		ML::QT qtY = ML::QT(ML::Vec3(0, 1, 0), -def);
		//this->All_Qt *= ML::QT(ML::Vec3(0, 1, 0), radian);
		D3DXMatrixAffineTransformation(&matR, 1.0f, &this->center_of_Body, &qtY, NULL);		
		

		//���ʃx�N�g����]
		this->front_Vec = matR.TransformNormal(this->front_Vec);
		//�֐ߑS�̂���]������
		this->joint[Motion::Joint_Name::Waist]->Rotated_by_Prev_Joint(&matR,qtY);
		/*this->joint[2]->Rotated_by_Prev_Joint(&matR, qtY);
		this->joint[5]->Rotated_by_Prev_Joint(&matR, qtY);*/
		this->joint[Motion::Joint_Name::Left_Hip]->Rotated_by_Prev_Joint(&matR, qtY);
		this->joint[Motion::Joint_Name::Right_Hip]->Rotated_by_Prev_Joint(&matR, qtY);

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

		//�A���s���|�C���g�Ȃ炻�̃C���f�b�N�X��ۑ�
		if (now[this->motion_Index].repeat_Point)
		{
			this->repeat_Index = this->motion_Index;
		}

		//���݃J�E���g���������ԓ��Ȃ�
		if (this->motionCnt <= now[this->motion_Index].duration)
		{
			//�f�[�^�ɏ]����]���s��
			ML::Mat4x4 matR;
			//X���N�H�[�^�j�I���AY���N�H�[�^�j�I���AZ���N�H�[�^�j�I���A�S�̂���������N�H�[�^�j�I��
			ML::QT qtx, qty, qtz, qtA;
			//���[�V�����f�[�^�ŉ�]���s����]��
			ML::Vec3 ankerX, ankerY, ankerZ;
			ankerX = ML::Vec3(1, 0, 0);
			ankerY = ML::Vec3(0, 1, 0);
			ankerZ = ML::Vec3(0, 0, 1);
			//�S��Y����]�ɑΉ�����A���J�[��]
			ML::Mat4x4 matY;
			matY.RotationY(this->All_RotY);
			ankerX = matY.TransformCoord(ankerX);
			ankerY = matY.TransformCoord(ankerY);
			ankerZ = matY.TransformCoord(ankerZ);
			//���[�V�����J�n
			for (int i = 0; i < JOINT_ON_HUMAN; i++)
			{
				//�����̃f�[�^�Ȃ玟��
				if (now[this->motion_Index].joint[i].Is_Zero_Vec())
				{
					continue;
				}				

				//�N�H�[�^�j�I���쐬
				qtx = ML::QT(ankerX, ML::ToRadian((now[this->motion_Index].joint[i].x) / now[this->motion_Index].duration));
				qty = ML::QT(ankerY, ML::ToRadian((now[this->motion_Index].joint[i].y) / now[this->motion_Index].duration));
				qtz = ML::QT(ankerZ, ML::ToRadian((now[this->motion_Index].joint[i].z) / now[this->motion_Index].duration));
				//�N�H�[�^�j�I������
				qtA = qtx * qty * qtz;
				
				//��]�s��쐬
				matR.Identity();
				D3DXMatrixAffineTransformation(&matR, 1.0f, &this->joint[i]->Get_Pos(), &qtA, NULL);
				//�e�֐߂���]
				this->joint[i]->Rotate_Bone(&matR, qtA);
			}
		}
		//�J�E���g���x�����Ԃ𒴂����ꍇ
		else
		{
			//����I�����
			if (this->motion_Index >= now.size() - 1)
			{
				//�A���s��������ꍇ
				if (this->Is_Ok_to_Repeat())
				{
					//���݃C���f�b�N�X��T���Ă������C���f�b�N�X�ɏ㏑������
					this->motion_Index = this->repeat_Index;
					this->motionCnt = 0;
				}
				else
				{
					//���̃��[�V�������󂩊m�F����
					this->Next_Motion_or_None();
				}
			}
			//����������ꍇ
			else
			{
				this->motion_Index++;
				this->motionCnt = 0;
			}			
		}

	}
	//��̎�
	else
	{
		//�A���s���C���f�b�N�X�������l�ɂ���
		this->repeat_Index = 0;
		//���̃��[�V������T��
		if (!this->Next_Motion_or_None())
		{
			//���̃��[�V��������̏ꍇ�͂������X�^���f�B���O��
			this->To_Standing(false);
		}
	}


	//���[�V�������󂩂ɂȂ��Ɋւ�炸�J�E���g�͐i��
	this->motionCnt++;
	//�A���s���t���O�͖���false�ɖ߂�(���肵�Ă����~�܂�悤��)
	this->repeat_Flag = false;
}

bool Bone::Next_Motion_or_None()
{
	//�\�񃂁[�V����������ꍇ���݃��[�V�����ɒu��������
	if (this->next_Motion != "")
	{
		this->now_Motion = this->next_Motion;
		//�\��͋�ɂ���
		this->next_Motion = "";
		//�J�E���g��0��
		this->motionCnt = 0;
		this->motion_Index = 0;
		//���̃��[�V����������O�ɃX�^���f�B���O�ɖ߂���		
		this->To_Standing(true);
		return true;
	}
	else
	{
		//���[�V��������ɂ���
		this->now_Motion = "";
		this->motion_Index = 0;
		return false;
	}
}

void Bone::Set_Next_Motion(const string& next)
{
	//�������[�V�������J��Ԃ�������
	//Repeat_Now_Motion()��ʂ��Ă݂̂ɂ���(2018/07/10)

	//�O�̂��߂ɓo�^����Ă���̂����m�F
	if (this->motions.count(next) >= 1)
	{
		//���̃��[�V�����ƈႤ���[�V�����Ȃ�
		if (this->now_Motion != next)
		{
			//���̃��[�V������\��
			this->next_Motion = next;
		}
	}
}

void Bone::Repeat_Now_Motioin()
{
	this->repeat_Flag = true;
}

bool Bone::Is_Ok_to_Repeat()
{
	return (this->repeat_Flag == true && this->next_Motion == "" && repeat_Index != 0) ? true : false;
}

//�S���ɑ΂��Đ��ڂ���Ȃ��o�O������(2018/07/19)
void Bone::To_Standing(bool ASAP) const
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
		float sx = 0.0f;

		ML::Vec3 bone_Vec = this->joint[i]->Get_To_Bone().Normalize();
		
		//�{�[�����ƊO�ς�sin�l�����
		MyMath::Vector_Cross(&sx, bone_Vec, standing_Vec);

		//��]���錾
		ML::Vec3 anker;
		MyMath::Get_Normal_to_Vector_Cross(&anker, bone_Vec, standing_Vec);	
		//������]�����[���x�N�g���Ȃ玟�Ɉڂ�
		if (anker.Is_Zero_Vec())
		{
			continue;
		}

		//anker = anker.Normalize();
		
		//�֐߈���߂�����Ɏ��̊֐߂𐮓�
		if (abs(sx) <= abs(sinf(ML::ToRadian(1))))
		{
			continue;
		}
		else if (abs(sx) <= abs(sinf(ML::ToRadian(3))))
		{
			//�����ɂȂ��]����					
			//�N�H�[�^�j�I���錾
			ML::QT remainX = ML::QT(anker, asin(sx));
			//��]�s��錾
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, 1.0f, &this->joint[i]->Get_Pos(), &remainX, NULL);
			//��]
			this->joint[i]->Rotate_Bone(&matR, remainX);
			continue;
		}
		else
		{			
			//�c���]�ʂ̔�������]�������Ԃ�
			//�t���O�������Ă����1�t���[���ōς܂���
			float progress = 0.0f;
			ASAP ? progress = 1.0f : progress = 2.0f;
			//�N�H�[�^�j�I���錾
			ML::QT remainX = ML::QT(anker, asin(sx) / progress);
			//��]�s��錾
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, 1.0f, &this->joint[i]->Get_Pos(), &remainX, NULL);
			//��]
			this->joint[i]->Rotate_Bone(&matR, remainX);			
			if (!ASAP)
			{
				return;
			}
		}
	}
}

void Bone::Render() const
{
	//�֐ߑS�̂Ƀ����_�����O���߂���
	for (int i = 0; i < JOINT_ON_HUMAN; i++)
	{
		this->joint[i]->Render(this->tall);
	}
}

void Bone::Registrate_Motion(const std::vector<Motion::Motion_Data>& d, const string& motion_Name)
{
	//�����̃��[�V�����f�[�^�����m�F������ɓo�^����
	if (d.size() == 0)
	{
		return;
	}
	this->motions.insert({ motion_Name,d });
}