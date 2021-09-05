#pragma once

/// <summary>
/// Scene02_CutScene Class
/// Writer : 최 요 환
/// 
/// 인트로 다음에 컷씬 이미지와 키셋팅 이미지 보여주는 씬
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
	SCENE_STATE m_State;		// 씬 상태

	class Texture2D* m_texBG;	// BG(컷씬/키셋팅)
};

