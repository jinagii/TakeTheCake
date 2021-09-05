#pragma once
/// <summary>
/// Scene_01 Class
/// Writer : �� �� ȯ
/// 
/// �׽�Ʈ�� �� 01
///  
/// Flocking �ùķ��̼ǿ�
/// ���߿� �ΰ��ӿ����� �������
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
	void LoadProp();	// ���� �ε�
	
	virtual void ShowDebugInformation(float dTime);

	void CameraMove(const float dTime);

	/// ���� ���
	static GAME_MODE sm_GameMode;
	static void SetGameMode(const GAME_MODE gameMode);

	/// �� ������ On/Off �� bool ����
	static bool m_bIsMapRender;

public:
	// Key�Է� ����
	virtual void OnMouseDown(HWND hWnd, int x, int y) override;
	virtual void OnMouseUp(int x, int y) override;
	virtual void OnMouseMove(int btnState, int x, int y) override;
	virtual void OnMouseWheel(SHORT wheelDelta) override;

	virtual void UpdateUI(float dTime);
	virtual void RenderUI();
	
	void TimerUpdate();
	void TimerRender();

	// ���� ���� �� ������ ���� ������Ʈ�� ����
	void SpawnAnt();

	// ������ ���� ��
	static int sm_AntCount;

	// ���� ���� ��� (������ ������ �ƴѰ�?)
	static ePlayState m_GamePlayState;

	/// �ΰ��� Ÿ�̸�
	static bool sm_IsTimerStop;
	static float sm_Timer;

	static void AddInGameTimer(const float val);

	void ChangeBGM(ePlayState playState);

	void OnPickCakeUp();	// ����ũ�� ����� ��
	void OnStandBy();		// �Ϲ� ���·� ��ȯ �Ǿ��� ��

private:
	/// ������� �Ǻ�
	bool m_bIsDebug;

	/// �׺���̼� �Ž� Ư�����
	class ClNavigation* m_pNavigation;

	/// ������Ʈ �Ŵ���
	class ObjectManager* m_pObjectManager;		// �Ϲ�   ������Ʈ
	class ObjectManager* m_pDebugObjectManager;	// ����� ������Ʈ

	class Player* m_pPlayer;

	// dTime �� ���ȭ
	float m_dTime;	// �Ķ���ͷ� dTime�� ���� �ʴ� Render() ��� ����ϱ� ���� ����

	// ī�޶� ���� ������ �÷���
	bool m_CameraFlag;

	/// UI ���
	// Ÿ�̸� ����
	class Texture2D* m_texTime;				// �߾� TIME UI
	class Texture2D* m_texTimeNum_100;		// Ÿ�̸� 100�� �ڸ�
	class Texture2D* m_texTimeNum_10;		// Ÿ�̸�  10�� �ڸ�
	class Texture2D* m_texTimeNum_1;		// Ÿ�̸�   1�� �ڸ�

	/// ���� �� ����
	// x 0 ~ 9
	class Texture2D* m_texAnt;
	class Texture2D* m_texAntX;
	class Texture2D* m_texAntNum;

	// �ϴ� off, on cake
	class Texture2D* m_texAntCount[10];
	class Texture2D* m_texCupCake;

	/// Sound
	class Sound* m_sndBGM;
	
private:
	std::string ConvertNumberToString(int number);
};

