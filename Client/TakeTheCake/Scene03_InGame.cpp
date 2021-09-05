#include "pch.h"

#include "IDXRenderer.h"
#include "DXObjectFactory.h"
#include "DXTKFont.h"
#include "Camera.h"
#include "DebugCircle.h"

#include "ObjectManager.h"
#include "Player.h"
#include "Flock.h"
#include "Follower.h"

// 기믹
#include "CupCake.h"
#include "Fire.h"
#include "Knife.h"
#include "Trap.h"

// 프랍
#include "Prop.h"

// 네비게이션 매쉬
#include "Navigation.h"
#include "ClNavigation.h"

#include "Texture2D.h"
#include "Scene03_InGame.h"

GAME_MODE Scene03_InGame::sm_GameMode = GAME_MODE::Play;
bool Scene03_InGame::sm_IsTimerStop = false;
float Scene03_InGame::sm_Timer = 0.0f;



int Scene03_InGame::sm_AntCount = 0;

ePlayState Scene03_InGame::m_GamePlayState = ePlayState::NoCake;

bool Scene03_InGame::m_bIsMapRender = true;

Scene03_InGame::Scene03_InGame()
   : IScene()
   , m_bIsDebug(false)
   , m_pNavigation(nullptr)
   , m_pObjectManager(nullptr)
   , m_pDebugObjectManager(nullptr)
   , m_pPlayer(nullptr)
   , m_dTime(0.0f)
   , m_CameraFlag(false)
   , m_texTime(nullptr)
   , m_texTimeNum_100(nullptr)
   , m_texTimeNum_10(nullptr)
   , m_texTimeNum_1(nullptr)
   , m_texAnt(nullptr)
   , m_texAntX(nullptr)
   , m_texAntNum(nullptr)
   , m_texAntCount()
   , m_texCupCake(nullptr)
   , m_sndBGM(nullptr)
{
   /// 내비메쉬 생성
   m_pNavigation = dynamic_cast<ClNavigation*>(ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::Navigation, 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.1f, 0.0f }));

   /// 오브젝트/디버그 매니저 생성
   m_pObjectManager = new ObjectManager();
   m_pDebugObjectManager = new ObjectManager();

   LoadObject();
   LoadUIObject();
   LoadDebugObject();

   for (auto antCount : m_texAntCount)
   {
      antCount = nullptr;
   }
}

Scene03_InGame::~Scene03_InGame()
{
   Release();

   ReleaseCOM(m_pObjectManager);
   SAFE_DELETE(m_pObjectManager);

   ReleaseCOM(m_pDebugObjectManager);
   SAFE_DELETE(m_pDebugObjectManager);
}

HRESULT Scene03_InGame::Initialize()
{
   //////////////////////////////////////////////////////////
   sm_GameMode = GAME_MODE::Play;
   m_bIsDebug = false;
   sm_IsTimerStop = false;
   m_bIsMapRender = true;

   /// 오브젝트 리셋
   m_pObjectManager->Initialize();

   /// 타이머 초기화
   sm_Timer = 200.0f;

   /// 케이크를 집었나 안 집었나를 초기화 해 주자.
   m_GamePlayState = ePlayState::NoCake;

   /// 사운드 BGM 연결
   //m_sndBGM = SoundManager::GetInstance()->FindSound(SOUND_INGAME_BGM);
   //m_sndBGM->soundSeek(0);
   //m_sndBGM->soundPlayLooping();
   this->ChangeBGM(m_GamePlayState);

   // 플레이어의 상태를 초기화해준다.
   m_pPlayer->OnStandBy();

   // 구출한 개미 수 0으로 초기화
   sm_AntCount = 0;

   return S_OK;
}

void Scene03_InGame::Release()
{

}

