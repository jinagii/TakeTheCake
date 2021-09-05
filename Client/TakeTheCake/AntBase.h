#pragma once

/// <summary>
/// AntBase Class
/// Writer : YoKing
/// 
/// 무리를 지어 움직이는 개미 클래스
/// Boid 클래스를 상속받아 움직이고
/// 엔진에서 메쉬를 만들어와 렌더링한다.
/// 
/// First Create	  : 2021.08.04
/// Last Modification : 2021.08.23
/// </summary>

#include "Boid.h"

class AntBase : public Boid
{
public:
	AntBase(class IDXObjectBase* pMesh);
	~AntBase();

	virtual void Initialize() override;

	virtual void Release()	override;
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;

	void UpdateMesh(class Camera* pCamera, float dTime);

private:
	/// DX 엔진 오브젝트
	IDXObjectBase* m_pMesh;	// 메쉬

};

