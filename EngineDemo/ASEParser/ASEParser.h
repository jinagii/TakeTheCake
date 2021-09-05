#pragma once

#include <vector>
#include <list>
#include <SimpleMath.h>

#include "Lexer.h"
#include "ParsingData.h"
#include "Animation.h"
#include "AlignedAllocationPolicy.h"


using namespace std;

class ASELexer;



enum eObjectType
{
	eObjectType_ObjectRoot, eObjectType_Geomobject, eObjectType_Shape,
	eObjectType_Camera, eObjectType_Light, eObjectType_HelperObject,
	eObjectType_MarkerObject, eObjectType_SkinnedMesh, eObjectType_Bone,
	eObjectType_CMeshPlane, eObjectType_CBullet_Trace,
	//
	eObjectType_BasicSphere, eObjectType_Missile,
	eObjectType_MeshBillboard, eObjectType_PointSprite,			// CMesh���..
	eObjectType_End,
};

// �ļ� ���ο��� Ư�� ��带 ������ �۵��Ѵ�. (�ļ��� ��ͱ��� ����)
enum eParsingmode
{
	eNone, eGeomobject, eAnimation, eShape, eCamera, eLight, eHelperObject,
	eSkin_InitTM, eBone_List,
	eEnd,
};

/// ASE�� �Ľ��ϱ� ���� Ŭ����. �⺻������ �־����� LexerŬ������ �̿��Ѵ�.

/// �л� ���� ASE Parser�� ���ÿ����� �ణ �����ߴ�. 

class ASEParser : public AlignedAllocationPolicy<16>
{
public:
	ASEParser();
	~ASEParser();

	/// �ʱ�ȭ, �ε�
	bool Initialize();
	bool LoadFile(LPSTR p_File);		// ASE������ �о, ��ü ����Ʈ�� �ִ´�.

	/// 1�� ��ȯ
	// CScenedata ��ȯ
	bool TranslateToD3DFormat_scene(Scenedata* pScene);
	bool VertexOptimization(Mesh* pMesh);		// ���ؽ�(�븻), �ؽ��� ������ ����ȭ�� �� �ش�.
	bool ConvertAll(Mesh* pMesh);			// ��� �׳� ��ȯ (���ÿ�)

	/// Node TM : worldnodeTM���� localNodeTM �� �������ְ�
	/// ������ ����(�θ�Ÿ�� localNode�� ���ϴ� ����Լ�)�� ��� ������Ʈ�� ������� 
	/// �׷��� meshobject�� �޼ҵ带 �������
	bool SetLocalTM(Mesh* pMesh);			// Mesh���� local�� ������ local setup

	bool SetBoneData(Mesh* pMesh);

	void CheckNegativeScale(Mesh* meshData);

private:
	// ��ū�� �����س��� ���� ��Ʈ��
	char m_TokenString[256];
	int m_Token;
	int m_ParsingMode;				// ���� ���. ��� ���� ���� ��������. nodename�̶����.. row123, mapname���..


public:
	/// �⺻���� �о���� �����
	int m_data_asciiexport;			// *3DSMAX_ASCIIEXPORT	200 - �� ó�� ������ ����ε� ���� �ǹ������� �𸣰ڰ�, �ϴ� ���ǻ� �о �������ش�.
	Scenedata m_SceneData;			// ���� Scene�� �����Ͱ� �� ����ü

	/// ��Ʈ���� ����
	int	m_materialcount;					// Material�� �� ��ΰ�?
	//ASEMaterial* m_materialdata;			// Material�� �����Ͱ� �� ����ü
	//list<ASEMaterial*> m_list_materialdata;	// Material���� ����Ʈ

	/// �� ���� N���� �޽ð� ������ �� �ִ�.
	Mesh* m_OneMeshData;						// �޽� �ϳ��� �����Ͱ� �� ����ü
	vector<Mesh*> m_MeshData_v;				// �޽õ�

	/// ������ ������Ʈ�� �Ľ��� �� �ʿ��� �߰� ����
	//( �ļ� ���ο��� ������ ������Ʈ�� GeomObject�� �����ϰ� ��޵ȴ�)
	ShapeLine* m_nowshapeline;				// ���� �а� �ִ� Line
	ShapeVertex* m_nowshapevertex;			// ���� �а� �ִ� ShapeVertex

	/// �ִϸ��̼�
	Animation* m_Animation;					// �� ������Ʈ�� �ִϸ��̼� ������(�Ľ̿�)
	list<Animation*> m_Animation_l;		// �� �������� ����Ʈ
   vector<Animation*> m_AnimationByName_v;		/// (�ӽ�)�������� �ִϸ� ���� ��


public:
	// Export
	Mesh* GetMesh(int index);
	Animation* GetAnimation(string aniName);


	//	Lexer
private:
	ASELexer* m_Lexer;

	// Parsing_DivergeRecursiveALL() ���ο��� ���� ���� �Լ���.
private:
	/// �Ľ��� �ð� �Ǵ� �Ŵ��� ����Լ�. ���⼭ ��� ��ū�� ó���� �Ѵ�.
	void ParsingDivergeRecursiveALL(int depth);

	int Parsing_NumberLong();			// long�� �о �������ش�.
	float Parsing_NumberFloat();		// float
	LPSTR Parsing_String();				// string
	int Parsing_NumberInt();			// int
	Vector3 Parsing_NumberVector3();	// 3���� Float�� ���� �ϳ��� �о �������ش�.

	void Create_onemesh_to_list();		// �޽ø� �ϳ� �������� �����ϰ�, ����Ʈ�� �ִ´�.
	//void Create_materialdata_to_list();	// ��Ʈ���� �ϳ���
	void Create_animationdata_to_list(); // �ִϸ��̼� ������(�� ���� ������Ʈ ����) �ϳ��� �������� �����ϰ�, ����Ʈ�� �ִ´�.
	void Create_onevertex_to_list();	// ���ؽ� �ϳ���
};

