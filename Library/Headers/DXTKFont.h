#pragma once

#include "DXDefine.h"
#include <tchar.h>
#include "SpriteFont.h"
#include "SpriteBatch.h"

// DirectX tool kit
class DXTKFont
{
public:
	DXTKFont();
	~DXTKFont();

	void Initialize(ID3D11Device* pDevice, ID3D11RasterizerState* rs, ID3D11DepthStencilState* ds);

	void DrawTextTest();
	void DrawTextWithColor(int x, int y, XMFLOAT4 color, TCHAR* text, ...); // '...'이 뭐지? - 가변인자 

private:
	DirectX::SpriteBatch* m_pSpriteBatch;
	DirectX::SpriteFont* m_pSpriteFont;

	// 그냥 사용시 문제생김
	ID3D11RasterizerState* m_pRasterizerState;
	ID3D11DepthStencilState* m_pDepthStencilState;
};