void Scene03_InGame::Update(float dTime)
{
   m_dTime = dTime;

   // T 누르면 타이머 상태 전환
   if (KeyManager::GetInstance()->IsOnceKeyDown('T'))
   {
      sm_IsTimerStop = !sm_IsTimerStop;
   }

   if (sm_IsTimerStop == false)
   {
      /// 타이머 업데이트
      TimerUpdate();
   }

   if (KeyManager::GetInstance()->IsOnceKeyDown(VK_TAB))
   {
      m_bIsDebug = !m_bIsDebug;
   }

   /// Object 업데이트
   m_pObjectManager->Update(m_pCamera, dTime);

   /// UI Object 업데이트
   UpdateUI(dTime);

   /// DebugObject 업데이트
   m_pDebugObjectManager->Update(m_pCamera, dTime);


   /// 씬 전환 체크
   if (KeyManager::GetInstance()->IsStayKeyDown(VK_F1))
   {
      // 사운드 중지
      m_sndBGM->soundPause();

      SceneManager::GetInstance()->ChangeScene(SCENE_04_CLEAR);

      return;
   }

   switch (sm_GameMode)
   {
      case GAME_MODE::Play:

         break;
      case GAME_MODE::Clear:
         // 사운드 중지
         m_sndBGM->soundSeek(0);
         m_sndBGM->soundPause();
         SceneManager::GetInstance()->ChangeScene(SCENE_04_CLEAR);
         return;

      case GAME_MODE::Failed:
      case GAME_MODE::TimeOver:
         // 사운드 중지
         m_sndBGM->soundSeek(0);
         m_sndBGM->soundPause();
         SceneManager::GetInstance()->ChangeScene(SCENE_05_OVER);
         return;

      default:
         break;
   }

   /// 타이머가 다 된 경우
   if (sm_Timer <= 0.0f)
   {
      // 사운드 중지
      m_sndBGM->soundPause();

      // 타임오버 씬으로
      SceneManager::GetInstance()->ChangeScene(SCENE_05_OVER);

      return;
   }

   /// 플레이어가 사망한 경우
   Player* player = dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"));
   if (player->IsDead())
   {
      static float deadTimer = 3.0f;

      deadTimer -= m_dTime;

      if (deadTimer < 0.0f)
      {
         sm_GameMode = GAME_MODE::Failed;

         // 사운드 중지
         m_sndBGM->soundPause();

         SceneManager::GetInstance()->ChangeScene(SCENE_05_OVER);
      }

      return;
   }

   // 리셋
   if (KeyManager::GetInstance()->IsOnceKeyDown('R'))
   {
      Initialize();
      return;
   }

   if (KeyManager::GetInstance()->IsOnceKeyDown('Z'))
   {
      if (sm_AntCount < 10)
      {
         sm_AntCount += 1;

         AddInGameTimer(10.0f);
      }
   }

   if (KeyManager::GetInstance()->IsOnceKeyDown('X'))
   {
      if (sm_AntCount > 0)
      {
         sm_AntCount -= 1;
      }
   }




   if (KeyManager::GetInstance()->IsOnceKeyDown('O'))
   {
      player->SetPos({ 20.35f, 20.015f, 23.472f });
   }

   if (KeyManager::GetInstance()->IsOnceKeyDown('P'))
   {
      player->SetPos({ 15.0f, 20.1f, -62.0f });
   }

   /// 카메라 모드 변경
   if (KeyManager::GetInstance()->IsOnceKeyDown('C'))
   {
      m_CameraFlag = !m_CameraFlag;
   }

   /// 카메라 이동
   CameraMove(dTime);	// W, A, S, D

   /// 맵(Prop) 렌더링 전환
   if (KeyManager::GetInstance()->IsOnceKeyDown('M'))
   {
      m_bIsMapRender = !m_bIsMapRender;
   }

}

void Scene03_InGame::Render()
{
   // 디버깅 모드 true 일 떄만
   if (m_bIsDebug)
   {
      /// 네비게이션 매쉬 렌더
      m_pNavigation->Render();
   }

   /// Object 렌더
   m_pObjectManager->Render();

   /// Ui 렌더
   RenderUI();

   // 디버깅 모드 true 일 떄만
   if (m_bIsDebug)
   {
      /// 디버그 정보 렌더
      // 디버그 오브젝트 렌더
      m_pDebugObjectManager->Render();

      // 텍스트 렌더
      ShowDebugInformation(m_dTime);
   }
}

