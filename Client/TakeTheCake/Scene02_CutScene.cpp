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

	/// UI BG 연결
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
			// 사운드 중지
			Scene01_Intro::sm_sndBGM->soundPause();

			// 인게임 신으로 변경
			SceneManager::GetInstance()->ChangeScene(SCENE_03_INGAME);

			return;
		}

		/// 키셋 상태로 변경
		m_State = SCENE_STATE::KetSet;	
		m_texBG = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_KEYSET_2); // 이미지 교체
	}
	m_texBG->Update(m_pCamera, dTime);
}

void Scene02_CutScene::Render()
{
	m_texBG->Render();
}
