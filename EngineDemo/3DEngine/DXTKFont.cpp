
#include "DXTKFont.h"

#define SafeDelete(x) { delete x; x = nullptr; }
#define Safe_Release(x) { if(x) { x->Release(); x = nullptr; } } // 예전 스타일

DXTKFont::DXTKFont()
	: m_pSpriteBatch(nullptr), m_pSpriteFont(m_pSpriteFont),
	m_pRasterizerState(nullptr), m_pDepthStencilState(nullptr)
{

}

DXTKFont::~DXTKFont()
{
	SafeDelete(m_pSpriteFont);
	SafeDelete(m_pSpriteBatch);
}

void DXTKFont::Initialize(ID3D11Device* pDevice, ID3D11RasterizerState* rs, ID3D11DepthStencilState* ds)
{

	ID3D11DeviceContext* _pDC = nullptr;
	pDevice->GetImmediateContext(&_pDC);

	m_pSpriteBatch = new DirectX::SpriteBatch(_pDC);

	TCHAR* _fileName = (TCHAR*)L"../../Library/Font/gulim9k.spritefont";

	m_pSpriteFont = new DirectX::SpriteFont(pDevice, _fileName);
	m_pSpriteFont->SetLineSpacing(14.0f);

	Safe_Release(_pDC);

	m_pRasterizerState = rs;
	m_pDepthStencilState = ds;
}

void DXTKFont::DrawTextTest()
{
	m_pSpriteBatch->Begin();

	m_pSpriteFont->DrawString(m_pSpriteBatch, L"Hi, Hello, 안녕, Font Test", Vector2(10.0f, 10.0f));

	m_pSpriteBatch->End();
}

void DXTKFont::DrawTextWithColor(int x, int y, XMFLOAT4 color, TCHAR* text, ...)
{
	TCHAR _buffer[1024] = L"";
	va_list _vl;
	va_start(_vl, text);
	_vstprintf(_buffer, 1024, text, _vl);
	va_end(_vl);

	m_pSpriteBatch->Begin();
	m_pSpriteFont->DrawString(m_pSpriteBatch, _buffer, 
									  XMFLOAT2((float)x, (float)y), 
									  Vector4(color));
	m_pSpriteBatch->End();
}