void Scene03_InGame::LoadObject()
{
   IObjectBase* object;

   /// 플레이어 생성
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PLAYER, 0.3f, 0.0f, 0.0f, 0.0f, { 5.0f, 20.1f, -62.0f });	/// 시작 포지션 절대 변경 하면 안된다.

   m_pObjectManager->AddObject("Player", object);
   m_pPlayer = dynamic_cast<Player*>(object);
   m_pPlayer->SetNavigationMesh(m_pNavigation);	// 플레이어를 받아놓는다.


   /// Flock 생성
   //object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::FLOCK);
   // 무리에 타겟 설정
   //dynamic_cast<Flock*>(object)->SetTarget(m_pObjectManager->FindObject("Player"));
   //m_pObjectManager->AddObject("Flock", object);

   /// 맵(프랍)
   LoadProp();

   ///////////////////////////////////////////////////////////////////////////

   /// 기믹

   ///////////////////////////////////////////////////////////////////////////
   /// 컵케익
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::CUPCAKE, 1.0f, 0.0f, 0.0f, 0.0f, { 29.35f, 20.015f, 22.472f });
   dynamic_cast<CupCake*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("CupCake_1", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::CUPCAKE, 1.0f, 0.0f, 0.0f, 0.0f, { 33.796f, 20.105f, 18.879f });
   dynamic_cast<CupCake*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("CupCake_2", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::CUPCAKE, 1.0f, 0.0f, 0.0f, 0.0f, { 23.163f, 20.105f, 17.971f });
   dynamic_cast<CupCake*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("CupCake_3", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::CUPCAKE, 1.0f, 0.0f, 0.0f, 0.0f, { 26.216f, 20.105f, 14.477f });
   dynamic_cast<CupCake*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("CupCake_4", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::CUPCAKE, 1.0f, 0.0f, 0.0f, 0.0f, { 32.318f, 20.105f, 12.834f });
   dynamic_cast<CupCake*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("CupCake_5", object);

   /// 불
   // 불 1
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::FIRE_A, 3.5f, 0.0f, 0.0f, 0.0f, { 60.25f, 19.4f, -6.0f });
   dynamic_cast<Fire*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("OvenFire_1", object);

   // 불 2
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::FIRE_B, 2.6f, 0.0f, 0.0f, 0.0f, { 60.3f, 19.4f, 5.0f });
   dynamic_cast<Fire*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("OvenFire_2", object);

   // 불 3
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::FIRE_C, 1.7f, 0.0f, 0.0f, 0.0f, { 67.16f, 19.4f, 5.2f });
   dynamic_cast<Fire*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("OvenFire_3", object);

   // 불 4
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::FIRE_D, 1.5f, 0.0f, 0.0f, 0.0f, { 67.3f, 19.4f, -5.68f });
   dynamic_cast<Fire*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("OvenFire_4", object);

   /// 나이프
   // 나이프 1
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::KNIFE, 3.3f, 0.0f, 0.0f, 0.0f, { 44.705, 20.031f, -64.07f });
   dynamic_cast<Knife*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("Knife_1", object);

   // 나이프 2
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::KNIFE, 3.3f, 0.0f, 0.0f, 0.0f, { 39.262f, 20.031f, -61.318f });
   dynamic_cast<Knife*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("Knife_2", object);

   // 나이프 3
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::KNIFE, 3.3f, 0.0f, 0.0f, 0.0f, { 57.099f, 20.031f, -25.158f });
   dynamic_cast<Knife*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("Knife_3", object);

   // 나이프 4
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::KNIFE, 3.3f, 0.0f, 0.0f, 0.0f, { 62.721f, 20.031f, 22.547f });
   dynamic_cast<Knife*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("Knife_4", object);

   // 나이프 5
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::KNIFE, 3.3f, 0.0f, 0.0f, 0.0f, { 56.0034, 20.031f, -9.77029f });
   dynamic_cast<Knife*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("Knife_5", object);

   // 나이프 6
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::KNIFE, 3.3f, 0.0f, 0.0f, 0.0f, { 65.0034, 20.031f, 0.453563f });
   dynamic_cast<Knife*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("Knife_6", object);

   /// 트랩
   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 16.142f, 20.05f, -70.8f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   //dynamic_cast<Trap*>(object)->Set
   m_pObjectManager->AddObject("trap_1", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 21.896, 20.05f, -54.25f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_2", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 19.364f, 20.05f, -65.462f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_3", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 28.894f, 20.05f, -60.345f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_4", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 64.5f, 20.05f, -53.954f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_5", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 71.235f, 20.05f, -49.527f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_6", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 72.443f, 20.05f, -39.27f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_7", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 71.74f, 20.05f, -15.013f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_8", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 66.745f, 20.05f, 13.971f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_9", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 57.339f, 20.05f, 28.303f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_10", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 72.9127, 20.05f, 44.621 });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_11", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 52.659f, 20.05f, 46.301f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_12", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 22.231f, 20.05f, 40.826f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_13", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 21.025f, 20.05f, 23.469f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_14", object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::TRAP, 1.4f, 0.0f, 0.0f, 0.0f, { 30.395f, 20.05f, 10.176f });
   dynamic_cast<Trap*>(object)->SetTarget(m_pPlayer);
   m_pObjectManager->AddObject("trap_15", object);

}

