#include "pch.h"
#include "Texture2D.h"
#include "Scene01_Intro.h"
#include "Scene02_CutScene.h"

Scene02_CutScene::Scene02_CutScene()
    : IScene()
	, m_State(SCENE_STATE::CutScene)
    , m_texBG(nullptr)
{
}

Scene02_CutScene::~Scene02_CutScene()
{
    Release();
}

HRESULT Scene02_CutScene::Initialize()
{
	m_State = SCENE_STATE::CutScene;

	/// UI BG ����
	m_texBG = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_CUTSCENE);

    return S_OK;
}

void Scene02_CutScene::Release()
{
}

void Scene02_CutScene::Update(float dTime)
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		if (m_State == SCENE_STATE::KetSet)
		{
			// ���� ����
			Scene01_Intro::sm_sndBGM->soundPause();

			// �ΰ��� ������ ����
			SceneManager::GetInstance()->ChangeScene(SCENE_03_INGAME);

			return;
		}

		/// Ű�� ���·� ����
		m_State = SCENE_STATE::KetSet;	
		m_texBG = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_KEYSET_2); // �̹��� ��ü
	}
	m_texBG->Update(m_pCamera, dTime);
}

void Scene02_CutScene::Render()
{
	m_texBG->Render();
}
