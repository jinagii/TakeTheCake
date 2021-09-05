#pragma once

/// <summary>
/// Scene01_Intro Class
/// Writer : �� �� ȯ
/// 
/// ���� ���۽� ���� ���� �����ϴ� intro ��
///  
/// 
/// First Create	  : 2021.08.21
/// Last Modification : 2021.08.21 
/// </summary>

#include "IScene.h"

class Scene01_Intro : public IScene
{
public:
	Scene01_Intro();
	virtual ~Scene01_Intro();

	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual void Update(float dTime) override;
	virtual void Render() override;

	virtual void LoadUIObject() override;

	virtual void UIUpdate(float dTime);
	void UIRender();

	/// Sound
	static class Sound* sm_sndBGM;

private:
	/// Texture UI
	class Texture2D* m_texIntro[14];

	float m_dTime;


};