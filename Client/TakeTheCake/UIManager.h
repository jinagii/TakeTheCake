#pragma once

/// <summary>
/// UIManager Class
/// Writer : YoKing
/// 
/// 게임에서 사용할 Texture2D(이미지)를 모아 놓고 관리하는 크래스
/// UI 풀이 존재하고, 추가/제거 및 찾기가 가능
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

	// Texture2D 추가
	class Texture2D* AddTexture2D(const std::string textureName, const int x, const int y, const int width, const int height);

	// Texture2D 찾기
	class Texture2D* FindTexture2D(std::string textureName);

	// 리스트에서 모든 Texture2D 제거
	void DeleteAll();

private:
	// 초기화 여부
	bool m_IsInitialized;

	using MapTexture2DList = std::unordered_map<std::string, class Texture2D*>;
	using MapTexture2DIter = std::unordered_map<std::string, class Texture2D*>::iterator;

	// 텍스쳐 모아 두는 리스트
	MapTexture2DList m_Texture2DList;
};

