#pragma once
#include <random>

/// <summary>
/// CommonMacro
/// Writer : 최 요 환
/// 
/// 흔히 사용할 법한 기능을 모아 보자
/// 
/// First Create	  : 2021.08.04
/// Last Modification : 2021.08.16 
/// </summary>
/// 
namespace CommonMacro
{
	// 랜덤 실수 생성 ( Min ~ Max )
	inline float MakeRandomFloat(const float Min, const float Max)
	{
		static std::random_device rd;
		static std::default_random_engine e(rd());

		static std::uniform_real_distribution<float> dis(Min, Max);

		// 랜덤 수 리턴
		return static_cast<float>( dis(e) );
	}

	// 두 점 사이의 거리 구하기
	inline float GetDistance(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		float d = (x2 - x1) * (x2 - x1)
			+ (y2 - y1) * (y2 - y1)
			+ (z2 - z1) * (z2 - z1);

		return sqrt(d);
	}

	//// 출력 정보
	//inline void DRTrace(TCHAR* text, ...)
	//{
	//	TCHAR _buffer[1024] = L"";
	//	va_list vl;
	//	va_start(vl, text);
	//	_vstprintf(_buffer, 1024, text, vl);
	//	va_end(vl);
	//
	//	OutputDebugString(_buffer);
	//	OutputDebugString(L"\n");
	//}

}
