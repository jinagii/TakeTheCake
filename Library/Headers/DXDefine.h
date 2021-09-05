#pragma once

#include <crtdbg.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"dxgi.lib")

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>
#include "dxerr.h"
#include <SimpleMath.h>

using namespace DirectX;
using namespace DirectX::PackedVector; // 사용하는것인가?
using namespace DirectX::SimpleMath;

using namespace std;

#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "LightHelper.h"
#include "MathHelper.h"

#include <winerror.h>



#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)																		\
	{																					\
		HRESULT hr = (x);															\
		if(FAILED(hr))																\
		{																				\
			LPWSTR output;                                 				\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |					\
							  FORMAT_MESSAGE_IGNORE_INSERTS |				\
							  FORMAT_MESSAGE_ALLOCATE_BUFFER,				\
							  NULL,													\
							  hr,														\
							  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
							  (LPTSTR) &output,									\
							  0,														\
							  NULL);													\
			MessageBox(NULL, output, L"Error", MB_OK);					\
		}																				\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

// COM 객체를 편하게 릴리즈하기 위해
#define ReleaseCOM(x) { if(x) { x->Release(); x = nullptr; } } // 예전 스타일
#define SafeDelete(x) { delete x; x = nullptr; }

//
//#ifndef D3DX11INLINE
//#ifdef _MSC_VER
//#if (_MSC_VER >= 1200)
//#define D3DX11INLINE __forceinline
//#else
//#define D3DX11INLINE __inline
//#endif
//#else
//#ifdef __cplusplus
//#define D3DX11INLINE inline
//#else
//#define D3DX11INLINE
//#endif
//#endif
//#endif
//
//
//class TextHelper
//{
//public:
//
//	template<typename T>
//	static D3DX11INLINE std::wstring ToString(const T& s)
//	{
//		std::wostringstream oss;
//		oss << s;
//
//		return oss.str();
//	}
//
//	template<typename T>
//	static D3DX11INLINE T FromString(const std::wstring& s)
//	{
//		T x;
//		std::wistringstream iss(s);
//		iss >> x;
//
//		return x;
//	}
//};
//
//
///// Utility class for converting between types and formats.
/////</summary>
//class Convert
//{
//public:
//	///<summary>
//	/// Converts XMVECTOR to XMCOLOR, where XMVECTOR represents a color.
//	///</summary>
//	static D3DX11INLINE XMCOLOR ToXmColor(FXMVECTOR v)
//	{
//		XMCOLOR dest;
//		XMStoreColor(&dest, v);
//		return dest;
//	}
//
//	///<summary>
//	/// Converts XMVECTOR to XMFLOAT4, where XMVECTOR represents a color.
//	///</summary>
//	static D3DX11INLINE Vector4 ToXmFloat4(FXMVECTOR v)
//	{
//		Vector4 dest;
//		XMStoreFloat4(&dest, v);
//		return dest;
//	}
//
//	static D3DX11INLINE UINT ArgbToAbgr(UINT argb)
//	{
//		BYTE A = (argb >> 24) & 0xff;
//		BYTE R = (argb >> 16) & 0xff;
//		BYTE G = (argb >> 8) & 0xff;
//		BYTE B = (argb >> 0) & 0xff;
//
//		return (A << 24) | (B << 16) | (G << 8) | (R << 0);
//	}
//
//};

#include <tchar.h>
#include <stdarg.h>
#include <stdio.h>

/// TRACE
inline void Trace(TCHAR* text, ...)
{
	TCHAR _buffer[1024] = L"";
	va_list vl;
	va_start(vl, text);
	_vstprintf(_buffer, 1024, text, vl);
	va_end(vl);

	OutputDebugString(_buffer);
	OutputDebugString(L"\n");
}