#include "pch.h"
#include "Texture2D.h"
#include "Scene03_InGame.h"
#include "Scene05_GameOver.h"

Scene05_GameOver::Scene05_GameOver()
    : IScene()
    , m_texBG(nullptr)
    , m_sndBGM(nullptr)
{
}

Scene05_GameOver::~Scene05_GameOver()
{
    Release();
}

HRESULT Scene05_GameOver::Initialize()
{
    // �ؽ���2D BG ����
    switch (Scene03_InGame::sm_GameMode)
    {
    case GAME_MODE::Failed:
        m_texBG = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_FAILED);
        break;

    case GAME_MODE::TimeOver:
        m_texBG = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_TIMEOVER);
        break;
    default:
        break;
    }
   

    // ���� ����
    m_sndBGM = SoundManager::GetInstance()->FindSound(SOUND_GAME_OVER);
    m_sndBGM->soundSeek(0);
    m_sndBGM->soundPlayLooping();

    return S_OK;
}

void Scene05_GameOver::Release()
{
}

void Scene05_GameOver::Update(float dTime)
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

void Scene05_GameOver::Render()
{
    m_texBG->Render();
}
