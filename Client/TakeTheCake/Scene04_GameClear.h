#pragma once

/// <summary>
/// Scene04_GameClear Class
/// Writer : 최 요 환
/// 
/// 게임 클리어시 전환되는 씬
/// 
/// 클리어시 화면(이미지)을 보여준다.
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
	class Texture2D* m_texBG;	// 클리어 화면 BG
	class Sound* m_sndBGM;		// BGM
};