void Scene03_InGame::LoadDebugObject()
{
   IObjectBase* debugObject;

   // 축 생성
   debugObject = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::AXIS);
   m_pDebugObjectManager->AddObject("Axis", debugObject);

   // 그리드 생성
   debugObject = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::GRID);
   m_pDebugObjectManager->AddObject("Grid", debugObject);
}

void Scene03_InGame::LoadUIObject()
{
   m_texTime = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_TIME);

   m_texTimeNum_100 = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_TIMENUM_100_0);
   m_texTimeNum_10 = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_TIMENUM_10_0);
   m_texTimeNum_1 = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_TIMENUM_1_0);

   m_texAnt = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_ANT);
   m_texAntX = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_ANTNUM_X);
   m_texAntNum = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_ANTNUM_1_5);


   // 하단 구출한 개미 UI
   for (int i = 0; i <= 9; i++)
   {
      std::string strAntCount = "ant_count_ant_";
      strAntCount += to_string(i + 1);
      strAntCount += "_off";

      m_texAntCount[i] = UIManager::GetInstance()->FindTexture2D(strAntCount);
   }

   // 컵케이크
   m_texCupCake = UIManager::GetInstance()->FindTexture2D(TEXTURE2D_ANTCOUNT_CUPCAKE);
}

void Scene03_InGame::LoadProp()
{
   IObjectBase* object;

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_APPLE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_APPLE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_AVOCADO, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_AVOCADO, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_AVOCADO_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_AVOCADO_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_AVOCADOSEED, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_AVOCADOSEED, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_BOTTOM, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_BOTTOM, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_BOTTOM_DOOR, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_BOTTOM_DOOR, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_CABIENT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_CABIENT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_DOOR, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_DOOR, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_DOOR_GLASS, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_DOOR_GLASS, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_KITCHEN, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_KITCHEN, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_REFRIGERATOR, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_REFRIGERATOR, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_TABLE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_TABLE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_TABLE_CLOTH, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_TABLE_CLOTH, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOARD, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOARD, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_1_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_1_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_1_02, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_1_02, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_1_03, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_1_03, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_1_04, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_1_04, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_1_05, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_1_05, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_1_06, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_1_06, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BREAD_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BREAD_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BREAD_1_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BREAD_1_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BREAD_3, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BREAD_3, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BREAD_3_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BREAD_3_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BREAD_4, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BREAD_4, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BUTTER, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BUTTER, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CANDY_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CANDY_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CANDY_1_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CANDY_1_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CANDY_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CANDY_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CANDY_2_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CANDY_2_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CANDY_3, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CANDY_3, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CANDY_3_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CANDY_3_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CANDYROLL, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CANDYROLL, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CANDYROLL_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CANDYROLL_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CARROT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CARROT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHEESE_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHEESE_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHEESE_1_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHEESE_1_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHEESE_1_02, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHEESE_1_02, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHEESE_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHEESE_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHEESE_2_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHEESE_2_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHOCOBAR, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHOCOBAR, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHOCOBAR_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHOCOBAR_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHOCOLATE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHOCOLATE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_COFFEEGRINDER, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_COFFEEGRINDER, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CUP_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CUP_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CUP_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CUP_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CUTMUSHROOM_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CUTMUSHROOM_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CUTMMUSHROOM, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CUTMMUSHROOM, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_DISH_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_DISH_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_DISH_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_DISH_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_DISH_2_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_DISH_2_01, object);


   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_DISH_2_02, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_DISH_2_02, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_DISH_3, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_DISH_3, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_DISH_3_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_DISH_3_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_EGGPLANT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_EGGPLANT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLOUR, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLOUR, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLOWERPOT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLOWERPOT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLOWERVASE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLOWERVASE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FORK, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FORK, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_JAM, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_JAM, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_LADLE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_LADLE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_LID, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_LID, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_M_PLATES, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_M_PLATES, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_M_WOODEN_PLATE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_M_WOODEN_PLATE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MICROWAVE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MICROWAVE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MILK, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MILK, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MIXERMACHINE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MIXERMACHINE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MIXERBALL, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MIXERBALL, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_NAIL_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_NAIL_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_NAIL_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_NAIL_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_NAIL_3, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_NAIL_3, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_OVEN, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_OVEN, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_OVEN_BUTTON_A, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_OVEN_BUTTON_A, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_OVEN_BUTTON_B, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_OVEN_BUTTON_B, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_OVEN_GLASS, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_OVEN_GLASS, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_OVEN_TOP, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_OVEN_TOP, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_POT_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_POT_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_POT_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_POT_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_POT_3, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_POT_3, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_PUMKIN, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_PUMKIN, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SAUCE_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SAUCE_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SAUCE_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SAUCE_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SINK, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SINK, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SOUP_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SOUP_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SOUP_1_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SOUP_1_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SOUP_1_02, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SOUP_1_02, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SOUP_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SOUP_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SOUP_2_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SOUP_2_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SATULA_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SATULA_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SPATULA_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SPATULA_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SPOONHOLDER, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SPOONHOLDER, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_START_TABLE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_START_TABLE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TEAPOT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TEAPOT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOASTER, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOASTER, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOP_A, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOP_A, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOP_B, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOP_B, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOP_TOP_DOOR, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOP_TOP_DOOR, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOP_TULIP, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOP_TULIP, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOP_WHISK, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOP_WHISK, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOP_WINDOW, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOP_WINDOW, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOP_WINE_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOP_WINE_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOP_WINE_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOP_WINE_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_UMBRELLA, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_UMBRELLA, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TOYCAR, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TOYCAR, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_TINCASE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_TINCASE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SFLOWERPOT_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SFLOWERPOT_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SFLOWERPOT_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SFLOWERPOT_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_ROADBOARD, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_ROADBOARD, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_RADIO, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_RADIO, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_POTATO_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_POTATO_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_POTATO_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_POTATO_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_POTATO_3, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_POTATO_3, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_PLANT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_PLANT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_PARCEL_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_PARCEL_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_PARCEL_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_PARCEL_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_OVENMITT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_OVENMITT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MEMO_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MEMO_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MEMO_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MEMO_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MAT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MAT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_LFLOWERPOT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_LFLOWERPOT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_JAR, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_JAR, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_ICEBOX, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_ICEBOX, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FRAME, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FRAME, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FORK_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FORK_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_3, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_3, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_4, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_4, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_5, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_5, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_6, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_6, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_7, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_7, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_8, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_8, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_9, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_9, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_10, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_10, object);


   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_11, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_11, object);


   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_12, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_12, object);


   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_13, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_13, object);


   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_FLAG_14, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_FLAG_14, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CUP_1_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CUP_1_01, object);


   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CRUET_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CRUET_1, object);


   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CRUET_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CRUET_2, object);


   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_COFFEECUP_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_COFFEECUP_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_COFFEECUP_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_COFFEECUP_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CLOTH, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CLOTH, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CLOCK, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CLOCK, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHOCOLATE_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHOCOLATE_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHOCOBAR_02, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHOCOBAR_02, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHAIR_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHAIR_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CHAIR_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CHAIR_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CEILINGFAN, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CEILINGFAN, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CASE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CASE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CARROT_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CARROT_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CARPET, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CARPET, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CACTUS, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CACTUS, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BROOM, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BROOM, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOKCABINET_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOKCABINET_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOKCABINET_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOKCABINET_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOKCABINET_2_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOKCABINET_2_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOKCABINET_3, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOKCABINET_3, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_1_07, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_1_07, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_1_08, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_1_08, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BOOK_2_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BOOK_2_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_ANTBOID, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_ANTBOID, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_BG_CABINET, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_BG_CABINET, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_HANGINGPLANT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_HANGINGPLANT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_WATERINGCAN, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_WATERINGCAN, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_PARTYFLAG, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_PARTYFLAG, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SHELF, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SHELF, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_COOKIETRAY, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_COOKIETRAY, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SPRAYCREAM, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SPRAYCREAM, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MEMO_3_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MEMO_3_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MEMO_3_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MEMO_3_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_PHONE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_PHONE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CUP_2_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CUP_2_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CUP_2_02, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CUP_2_02, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_HANGINGPLANT, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_HANGINGPLANT, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_WATERINGCAN, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_WATERINGCAN, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_PARTYFLAG, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_PARTYFLAG, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SHELF, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SHELF, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_COOKIETRAY, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_COOKIETRAY, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_SPRAYCREAM, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_SPRAYCREAM, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MEMO_3_1, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MEMO_3_1, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_MEMO_3_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_MEMO_3_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_PHONE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_PHONE, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CUP_2_01, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CUP_2_01, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_CUP_2_02, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_CUP_2_02, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_N_CANDY, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_N_CANDY, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_N_PLASTICBOX, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_N_PLASTICBOX, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_N_SOUP, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_N_SOUP, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_N_SOUP_2, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_N_SOUP_2, object);

   object = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::PROP);
   dynamic_cast<Prop*>(object)->SetMesh(ObjectFactory::GetInstance()->CreateMesh(PROP_N_ETCKNIFE, 1.0f, 0.0f, 0.0f, 0.0f, Vector3::Zero));
   m_pObjectManager->AddObject(PROP_N_ETCKNIFE, object);
   //*/
}

