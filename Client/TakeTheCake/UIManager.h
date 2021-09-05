#pragma once

/// <summary>
/// UIManager Class
/// Writer : YoKing
/// 
/// ���ӿ��� ����� Texture2D(�̹���)�� ��� ���� �����ϴ� ũ����
/// UI Ǯ�� �����ϰ�, �߰�/���� �� ã�Ⱑ ����
/// 
/// First Create	  : 2021.08.18
/// Last Modification : 2021.08.18
/// </summary>

#include "SingletonBase.h"

class UIManager : public SingletonBase<UIManager>
{
public:
	UIManager();
	~UIManager();

	HRESULT Initialize() override;
	void Release();

	// Texture2D �߰�
	class Texture2D* AddTexture2D(const std::string textureName, const int x, const int y, const int width, const int height);

	// Texture2D ã��
	class Texture2D* FindTexture2D(std::string textureName);

	// ����Ʈ���� ��� Texture2D ����
	void DeleteAll();

private:
	// �ʱ�ȭ ����
	bool m_IsInitialized;

	using MapTexture2DList = std::unordered_map<std::string, class Texture2D*>;
	using MapTexture2DIter = std::unordered_map<std::string, class Texture2D*>::iterator;

	// �ؽ��� ��� �δ� ����Ʈ
	MapTexture2DList m_Texture2DList;
};

