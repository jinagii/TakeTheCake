#pragma once

/// <summary>
/// Grid Class
/// Writer : YoKing
/// 
/// 디버깅용 그리드 클래스
/// 
/// 
/// First Create	  : 2021.08.19
/// Last Modification : 2021.08.19
/// </summary>

#include "IObjectBase.h"

class Grid : public IObjectBase
{
public:
	Grid(class IDXObjectBase* pGrid);
	virtual ~Grid();

	virtual void Initialize() override;
	virtual void Release()  override;
	virtual void Update(class Camera* pCamera, float dTime) override;
	virtual void Render() override;

protected:

private:
	class IDXObjectBase* m_pGrid;

};