void Scene03_InGame::ShowDebugInformation(float dTime)
{
   Vector4 _black(0.f, 0.f, 0.f, 1.f);
   Vector4 _red(1.f, 0.f, 0.f, 1.f);

   int _yPos = 50;
   m_pFont->DrawTextWithColor(0, _yPos, _black, (TCHAR*)L"%d / %f / %s", 55, 3.14f, L"Debug Test");

   // 피쳐레벨
   m_pFont->DrawTextWithColor(0, _yPos += 28, _black, (TCHAR*)L"Feature Level : %x", DXDevice::g_FeatureLevel);

   // 클라이언트 영역의 해상도
   m_pFont->DrawTextWithColor(0, _yPos += 28, _black, (TCHAR*)L"Client Resolution: %dx%d", DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight);

   // 카메라 정보
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"WASD/QE : 카메라 조정");
   m_pFont->DrawTextWithColor(0, _yPos += 28, _black, (TCHAR*)L"Camera Pos : %.2f / %.2f / %.2f", m_pCamera->GetPosition().x, m_pCamera->GetPosition().y, m_pCamera->GetPosition().z);

   // 기타 정보 추후 입력
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"SPACE BAR : 모델 전환 / 방향키 : 모델 위치 변경");
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"1 ~ 7 : Light Effect 변경");

   // 오브젝트 이름
   /// string to const wchar_t로 바꾸는거 아래 처럼 하면된다
   wstring str;
   //str.assign(m_ObjectsIter->first.begin(), m_ObjectsIter->first.end());
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"현재 모델명 : %s", str.c_str());

   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"현재 dTime : %f", dTime);

   // 플레이어의 Hp
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"Hp : %d", dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"))->GetHp());

   // 이동, 회전 속도
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"MovePower : %f", dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"))->GetMovePower());
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"RotatePower : %f", dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"))->GetRotatePower());

   ////////////////////////////////////////////////////////////////////////////////////////////

   int time = static_cast<int>(sm_Timer);

   /// 인게임 타이머 숫자 분리
   int _digit100 = time / 100;	// 100의 자리 분리

   time %= 100;
   int _digit10 = time / 10;

   time %= 10;
   int _digit1 = time;

   // 타이머
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"현재 남은 시간 : %f", sm_Timer);
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"100의 자리 : %d", _digit100);
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L" 10의 자리 : %d", _digit10);
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"  1의 자리 : %d", _digit1);

   /////////////////////////////////////////////////////////////////////////////////////////////
}

