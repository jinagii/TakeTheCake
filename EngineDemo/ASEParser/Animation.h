#pragma once

#include "AlignedAllocationPolicy.h"
#include <SimpleMath.h>

/// Animation Data�� �����ϱ� ���� Ŭ����


class AnimationPos : public AlignedAllocationPolicy<16>
{
public:
	int m_Frame;
	Vector3 m_Pos;
};

class AnimationRot : public AlignedAllocationPolicy<16>
{
public:
	int m_Frame;
	Vector3 m_Rot;
	float m_Angle;
	Quaternion m_AccumulatedQTRot; // ��뺯���̱� ������ ���� ����� �����ؼ� ������ �ִ´� (�� ���ݱ����� ���뷮���� ��ȯ�Ѵ�)
};

class AnimationScl : public AlignedAllocationPolicy<16>
{
public:
	int m_time;
	Vector3 m_scale;
	Vector3 m_scaleaxis;
	float m_scaleaxisang;
	Quaternion m_scalerotQT_accumulation;
};


///--------------------------------------------------
/// �� Ŭ������ ������ Animation Ŭ����
///--------------------------------------------------
class Animation : public AlignedAllocationPolicy<16>
{
public:

	// �ִϸ��̼� �������� �̸�. ������ ���������̴�
	string m_NodeNameAni;
	string m_AnimationName; /// �ӽ�
	vector<AnimationPos*> m_Position_v;
	vector<AnimationRot*> m_Rotation_v;
	vector<AnimationScl*> m_Scale_v;

	int m_ticksperFrame;	// �� �������� ƽ ũ��. �̰��� AnimationTable�� ������ ������ ������Ʈ���� �ٸ� �� �ִ�.
	int m_minKeyframe;		// Ű �����Ӱ��� �ּ�ũ��. �̰��� ���ǰͰ� �� ��ġ������ �ʴ´�.
};