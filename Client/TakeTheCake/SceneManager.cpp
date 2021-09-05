#include "pch.h"
#include "DXObjectFactory.h"
#include "IScene.h"
#include "SceneManager.h"

SceneManager::SceneManager()
	: m_CurrentScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

HRESULT SceneManager::Initialize()
{
	// 초기화할 것이 있다면 여기서 한다

	return S_OK;
}

void SceneManager::Release()
{
	// iterator 설정
	umSceneListIter sceneListIter = m_SceneList.begin();

	for ( ; sceneListIter != m_SceneList.end(); ++sceneListIter)
	{
		// 씬 nullptr 체크
		if (sceneListIter->second != nullptr)
		{
			// 현재 돌아가는 씬일 경우
			if (sceneListIter->second == m_CurrentScene)
			{
				// 씬에서 해제해야할 데이터들 릴리즈
				sceneListIter->second->Release();
			}

			// 씬을 메모리에서 해제
			SAFE_DELETE(sceneListIter->second);

			// 리스트에서 제거
			m_SceneList.erase(sceneListIter);
		}
	}

	// 모든 요소 제거
	m_SceneList.clear();	// 위에서 erase를 해주지만 최후에 깔끔히?

}

void SceneManager::Update(const float dTime)
{
	if (m_CurrentScene) m_CurrentScene->Update(dTime);
}

void SceneManager::Render()
{
	if (m_CurrentScene) m_CurrentScene->Render();
}

IScene* SceneManager::AddScene(const std::string sceneName, IScene(* const scene) )
{
	// nullptr check
	if (!scene) return nullptr;

	m_SceneList.insert( { sceneName, scene } );

	return scene;
}

HRESULT SceneManager::ChangeScene(const std::string sceneName)
{
	umSceneListIter find = m_SceneList.find(sceneName);

	// Not found Scene
	if (find == m_SceneList.end()) return E_FAIL;

	// Try Change to current scene
	if (find->second == m_CurrentScene) return S_OK;

	// The initialize of the find scene succeesed
	if (SUCCEEDED( find->second->Initialize()) )
	{
		// 현재 씬에 다른 씬이 있다면 해당 씬을 Release 한다
		if (m_CurrentScene) m_CurrentScene->Release();

		// 바꾸려는 씬으로 변경한다
		m_CurrentScene = find->second;

		// Scene change succeese
		return S_OK;
	}

	// Scene change fail
	return E_FAIL;
}

IScene* SceneManager::GetCurrentScene() const
{
	return m_CurrentScene;
}
