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
	// �ʱ�ȭ�� ���� �ִٸ� ���⼭ �Ѵ�

	return S_OK;
}

void SceneManager::Release()
{
	// iterator ����
	umSceneListIter sceneListIter = m_SceneList.begin();

	for ( ; sceneListIter != m_SceneList.end(); ++sceneListIter)
	{
		// �� nullptr üũ
		if (sceneListIter->second != nullptr)
		{
			// ���� ���ư��� ���� ���
			if (sceneListIter->second == m_CurrentScene)
			{
				// ������ �����ؾ��� �����͵� ������
				sceneListIter->second->Release();
			}

			// ���� �޸𸮿��� ����
			SAFE_DELETE(sceneListIter->second);

			// ����Ʈ���� ����
			m_SceneList.erase(sceneListIter);
		}
	}

	// ��� ��� ����
	m_SceneList.clear();	// ������ erase�� �������� ���Ŀ� �����?

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
		// ���� ���� �ٸ� ���� �ִٸ� �ش� ���� Release �Ѵ�
		if (m_CurrentScene) m_CurrentScene->Release();

		// �ٲٷ��� ������ �����Ѵ�
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
