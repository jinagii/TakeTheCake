#pragma once

/// <summary>
/// Scene05_GameOver Class
/// Writer : �� �� ȯ
/// 
/// ���� ������ ��ȯ�Ǵ� ��
/// 
/// ���ӿ��� ȭ��(�̹���)�� �����ش�.
///  
/// 
/// First Create	  : 2021.08.23
/// Last Modification : 2021.08.23 
/// </summary>

#include "IScene.h"

class Scene05_GameOver : public IScene
{
public:
	Scene05_GameOver();
	virtual ~Scene05_GameOver();

	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual void Update(float dTime) override;
	virtual void Render() override;

protected:

private:
	class Texture2D* m_texBG;	// BG
	class Sound* m_sndBGM;		// BGM
};

