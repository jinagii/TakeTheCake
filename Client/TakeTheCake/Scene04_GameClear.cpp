#include "pch.h"
#include "Texture2D.h"
#include "Scene04_GameClear.h"

Scene04_GameClear::Scene04_GameClear()
	: IScene()
	, m_texBG(nullptr)
	, m_sndBGM(nullptr)
{
}

Scene04_GameClear::~Scene04_GameClear()
{
	Release();
}

HRESULT Scene04_GameClear::Initialize()
{
	// �ؽ���2D BG ����
	m_texBG = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_CLEAR);

	// ���� ����
	m_sndBGM = SoundManager::GetInstance()->FindSound(SOUND_GAME_CLEAR);
	m_sndBGM->soundSeek(0);
	m_sndBGM->soundPlayLooping();


	return S_OK;
}

void Scene04_GameClear::Release()
{
	
}

void Scene04_GameClear::Update(float dTime)
{
	// �� ��ȯ �׽�Ʈ
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_SPACE))
	{
		// ���� ����
		m_sndBGM->soundSeek(0);
		m_sndBGM->soundPause();

		SceneManager::GetInstance()->ChangeScene(SCENE_01_INTRO);

		return;
	}

	m_texBG->Update(m_pCamera, dTime);
}

void Scene04_GameClear::Render()
{
	m_texBG->Render();

}
