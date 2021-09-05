#pragma once
/// <summary>
/// Scene_01 Class
/// Writer : 최 요 환
/// 
/// 테스트용 씬 01
///  
/// Flocking 시뮬레이션용
/// 나중에 인게임용으로 사용하자
/// 
/// First Create	  : 2021.08.06
/// Last Modification : 2021.08.12 
/// </summary>

#include "IScene.h"

class IDXObjectBase;
class IDXRenderer;

enum class GAME_MODE
{
	Play,
	Clear,
	Failed,
	TimeOver,
};

enum class ePlayState
{
	NoCake, PickUpCake,
};


class Scene03_InGame : public IScene
{
public:
	Scene03_InGame();
	virtual ~Scene03_InGame();

	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual void Update(float dTime) override;
	virtual void Render() override;

	virtual void LoadObject() override;
	virtual void LoadDebugObject() override;
	virtual void LoadUIObject() override;
	void LoadProp();	// 프랍 로드
	
	virtual void ShowDebugInformation(float dTime);

	void CameraMove(const float dTime);

	/// 게임 모드
	static GAME_MODE sm_GameMode;
	static void SetGameMode(const GAME_MODE gameMode);

	/// 맵 렌더링 On/Off 용 bool 변수
	static bool m_bIsMapRender;

public:
	// Key입력 관련
	virtual void OnMouseDown(HWND hWnd, int x, int y) override;
	virtual void OnMouseUp(int x, int y) override;
	virtual void OnMouseMove(int btnState, int x, int y) override;
	virtual void OnMouseWheel(SHORT wheelDelta) override;

	virtual void UpdateUI(float dTime);
	virtual void RenderUI();
	
	void TimerUpdate();
	void TimerRender();

	// 개미 구출 시 실제로 개미 오브젝트를 생성
	void SpawnAnt();

	// 구출한 개미 수
	static int sm_AntCount;

	// 게임 진행 모드 (케익을 집었나 아닌가?)
	static ePlayState m_GamePlayState;

	/// 인게임 타이머
	static bool sm_IsTimerStop;
	static float sm_Timer;

	static void AddInGameTimer(const float val);

	void ChangeBGM(ePlayState playState);

	void OnPickCakeUp();	// 케이크를 들었을 때
	void OnStandBy();		// 일반 상태로 전환 되었을 때

private:
	/// 디버깅모드 판별
	bool m_bIsDebug;

	/// 네비게이션 매쉬 특별취급
	class ClNavigation* m_pNavigation;

	/// 오브젝트 매니져
	class ObjectManager* m_pObjectManager;		// 일반   오브젝트
	class ObjectManager* m_pDebugObjectManager;	// 디버깅 오브젝트

	class Player* m_pPlayer;

	// dTime 의 멤버화
	float m_dTime;	// 파라미터로 dTime을 받지 않는 Render() 등에서 사용하기 위해 보관

	// 카메라 시점 고정용 플래그
	bool m_CameraFlag;

	/// UI 멤버
	// 타이머 관련
	class Texture2D* m_texTime;				// 중앙 TIME UI
	class Texture2D* m_texTimeNum_100;		// 타이머 100의 자리
	class Texture2D* m_texTimeNum_10;		// 타이머  10의 자리
	class Texture2D* m_texTimeNum_1;		// 타이머   1의 자리

	/// 개미 수 관련
	// x 0 ~ 9
	class Texture2D* m_texAnt;
	class Texture2D* m_texAntX;
	class Texture2D* m_texAntNum;

	// 하단 off, on cake
	class Texture2D* m_texAntCount[10];
	class Texture2D* m_texCupCake;

	/// Sound
	class Sound* m_sndBGM;
	
private:
	std::string ConvertNumberToString(int number);
};

