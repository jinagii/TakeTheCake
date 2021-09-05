#pragma once

/// <summary>
/// Texture2D Class
/// Writer : YoKing
/// 
/// 게임에서 보여질 2D 이미지 텍스쳐 클래스
/// 
/// 
/// First Create	  : 2021.08.04
/// Last Modification : 2021.08.15
/// </summary>


class Texture2D 
{
public:
	Texture2D(class IDXObjectBase* pTexture2D);
	~Texture2D();

	void Initialize();

	void Release();
	void Update(class Camera* pCamera, float dTime);
	void Render();


protected:

private:
	/// DX 엔진 오브젝트
	class IDXObjectBase* m_pDxObject;	// Texture
};

