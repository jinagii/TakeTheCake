#include "pch.h"
#include "DXDevice.h"
#include "IDXRenderer.h"
#include "DXObjectFactory.h"
#include "DXRenderState.h"
#include "DXTKFont.h"
#include "Camera.h"

#include "Texture2D.h"
#include "IObjectBase.h"
#include "ObjectManager.h"
#include "Scene01_Intro.h"

Sound* Scene01_Intro::sm_sndBGM = nullptr;

Scene01_Intro::Scene01_Intro()
	: IScene()
	, m_texIntro()
	, m_dTime(0.0f)
{
}

Scene01_Intro::~Scene01_Intro()
{
	Release();
}

HRESULT Scene01_Intro::Initialize()
{
	//////////////////////////////////////////////////////////

	/// UI 로드
	LoadUIObject();

	/// 디버그 오브젝트 생성
	LoadDebugObject();

	/// Sound 연결 및 재생
	sm_sndBGM = SoundManager::GetInstance()->FindSound(SOUND_TITLE_BGM);
	sm_sndBGM->soundSeek(0);
	sm_sndBGM->soundPlayLooping();

	return S_OK;
}

void Scene01_Intro::Release()
{
	/// 사운드 정지
	//sm_sndBGM->soundPause();
}

void Scene01_Intro::Update(float dTime)
{
	m_dTime = dTime;

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		// 사운드 중지
		//sm_sndBGM->soundPause();

		// 인게임 신으로 변경
		SceneManager::GetInstance()->ChangeScene(SCENE_02_CUTSCENE);

		return;
	}

	// UI 업데이트
	UIUpdate(dTime);

}

void Scene01_Intro::Render()
{
	/// UI 렌더
	UIRender();
}

void Scene01_Intro::LoadUIObject()
{
	/// <summary>
	/// UI는 UIManager 에서 관리하는 리스트에서 가져와서 멤버 변수에 할당하고
	/// 각각 필요한 시점에 렌더링 한다.
	/// </summary>

	

	m_texIntro[0] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_00);
	m_texIntro[1] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_01);
	m_texIntro[2] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_02);
	m_texIntro[3] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_03);
	m_texIntro[4] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_04);
	m_texIntro[5] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_05);
	m_texIntro[6] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_06);
	m_texIntro[7] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_07);
	m_texIntro[8] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_08);
	m_texIntro[9] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_09);
	m_texIntro[10] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_10);
	m_texIntro[11] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_11);
	m_texIntro[12] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_12);
	m_texIntro[13] = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_INTRO_13);
}

void Scene01_Intro::UIUpdate(float dTime)
{
	for (int i = 0; i < 13; i++)
	{
		m_texIntro[i]->Update(m_pCamera, dTime);
	}

	
}   

void Scene01_Intro::UIRender()
{
	static float timer = 0.05f;
	static int idx = 0;

	timer -= m_dTime;

	if (timer < 0.0f)
	{
		idx++;
		if (idx > 12)
		{
			idx = 0;
		}

		timer = 0.06f;
	}
	m_texIntro[idx]->Render();
}
