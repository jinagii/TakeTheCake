#pragma once

/// <summary>
/// SceneManager Class
/// Writer : YoKing
/// 
/// 씬의 추가, 제거, 변경 및 업데이트를 할 객체의 클래스
/// 
/// First Create	  : 2021.08.18
/// Last Modification : 2021.08.18
/// </summary>

#include "SingletonBase.h"

class SceneManager : public SingletonBase<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	HRESULT Initialize() override;
	void Release();
	void Update(const float dTime);
	void Render();

	// 씬 추가
	class IScene* AddScene(const std::string sceneName, IScene(* const scene) );

	// 씬 변경
	HRESULT ChangeScene(const std::string sceneName);

	// 현재 씬 반환
	class IScene* GetCurrentScene() const;

protected:

private:
	// using 선언으로 재정의
	using umSceneList = std::unordered_map<std::string, IScene*>;
	using umSceneListIter = std::unordered_map<std::string, IScene*>::iterator;
	umSceneList m_SceneList;		// 씬 리스트

	class IScene* m_CurrentScene;	// 현재 씬
};