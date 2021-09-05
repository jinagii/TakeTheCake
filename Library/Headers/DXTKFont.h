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
	void DrawTextWithColor(int x, int y, XMFLOAT4 color, TCHAR* text, ...); // '...'�� ����? - �������� 

private:
	DirectX::SpriteBatch* m_pSpriteBatch;
	DirectX::SpriteFont* m_pSpriteFont;

	// �׳� ���� ��������
	ID3D11RasterizerState* m_pRasterizerState;
	ID3D11DepthStencilState* m_pDepthStencilState;
};

