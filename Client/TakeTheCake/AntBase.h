#pragma once

/// <summary>
/// AntBase Class
/// Writer : YoKing
/// 
/// ������ ���� �����̴� ���� Ŭ����
/// Boid Ŭ������ ��ӹ޾� �����̰�
/// �������� �޽��� ������ �������Ѵ�.
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
	/// DX ���� ������Ʈ
	IDXObjectBase* m_pMesh;	// �޽�

};

