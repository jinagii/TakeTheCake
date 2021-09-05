#pragma once

#include "DXDefine.h"

/// RenderState들을 생성하는 클래스 하나씩만있으면된다
/// 2021.8.1 JinHak

class DXRenderState
{
public:
	static DXRenderState* GetInstance();
	~DXRenderState();

	static void Initialize(ID3D11Device* pDevice);
	
	// RenderState
	static ID3D11RasterizerState* g_WireFrame;
	static ID3D11RasterizerState* g_SolidFrame;
	static ID3D11DepthStencilState* g_NormalDSS;
	
	static ID3D11DepthStencilState* g_DepthStencilState;
	static ID3D11DepthStencilState* g_OffDepthStencilState;



private:
	DXRenderState() {};

	static DXRenderState* RenderState;

};

