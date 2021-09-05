#pragma once

/// <summary>
/// Prop Class
/// Writer : YoKing
/// 
/// ���ӿ��� ����ϴ� ��ǰ��
/// ������ �𿩼� ��ü���� ���� �����Ѵ�.
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

	/// Ȯ�强�� ����... ���߿� �޽��� �����ؾ��� ���� ����ٸ�...
	IDXObjectBase* GetMesh() const;
	void SetMesh(class IDXObjectBase* pMesh);

private:

	/// DX ���� ������Ʈ
	IDXObjectBase* m_pMesh;	// �޽�
};

