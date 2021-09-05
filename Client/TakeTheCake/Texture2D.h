#pragma once

/// <summary>
/// Texture2D Class
/// Writer : YoKing
/// 
/// ���ӿ��� ������ 2D �̹��� �ؽ��� Ŭ����
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
	/// DX ���� ������Ʈ
	class IDXObjectBase* m_pDxObject;	// Texture
};

