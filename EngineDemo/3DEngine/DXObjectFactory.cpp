
#include "Camera.h"
#include "ASEParser.h"
#include "GeometryHelper.h"
//////////////////////////////////////////////////////////////////////////
#include "MeshObject.h"
#include "Camera.h"
#include "ImageObject.h"
#include "HelperObject.h"
//#include "Axis.h"
//#include "Grid.h"
#include "Skull.h"
#include "Navigation.h"
//////////////////////////////////////////////////////////////////////////

#include "TextureManager.h"
#include "DXObjectFactory.h"

DXObjectFactory::DXObjectFactory(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
   : m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
   //m_pASEParser(nullptr)
{  
   m_pTextureManager = new TextureManager(pDevice);
   m_pTextureManager->SaveTexture();

   m_pGeometryHelper = new GeometryHelper(pDevice);
}

DXObjectFactory::~DXObjectFactory()
{
   //SafeDelete(m_pASEParser);
}

IDXObjectBase* DXObjectFactory::CreateObjectFromASEParser(string name, float circleSize, float angleY, float angleX, float angleZ, Vector3 pos)
{
   for (auto _dataIter : m_ASEData_um)
   {
      if (_dataIter.first == name)
      {
         // 불러온 데이터를 넣을 메쉬오브젝트 생성
         MeshObject* _newMeshObject = new MeshObject(m_pDevice, m_pDeviceContext);
         _newMeshObject->Initialize(_dataIter.second); // 데이터 넣어줌

         // 메쉬를 돌면서 texture들이 있는지 확인하고 넣어줌
         for (auto _meshIter : _dataIter.second->m_MeshData_v)
         {
            if (_meshIter->m_IsTextureSet == true)
            {
               _newMeshObject->SetDiffuseMapSRV(m_pTextureManager->GetDiffuseMapByName(name));
            }    

            // 노말맵 있는지 확인하고 넣어줌
            for (auto _normalNameIter : m_pTextureManager->GetNormalMap_um())
            {
               if (_normalNameIter.first == name)
               {
                  _newMeshObject->SetNormalMapSRV(m_pTextureManager->GetNormalMapByName(name));
               }           
            }
         }

         

         // 데이터들로 geometry 형성시켜줌
         _newMeshObject->BuildGeometryBuffers();

         // S R T 초기값 설정         
         _newMeshObject->SetScaleTM(1.0f);
         _newMeshObject->SetRotationTM(Matrix::CreateFromYawPitchRoll(angleY, angleX, angleZ));
         _newMeshObject->SetTransTM(XMMatrixTranslation(pos.x, pos.y, pos.z));

         // forward 벡터 조정
         _newMeshObject->SetAngle(angleY);
         //_newMeshObject->SetForwardVec(XMVector3TransformNormal(-_newMeshObject->GetForwardVec(), XMMatrixRotationY(angleY)));
         _newMeshObject->SetDebugCircle(circleSize);

         Matrix _sclMat = _newMeshObject->GetScaleTM();
         Matrix _rotMat = _newMeshObject->GetRotationTM();
         Matrix _traMat = _newMeshObject->GetTransTM();

         _newMeshObject->SetWorld(_sclMat * _rotMat * _traMat);         

         return _newMeshObject;
      }
   }

   return nullptr;
}

IDXObjectBase* DXObjectFactory::CreateImageObject(string name, int posX, int posY, int width, int height)
{
   ImageObject* _newImage = new ImageObject(m_pDevice, m_pDeviceContext);

   _newImage->SetImageSRV(m_pTextureManager->GetPNGImageByName(name));
   _newImage->Initialize(posX, posY, width, height);
   DXGeometryBuffer* _newBuffer = m_pGeometryHelper->BuildGeometryBuffer_Image();
   _newImage->LoadGeometry(_newBuffer);

   return _newImage;
}

IDXObjectBase* DXObjectFactory::CreateGrid()
{
   HelperObject* _newAxis = new HelperObject(m_pDevice, m_pDeviceContext);

   // Geometry, FX, vertex 등을 초기화
   _newAxis->Initialize();
   DXGeometryBuffer* _newBuffer = m_pGeometryHelper->BuildGeometryBuffer_Grid();
   _newAxis->LoadGeometry(_newBuffer);
   return _newAxis;
}

IDXObjectBase* DXObjectFactory::CreateAxis()
{
   HelperObject* _newGrid = new HelperObject(m_pDevice, m_pDeviceContext);

   // Geometry, FX, vertex 등을 초기화
   _newGrid->Initialize();
   DXGeometryBuffer* _newBuffer = m_pGeometryHelper->BuildGeometryBuffer_Axis();
   _newGrid->LoadGeometry(_newBuffer);
   return _newGrid;
}

IDXObjectBase* DXObjectFactory::CreateNavigationMesh()
{
   Navigation* _newNavi = new Navigation(m_pDevice, m_pDeviceContext);

   // Geometry, FX, vertex 등을 초기화
   _newNavi->Initialize(m_ASEData_um.at("navimesh"));

   return _newNavi;
}

IDXObjectBase* DXObjectFactory::CreateSkull()
{
   Skull* _newSkull = new Skull(m_pDevice, m_pDeviceContext);

   _newSkull->Initialize();
   DXGeometryBuffer* _newBuffer = m_pGeometryHelper->BuildGeometryBuffer_Skull();
   _newSkull->LoadGeometry(_newBuffer);

   return _newSkull;
}

Camera* DXObjectFactory::CreateCameraAt(Vector3 pos, Vector3 target, Vector3 worldUp)
{
   Camera* _newCamera = new Camera();

   _newCamera->LookAt(pos, target, worldUp);

   return _newCamera;
}

void DXObjectFactory::LoadASEData(string objectName, string filePath)
{
   /// 경로와 objectName으로 로드하는것 
   /// 동적으로 구현할 수 있음   
   ASEParser* _pASEParser = new ASEParser();

   _pASEParser->Initialize();

   _pASEParser->LoadFile((LPSTR)filePath.c_str());

   // Parser가 불러온 mesh들의 내용(localTM이나 vertexOptization 등)을 셋업한다
   for (auto _mesh : _pASEParser->m_MeshData_v)
   {
      _pASEParser->CheckNegativeScale(_mesh);
      _pASEParser->VertexOptimization(_mesh);
      //_pASEParser->ConvertAll(_mesh);
      //_pASEParser->SetBoneData(_mesh);
      _pASEParser->SetLocalTM(_mesh);     
   }

   // 나중에 키로 parsing data 찾을 수 있음
   //m_ASEDataMap.insert(make_pair(objectName, _pASEParser));
   m_ASEData_um.insert({ objectName, _pASEParser }); 
}

void DXObjectFactory::Initialize()
{
   /// 이 파일들을 불러들일때 메모리밀리는 현상이 발생
   // test 일단되는 ase파일들 
   //LoadASEData("ant", "../../Library/ASEFile/RecycleBin/ase_ant_2.ase");
   //LoadASEData("carrot", "../../Library/ASEFile/RecycleBin/test_carrot.ase");
   //LoadASEData("flowerpot", "../../Library/ASEFile/RecycleBin/test_flowerpot.ase");
   //LoadASEData("plate", "../../Library/ASEFile/RecycleBin/test_m_plates.ase");
   //LoadASEData("bottom", "../../Library/ASEFile/RecycleBin/test2_bottom.ase");
   //LoadASEData("can", "../../Library/ASEFile/RecycleBin/can_01.ase");
   //LoadASEData("IkJoe", "../../Library/ASEFile/RecycleBin/03IK-Joe.ase");
   //LoadASEData("ani_dead",               "../../Library/ASEFile/ani_dead.ase");
   //LoadASEData("ani_interaction",        "../../Library/ASEFile/ani_interaction.ase");
   //LoadASEData("ani_standing",           "../../Library/ASEFile/ani_standing.ase");
   //LoadASEData("ani_walk",               "../../Library/ASEFile/ani_walk.ase");
      
   // ASE 파일을 로드   
   ///*
   LoadASEData("navimesh",               "../../Library/ASEFile/navimesh.ase");
   //LoadASEData("ant",                    "../../Library/ASEFile/ant.ase");
   LoadASEData("ant",                    "../../Library/ASEFile/ant_ani1.ase");
   LoadASEData("antboid",                "../../Library/ASEFile/antboid.ase");     
   LoadASEData("apple",                  "../../Library/ASEFile/apple.ase");
   LoadASEData("avocado",                "../../Library/ASEFile/avocado.ase");
   LoadASEData("avocado_01",             "../../Library/ASEFile/avocado_01.ase");
   LoadASEData("avocadoseed",            "../../Library/ASEFile/avocadoseed.ase");
   LoadASEData("bg_bottom",              "../../Library/ASEFile/bg_bottom.ase");
   LoadASEData("bg_bottom_door",         "../../Library/ASEFile/bg_bottom_door.ase");
   LoadASEData("bg_cabient",             "../../Library/ASEFile/bg_cabient.ase");
   LoadASEData("bg_door",                "../../Library/ASEFile/bg_door.ase");
   LoadASEData("bg_door_glass",          "../../Library/ASEFile/bg_door_glass.ase");
   LoadASEData("bg_kitchen",             "../../Library/ASEFile/bg_kitchen.ase");
   LoadASEData("bg_refrigerator",        "../../Library/ASEFile/bg_refrigerator.ase");
   LoadASEData("bg_table",               "../../Library/ASEFile/bg_table.ase");
   LoadASEData("bg_table_cloth",         "../../Library/ASEFile/bg_table_cloth.ase");
   LoadASEData("board",                  "../../Library/ASEFile/board.ase");
   LoadASEData("book1",                  "../../Library/ASEFile/book1.ase");
   LoadASEData("book1_01",               "../../Library/ASEFile/book1_01.ase");
   LoadASEData("book1_02",               "../../Library/ASEFile/book1_02.ase"); 
   LoadASEData("book1_03",               "../../Library/ASEFile/book1_03.ase");
   LoadASEData("book1_04",               "../../Library/ASEFile/book1_04.ase");
   LoadASEData("book1_05",               "../../Library/ASEFile/book1_05.ase");
   LoadASEData("book1_06",               "../../Library/ASEFile/book1_06.ase");
   LoadASEData("book1_07",               "../../Library/ASEFile/book1_07.ase");
   LoadASEData("book1_08",               "../../Library/ASEFile/book1_08.ase");
   LoadASEData("book2",                  "../../Library/ASEFile/book2.ase");
   LoadASEData("book2_01",               "../../Library/ASEFile/book2_01.ase");
   LoadASEData("bookcabinet1",           "../../Library/ASEFile/bookcabinet1.ase");
   LoadASEData("bookcabinet2",           "../../Library/ASEFile/bookcabinet2.ase");
   LoadASEData("bookcabinet2_01",        "../../Library/ASEFile/bookcabinet2_01.ase");
   LoadASEData("bookcabinet3",           "../../Library/ASEFile/bookcabinet3.ase");
   LoadASEData("boot",                   "../../Library/ASEFile/boot.ase");
   LoadASEData("bread1",                 "../../Library/ASEFile/bread1.ase");
   LoadASEData("bread1_01",              "../../Library/ASEFile/bread1_01.ase");
   LoadASEData("bread3",                 "../../Library/ASEFile/bread3.ase");
   LoadASEData("bread3_01",              "../../Library/ASEFile/bread3_01.ase"); 
   LoadASEData("bread4",                 "../../Library/ASEFile/bread4.ase");
   LoadASEData("broom",                  "../../Library/ASEFile/broom.ase");
   LoadASEData("butter",                 "../../Library/ASEFile/butter.ase");
   LoadASEData("cactus",                 "../../Library/ASEFile/cactus.ase");
   LoadASEData("candy1",                 "../../Library/ASEFile/candy1.ase");
   LoadASEData("candy1_01",              "../../Library/ASEFile/candy1_01.ase");
   LoadASEData("candy2",                 "../../Library/ASEFile/candy2.ase");
   LoadASEData("candy2_01",              "../../Library/ASEFile/candy2_01.ase");
   LoadASEData("candy3",                 "../../Library/ASEFile/candy3.ase");
   LoadASEData("candy3_01",              "../../Library/ASEFile/candy3_01.ase");
   LoadASEData("candyroll",              "../../Library/ASEFile/candyroll.ase");
   LoadASEData("candyroll_01",           "../../Library/ASEFile/candyroll_01.ase");
   LoadASEData("carpet",                 "../../Library/ASEFile/carpet.ase");
   LoadASEData("carrot",                 "../../Library/ASEFile/carrot.ase");
   LoadASEData("carrot2",                "../../Library/ASEFile/carrot2.ase");
   LoadASEData("case",                   "../../Library/ASEFile/case.ase");
   LoadASEData("ceilingfan",             "../../Library/ASEFile/ceilingfan.ase");
   LoadASEData("chair",                  "../../Library/ASEFile/chair.ase");
   LoadASEData("chair2",                 "../../Library/ASEFile/chair2.ase");
   LoadASEData("cheese1",                "../../Library/ASEFile/cheese1.ase");
   LoadASEData("cheese1_01",             "../../Library/ASEFile/cheese1_01.ase");
   LoadASEData("cheese1_02",             "../../Library/ASEFile/cheese1_02.ase");
   LoadASEData("cheese2",                "../../Library/ASEFile/cheese2.ase");
   LoadASEData("cheese2_01",             "../../Library/ASEFile/cheese2_01.ase");
   //LoadASEData("cherry",                 "../../Library/ASEFile/cherry.ase");
   LoadASEData("chocobar",               "../../Library/ASEFile/chocobar.ase");
   LoadASEData("chocobar_01",            "../../Library/ASEFile/chocobar_01.ase");
   LoadASEData("chocobar_02",            "../../Library/ASEFile/chocobar_02.ase");
   LoadASEData("chocolate",              "../../Library/ASEFile/chocolate.ase");
   LoadASEData("chocolate_01",           "../../Library/ASEFile/chocolate_01.ase");
   LoadASEData("clock",                  "../../Library/ASEFile/clock.ase");
   LoadASEData("cloth",                  "../../Library/ASEFile/cloth.ase");
   LoadASEData("coffeecup",              "../../Library/ASEFile/coffeecup.ase");
   LoadASEData("coffeecup2",             "../../Library/ASEFile/coffeecup2.ase");
   LoadASEData("coffeegrinder",          "../../Library/ASEFile/coffeegrinder.ase");
   LoadASEData("cruet",                  "../../Library/ASEFile/cruet.ase");
   LoadASEData("cruet_2",                "../../Library/ASEFile/cruet_2.ase");
   LoadASEData("cup1",                   "../../Library/ASEFile/cup1.ase");
   LoadASEData("cup1_01",                "../../Library/ASEFile/cup1_01.ase");
   LoadASEData("cup2",                   "../../Library/ASEFile/cup2.ase");
   LoadASEData("cupcake",                "../../Library/ASEFile/cupcake.ase");
   LoadASEData("cupcake_01",             "../../Library/ASEFile/cupcake_01.ase");
   LoadASEData("cupcake_02",             "../../Library/ASEFile/cupcake_02.ase");
   LoadASEData("cupcake_03",             "../../Library/ASEFile/cupcake_03.ase");
   LoadASEData("cupcake_04",             "../../Library/ASEFile/cupcake_04.ase");
   LoadASEData("cutmushroom_01",         "../../Library/ASEFile/cutmushroom_01.ase");
   LoadASEData("cutmushroom",            "../../Library/ASEFile/cutmushroom.ase");
   LoadASEData("dish1",                  "../../Library/ASEFile/dish1.ase");
   LoadASEData("dish2",                  "../../Library/ASEFile/dish2.ase");
   LoadASEData("dish2_01",               "../../Library/ASEFile/dish2_01.ase");
   LoadASEData("dish2_02",               "../../Library/ASEFile/dish2_02.ase");
   LoadASEData("dish3",                  "../../Library/ASEFile/dish3.ase");
   LoadASEData("dish3_01",               "../../Library/ASEFile/dish3_01.ase");
   LoadASEData("eggplant",               "../../Library/ASEFile/eggplant.ase");
   LoadASEData("etc_knife",              "../../Library/ASEFile/etc_knife.ase");
   LoadASEData("flag",                   "../../Library/ASEFile/flag.ase");
   LoadASEData("flag_1",                 "../../Library/ASEFile/flag_1.ase");
   LoadASEData("flag_2",                 "../../Library/ASEFile/flag_2.ase");
   LoadASEData("flag_3",                 "../../Library/ASEFile/flag_3.ase");
   LoadASEData("flag_4",                 "../../Library/ASEFile/flag_4.ase");
   LoadASEData("flag_5",                 "../../Library/ASEFile/flag_5.ase");
   LoadASEData("flag_6",                 "../../Library/ASEFile/flag_6.ase");
   LoadASEData("flag_7",                 "../../Library/ASEFile/flag_7.ase");
   LoadASEData("flag_8",                 "../../Library/ASEFile/flag_8.ase");
   LoadASEData("flag_9",                 "../../Library/ASEFile/flag_9.ase");
   LoadASEData("flag_10",                "../../Library/ASEFile/flag_10.ase");
   LoadASEData("flag_11",                "../../Library/ASEFile/flag_11.ase");
   LoadASEData("flag_12",                "../../Library/ASEFile/flag_12.ase");
   LoadASEData("flag_13",                "../../Library/ASEFile/flag_13.ase");
   LoadASEData("flag_14",                "../../Library/ASEFile/flag_14.ase");
   LoadASEData("flour",                  "../../Library/ASEFile/flour.ase");
   LoadASEData("flowerpot",              "../../Library/ASEFile/flowerpot.ase");
   LoadASEData("flowervase",             "../../Library/ASEFile/flowervase.ase");
   LoadASEData("fork",                   "../../Library/ASEFile/fork.ase");
   LoadASEData("fork_01",                "../../Library/ASEFile/fork_01.ase");
   LoadASEData("frame",                  "../../Library/ASEFile/frame.ase");
   LoadASEData("gimmick_knife",          "../../Library/ASEFile/gimmick_knife.ase");
   LoadASEData("gimmick_knife_2",        "../../Library/ASEFile/gimmick_knife_2.ase");
   LoadASEData("gimmick_knife_3",        "../../Library/ASEFile/gimmick_knife_3.ase");
   LoadASEData("gimmick_knife_4",        "../../Library/ASEFile/gimmick_knife_4.ase");
   LoadASEData("gimmick_knife_5",        "../../Library/ASEFile/gimmick_knife_5.ase");
   LoadASEData("icebox",                 "../../Library/ASEFile/icebox.ase");
   LoadASEData("jam",                    "../../Library/ASEFile/jam.ase");
   LoadASEData("jar",                    "../../Library/ASEFile/jar.ase");
   LoadASEData("ladle",                  "../../Library/ASEFile/ladle.ase");
   LoadASEData("lflowerpot",             "../../Library/ASEFile/lflowerpot.ase");
   LoadASEData("lid",                    "../../Library/ASEFile/lid.ase");
   LoadASEData("m_plates",               "../../Library/ASEFile/m_plates.ase");
   LoadASEData("m_wooden_plate",         "../../Library/ASEFile/m_wooden_plate.ase");
   LoadASEData("mat",                    "../../Library/ASEFile/mat.ase");
   LoadASEData("memo",                   "../../Library/ASEFile/memo.ase");
   LoadASEData("memo2",                  "../../Library/ASEFile/memo2.ase");
   LoadASEData("microwave",              "../../Library/ASEFile/microwave.ase");
   LoadASEData("milk",                   "../../Library/ASEFile/milk.ase");
   LoadASEData("mixermachine",           "../../Library/ASEFile/mixermachine.ase");
   LoadASEData("mixerball",              "../../Library/ASEFile/mixerball.ase");
   LoadASEData("nail1",                  "../../Library/ASEFile/nail1.ase");  
   LoadASEData("nail2",                  "../../Library/ASEFile/nail2.ase");
   LoadASEData("nail3",                  "../../Library/ASEFile/nail3.ase");
   LoadASEData("oven",                   "../../Library/ASEFile/oven.ase");
   LoadASEData("oven_button_a",          "../../Library/ASEFile/oven_button_a.ase");
   LoadASEData("oven_button_b",          "../../Library/ASEFile/oven_button_b.ase");
   LoadASEData("oven_fire_a_off",        "../../Library/ASEFile/oven_fire_a_off.ase");
   LoadASEData("oven_fire_a_on",         "../../Library/ASEFile/oven_fire_a_on.ase");
   LoadASEData("oven_fire_b_off",        "../../Library/ASEFile/oven_fire_b_off.ase");
   LoadASEData("oven_fire_b_on",         "../../Library/ASEFile/oven_fire_b_on.ase");
   LoadASEData("oven_fire_c_off",        "../../Library/ASEFile/oven_fire_c_off.ase");
   LoadASEData("oven_fire_c_on",         "../../Library/ASEFile/oven_fire_c_on.ase");	
   LoadASEData("oven_fire_d_off",        "../../Library/ASEFile/oven_fire_d_off.ase");
   LoadASEData("oven_fire_d_on",         "../../Library/ASEFile/oven_fire_d_on.ase");
   LoadASEData("oven_glass",             "../../Library/ASEFile/oven_glass.ase");
   LoadASEData("oven_top",               "../../Library/ASEFile/oven_top.ase");
   LoadASEData("ovenmitt",               "../../Library/ASEFile/ovenmitt.ase");
   LoadASEData("parcel1",                "../../Library/ASEFile/parcel1.ase");
   LoadASEData("parcel2",                "../../Library/ASEFile/parcel2.ase");
   LoadASEData("plant",                  "../../Library/ASEFile/plant.ase");
   LoadASEData("plasticbox",             "../../Library/ASEFile/plasticbox.ase");
   LoadASEData("pot1",                   "../../Library/ASEFile/pot1.ase");
   LoadASEData("pot2",                   "../../Library/ASEFile/pot2.ase");
   LoadASEData("pot3",                   "../../Library/ASEFile/pot3.ase");
   LoadASEData("potato",                 "../../Library/ASEFile/potato.ase");
   LoadASEData("potato2",                "../../Library/ASEFile/potato2.ase");
   LoadASEData("potato3",                "../../Library/ASEFile/potato3.ase");
   LoadASEData("pumpkin",                "../../Library/ASEFile/pumpkin.ase"); 
   LoadASEData("radio",                  "../../Library/ASEFile/radio.ase"); 
   LoadASEData("roadboard",              "../../Library/ASEFile/roadboard.ase"); 
   LoadASEData("sauce",                  "../../Library/ASEFile/sauce.ase");
   LoadASEData("sauce2",                 "../../Library/ASEFile/sauce2.ase");
   LoadASEData("sflowerpot",             "../../Library/ASEFile/sflowerpot.ase");
   LoadASEData("sflowerpot2",            "../../Library/ASEFile/sflowerpot2.ase");
   LoadASEData("sink",                   "../../Library/ASEFile/sink.ase");
   LoadASEData("soup1",                  "../../Library/ASEFile/soup1.ase");
   LoadASEData("soup1_01",               "../../Library/ASEFile/soup1_01.ase");
   LoadASEData("soup1_02",               "../../Library/ASEFile/soup1_02.ase");
   LoadASEData("soup2",                  "../../Library/ASEFile/soup2.ase");
   LoadASEData("soup2_01",               "../../Library/ASEFile/soup2_01.ase");
   LoadASEData("soup2_02",               "../../Library/ASEFile/soup2_02.ase");
   LoadASEData("spatula",                "../../Library/ASEFile/spatula.ase");
   LoadASEData("spatula_01",             "../../Library/ASEFile/spatula_01.ase");
   LoadASEData("spoonholder",            "../../Library/ASEFile/spoonholder.ase");
   LoadASEData("start_table",            "../../Library/ASEFile/start_table.ase");
   LoadASEData("teapot",                 "../../Library/ASEFile/teapot.ase");
   LoadASEData("tincase",                "../../Library/ASEFile/tincase.ase");
   LoadASEData("toaster",                "../../Library/ASEFile/toaster.ase");
   LoadASEData("top_a",                  "../../Library/ASEFile/top_a.ase");
   LoadASEData("top_b",                  "../../Library/ASEFile/top_b.ase");
   LoadASEData("top_door",               "../../Library/ASEFile/top_door.ase");   
   LoadASEData("toycar",                 "../../Library/ASEFile/toycar.ase");
   LoadASEData("trap",                   "../../Library/ASEFile/trap.ase");
   LoadASEData("trap_1",                 "../../Library/ASEFile/trap_1.ase");
   LoadASEData("trap_2",                 "../../Library/ASEFile/trap_2.ase");
   LoadASEData("trap_3",                 "../../Library/ASEFile/trap_3.ase");
   LoadASEData("trap_4",                 "../../Library/ASEFile/trap_4.ase");
   LoadASEData("trap_5",                 "../../Library/ASEFile/trap_5.ase");
   LoadASEData("trap_6",                 "../../Library/ASEFile/trap_6.ase");
   LoadASEData("trap_7",                 "../../Library/ASEFile/trap_7.ase");
   LoadASEData("trap_8",                 "../../Library/ASEFile/trap_8.ase");
   LoadASEData("trap_9",                 "../../Library/ASEFile/trap_9.ase");
   LoadASEData("trap_10",                "../../Library/ASEFile/trap_10.ase");
   LoadASEData("trap_11",                "../../Library/ASEFile/trap_11.ase");
   LoadASEData("trap_12",                "../../Library/ASEFile/trap_12.ase");
   LoadASEData("trap_13",                "../../Library/ASEFile/trap_13.ase");
   LoadASEData("trap_14",                "../../Library/ASEFile/trap_14.ase");
   LoadASEData("trapant",                "../../Library/ASEFile/trapant.ase");
   LoadASEData("trapant_1",              "../../Library/ASEFile/trapant_1.ase");
   LoadASEData("trapant_2",              "../../Library/ASEFile/trapant_2.ase");
   LoadASEData("trapant_3",              "../../Library/ASEFile/trapant_3.ase");
   LoadASEData("trapant_4",              "../../Library/ASEFile/trapant_4.ase");
   LoadASEData("trapant_5",              "../../Library/ASEFile/trapant_5.ase");
   LoadASEData("trapant_6",              "../../Library/ASEFile/trapant_6.ase");
   LoadASEData("trapant_7",              "../../Library/ASEFile/trapant_7.ase");
   LoadASEData("trapant_8",              "../../Library/ASEFile/trapant_8.ase");
   LoadASEData("trapant_9",              "../../Library/ASEFile/trapant_9.ase");
   LoadASEData("trapant_10",             "../../Library/ASEFile/trapant_10.ase");
   LoadASEData("trapant_11",             "../../Library/ASEFile/trapant_11.ase");
   LoadASEData("trapant_12",             "../../Library/ASEFile/trapant_12.ase");
   LoadASEData("trapant_13",             "../../Library/ASEFile/trapant_13.ase");
   LoadASEData("trapant_14",             "../../Library/ASEFile/trapant_14.ase");
   LoadASEData("tulip",                  "../../Library/ASEFile/tulip.ase");
   LoadASEData("umbrella",               "../../Library/ASEFile/umbrella.ase");
   LoadASEData("whisk",                  "../../Library/ASEFile/whisk.ase"); 
   LoadASEData("window",                 "../../Library/ASEFile/window.ase");
   LoadASEData("wine1",                  "../../Library/ASEFile/wine1.ase");
   LoadASEData("wine2",                  "../../Library/ASEFile/wine2.ase");
   //*/
   // 8.25 추가
   LoadASEData("hangingplant",           "../../Library/ASEFile/hangingplant.ase");
   LoadASEData("wateringcan",            "../../Library/ASEFile/wateringcan.ase");
   LoadASEData("partyflag",              "../../Library/ASEFile/partyflag.ase");
   LoadASEData("shelf",                  "../../Library/ASEFile/shelf.ase");
   LoadASEData("cookietray",             "../../Library/ASEFile/cookietray.ase");
   LoadASEData("spraycream",             "../../Library/ASEFile/spraycream.ase");
   LoadASEData("memo3_1",                "../../Library/ASEFile/memo3_1.ase");
   LoadASEData("memo3_2",                "../../Library/ASEFile/memo3_2.ase");
   LoadASEData("phone",                  "../../Library/ASEFile/phone.ase");
   LoadASEData("cup2_01",                "../../Library/ASEFile/cup2_01.ase");
   LoadASEData("cup2_02",                "../../Library/ASEFile/cup2_02.ase");

   // 8.26 추가
   LoadASEData("n_candy",                "../../Library/ASEFile/n_candy.ase");
   LoadASEData("n_plasticbox",           "../../Library/ASEFile/n_plasticbox.ase");
   LoadASEData("n_soup",                 "../../Library/ASEFile/n_soup.ase");
   LoadASEData("n_soup_2",               "../../Library/ASEFile/n_soup_2.ase");
   LoadASEData("n_etcknife",             "../../Library/ASEFile/n_etcknife.ase");
   LoadASEData("n_cupboard",             "../../Library/ASEFile/n_cupboard.ase");

   



}

ASEParser* DXObjectFactory::GetASEData(std::string name)
{
   return m_ASEData_um[name];
}
