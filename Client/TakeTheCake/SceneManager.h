#pragma once

/// <summary>
/// SceneManager Class
/// Writer : YoKing
/// 
/// ���� �߰�, ����, ���� �� ������Ʈ�� �� ��ü�� Ŭ����
/// 
/// First Create	  : 2021.08.18
/// Last Modification : 2021.08.18
/// </summary>

#include "SingletonBase.h"

class SceneManager : public SingletonBase<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	HRESULT Initialize() override;
	void Release();
	void Update(const float dTime);
	void Render();

	// �� �߰�
	class IScene* AddScene(const std::string sceneName, IScene(* const scene) );

	// �� ����
	HRESULT ChangeScene(const std::string sceneName);

	// ���� �� ��ȯ
	class IScene* GetCurrentScene() const;

protected:

private:
	// using �������� ������
	using umSceneList = std::unordered_map<std::string, IScene*>;
	using umSceneListIter = std::unordered_map<std::string, IScene*>::iterator;
	umSceneList m_SceneList;		// �� ����Ʈ

	class IScene* m_CurrentScene;	// ���� ��
};