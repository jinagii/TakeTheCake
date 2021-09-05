#pragma once

/// <summary>
/// Scene02_CutScene Class
/// Writer : �� �� ȯ
/// 
/// ��Ʈ�� ������ �ƾ� �̹����� Ű���� �̹��� �����ִ� ��
///  
/// 
/// First Create	  : 2021.08.23
/// Last Modification : 2021.08.23 
/// </summary>

#include "IScene.h"

enum class SCENE_STATE
{
	CutScene,
	KetSet,
};

class Scene02_CutScene : public IScene
{
public:
	Scene02_CutScene();
	virtual ~Scene02_CutScene();

	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual void Update(float dTime) override;
	virtual void Render() override;

protected:

private:
	SCENE_STATE m_State;		// �� ����

	class Texture2D* m_texBG;	// BG(�ƾ�/Ű����)
};

