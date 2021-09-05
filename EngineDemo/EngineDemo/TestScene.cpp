#include "DXObjectFactory.h"
#include "DXDefine.h"
#include "IDXObjectBase.h"
#include "Camera.h"
#include "DXDevice.h"
#include "DXRenderState.h"
#include "Boid.h"
#include "Navigation.h"

#include "DebugCircle.h"

#include "TestScene.h"

TestScene::TestScene()
   : m_DebugCircles(), m_DeltaTime(), m_PrevMousePos(), m_pCamera(), m_pFont(), m_pNavigationMesh()
   , m_PlayerPastPosition(5.0f, 21.0f, -62.0f)
{

}

TestScene::~TestScene()
{
   SafeDelete(m_pCamera);
   SafeDelete(m_pFont);
   //SafeDelete(m_pSky);

}

void TestScene::Initialize(DXObjectFactory* pFactory)
{
   m_pCamera = new Camera();
   m_pCamera->LookAt(Vector3(8.0f, 8.0f, -8.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
   m_pFont = new DXTKFont();
   m_pFont->Initialize(DXDevice::g_Device, DXRenderState::g_SolidFrame, DXRenderState::g_NormalDSS);

   SetLensOnResize(DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight);

   //m_pSky = new Sky(DXDevice::g_Device, L"../../Library/Textures/Snow_CubeMap.dds", 5000.0f);

   /// Helpers
   IDXObjectBase* _newGrid = pFactory->CreateGrid();
   IDXObjectBase* _newAxis = pFactory->CreateAxis();
   //m_HelperObject_um.insert({ "Grid", _newGrid });
   m_HelperObject_um.insert({ "Axis", _newAxis });
   //m_HelperObject_um.insert({ "Navi", _newNavi });

   /// Objects
   //IDXObjectBase* _newAnt0 = pFactory->CreateObjectFromASEParser("ant", 1.0f, 0.0f, 0.0f, 0.0f, { 2.0f, 0.0f, 0.0f }); // 초반위치 (5,20,-62)
   IDXObjectBase* _newAnt1 = pFactory->CreateObjectFromASEParser("ant", 1.0f, 0.0f, 0.0f, 0.0f, { 5.0f, 21.0f, -62.0f });
   IDXObjectBase* _newAnt2 = pFactory->CreateObjectFromASEParser("cupcake", 0.5f, 0.0f, 0.0f, 0.0f, { 6.0f, 20.2f, -60.0f });
   //IDXObjectBase* _newAnt3 = pFactory->CreateObjectFromASEParser("ani_interaction", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, -5.0f });
   //IDXObjectBase* _newAnt4 = pFactory->CreateObjectFromASEParser("ani_standing", 1.0f, 0.0f, 0.0f, 0.0f, { -5.0f, 0.0f, 0.0f });
   //IDXObjectBase* _newAnt5 = pFactory->CreateObjectFromASEParser("ani_walk", 1.0f, 0.0f, 0.0f, 0.0f, { 5.0f, 0.0f, 0.0f });
   //IDXObjectBase* _newFlowerpot = pFactory->CreateObjectFromASEParser("flowerpot", 1.f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   //IDXObjectBase* _newPlate = pFactory->CreateObjectFromASEParser("plate", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   //IDXObjectBase* _newBottom = pFactory->CreateObjectFromASEParser("bottom", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   //IDXObjectBase* _new1 = pFactory->CreateObjectFromASEParser("IkJoe", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   //_new1->SetScaleTM(XMMatrixIdentity() * 0.03f);

   //IDXObjectBase* _new121 = pFactory->CreateObjectFromASEParser("ani_mesh_on", .05f, 0.0f, 0.0f, 0.0f, { 1.0f, 0.0f, 0.0f });
   //_new121->SetScaleTM(XMMatrixIdentity() * 0.05f);

   //IDXObjectBase* _newimage = pFactory->CreateImageObject("ant_UI", 111, 111, 111, 110);
   //IDXObjectBase* _newimage = pFactory->CreateImageObject("clear", 2, 2, 1920, 1080);

   _newAnt1->GetDebugCircle()->SetDebugIndex(1);
   _newAnt2->GetDebugCircle()->SetDebugIndex(2);
   //_newAnt3->GetDebugCircle()->SetDebugIndex(3);
   //_newAnt4->GetDebugCircle()->SetDebugIndex(4);
   //_newAnt5->GetDebugCircle()->SetDebugIndex(5);

   //m_DebugCircles.push_back(_newAnt0->GetDebugCircle());
   m_DebugCircles.push_back(_newAnt1->GetDebugCircle());
   m_DebugCircles.push_back(_newAnt2->GetDebugCircle());
   //m_DebugCircles.push_back(_newAnt3->GetDebugCircle());
   //m_DebugCircles.push_back(_newAnt4->GetDebugCircle());
   //m_DebugCircles.push_back(_newAnt5->GetDebugCircle());


   _newAnt2->SetScaleTM(5.0f);

   //m_Objects_um.insert({ "Ant_new120", _newAnt0 });
   m_Objects_um.insert({ "Ant_new121", _newAnt1 });
   m_Objects_um.insert({ "Ant_new122", _newAnt2 });
   //m_Objects_um.insert({ "Ant_new123", _newAnt3 });
   //m_Objects_um.insert({ "Ant_new124", _newAnt4 });
   //m_Objects_um.insert({ "Ant_new125", _newAnt5 });


   /*
   IDXObjectBase* _new1  = pFactory->CreateObjectFromASEParser("ant", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new2 = pFactory->CreateObjectFromASEParser("apple", 1.0f, 0.0f, 0.0f, 0.0f, {0.0f, 0.0f, 0.0f});
   IDXObjectBase* _new3  = pFactory->CreateObjectFromASEParser("avocado", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new4  = pFactory->CreateObjectFromASEParser("avocado_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new5  = pFactory->CreateObjectFromASEParser("avocadoseed", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new6  = pFactory->CreateObjectFromASEParser("bg_bottom", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new7  = pFactory->CreateObjectFromASEParser("bg_bottom_door", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new8  = pFactory->CreateObjectFromASEParser("bg_cabient", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new9  = pFactory->CreateObjectFromASEParser("bg_door", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new10 = pFactory->CreateObjectFromASEParser("bg_door_glass", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new11 = pFactory->CreateObjectFromASEParser("bg_kitchen", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new12 = pFactory->CreateObjectFromASEParser("bg_refrigerator", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new13 = pFactory->CreateObjectFromASEParser("bg_table", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new14 = pFactory->CreateObjectFromASEParser("bg_table_cloth", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new15 = pFactory->CreateObjectFromASEParser("board", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new16 = pFactory->CreateObjectFromASEParser("book2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new17 = pFactory->CreateObjectFromASEParser("book1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new18 = pFactory->CreateObjectFromASEParser("book1_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new19 = pFactory->CreateObjectFromASEParser("book1_02", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new20 = pFactory->CreateObjectFromASEParser("book1_03", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new21 = pFactory->CreateObjectFromASEParser("book1_04", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new22 = pFactory->CreateObjectFromASEParser("book1_05", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new23 = pFactory->CreateObjectFromASEParser("book1_06", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new24 = pFactory->CreateObjectFromASEParser("bread1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new25 = pFactory->CreateObjectFromASEParser("bread1_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new26 = pFactory->CreateObjectFromASEParser("bread3", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new27 = pFactory->CreateObjectFromASEParser("bread3_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new28 = pFactory->CreateObjectFromASEParser("bread4", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new29 = pFactory->CreateObjectFromASEParser("butter", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new30 = pFactory->CreateObjectFromASEParser("candy1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new31 = pFactory->CreateObjectFromASEParser("candy1_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new32 = pFactory->CreateObjectFromASEParser("candy2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new33 = pFactory->CreateObjectFromASEParser("candy2_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new34 = pFactory->CreateObjectFromASEParser("candy3", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new35 = pFactory->CreateObjectFromASEParser("candy3_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new36 = pFactory->CreateObjectFromASEParser("candyroll", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new37 = pFactory->CreateObjectFromASEParser("candyroll_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new38 = pFactory->CreateObjectFromASEParser("carrot", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new39 = pFactory->CreateObjectFromASEParser("cheese1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new40 = pFactory->CreateObjectFromASEParser("cheese1_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new41 = pFactory->CreateObjectFromASEParser("cheese1_02", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new42 = pFactory->CreateObjectFromASEParser("cheese2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new43 = pFactory->CreateObjectFromASEParser("cheese2_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new44 = pFactory->CreateObjectFromASEParser("cherry", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new45 = pFactory->CreateObjectFromASEParser("chocobar", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new46 = pFactory->CreateObjectFromASEParser("chocobar_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new47 = pFactory->CreateObjectFromASEParser("chocolate", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new48 = pFactory->CreateObjectFromASEParser("coffeegrinder", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new49 = pFactory->CreateObjectFromASEParser("cup1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new50 = pFactory->CreateObjectFromASEParser("cup2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new51 = pFactory->CreateObjectFromASEParser("cupcake", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new52 = pFactory->CreateObjectFromASEParser("cutmushroom_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new53 = pFactory->CreateObjectFromASEParser("cutmushroom", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new54 = pFactory->CreateObjectFromASEParser("dish1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new55 = pFactory->CreateObjectFromASEParser("dish2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new56 = pFactory->CreateObjectFromASEParser("dish2_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new57 = pFactory->CreateObjectFromASEParser("dish2_02", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new58 = pFactory->CreateObjectFromASEParser("dish3", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new59 = pFactory->CreateObjectFromASEParser("dish3_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new60 = pFactory->CreateObjectFromASEParser("eggplant", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new61 = pFactory->CreateObjectFromASEParser("etc_knife", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new62 = pFactory->CreateObjectFromASEParser("flag", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new63 = pFactory->CreateObjectFromASEParser("flour", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new64 = pFactory->CreateObjectFromASEParser("flowerpot", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new65 = pFactory->CreateObjectFromASEParser("flowervase", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new66 = pFactory->CreateObjectFromASEParser("fork", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new67 = pFactory->CreateObjectFromASEParser("gimmick_knife", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new68 = pFactory->CreateObjectFromASEParser("jam", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new69 = pFactory->CreateObjectFromASEParser("ladle", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new70 = pFactory->CreateObjectFromASEParser("lid", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new71 = pFactory->CreateObjectFromASEParser("m_plates", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new72 = pFactory->CreateObjectFromASEParser("m_wooden_plate", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new73 = pFactory->CreateObjectFromASEParser("microwave", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new74 = pFactory->CreateObjectFromASEParser("milk", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new75 = pFactory->CreateObjectFromASEParser("mixermachine", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new76 = pFactory->CreateObjectFromASEParser("mixerball", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new77 = pFactory->CreateObjectFromASEParser("nail1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new78 = pFactory->CreateObjectFromASEParser("nail2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new79 = pFactory->CreateObjectFromASEParser("nail3", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new80 = pFactory->CreateObjectFromASEParser("oven", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new81 = pFactory->CreateObjectFromASEParser("oven_button_a", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new82 = pFactory->CreateObjectFromASEParser("oven_button_b", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new83 = pFactory->CreateObjectFromASEParser("oven_fire_a_off", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new84 = pFactory->CreateObjectFromASEParser("oven_fire_a_on", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new85 = pFactory->CreateObjectFromASEParser("oven_fire_b_off", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new86 = pFactory->CreateObjectFromASEParser("oven_fire_b_on", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new87 = pFactory->CreateObjectFromASEParser("oven_fire_c_off", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new88 = pFactory->CreateObjectFromASEParser("oven_fire_c_on", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new89 = pFactory->CreateObjectFromASEParser("oven_fire_d_off", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new90 = pFactory->CreateObjectFromASEParser("oven_fire_d_on", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new91 = pFactory->CreateObjectFromASEParser("oven_glass", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new92 = pFactory->CreateObjectFromASEParser("oven_top", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new93 = pFactory->CreateObjectFromASEParser("pot1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new94 = pFactory->CreateObjectFromASEParser("pot2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new95 = pFactory->CreateObjectFromASEParser("pot3", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new96 = pFactory->CreateObjectFromASEParser("pumpkin", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new97 = pFactory->CreateObjectFromASEParser("sauce", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new98 = pFactory->CreateObjectFromASEParser("sauce2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new99 = pFactory->CreateObjectFromASEParser("sink", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new100 = pFactory->CreateObjectFromASEParser("soup1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new101 = pFactory->CreateObjectFromASEParser("soup1_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new102 = pFactory->CreateObjectFromASEParser("soup1_02", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new103 = pFactory->CreateObjectFromASEParser("soup2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new104 = pFactory->CreateObjectFromASEParser("soup2_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new105 = pFactory->CreateObjectFromASEParser("soup2_02", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new106 = pFactory->CreateObjectFromASEParser("spatula", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new107 = pFactory->CreateObjectFromASEParser("spatula_01", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new108 = pFactory->CreateObjectFromASEParser("spoonholder", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new109 = pFactory->CreateObjectFromASEParser("start_table", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new110 = pFactory->CreateObjectFromASEParser("teapot", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new111 = pFactory->CreateObjectFromASEParser("toaster", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new112 = pFactory->CreateObjectFromASEParser("top_a", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new113 = pFactory->CreateObjectFromASEParser("top_b", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new114 = pFactory->CreateObjectFromASEParser("top_door", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new115 = pFactory->CreateObjectFromASEParser("trap", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new116 = pFactory->CreateObjectFromASEParser("tulip", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new117 = pFactory->CreateObjectFromASEParser("whisk", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new118 = pFactory->CreateObjectFromASEParser("window", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new119 = pFactory->CreateObjectFromASEParser("wine1", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new120 = pFactory->CreateObjectFromASEParser("wine2", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new121 = pFactory->CreateObjectFromASEParser("ani_mesh_on", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });
   IDXObjectBase* _new122 = pFactory->CreateObjectFromASEParser("ani_mesh_off", 1.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 0.0f, 0.0f });

   m_Objects_um.insert({ "Ant_new1", _new1 });
   m_Objects_um.insert({"Ant_new2", _new2});
   m_Objects_um.insert({ "Ant_new3", _new3 });
   m_Objects_um.insert({ "Ant_new4", _new4 });
   m_Objects_um.insert({ "Ant_new5", _new5 });
   m_Objects_um.insert({ "Ant_new6", _new6 });
   m_Objects_um.insert({ "Ant_new7", _new7 });
   m_Objects_um.insert({ "Ant_new8", _new8 });
   m_Objects_um.insert({ "Ant_new9", _new9 });
   m_Objects_um.insert({ "Ant_new10", _new10 });
   m_Objects_um.insert({ "Ant_new11", _new11 });
   m_Objects_um.insert({ "Ant_new12", _new12 });
   m_Objects_um.insert({ "Ant_new13", _new13 });
   m_Objects_um.insert({ "Ant_new14", _new14 });
   m_Objects_um.insert({ "Ant_new15", _new15 });
   m_Objects_um.insert({ "Ant_new16", _new16 });
   m_Objects_um.insert({ "Ant_new17", _new17 });
   m_Objects_um.insert({ "Ant_new18", _new18 });
   m_Objects_um.insert({ "Ant_new19", _new19 });
   m_Objects_um.insert({ "Ant_new20", _new20 });
   m_Objects_um.insert({ "Ant_new21", _new21 });
   m_Objects_um.insert({ "Ant_new22", _new22 });
   m_Objects_um.insert({ "Ant_new23", _new23 });
   m_Objects_um.insert({ "Ant_new24", _new24 });
   m_Objects_um.insert({ "Ant_new25", _new25 });
   m_Objects_um.insert({ "Ant_new26", _new26 });
   m_Objects_um.insert({ "Ant_new27", _new27 });
   m_Objects_um.insert({ "Ant_new28", _new28 });
   m_Objects_um.insert({ "Ant_new29", _new29 });
   m_Objects_um.insert({ "Ant_new30", _new30 });
   m_Objects_um.insert({ "Ant_new31", _new31 });
   m_Objects_um.insert({ "Ant_new32", _new32 });
   m_Objects_um.insert({ "Ant_new33", _new33 });
   m_Objects_um.insert({ "Ant_new34", _new34 });
   m_Objects_um.insert({ "Ant_new35", _new35 });
   m_Objects_um.insert({ "Ant_new36", _new36 });
   m_Objects_um.insert({ "Ant_new37", _new37 });
   m_Objects_um.insert({ "Ant_new38", _new38 });
   m_Objects_um.insert({ "Ant_new39", _new39 });
   m_Objects_um.insert({ "Ant_new40", _new40 });
   m_Objects_um.insert({ "Ant_new41", _new41 });
   m_Objects_um.insert({ "Ant_new42", _new42 });
   m_Objects_um.insert({ "Ant_new43", _new43 });
   m_Objects_um.insert({ "Ant_new44", _new44 });
   m_Objects_um.insert({ "Ant_new45", _new45 });
   m_Objects_um.insert({ "Ant_new46", _new46 });
   m_Objects_um.insert({ "Ant_new47", _new47 });
   m_Objects_um.insert({ "Ant_new48", _new48 });
   m_Objects_um.insert({ "Ant_new49", _new49 });
   m_Objects_um.insert({ "Ant_new50", _new50 });
   m_Objects_um.insert({ "Ant_new51", _new51 });
   m_Objects_um.insert({ "Ant_new52", _new52 });
   m_Objects_um.insert({ "Ant_new53", _new53 });
   m_Objects_um.insert({ "Ant_new54", _new54 });
   m_Objects_um.insert({ "Ant_new55", _new55 });
   m_Objects_um.insert({ "Ant_new56", _new56 });
   m_Objects_um.insert({ "Ant_new57", _new57 });
   m_Objects_um.insert({ "Ant_new58", _new58 });
   m_Objects_um.insert({ "Ant_new59", _new59 });
   m_Objects_um.insert({ "Ant_new60", _new60 });
   m_Objects_um.insert({ "Ant_new61", _new61 });
   m_Objects_um.insert({ "Ant_new62", _new62 });
   m_Objects_um.insert({ "Ant_new63", _new63 });
   m_Objects_um.insert({ "Ant_new64", _new64 });
   m_Objects_um.insert({ "Ant_new65", _new65 });
   m_Objects_um.insert({ "Ant_new66", _new66 });
   m_Objects_um.insert({ "Ant_new67", _new67 });
   m_Objects_um.insert({ "Ant_new68", _new68 });
   m_Objects_um.insert({ "Ant_new69", _new69 });
   m_Objects_um.insert({ "Ant_new70", _new70 });
   m_Objects_um.insert({ "Ant_new71", _new71 });
   m_Objects_um.insert({ "Ant_new72", _new72 });
   m_Objects_um.insert({ "Ant_new73", _new73 });
   m_Objects_um.insert({ "Ant_new74", _new74 });
   m_Objects_um.insert({ "Ant_new75", _new75 });
   m_Objects_um.insert({ "Ant_new76", _new76 });
   m_Objects_um.insert({ "Ant_new77", _new77 });
   m_Objects_um.insert({ "Ant_new78", _new78 });
   m_Objects_um.insert({ "Ant_new79", _new79 });
   m_Objects_um.insert({ "Ant_new80", _new80 });
   m_Objects_um.insert({ "Ant_new81", _new81 });
   m_Objects_um.insert({ "Ant_new82", _new82 });
   m_Objects_um.insert({ "Ant_new83", _new83 });
   m_Objects_um.insert({ "Ant_new84", _new84 });
   m_Objects_um.insert({ "Ant_new85", _new85 });
   m_Objects_um.insert({ "Ant_new86", _new86 });
   m_Objects_um.insert({ "Ant_new87", _new87 });
   m_Objects_um.insert({ "Ant_new88", _new88 });
   m_Objects_um.insert({ "Ant_new89", _new89 });
   m_Objects_um.insert({ "Ant_new90", _new90 });
   m_Objects_um.insert({ "Ant_new91", _new91 });
   m_Objects_um.insert({ "Ant_new92", _new92 });
   m_Objects_um.insert({ "Ant_new93", _new93 });
   m_Objects_um.insert({ "Ant_new94", _new94 });
   m_Objects_um.insert({ "Ant_new95", _new95 });
   m_Objects_um.insert({ "Ant_new96", _new96 });
   m_Objects_um.insert({ "Ant_new97", _new97 });
   m_Objects_um.insert({ "Ant_new98", _new98 });
   m_Objects_um.insert({ "Ant_new99", _new99 });
   m_Objects_um.insert({ "Ant_new100", _new100 });
   m_Objects_um.insert({ "Ant_new101", _new101 });
   m_Objects_um.insert({ "Ant_new102", _new102 });
   m_Objects_um.insert({ "Ant_new103", _new103 });
   m_Objects_um.insert({ "Ant_new104", _new104 });
   m_Objects_um.insert({ "Ant_new105", _new105 });
   m_Objects_um.insert({ "Ant_new106", _new106 });
   m_Objects_um.insert({ "Ant_new107", _new107 });
   m_Objects_um.insert({ "Ant_new108", _new108 });
   m_Objects_um.insert({ "Ant_new109", _new109 });
   m_Objects_um.insert({ "Ant_new110", _new110 });
   m_Objects_um.insert({ "Ant_new111", _new111 });
   m_Objects_um.insert({ "Ant_new112", _new112 });
   m_Objects_um.insert({ "Ant_new113", _new113 });
   m_Objects_um.insert({ "Ant_new114", _new114 });
   m_Objects_um.insert({ "Ant_new115", _new115 });
   m_Objects_um.insert({ "Ant_new116", _new116 });
   m_Objects_um.insert({ "Ant_new117", _new117 });
   m_Objects_um.insert({ "Ant_new118", _new118 });
   m_Objects_um.insert({ "Ant_new119", _new119 });
   m_Objects_um.insert({ "Ant_new120", _new120 });
   m_Objects_um.insert({ "Ant_new121", _new121 });
   m_Objects_um.insert({ "Ant_new122", _new122 });
   //*/





   //IDXObjectBase* _newAnt = pFactory->CreateObjectFromASEParser("ant", 1.0f, MathHelper::Pi/2, 0.0f, 0.0f, { 1.0f, 0.0f, 2.0f });
   //IDXObjectBase* _newPlate = pFactory->CreateObjectFromASEParser("apple", 1.f, 0.0f, 0.0f, 0.0f, { .0f, 0.0f, .0f });
   //IDXObjectBase* _newFlowerPot = pFactory->CreateObjectFromASEParser("avocado", 1.f, 0.0f, 0.0f, 0.0f, { .0f, 0.0f, .0f });

   //IDXObjectBase* _newCarrot = pFactory->CreateObjectFromASEParser("avocado_01", 1.f, 0.0f, 0.0f, 0.0f, { .0f, 0.0f, .0f });
   //IDXObjectBase* _newBottom = pFactory->CreateObjectFromASEParser("avocadoseed", 1.f, 0.0f, 0.0f, 0.0f, { .0f, 0.0f, .0f });


   //m_Objects_um.insert({ "Ant", _newAnt });
   //m_Objects_um.insert({ "Ant2", _newCarrot });
   //m_Objects_um.insert({ "Ant3", _newFlowerpot });
   //m_Objects_um.insert({ "Ant4", _newPlate });
   //m_Objects_um.insert({ "Ant5", _newBottom });
   //m_Objects_um.insert({ "Ant6", _newCan });

   //m_Objects_um.insert({ "Test1", _newPlate });
   //m_Objects_um.insert({ "Test2", _newFlowerPot });
   //m_Objects_um.insert({ "Test3", _newCarrot });
   //m_Objects_um.insert({ "Test4", _newBottom });
   m_pNavigationMesh = (Navigation*)pFactory->CreateNavigationMesh();


   m_ObjectsIter = m_Objects_um.find("Ant_new121");
}

void TestScene::Update(float dTime)
{
   // 카메라 조작
   if (GetAsyncKeyState('W') & 0x8000)
   {
      m_pCamera->MoveForward(10.0f * dTime);
   }
   if (GetAsyncKeyState('S') & 0x8000)
   {
      m_pCamera->MoveForward(-10.0f * dTime);
   }
   if (GetAsyncKeyState('A') & 0x8000)
   {
      m_pCamera->MoveSide(-10.0f * dTime);
   }
   if (GetAsyncKeyState('D') & 0x8000)
   {
      m_pCamera->MoveSide(10.0f * dTime);
   }
   if (GetAsyncKeyState('Q') & 0x8000)
   {
      m_pCamera->MoveUpDown(-10.0f * dTime);
   }
   if (GetAsyncKeyState('E') & 0x8000)
   {
      m_pCamera->MoveUpDown(10.0f * dTime);
   }

   m_DeltaTime = dTime;


   /// 개미 조작키 구현 /////////////////////////////////////////////////////////////


   if (GetAsyncKeyState('Z') & 0x8000)
   {
      m_ObjectsIter->second->SetStatus("walking");
   }
   if (GetAsyncKeyState('X') & 0x8000)
   {
      m_ObjectsIter->second->SetStatus("idle");
   }
   if (GetAsyncKeyState('C') & 0x8000)
   {
      m_ObjectsIter->second->SetStatus("dead");
   }
   if (GetAsyncKeyState('V') & 0x8000)
   {
      m_ObjectsIter->second->SetStatus("interaction");
   }
   // Object의 worldTM을 decompose로 S R T 나눠서 나중에 합산해서 다시 복원
   //Vector3 _objScl; // 크기
   //Quaternion _objRot; // 회전
   //Vector3 _objPos; // 위치
   //m_ObjectsIter->second->GetWorld().Decompose(_objScl, _objRot, _objPos);
   //
   //// Object의 forward 벡터로 쓸 벡터를 만들고 이를 회전할 행렬을 만든다
   ////static float _angle = 0.0f;
   //Matrix _forwardRotMat = XMMatrixRotationY(m_ObjectsIter->second->GetAngle()); // Y축으로 회전   
   //
   //Vector3 _forwardVec = XMVector3TransformNormal(m_ObjectsIter->second->GetForwardVec(), _forwardRotMat);
   ////m_ObjectsIter->second->SetForwardVec(_forwardVec);  // XMVector3TransformNormal: 지정된 행렬로 법선벡터를 변형
   //
   //// Object를 회전시킨다
   //_objRot = XMQuaternionRotationMatrix(_forwardRotMat);

   //m_ObjectsIter->second->SetRotation(XMQuaternionRotationMatrix(_forwardRotMat));

   for (UINT i = 0; i < m_DebugCircles.size(); i++)
   {
      for (UINT j = i + 1; j < m_DebugCircles.size(); j++)
      {
         m_DebugCircles[i]->CollisionCheck(m_DebugCircles[j]);
      }
   }

   //for (pair<string,IDXObjectBase*> _obejct_i : m_Objects_um)
   //{
   //   static int i = 0;
   //   for (pair<string, IDXObjectBase*> _obejct_j : m_Objects_um)
   //   {
   //      if (_obejct_i.first == _obejct_j.first)
   //      {
   //         continue;
   //      }
   //      
   //      _obejct_i.second->GetDebugCircle()->CollisionCheck(_obejct_j.second->GetDebugCircle());
   //   }
   //}


   if (m_pNavigationMesh->GetIsInTriangle() == true)
   {
      m_PlayerPastPosition = { m_ObjectsIter->second->GetTransTM()._41, m_ObjectsIter->second->GetTransTM()._42, m_ObjectsIter->second->GetTransTM()._43 };

      if (GetAsyncKeyState(VK_UP) & 0x8000)
      {
         m_ObjectsIter->second->MoveForward(dTime, 5.0f);
      }
      if (GetAsyncKeyState(VK_DOWN) & 0x8000)
      {
         m_ObjectsIter->second->MoveForward(dTime, -5.0f);
      }
      if (GetAsyncKeyState(VK_LEFT) & 0x8000)
      {
         m_ObjectsIter->second->RotateObject(dTime, -2.0f);
      }
      if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
      {
         m_ObjectsIter->second->RotateObject(dTime, 2.0f);
      }
   }

   if (m_pNavigationMesh->GetIsInTriangle() == false)
   {
      Vector3 _nowPos = { m_ObjectsIter->second->GetTransTM()._41, m_ObjectsIter->second->GetTransTM()._42, m_ObjectsIter->second->GetTransTM()._43 };

      Vector3 _backVec = (_nowPos - m_PlayerPastPosition);
      _backVec.Normalize();

      m_ObjectsIter->second->SetTransTM(Matrix::CreateTranslation(
         m_ObjectsIter->second->GetTransTM()._41 - _backVec.x * dTime * 10.0f,
         m_ObjectsIter->second->GetTransTM()._42 - _backVec.y * dTime * 10.0f,
         m_ObjectsIter->second->GetTransTM()._43 - _backVec.z * dTime * 10.0f));
   }



   // S R T 를 변환행렬로 바꿔준다
   //Matrix _scaleMat = Matrix::CreateScale(_objScl);
   //Matrix _rotMat = Matrix::CreateFromQuaternion(_objRot);
   //Matrix _transMat = Matrix::CreateTranslation(_objPos);
   // 개미 월드 매트릭스를 업데이트 해준다
   //if (m_ObjectsIter->first == "Ant")
   //{
   //   //Matrix _world = m_ObjectsIter->second->GetWorld();
   //   m_ObjectsIter->second->SetWorld(_scaleMat * _rotMat * _transMat);
   //}

   /// 카메라 3인칭으로 바꾸기 /////////////////////////////////////////////////
   static bool _cameraFlag = false;

   m_pCamera->UpdateViewMatrix();
   if (_cameraFlag == false)
   {
      Vector3 _objPos = { m_ObjectsIter->second->GetTransTM()._41, m_ObjectsIter->second->GetTransTM()._42, m_ObjectsIter->second->GetTransTM()._43 };
      Matrix _forwardRotMat = XMMatrixRotationY(m_ObjectsIter->second->GetAngle());

      Vector3 _cameraPos = XMVector3Transform({ _objPos.x, _objPos.y, _objPos.z }, _forwardRotMat); // 카메라 회전 - 오브젝회전만큼
      Vector3 _cameraVec;

      _cameraVec = m_ObjectsIter->second->GetForwardVec() * 20;
      _cameraVec.x *= -1;
      _cameraVec.y += 15;
      _cameraVec.z *= -1;

      _cameraVec += _objPos;

      m_pCamera->SetPosition(_cameraPos);
      m_pCamera->LookAt(_cameraVec, _objPos, { 0.0f, 1.0f, 0.0f });
   }
   if (GetAsyncKeyState(VK_SPACE) & 0x0001)
   {
      _cameraFlag = !_cameraFlag;
   }


   //////////////////////////////////////////////////////////////////////////

   for (auto _helperObjIter : m_HelperObject_um)
   {
      _helperObjIter.second->Update(m_pCamera, dTime);
   }

   if (GetAsyncKeyState(VK_SPACE) & 0x0001)
   {
      m_ObjectsIter++;
      if (m_ObjectsIter == m_Objects_um.end())
      {
         m_ObjectsIter = m_Objects_um.begin();
      }
   }

   for (auto _objectIter : m_Objects_um)
   {
      _objectIter.second->Update(m_pCamera, dTime);
   }

   m_pNavigationMesh->PlayerPositionUpdate({ m_ObjectsIter->second->GetWorld()._41, m_ObjectsIter->second->GetWorld()._42, m_ObjectsIter->second->GetWorld()._43 });

   m_pNavigationMesh->Update(m_pCamera, dTime);

}

//m_ObjectsIter->second->Update(m_pCamera, dTime);

void TestScene::Render()
{
   for (auto _helperObjIter : m_HelperObject_um)
   {
      _helperObjIter.second->Render();
   }


   for (auto _objectIter : m_Objects_um)
   {
      _objectIter.second->Render();
   }

   m_pNavigationMesh->Render();

   // sky
   //StaticEffect::BasicFX->SetCubeMap(m_pSky->CubeMapSRV());
   //StaticEffect::NormalMapFX->SetCubeMap(m_pSky->CubeMapSRV());
   //m_pSky->Draw(DXDevice::g_DeviceContext, *m_pCamera);


   ShowDebugInformation();
}

void TestScene::Finalize()
{

}

void TestScene::ShowDebugInformation()
{
   Vector4 _black(0.f, 0.f, 0.f, 1.f);
   Vector4 _red(1.f, 0.f, 0.f, 1.f);


   int _yPos = 50;
   m_pFont->DrawTextWithColor(0, _yPos, _black, (TCHAR*)L"%d / %f / %s", 55, 3.14f, L"Debug Information");

   m_pFont->DrawTextWithColor(0, _yPos += 28, _black, (TCHAR*)L"Delta Time : %f ", m_DeltaTime);

   // 피쳐레벨
   m_pFont->DrawTextWithColor(0, _yPos += 28, _black, (TCHAR*)L"Feature Level : %x", DXDevice::g_FeatureLevel);

   // 클라이언트 영역의 해상도
   m_pFont->DrawTextWithColor(0, _yPos += 28, _black, (TCHAR*)L"Client Resolution: %dx%d", DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight);

   // 카메라 정보
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"WASD/QE : 카메라 조정");
   m_pFont->DrawTextWithColor(0, _yPos += 28, _black, (TCHAR*)L"Camera Pos : %.2f / %.2f / %.2f", m_pCamera->GetPosition().x, m_pCamera->GetPosition().y, m_pCamera->GetPosition().z);

   // 기타 정보 추후 입력
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"SPACE BAR : 카메라 뷰 전환 / 방향키 : 모델 위치 변경");
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"1 ~ 7 : Light Effect 변경");

   // 오브젝트 이름
   /// string to const wchar_t로 바꾸는거 아래 처럼 하면된다
   wstring str;
   str.assign(m_ObjectsIter->first.begin(), m_ObjectsIter->first.end());
   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"현재 모델명 : %s", str.c_str());

   m_pFont->DrawTextWithColor(0, _yPos += 28, _red, (TCHAR*)L"현재 모델 Pos : %.2f / %.2f / %.2f", m_ObjectsIter->second->GetTransTM()._41, m_ObjectsIter->second->GetTransTM()._42, m_ObjectsIter->second->GetTransTM()._43);

}

void TestScene::OnMouseDown(HWND hWnd, int x, int y)
{
   m_PrevMousePos.x = x;
   m_PrevMousePos.y = y;
   SetCapture(hWnd);
}

void TestScene::OnMouseUp(int x, int y)
{
   ReleaseCapture();
}

void TestScene::OnMouseMove(int btnState, int x, int y)
{
   if ((btnState & MK_RBUTTON) != 0)
   {
      // Make each pixel correspond to a quarter of a degree.
      float _dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_PrevMousePos.x));
      float _dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_PrevMousePos.y));

      m_pCamera->Pitch(_dy);
      m_pCamera->RotateY(_dx);
   }

   m_PrevMousePos.x = x;
   m_PrevMousePos.y = y;
}

void TestScene::SetLensOnResize(int width, int height)
{
   m_pCamera->SetLens(0.25f * MathHelper::Pi, static_cast<float>(width) / height, 1.0f, 10000.0f);
}
