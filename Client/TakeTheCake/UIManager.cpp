#include "pch.h"
#include "Texture2D.h"
#include "ObjectFactory.h"
#include "UIManager.h"

UIManager::UIManager()
    : m_IsInitialized(false)
{
}

UIManager::~UIManager()
{
    Release();
}

HRESULT UIManager::Initialize()
{
    /// 이미 초기화 되어 있으면 다시 하지 않는다.
    if (m_IsInitialized) return S_OK;

    // 초기화상태된로 변경
    m_IsInitialized = true;

    int width  = DXDevice::g_ScreenWidth;
    int height = DXDevice::g_ScreenHeight;

    // BG 관련
    AddTexture2D(TEXTURE2D_INTRO_00,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_01,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_02,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_03,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_04,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_05,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_06,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_07,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_08,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_09,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_10,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_11,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_12,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_INTRO_13,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_CLEAR,      0, 0, width, height);
    AddTexture2D(TEXTURE2D_CUTSCENE,   0, 0, width, height);
    AddTexture2D(TEXTURE2D_FAILED,     0, 0, width, height);
    AddTexture2D(TEXTURE2D_KEYSET_2,   0, 0, width, height);
    
    AddTexture2D(TEXTURE2D_TIMEOVER,   0, 0, width, height); // 타임 오버

    // 임시 변수
    Texture2D* texture2D;

    // 개미 숫자 관련
    AddTexture2D(TEXTURE2D_ANT,        1660, 50, 57, 63);
    AddTexture2D(TEXTURE2D_ANTNUM_X,   1736, 58, 32, 49);

    // 1의 자리
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_0", 1781, 45, 39, 55);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_0, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_1", 1781, 45, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_1, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_2", 1781, 45, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_2, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_3", 1781, 45, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_3, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_4", 1781, 45, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_4, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_5", 1781, 45, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_5, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_6", 1781, 45, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_6, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_7", 1781, 45, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_7, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_8", 1781, 45, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_8, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("num_UI_9", 1781, 45, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_ANTNUM_1_9, texture2D });


    // 타임/타이머 관련
    AddTexture2D(TEXTURE2D_TIME,       45, 31, 154, 55);

    // 100의 자리
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_0",  47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_0, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_1",  47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_1, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_2", 47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_2, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_3", 47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_3, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_4", 47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_4, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_5", 47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_5, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_6", 47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_6, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_7", 47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_7, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_8", 47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_8, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_9", 47, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_100_9, texture2D });

    // 10의 자리
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_0",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_0, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_1",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_1, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_2",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_2, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_3",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_3, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_4",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_4, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_5",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_5, texture2D }); 
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_6",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_6, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_7",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_7, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_8",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_8, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_9",  99, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_10_9, texture2D });


    // 1의 자리
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_0", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_0, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_1", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_1, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_2", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_2, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_3", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_3, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_4", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_4, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_5", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_5, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_6", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_6, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_7", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_7, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_8", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_8, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("time_num_9", 153, 102, 60, 85);
    m_Texture2DList.insert({ TEXTURE2D_TIMENUM_1_9, texture2D });

    /// 인게임 하단 개미
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 686, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_01_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  686, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_01_ON, texture2D });

    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 739, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_02_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  739, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_02_ON, texture2D });

    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 792, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_03_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  792, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_03_ON, texture2D });

    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 845, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_04_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  845, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_04_ON, texture2D });

    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 898, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_05_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  898, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_05_ON, texture2D });

    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 951, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_06_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  951, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_06_ON, texture2D });

    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 1004, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_07_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  1004, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_07_ON, texture2D });

    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 1057, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_08_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  1057, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_08_ON, texture2D });

    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 1110, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_09_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  1110, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_09_ON, texture2D });

    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_off", 1163, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_10_OFF, texture2D });
    texture2D = ObjectFactory::GetInstance()->CreateImageUI("ant_count_ant_on",  1163, 926, 31, 60);
    m_Texture2DList.insert({ TEXTURE2D_ANTCOUNT_10_ON, texture2D });

    /// 컵케익
    AddTexture2D(TEXTURE2D_ANTCOUNT_CUPCAKE, 1226, 882, 82, 114);

    return S_OK;
}

void UIManager::Release()
{
    DeleteAll();
}

Texture2D* UIManager::AddTexture2D(const std::string textureName, const int x, const int y, const int width, const int height)
{
    Texture2D* texture2D = FindTexture2D(textureName);

    // 텍스쳐가 있으면 그 텍스쳐 리턴
    if (texture2D) return texture2D;

    // 해당 텍스쳐가 없으면 새로 만듬
    texture2D = ObjectFactory::GetInstance()->CreateImageUI(textureName, x, y, width, height);

    // 만들기 실패했을 때
    if (FAILED(texture2D))
    {
        // 다시 제거하고
        SAFE_DELETE(texture2D);

        // nullptr 반환
        return nullptr;
    }

    // 리스트에 텍스쳐 추가
    m_Texture2DList.insert({ textureName, texture2D });

    // 생성된 texture2D 반환
    return texture2D;
}

Texture2D* UIManager::FindTexture2D(std::string textureName)
{
    MapTexture2DIter iter = m_Texture2DList.find(textureName);

    // 해당 Texture2D가 있으면
    if (iter != m_Texture2DList.end())
    {
        // 그 Texture2D 리턴
        return iter->second;
    }

    // 없으면
    return nullptr;
}

void UIManager::DeleteAll()
{
    MapTexture2DIter iter = m_Texture2DList.begin();

    for (; iter != m_Texture2DList.end(); )
    {
        // 텍스쳐 정보가 있으면
        if (iter->second != nullptr)
        {
            // 메모리 해제
            ReleaseCOM(iter->second);
            SAFE_DELETE(iter->second);

            // 리스트에서 제거
            iter = m_Texture2DList.erase(iter);
        }

        // 다음으로
        else
        {
            ++iter;
        }
    }

    // 최후에 클리어 해주자
    m_Texture2DList.clear();

}
