#pragma once

/// <summary>
/// Prop Class
/// Writer : YoKing
/// 
/// 게임에서 사용하는 소품들
/// 프랍이 모여서 전체적인 맵을 구성한다.
/// 
/// First Create	  : 2021.08.23
/// Last Modification : 2021.08.23
/// </summary>

#include "IObjectBase.h"

class Prop : public IObjectBase
{
public:
	Prop(class IDXObjectBase* pMesh = nullptr);
	~Prop();

	virtual void Initialize() override;

	virtual void Release()	override;
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;

	/// 확장성을 위해... 나중에 메쉬를 변경해야할 일이 생긴다면...
	IDXObjectBase* GetMesh() const;
	void SetMesh(class IDXObjectBase* pMesh);

private:

	/// DX 엔진 오브젝트
	IDXObjectBase* m_pMesh;	// 메쉬
};

