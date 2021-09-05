#pragma once

#include "AlignedAllocationPolicy.h"
#include <SimpleMath.h>

/// Animation Data를 저장하기 위한 클래스


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
	Quaternion m_AccumulatedQTRot; // 상대변량이기 때문에 이전 값들과 누적해서 가지고 있는다 (즉 지금까지의 절대량으로 변환한다)
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
/// 위 클래스를 조합한 Animation 클래스
///--------------------------------------------------
class Animation : public AlignedAllocationPolicy<16>
{
public:

	// 애니메이션 데이터의 이름. 유일한 참조수단이다
	string m_NodeNameAni;
	string m_AnimationName; /// 임시
	vector<AnimationPos*> m_Position_v;
	vector<AnimationRot*> m_Rotation_v;
	vector<AnimationScl*> m_Scale_v;

	int m_ticksperFrame;	// 한 프레임의 틱 크기. 이것은 AnimationTable을 가지는 각각의 오브젝트마다 다를 수 있다.
	int m_minKeyframe;		// 키 프레임간의 최소크기. 이것의 위의것과 꼭 일치하지는 않는다.
};