void Scene03_InGame::CameraMove(const float dTime)
{
   m_pCamera->UpdateViewMatrix();

   /// 카메라 자유 이동 모드
   if (m_CameraFlag)
   {
      // 카메라 조작
      if (KeyManager::GetInstance()->IsStayKeyDown('W'))
         m_pCamera->MoveForward(10.0f * dTime);
      if (KeyManager::GetInstance()->IsStayKeyDown('S'))
         m_pCamera->MoveForward(-10.0f * dTime);
      if (KeyManager::GetInstance()->IsStayKeyDown('A'))
         m_pCamera->MoveSide(-10.0f * dTime);
      if (KeyManager::GetInstance()->IsStayKeyDown('D'))
         m_pCamera->MoveSide(10.0f * dTime);
      if (KeyManager::GetInstance()->IsStayKeyDown('Q'))
         m_pCamera->MoveUpDown(-10.0f * dTime);
      if (KeyManager::GetInstance()->IsStayKeyDown('E'))
         m_pCamera->MoveUpDown(10.0f * dTime);
   }

   /// 카메라 캐릭터 팔로잉 모드
   else
   {
      /// 플레이어가 없는 경우
      if (m_pObjectManager->FindObject("Player") == nullptr)
      {
         // 아무것도 하지말고 리턴
         return;
      }

      float x = dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"))->GetMesh()->GetTransTM()._41;
      float y = dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"))->GetMesh()->GetTransTM()._42 + 1;
      float z = dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"))->GetMesh()->GetTransTM()._43;
      Vector3 _objPos = { x, y, z };

      Matrix _forwardRotMat = XMMatrixRotationY(m_pObjectManager->FindObject("Player")->GetAngle());

      /// 의미 없는 코드
      /*
      Vector3 _cameraPos = XMVector3TransformNormal({ _objPos.x, _objPos.y, _objPos.z }, _forwardRotMat); // 카메라 회전 - 오브젝회전만큼
      Vector3 _cameraVec;

      _cameraVec = dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"))->GetMesh()->GetForwardVec() * 7;
      _cameraVec.x *= -1;
      _cameraVec.y += +1.5f;
      _cameraVec.z *= -1;

      _cameraVec += _objPos;

      m_pCamera->SetPosition(_cameraPos);	/// 아래에서 덮어씌운다.
      m_pCamera->LookAt(_cameraVec, _objPos, { 0.0f, 1.0f, 0.0f });
      */

      Player* _nowPlayer = dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"));

      if (_nowPlayer != nullptr)
      {
         Vector3 _cameraPos = _objPos + _nowPlayer->GetOffsetVec();

         m_pCamera->LookAt(_cameraPos, _objPos, { 0.0f, 1.0f, 0.0f });
      }
      else
      {
         // 플레이어가 존재하지 않을 때, 카메라를 어떻게 해야 한다.
      }

   }
}

