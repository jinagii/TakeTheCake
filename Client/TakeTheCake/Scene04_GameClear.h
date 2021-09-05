#pragma once

/// <summary>
/// Scene04_GameClear Class
/// Writer : �� �� ȯ
/// 
/// ���� Ŭ����� ��ȯ�Ǵ� ��
/// 
/// Ŭ����� ȭ��(�̹���)�� �����ش�.
///  
/// 
/// First Create	  : 2021.08.23
/// Last Modification : 2021.08.23 
/// </summary>

#include "IScene.h"

class Scene04_GameClear : public IScene
{
public:
	Scene04_GameClear();
	virtual ~Scene04_GameClear();

	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual void Update(float dTime) override;
	virtual void Render() override;

protected:

private:
	class Texture2D* m_texBG;	// Ŭ���� ȭ�� BG
	class Sound* m_sndBGM;		// BGM
};

