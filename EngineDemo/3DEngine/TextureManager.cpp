#include "TextureManager.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

TextureManager::TextureManager(ID3D11Device* pDevice)
	: m_pDevice(pDevice)
{

}

TextureManager::~TextureManager()
{
}

void TextureManager::LoadTexture(string modelName, wstring fileName)
{
	ID3D11Resource* _texResource = nullptr;
	ID3D11ShaderResourceView* _diffuseMap = nullptr;

	HR(CreateDDSTextureFromFile(m_pDevice, fileName.c_str(), &_texResource, &_diffuseMap));

	m_DiffuseMap_um.insert({ modelName , _diffuseMap });
}

void TextureManager::LoadNormalMap(string modelName, wstring fileName)
{
   ID3D11Resource* _texResource = nullptr;
   ID3D11ShaderResourceView* _diffuseMap = nullptr;

   HR(CreateDDSTextureFromFile(m_pDevice, fileName.c_str(), &_texResource, &_diffuseMap));

	m_NormalMap_um.insert({ modelName , _diffuseMap });
}

void TextureManager::LoadPNGImage(string name, wstring fileName)
{
   ID3D11Resource* _texResource = nullptr;
   ID3D11ShaderResourceView* _image = nullptr;

   HR(CreateWICTextureFromFile(m_pDevice, fileName.c_str(), &_texResource, &_image));

   m_PNGImages_um.insert({ name , _image });
}