void Scene03_InGame::SetGameMode(const GAME_MODE gameMode)
{
   sm_GameMode = GAME_MODE::Clear;
}

void Scene03_InGame::OnMouseDown(HWND hWnd, int x, int y)
{
   sm_PrevMousePos.x = x;
   sm_PrevMousePos.y = y;
   SetCapture(hWnd);
}

void Scene03_InGame::OnMouseUp(int x, int y)
{
   ReleaseCapture();
}

void Scene03_InGame::OnMouseMove(int btnState, int x, int y)
{
   if ((btnState & MK_RBUTTON) != 0)
   {
      // Make each pixel correspond to a quarter of a degree.
      float _dx = XMConvertToRadians(0.25f * static_cast<float>(x - sm_PrevMousePos.x));
      float _dy = XMConvertToRadians(0.25f * static_cast<float>(y - sm_PrevMousePos.y));

      m_pCamera->Pitch(_dy);
      m_pCamera->RotateY(_dx);
   }

   sm_PrevMousePos.x = x;
   sm_PrevMousePos.y = y;
}

void Scene03_InGame::OnMouseWheel(SHORT wheelDelta)
{
   m_pPlayer->SetCameraZoom(wheelDelta);
}

void Scene03_InGame::UpdateUI(float dTime)
{
   m_texTime->Update(m_pCamera, dTime);

   m_texTimeNum_100->Update(m_pCamera, dTime);
   m_texTimeNum_10->Update(m_pCamera, dTime);
   m_texTimeNum_1->Update(m_pCamera, dTime);

   m_texAnt->Update(m_pCamera, dTime);
   m_texAntX->Update(m_pCamera, dTime);
   m_texAntNum->Update(m_pCamera, dTime);

   // 구출한 개미수 만큼 On으로 교체
   for (int i = 0; i < sm_AntCount; i++)
   {
      if (i >= 10)
      {
         break;	// 배열 인덱스 오버플로우 버그 예방
      }

      std::string str = "ant_count_ant_";
      str += to_string(i + 1);

      str += "_on";

      m_texAntCount[i] = UIManager::GetInstance()->FindTexture2D(str);
   }

   for (int i = sm_AntCount; i < 10; i++)
   {
      std::string str = "ant_count_ant_";
      str += to_string(i + 1);

      str += "_off";

      m_texAntCount[i] = UIManager::GetInstance()->FindTexture2D(str);
   }

   for (auto antCountUI : m_texAntCount)
   {
      antCountUI->Update(m_pCamera, dTime);
   }

   // 라이프 개수 갱신
   Player* player = dynamic_cast<Player*>(m_pObjectManager->FindObject("Player"));
   int currentHp = player->GetHp();
   std::string str = "num_UI_1_";
   str += to_string(currentHp);
   m_texAntNum = UIManager::GetInstance()->FindTexture2D(str);


   m_texCupCake->Update(m_pCamera, dTime);
}

