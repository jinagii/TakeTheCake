#pragma once
#include <random>

/// <summary>
/// CommonMacro
/// Writer : �� �� ȯ
/// 
/// ���� ����� ���� ����� ��� ����
/// 
/// First Create	  : 2021.08.04
/// Last Modification : 2021.08.16 
/// </summary>
/// 
namespace CommonMacro
{
	// ���� �Ǽ� ���� ( Min ~ Max )
	inline float MakeRandomFloat(const float Min, const float Max)
	{
		static std::random_device rd;
		static std::default_random_engine e(rd());

		static std::uniform_real_distribution<float> dis(Min, Max);

		// ���� �� ����
		return static_cast<float>( dis(e) );
	}

	// �� �� ������ �Ÿ� ���ϱ�
	inline float GetDistance(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		float d = (x2 - x1) * (x2 - x1)
			+ (y2 - y1) * (y2 - y1)
			+ (z2 - z1) * (z2 - z1);

		return sqrt(d);
	}

	//// ��� ����
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
