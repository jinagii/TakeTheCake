#include "pch.h"
#include "IDXObjectBase.h"
#include "DebugCircle.h"
#include "ObjectFactory.h"
#include "Player.h"
#include "Prop.h"
#include "Scene03_InGame.h"
#include "Trap.h"

Trap::Trap(IDXObjectBase* pMesh)
   : m_pTarget(nullptr)
   , m_bAnt(true)
{
   SetMesh(pMesh);

   m_CollisionType = COLLISION_OBJECT_TYPE::Trap;

   /// 자신의 콜라이더를 벡터에 추가
   AddCollider(this->GetMesh()->GetDebugCircle());

   m_Ant = ObjectFactory::GetInstance()->CreateMesh(GIMMICK_TRAPANT, 1.0f, MathHelper::Pi / 2, 0.0f, 0.0f, m_Pos);
   Matrix transTM = Matrix::Identity;
   transTM._41 = m_Pos.x;
   transTM._42 = m_Pos.y;
   transTM._43 = m_Pos.z;
   m_Ant->SetTransTM(transTM);

   Initialize();
}

Trap::~Trap()
{
   Release();
}

void Trap::Initialize()
{
   IObjectBase::Initialize();

   SetColliderType();

   m_bAnt = true;
}

void Trap::Release()
{
   SAFE_DELETE(m_pMesh);
}

void Trap::Update(Camera* pCamera, float dTime)
{
   // 충돌 체크
   CheckCollision();

   UpdateMesh(pCamera, dTime);

   m_pMesh->Update(pCamera, dTime);

}

void Trap::Render()
{
   m_pMesh->Render();

   if (m_bAnt)
   {
      m_Ant->Render();
   }
}

void Trap::UpdateMesh(Camera* pCamera, float dTime)
{
   /////////////////////////////////////////////
   // Update Mesh
   float offsetX = m_Pos.x;
   float offsetY = m_Pos.y;
   float offsetZ = m_Pos.z;

   Matrix matrix;
   matrix = XMMatrixTranslation(offsetX, offsetY, offsetZ);

   m_pMesh->SetTransTM(matrix);
   m_pMesh->Update(pCamera, dTime);

   m_Ant->SetTransTM(matrix);
   m_Ant->Update(pCamera, dTime);
}

void Trap::SetTarget(IObjectBase* pTarget)
{
   // nullptr 체크
   if (!pTarget) return;

   // 타겟 설정
   this->m_pTarget = pTarget;

   Player* player = dynamic_cast<Player*>(m_pTarget);
   if (player)
   {
      AddCollider(player->GetMesh()->GetDebugCircle());
   }
}

void Trap::SetColliderType()
{
   /// 디버깅 circle에 인덱스(타입) 설정
   switch (m_CollisionType)
   {
      case COLLISION_OBJECT_TYPE::None:
         m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::None));
         break;

      case COLLISION_OBJECT_TYPE::Player:
         m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::Player));
         break;

      case COLLISION_OBJECT_TYPE::CupCake:
         m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::CupCake));
         break;

      case COLLISION_OBJECT_TYPE::Fire:
         m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::Fire));
         break;

      case COLLISION_OBJECT_TYPE::Knife:
         m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::Knife));
         break;

      case COLLISION_OBJECT_TYPE::Trap:
         m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::Trap));
         break;

      default:
         break;
   }
}

void Trap::AddCollider(DebugCircle* collider)
{
   m_DebugCircles.push_back(collider);
}

void Trap::CheckCollision()
{
   // 이미 구출됐다면.. 구출할 개미가 없다면
   if (m_bAnt == false)
   {
      return;
   }

   // 충돌 true / false 설정
   for (unsigned int i = 0; i < m_DebugCircles.size(); i++)
   {
      for (unsigned int j = i + 1; j < m_DebugCircles.size(); j++)
      {
         m_DebugCircles[i]->CollisionCheck(m_DebugCircles[j]);

      }
   }

   // 무엇이랑 충돌했는지 판별하여 로직 실행
   for (size_t i = 0; i < m_DebugCircles.size(); i++)
   {
      if (m_DebugCircles[i]->GetIsCollided())
      {
         // 인덱스 확인
         int index = m_DebugCircles[i]->GetDebugIndex();

         switch (index)
         {
            case static_cast<int>(COLLISION_OBJECT_TYPE::Player):

               // 스페이스바 입력 시 개미를 구출한다
               if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
               {
                  if (Scene03_InGame::sm_AntCount < 10)
                  {
                     Scene03_InGame::sm_AntCount++;
                  }

                  // 케익 운반 중일 때에 만
                  if (Scene03_InGame::m_GamePlayState == ePlayState::PickUpCake)
                  {
                     // 인게임씬 타이머 증가
                     Scene03_InGame::AddInGameTimer(10.0f);
                  }
                  m_bAnt = false;

                  Scene03_InGame* _nowScene = dynamic_cast<Scene03_InGame*>(SceneManager::GetInstance()->GetCurrentScene());
                  if (_nowScene != nullptr)
                  {
                     _nowScene->SpawnAnt();
                  }
               }
               return;;


            default:
               break;
         }
      }
   }
}