void Scene03_InGame::RenderUI()
{
   /// 타이머 렌더
   TimerRender();

   /// 개미 수(hp) 렌더
   m_texAnt->Render();
   m_texAntX->Render();
   m_texAntNum->Render();

   for (auto antCount : m_texAntCount)
   {
      antCount->Render();
   }

   if (sm_AntCount >= 10)
   {
      m_texCupCake->Render();
   }
}

void Scene03_InGame::TimerUpdate()
{
   if (m_GamePlayState == ePlayState::PickUpCake)
   {
      sm_Timer -= m_dTime;

      if (sm_Timer < 0.0f)
      {
         sm_GameMode = GAME_MODE::TimeOver;
      }

      int time = static_cast<int>(sm_Timer);

      /// 인게임 타이머 숫자 분리
      int _digit100 = time / 100;	// 100의 자리 분리

      time %= 100;
      int _digit10 = time / 10;

      time %= 10;
      int _digit1 = time;

      /// UI 숫자 변경
      // 100의 자리
      std::string str100 = "time_num_100_" + to_string(_digit100);
      m_texTimeNum_100 = UIManager::GetInstance()->FindTexture2D(str100);

      // 10의 자리
      std::string str10 = "time_num_10_" + to_string(_digit10);
      m_texTimeNum_10 = UIManager::GetInstance()->FindTexture2D(str10);

      // 1의 자리
      std::string str1 = "time_num_1_" + to_string(_digit1);
      m_texTimeNum_1 = UIManager::GetInstance()->FindTexture2D(str1);
   }
}

void Scene03_InGame::TimerRender()
{
   if (m_GamePlayState == ePlayState::PickUpCake)
   {
      m_texTime->Render();

      m_texTimeNum_100->Render();
      m_texTimeNum_10->Render();
      m_texTimeNum_1->Render();
   }
}

void Scene03_InGame::AddInGameTimer(const float val)
{
   // 컵케익 들었을 때만
   if (m_GamePlayState == ePlayState::PickUpCake)
   {
      // 타이머 증가
      sm_Timer += val;
   }
}

// 개미 구출 시 실제로 개미 오브젝트를 생성
void Scene03_InGame::SpawnAnt()
{
   IObjectBase* _newObject = ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::FOLLOWER,
      0.0f, 0.0f, 0.0f, 0.0f, m_pPlayer->GetPos());

   Follower* _newFollower = dynamic_cast<Follower*>(_newObject);
   _newFollower->SetTarget(m_pPlayer);

   std::string _followerName = "Follower_" + ConvertNumberToString(sm_AntCount);
   m_pObjectManager->AddObject(_followerName, _newFollower);
}

void Scene03_InGame::ChangeBGM(ePlayState playState)
{
   if (playState == ePlayState::NoCake)
   {
      /// 사운드 BGM 연결
      if (m_sndBGM != nullptr)
      {
         m_sndBGM->soundPause();
      }

      m_sndBGM = SoundManager::GetInstance()->FindSound(SOUND_INGAME_BGM);
      m_sndBGM->soundSeek(0);
      m_sndBGM->soundPlayLooping();
   }
   else
   {
      if (m_sndBGM != nullptr)
      {
         m_sndBGM->soundPause();
      }

      m_sndBGM = SoundManager::GetInstance()->FindSound(SOUND_INGAME_PICKUP_BGM);
      m_sndBGM->soundSeek(0);
      m_sndBGM->soundPlayLooping();
   }
}

void Scene03_InGame::OnPickCakeUp()
{
   m_pPlayer->OnPickCakeUp();
}

void Scene03_InGame::OnStandBy()
{
   m_pPlayer->OnStandBy();
}

std::string Scene03_InGame::ConvertNumberToString(int number)
{
   switch (number)
   {
      case 0: return "0";
      case 1: return "1";
      case 2: return "2";
      case 3: return "3";
      case 4: return "4";
      case 5: return "5";
      case 6: return "6";
      case 7: return "7";
      case 8: return "8";
      case 9: return "9";
      default: return "999";
   }
}

