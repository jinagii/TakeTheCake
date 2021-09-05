#pragma once

#include "AlignedAllocationPolicy.h"
#include "DXDefine.h"
#include "IDXObjectBase.h"

class ASEParser;
class TextureManager;
class GeometryHelper;
class MeshObject;
class ImageObject;
class Camera;
class Navigation;

/// ������Ʈ���� �����ϴ� Ŭ����
/// ASE���� �ҷ����� �����Ͽ� Object map�� �������ش�
/// 2021.7.22 JinHak

class DXObjectFactory : public AlignedAllocationPolicy<16>
{
public:
	DXObjectFactory(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~DXObjectFactory();

	IDXObjectBase* CreateObjectFromASEParser(string name, float scale, float angleY, float angleX, float angleZ, Vector3 pos);	
	IDXObjectBase* CreateImageObject(string name, int posX, int posY, int width, int height);

	// helper
	IDXObjectBase* CreateGrid();
	IDXObjectBase* CreateAxis();
	IDXObjectBase* CreateNavigationMesh();

	// just for test
	IDXObjectBase* CreateSkull();

	Camera* CreateCameraAt(Vector3 pos, Vector3 target, Vector3 worldUp);
	
	void LoadASEData(string objectName, string filePath);

	void Initialize();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	// Parser
	unordered_map<string, ASEParser*> m_ASEData_um; // Parsering�� �����͵� 
	// �ؽ��Ŀ��� �߰��� �κ�
	TextureManager* m_pTextureManager;
	//Matrix m_TexTransform;
	GeometryHelper* m_pGeometryHelper;

public:
	ASEParser* GetASEData(std::string name);

};

