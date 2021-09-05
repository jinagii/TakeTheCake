#pragma once

#include "IObjectBase.h"
class ClNavigation : public IObjectBase
{
public:
	ClNavigation(class IDXObjectBase* pNavigation);
	virtual ~ClNavigation();


	virtual void Initialize() override;
	virtual void Release()  override;
	virtual void Update(class Camera* pCamera, float dTime) override;
	virtual void Render() override;

	class Navigation* m_pDXNavigation;

};

