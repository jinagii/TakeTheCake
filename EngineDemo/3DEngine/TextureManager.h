#pragma once
#include "AlignedAllocationPolicy.h"
#include "DXDefine.h"
#include <unordered_map>

using namespace std;

/// ���� ���� ���ó�� �޽�������Ʈ�� init�Ҷ����� �ҷ����ʿ䰡 ����
/// �ؽ��İ� �ִ� ��η� ���� �����´�
/// ���� new �� �� �𵨿� �ؽ��� �����͸� �Ѱ��ش�
// render�� �� �ʿ��ϴ�
class TextureManager : public AlignedAllocationPolicy<16>
{
public:
	TextureManager(ID3D11Device* pDevice);
	~TextureManager();

	// 
	void LoadTexture(string modelName, wstring fileName);
	void LoadNormalMap(string modelName, wstring fileName);
	void LoadPNGImage(string name, wstring fileName);
	void SaveTexture();

	unordered_map<string, ID3D11ShaderResourceView*>& GetDiffuseMap_um() { return m_DiffuseMap_um; }
	ID3D11ShaderResourceView* GetDiffuseMapByName(string name) { return m_DiffuseMap_um.at(name); }

   unordered_map<string, ID3D11ShaderResourceView*>& GetNormalMap_um() { return m_NormalMap_um; }
	ID3D11ShaderResourceView* GetNormalMapByName(string name) { return m_NormalMap_um.at(name); }

   unordered_map<string, ID3D11ShaderResourceView*>& GetPNGImage_um() { return m_PNGImages_um; }
   ID3D11ShaderResourceView* GetPNGImageByName(string name) { return m_PNGImages_um.at(name); }

private:
	ID3D11Device* m_pDevice;
	unordered_map<string, ID3D11ShaderResourceView*> m_DiffuseMap_um;
	unordered_map<string, ID3D11ShaderResourceView*> m_NormalMap_um;
	unordered_map<string, ID3D11ShaderResourceView*> m_PNGImages_um;

};