void TextureManager::SaveTexture()
{
   /// 텍스쳐는 문제가 없는듯하다
	// DiffuseMap   
   //LoadTexture("ant", L"../../Library/Textures/RecycleBin/Ant_DiffuseMap.dds");
   //LoadTexture("carrot", L"../../Library/Textures/RecycleBin/test_carrot.dds");
   //LoadTexture("flowerpot", L"../../Library/Textures/RecycleBin/test_flowerpot.dds");
   //LoadTexture("plate", L"../../Library/Textures/RecycleBin/test_m_plates.dds");
   //LoadTexture("bottom", L"../../Library/Textures/RecycleBin/test_bottom.dds");
   //LoadTexture("can", L"../../Library/Textures/RecycleBin/can_01.dds");         
   LoadTexture("ant",            L"../../Library/Textures/ant.dds");



   //LoadTexture("ant",            L"../../Library/Textures/ant.dds");
   LoadTexture("antboid",        L"../../Library/Textures/antboid.dds");
   LoadTexture("apple",          L"../../Library/Textures/apple.dds");
   LoadTexture("avocado",        L"../../Library/Textures/avocado.dds");
   LoadTexture("avocado_01",     L"../../Library/Textures/avocado_01.dds");
   LoadTexture("avocadoseed",    L"../../Library/Textures/avocadoseed.dds");
   LoadTexture("bg_bottom",      L"../../Library/Textures/bg_bottom.dds");
   LoadTexture("bg_bottom_door", L"../../Library/Textures/bg_bottom_door.dds");
   LoadTexture("bg_cabient",     L"../../Library/Textures/bg_cabient.dds");
   LoadTexture("bg_door",        L"../../Library/Textures/bg_door.dds");
   LoadTexture("bg_door_glass",  L"../../Library/Textures/bg_door_glass.dds");
   LoadTexture("bg_kitchen",     L"../../Library/Textures/bg_kitchen.dds");
   LoadTexture("bg_refrigerator",L"../../Library/Textures/bg_refrigerator.dds");
   LoadTexture("bg_table",       L"../../Library/Textures/bg_table.dds");
   LoadTexture("bg_table_cloth", L"../../Library/Textures/bg_table_cloth.dds");
   LoadTexture("board",          L"../../Library/Textures/board.dds");
   LoadTexture("book1",          L"../../Library/Textures/book1.dds");
   LoadTexture("book1_01",       L"../../Library/Textures/book1_01.dds");
   LoadTexture("book1_02",       L"../../Library/Textures/book1_02.dds"); 
   LoadTexture("book1_03",       L"../../Library/Textures/book1_03.dds");
   LoadTexture("book1_04",       L"../../Library/Textures/book1_04.dds");
   LoadTexture("book1_05",       L"../../Library/Textures/book1_05.dds");
   LoadTexture("book1_06",       L"../../Library/Textures/book1_06.dds");
   LoadTexture("book1_07",       L"../../Library/Textures/book1_07.dds");
   LoadTexture("book1_08",       L"../../Library/Textures/book1_08.dds");
   LoadTexture("book2",          L"../../Library/Textures/book2.dds");
   LoadTexture("book2_01",       L"../../Library/Textures/book2_01.dds");
   LoadTexture("bookcabinet1",   L"../../Library/Textures/bookcabinet1.dds");
   LoadTexture("bookcabinet2",   L"../../Library/Textures/bookcabinet2.dds");
   LoadTexture("bookcabinet2_01",L"../../Library/Textures/bookcabinet2_01.dds");
   LoadTexture("bookcabinet3",   L"../../Library/Textures/bookcabinet3.dds");
   LoadTexture("boot",           L"../../Library/Textures/boot.dds");
   LoadTexture("bread1",         L"../../Library/Textures/bread1.dds");
   LoadTexture("bread1_01",      L"../../Library/Textures/bread1_01.dds");
   LoadTexture("bread3",         L"../../Library/Textures/bread3.dds");
   LoadTexture("bread3_01",      L"../../Library/Textures/bread3_01.dds");
   LoadTexture("bread4",         L"../../Library/Textures/bread4.dds");
   LoadTexture("broom",          L"../../Library/Textures/broom.dds");
   LoadTexture("butter",         L"../../Library/Textures/butter.dds");
   LoadTexture("cactus",         L"../../Library/Textures/cactus.dds");
   LoadTexture("candy1",         L"../../Library/Textures/candy1.dds");
   LoadTexture("candy1_01",      L"../../Library/Textures/candy1_01.dds");
   LoadTexture("candy2",         L"../../Library/Textures/candy2.dds");
   LoadTexture("candy2_01",      L"../../Library/Textures/candy2_01.dds");
   LoadTexture("candy3",         L"../../Library/Textures/candy3.dds");
   LoadTexture("candy3_01",      L"../../Library/Textures/candy3_01.dds");
   LoadTexture("candyroll",      L"../../Library/Textures/candyroll.dds");
   LoadTexture("candyroll_01",   L"../../Library/Textures/candyroll_01.dds");
   LoadTexture("carpet",         L"../../Library/Textures/carpet.dds");
   LoadTexture("carrot",         L"../../Library/Textures/carrot.dds");
   LoadTexture("carrot2",        L"../../Library/Textures/carrot2.dds");
   LoadTexture("case",           L"../../Library/Textures/case.dds");
   LoadTexture("ceilingfan",     L"../../Library/Textures/ceilingfan.dds");
   LoadTexture("chair",          L"../../Library/Textures/chair.dds");
   LoadTexture("chair2",         L"../../Library/Textures/chair2.dds");
   LoadTexture("cheese1",        L"../../Library/Textures/cheese1.dds");
   LoadTexture("cheese1_01",     L"../../Library/Textures/cheese1_01.dds");
   LoadTexture("cheese1_02",     L"../../Library/Textures/cheese1_02.dds");
   LoadTexture("cheese2",        L"../../Library/Textures/cheese2.dds");
   LoadTexture("cheese2_01",     L"../../Library/Textures/cheese2_01.dds");
   //LoadTexture("cherry",         L"../../Library/Textures/cherry.dds");
   LoadTexture("chocobar",       L"../../Library/Textures/chocobar.dds");
   LoadTexture("chocobar_01",    L"../../Library/Textures/chocobar_01.dds");
   LoadTexture("chocobar_02",    L"../../Library/Textures/chocobar_02.dds");
   LoadTexture("chocolate",      L"../../Library/Textures/chocolate.dds");
   LoadTexture("chocolate_01",   L"../../Library/Textures/chocolate_01.dds");
   LoadTexture("clock",          L"../../Library/Textures/clock.dds");
   LoadTexture("cloth",          L"../../Library/Textures/cloth.dds");
   LoadTexture("coffeecup",      L"../../Library/Textures/coffeecup.dds");
   LoadTexture("coffeecup2",     L"../../Library/Textures/coffeecup2.dds");
   LoadTexture("coffeegrinder",  L"../../Library/Textures/coffeegrinder.dds");
   LoadTexture("cruet",          L"../../Library/Textures/cruet.dds");
   LoadTexture("cruet_2",        L"../../Library/Textures/cruet_2.dds");
   LoadTexture("cup1",           L"../../Library/Textures/cup1.dds");
   LoadTexture("cup1_01",        L"../../Library/Textures/cup1_01.dds");
   LoadTexture("cup2",           L"../../Library/Textures/cup2.dds");
   LoadTexture("cupcake",        L"../../Library/Textures/cupcake.dds");
   LoadTexture("cupcake_01",     L"../../Library/Textures/cupcake_01.dds");
   LoadTexture("cupcake_02",     L"../../Library/Textures/cupcake_02.dds");
   LoadTexture("cupcake_03",     L"../../Library/Textures/cupcake_03.dds");
   LoadTexture("cupcake_04",     L"../../Library/Textures/cupcake_04.dds");
   LoadTexture("cutmushroom_01", L"../../Library/Textures/cutmushroom_01.dds");
   LoadTexture("cutmushroom",    L"../../Library/Textures/cutmushroom.dds");
   LoadTexture("dish1",          L"../../Library/Textures/dish1.dds");
   LoadTexture("dish2",          L"../../Library/Textures/dish2.dds");
   LoadTexture("dish2_01",       L"../../Library/Textures/dish2_01.dds");
   LoadTexture("dish2_02",       L"../../Library/Textures/dish2_02.dds");
   LoadTexture("dish3",          L"../../Library/Textures/dish3.dds");
   LoadTexture("dish3_01",       L"../../Library/Textures/dish3_01.dds");
   LoadTexture("eggplant",       L"../../Library/Textures/eggplant.dds");
   LoadTexture("etc_knife",      L"../../Library/Textures/etc_knife.dds");
   LoadTexture("flag",           L"../../Library/Textures/flag.dds");
   LoadTexture("flag_1",         L"../../Library/Textures/flag_1.dds");
   LoadTexture("flag_2",         L"../../Library/Textures/flag_2.dds");
   LoadTexture("flag_3",         L"../../Library/Textures/flag_3.dds");
   LoadTexture("flag_4",         L"../../Library/Textures/flag_4.dds");
   LoadTexture("flag_5",         L"../../Library/Textures/flag_5.dds");
   LoadTexture("flag_6",         L"../../Library/Textures/flag_6.dds");
   LoadTexture("flag_7",         L"../../Library/Textures/flag_7.dds");
   LoadTexture("flag_8",         L"../../Library/Textures/flag_8.dds");
   LoadTexture("flag_9",         L"../../Library/Textures/flag_9.dds");
   LoadTexture("flag_10",        L"../../Library/Textures/flag_10.dds");
   LoadTexture("flag_11",        L"../../Library/Textures/flag_11.dds");
   LoadTexture("flag_12",        L"../../Library/Textures/flag_12.dds");
   LoadTexture("flag_13",        L"../../Library/Textures/flag_13.dds");
   LoadTexture("flag_14",        L"../../Library/Textures/flag_14.dds");
   LoadTexture("flour",          L"../../Library/Textures/flour.dds");
   LoadTexture("flowerpot",      L"../../Library/Textures/flowerpot.dds");
   LoadTexture("flowervase",     L"../../Library/Textures/flowervase.dds");
   LoadTexture("fork",           L"../../Library/Textures/fork.dds");
   LoadTexture("fork_01",        L"../../Library/Textures/fork_01.dds");
   LoadTexture("frame",          L"../../Library/Textures/frame.dds");
   LoadTexture("gimmick_knife",  L"../../Library/Textures/gimmick_knife.dds");
   LoadTexture("gimmick_knife_2",L"../../Library/Textures/gimmick_knife_2.dds");
   LoadTexture("gimmick_knife_3",L"../../Library/Textures/gimmick_knife_3.dds");
   LoadTexture("gimmick_knife_4",L"../../Library/Textures/gimmick_knife_4.dds");
   LoadTexture("gimmick_knife_5",L"../../Library/Textures/gimmick_knife_5.dds");
   LoadTexture("icebox",         L"../../Library/Textures/icebox.dds");
   LoadTexture("jam",            L"../../Library/Textures/jam.dds");
   LoadTexture("jar",            L"../../Library/Textures/jar.dds");
   LoadTexture("ladle",          L"../../Library/Textures/ladle.dds");
   LoadTexture("lflowerpot",     L"../../Library/Textures/lflowerpot.dds");
   LoadTexture("lid",            L"../../Library/Textures/lid.dds");
	LoadTexture("m_plates",       L"../../Library/Textures/m_plates.dds");
	LoadTexture("m_wooden_plate", L"../../Library/Textures/m_wooden_plate.dds");
   LoadTexture("mat",            L"../../Library/Textures/mat.dds");
   LoadTexture("memo",           L"../../Library/Textures/memo.dds");
   LoadTexture("memo2",          L"../../Library/Textures/memo2.dds");
   LoadTexture("microwave",      L"../../Library/Textures/microwave.dds");
   LoadTexture("milk",           L"../../Library/Textures/milk.dds");
   LoadTexture("mixermachine",   L"../../Library/Textures/mixermachine.dds");
   LoadTexture("mixerball",      L"../../Library/Textures/mixerball.dds");
   LoadTexture("nail1",          L"../../Library/Textures/nail1.dds");
   LoadTexture("nail2",          L"../../Library/Textures/nail2.dds");
   LoadTexture("nail3",          L"../../Library/Textures/nail3.dds");
   LoadTexture("oven",           L"../../Library/Textures/oven.dds");
   LoadTexture("oven_button_a",  L"../../Library/Textures/oven_button_a.dds");
   LoadTexture("oven_button_b",  L"../../Library/Textures/oven_button_b.dds");
   LoadTexture("oven_fire_a_off",L"../../Library/Textures/oven_fire_a_off.dds");
   LoadTexture("oven_fire_a_on", L"../../Library/Textures/oven_fire_a_on.dds");
   LoadTexture("oven_fire_b_off",L"../../Library/Textures/oven_fire_b_off.dds");
   LoadTexture("oven_fire_b_on", L"../../Library/Textures/oven_fire_b_on.dds");
   LoadTexture("oven_fire_c_off",L"../../Library/Textures/oven_fire_c_off.dds");
   LoadTexture("oven_fire_c_on", L"../../Library/Textures/oven_fire_c_on.dds");	
   LoadTexture("oven_fire_d_off",L"../../Library/Textures/oven_fire_d_off.dds");
   LoadTexture("oven_fire_d_on", L"../../Library/Textures/oven_fire_d_on.dds");
   LoadTexture("oven_glass",     L"../../Library/Textures/oven_glass.dds");
   LoadTexture("oven_top",       L"../../Library/Textures/oven_top.dds");
   LoadTexture("ovenmitt",       L"../../Library/Textures/ovenmitt.dds");
   LoadTexture("parcel1",        L"../../Library/Textures/parcel1.dds");
   LoadTexture("parcel2",        L"../../Library/Textures/parcel2.dds");
   LoadTexture("plant",          L"../../Library/Textures/plant.dds");
   LoadTexture("plasticbox",     L"../../Library/Textures/plasticbox.dds");
   LoadTexture("pot1",           L"../../Library/Textures/pot1.dds");
   LoadTexture("pot2",           L"../../Library/Textures/pot2.dds");
   LoadTexture("pot3",           L"../../Library/Textures/pot3.dds");
   LoadTexture("potato",         L"../../Library/Textures/potato.dds");
   LoadTexture("potato2",        L"../../Library/Textures/potato2.dds");
   LoadTexture("potato3",        L"../../Library/Textures/potato3.dds");
   LoadTexture("pumpkin",        L"../../Library/Textures/pumpkin.dds"); 
   LoadTexture("radio",          L"../../Library/Textures/radio.dds");
   LoadTexture("roadboard",      L"../../Library/Textures/roadboard.dds");
   LoadTexture("sauce",          L"../../Library/Textures/sauce.dds");
   LoadTexture("sauce2",         L"../../Library/Textures/sauce2.dds");
   LoadTexture("sflowerpot",     L"../../Library/Textures/sflowerpot.dds");
   LoadTexture("sflowerpot2",    L"../../Library/Textures/sflowerpot2.dds");
   LoadTexture("sink",           L"../../Library/Textures/sink.dds");
   LoadTexture("soup1",          L"../../Library/Textures/soup1.dds");
   LoadTexture("soup1_01",       L"../../Library/Textures/soup1_01.dds");
   LoadTexture("soup1_02",       L"../../Library/Textures/soup1_02.dds");
   LoadTexture("soup2",          L"../../Library/Textures/soup2.dds");
   LoadTexture("soup2_01",       L"../../Library/Textures/soup2_01.dds");
   LoadTexture("soup2_02",       L"../../Library/Textures/soup2_02.dds");
   LoadTexture("spatula",        L"../../Library/Textures/spatula.dds");
   LoadTexture("spatula_01",     L"../../Library/Textures/spatula_01.dds");
   LoadTexture("spoonholder",    L"../../Library/Textures/spoonholder.dds");
   LoadTexture("start_table",    L"../../Library/Textures/start_table.dds");
   LoadTexture("teapot",         L"../../Library/Textures/teapot.dds");
   LoadTexture("tincase",        L"../../Library/Textures/tincase.dds");
   LoadTexture("toaster",        L"../../Library/Textures/toaster.dds");
   LoadTexture("top_a",          L"../../Library/Textures/top_a.dds");
   LoadTexture("top_b",          L"../../Library/Textures/top_b.dds");
   LoadTexture("top_door",       L"../../Library/Textures/top_door.dds");
   LoadTexture("toycar",         L"../../Library/Textures/toycar.dds");
   LoadTexture("trap",           L"../../Library/Textures/trap.dds");
   LoadTexture("trap_1",         L"../../Library/Textures/trap_1.dds");
   LoadTexture("trap_2",         L"../../Library/Textures/trap_2.dds");
   LoadTexture("trap_3",         L"../../Library/Textures/trap_3.dds");
   LoadTexture("trap_4",         L"../../Library/Textures/trap_4.dds");
   LoadTexture("trap_5",         L"../../Library/Textures/trap_5.dds");
   LoadTexture("trap_6",         L"../../Library/Textures/trap_6.dds");
   LoadTexture("trap_7",         L"../../Library/Textures/trap_7.dds");
   LoadTexture("trap_8",         L"../../Library/Textures/trap_8.dds");
   LoadTexture("trap_9",         L"../../Library/Textures/trap_9.dds");
   LoadTexture("trap_10",        L"../../Library/Textures/trap_10.dds");
   LoadTexture("trap_11",        L"../../Library/Textures/trap_11.dds");
   LoadTexture("trap_12",        L"../../Library/Textures/trap_12.dds");
   LoadTexture("trap_13",        L"../../Library/Textures/trap_13.dds");
   LoadTexture("trap_14",        L"../../Library/Textures/trap_14.dds");
   LoadTexture("trapant",        L"../../Library/Textures/trapant.dds");
   LoadTexture("trapant_1",      L"../../Library/Textures/trapant_1.dds");
   LoadTexture("trapant_2",      L"../../Library/Textures/trapant_2.dds");
   LoadTexture("trapant_3",      L"../../Library/Textures/trapant_3.dds");
   LoadTexture("trapant_4",      L"../../Library/Textures/trapant_4.dds");
   LoadTexture("trapant_5",      L"../../Library/Textures/trapant_5.dds");
   LoadTexture("trapant_6",      L"../../Library/Textures/trapant_6.dds");
   LoadTexture("trapant_7",      L"../../Library/Textures/trapant_7.dds");
   LoadTexture("trapant_8",      L"../../Library/Textures/trapant_8.dds");
   LoadTexture("trapant_9",      L"../../Library/Textures/trapant_9.dds");
   LoadTexture("trapant_10",     L"../../Library/Textures/trapant_10.dds");
   LoadTexture("trapant_11",     L"../../Library/Textures/trapant_11.dds");
   LoadTexture("trapant_12",     L"../../Library/Textures/trapant_12.dds");
   LoadTexture("trapant_13",     L"../../Library/Textures/trapant_13.dds");
   LoadTexture("trapant_14",     L"../../Library/Textures/trapant_14.dds");
   LoadTexture("tulip",          L"../../Library/Textures/tulip.dds");
   LoadTexture("umbrella",       L"../../Library/Textures/umbrella.dds");
   LoadTexture("whisk",          L"../../Library/Textures/whisk.dds");
   LoadTexture("window",         L"../../Library/Textures/window.dds");
   LoadTexture("wine1",          L"../../Library/Textures/wine1.dds");
   LoadTexture("wine2",          L"../../Library/Textures/wine2.dds");

      // 8.25 추가
   LoadTexture("hangingplant",   L"../../Library/Textures/hangingplant.dds");
   LoadTexture("wateringcan",    L"../../Library/Textures/wateringcan.dds");
   LoadTexture("partyflag",      L"../../Library/Textures/partyflag.dds");
   LoadTexture("shelf",          L"../../Library/Textures/shelf.dds");
   LoadTexture("cookietray",     L"../../Library/Textures/cookietray.dds");
   LoadTexture("spraycream",     L"../../Library/Textures/spraycream.dds");
   LoadTexture("memo3_1",        L"../../Library/Textures/memo3_1.dds");
   LoadTexture("memo3_2",        L"../../Library/Textures/memo3_2.dds");
   LoadTexture("phone",          L"../../Library/Textures/phone.dds");
   LoadTexture("cup2_01",        L"../../Library/Textures/cup2_01.dds");
   LoadTexture("cup2_02",        L"../../Library/Textures/cup2_02.dds");



	// NormalMap
	//LoadNormalMap("Genji", L"../../Library/Textures/Genji_NormalMap.dds");
    
   // UI image
   LoadPNGImage("ant_UI",        L"../../Library/Textures/UI/ant_UI.png");
   LoadPNGImage("clear",         L"../../Library/Textures/UI/clear.png");
   LoadPNGImage("cutscene",      L"../../Library/Textures/UI/cutscene.png");
   LoadPNGImage("failed",        L"../../Library/Textures/UI/failed.png");
   LoadPNGImage("KEYSET",        L"../../Library/Textures/UI/KEYSET.png");
   LoadPNGImage("num_UI_0",      L"../../Library/Textures/UI/num_UI_0.png");
   LoadPNGImage("num_UI_1",      L"../../Library/Textures/UI/num_UI_1.png");
   LoadPNGImage("num_UI_2",      L"../../Library/Textures/UI/num_UI_2.png");
   LoadPNGImage("num_UI_3",      L"../../Library/Textures/UI/num_UI_3.png");
   LoadPNGImage("num_UI_4",      L"../../Library/Textures/UI/num_UI_4.png");
   LoadPNGImage("num_UI_5",      L"../../Library/Textures/UI/num_UI_5.png");
   LoadPNGImage("num_UI_6",      L"../../Library/Textures/UI/num_UI_6.png");
   LoadPNGImage("num_UI_7",      L"../../Library/Textures/UI/num_UI_7.png");
   LoadPNGImage("num_UI_8",      L"../../Library/Textures/UI/num_UI_8.png");
   LoadPNGImage("num_UI_9",      L"../../Library/Textures/UI/num_UI_9.png");
   LoadPNGImage("num_UI_X",      L"../../Library/Textures/UI/num_UI_X.png");
   LoadPNGImage("time",          L"../../Library/Textures/UI/time.png");
   LoadPNGImage("time_num_0",    L"../../Library/Textures/UI/time_num_0.png");
   LoadPNGImage("time_num_1",    L"../../Library/Textures/UI/time_num_1.png");
   LoadPNGImage("time_num_2",    L"../../Library/Textures/UI/time_num_2.png");
   LoadPNGImage("time_num_3",    L"../../Library/Textures/UI/time_num_3.png");
   LoadPNGImage("time_num_4",    L"../../Library/Textures/UI/time_num_4.png");
   LoadPNGImage("time_num_5",    L"../../Library/Textures/UI/time_num_5.png");
   LoadPNGImage("time_num_6",    L"../../Library/Textures/UI/time_num_6.png");
   LoadPNGImage("time_num_7",    L"../../Library/Textures/UI/time_num_7.png");
   LoadPNGImage("time_num_8",    L"../../Library/Textures/UI/time_num_8.png");
   LoadPNGImage("time_num_9",    L"../../Library/Textures/UI/time_num_9.png");
   LoadPNGImage("timeover",      L"../../Library/Textures/UI/timeover.png");
   //LoadPNGImage("title00000",    L"../../Library/Textures/UI/title00000.jpg");

   // 추가
   LoadPNGImage("KEYSET_2",          L"../../Library/Textures/UI/KEYSET_2.png");
   LoadPNGImage("ant_count_all",     L"../../Library/Textures/UI/ant_count_all.png");
   LoadPNGImage("ant_count_ant_off", L"../../Library/Textures/UI/ant_count_ant_off.png");
   LoadPNGImage("ant_count_ant_on",  L"../../Library/Textures/UI/ant_count_ant_on.png");
   LoadPNGImage("ant_count_cupcake", L"../../Library/Textures/UI/ant_count_cupcake.png");
   LoadPNGImage("Comp1_00000",       L"../../Library/Textures/UI/Comp1_00000.png");
   LoadPNGImage("Comp1_00001",       L"../../Library/Textures/UI/Comp1_00001.png");
   LoadPNGImage("Comp1_00002",       L"../../Library/Textures/UI/Comp1_00002.png");
   LoadPNGImage("Comp1_00003",       L"../../Library/Textures/UI/Comp1_00003.png");
   LoadPNGImage("Comp1_00004",       L"../../Library/Textures/UI/Comp1_00004.png");
   LoadPNGImage("Comp1_00005",       L"../../Library/Textures/UI/Comp1_00005.png");
   LoadPNGImage("Comp1_00006",       L"../../Library/Textures/UI/Comp1_00006.png");

   // 8.26 추가
   LoadTexture("n_candy",            L"../../Library/Textures/n_candy.dds");
   LoadTexture("n_plasticbox",       L"../../Library/Textures/n_plasticbox.dds");
   LoadTexture("n_soup",             L"../../Library/Textures/n_soup.dds");
   LoadTexture("n_soup_2",           L"../../Library/Textures/n_soup_2.dds");
   LoadTexture("n_etcknife",         L"../../Library/Textures/n_etcknife.dds");
   LoadTexture("n_cupboard",         L"../../Library/Textures/n_cupboard.dds");
   LoadPNGImage("Comp1_00007",       L"../../Library/Textures/UI/Comp1_00007.png");
   LoadPNGImage("Comp1_00008",       L"../../Library/Textures/UI/Comp1_00008.png");
   LoadPNGImage("Comp1_00009",       L"../../Library/Textures/UI/Comp1_00009.png");
   LoadPNGImage("Comp1_00010",       L"../../Library/Textures/UI/Comp1_00010.png");
   LoadPNGImage("Comp1_00011",       L"../../Library/Textures/UI/Comp1_00011.png");
   LoadPNGImage("Comp1_00012",       L"../../Library/Textures/UI/Comp1_00012.png");
   LoadPNGImage("Comp1_00013",       L"../../Library/Textures/UI/Comp1_00013.png");
   LoadPNGImage("ant_count_cupcake_off",L"../../Library/Textures/UI/ant_count_cupcake_off.png");
   LoadPNGImage("AlarmUI",             L"../../Library/Textures/UI/AlarmUI.png");



}
