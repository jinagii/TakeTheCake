#pragma once

#include "AlignedAllocationPolicy.h"
#include <vector>
#include <string>
#include <SimpleMath.h>

using namespace DirectX;
using namespace DirectX::PackedVector; // ����ϴ°��ΰ�?
using namespace DirectX::SimpleMath;

using namespace std;


/// 3DSMAX�� ASE����� �������� ���� ��
/// 

struct IndexList : public AlignedAllocationPolicy<16>
{
	int Index[3];
};

/// �� ���� Vertex
struct Vertex : public AlignedAllocationPolicy<16>
{
	Vertex();
	~Vertex();

	Vector3 m_Pos; // ��ǥ���� ��ġ��
	Vector3 m_Normal; // �븻��

	bool m_isnormalset;
	float m_U, m_V; // �ؽ��� ��ǥ
		
	bool m_istextureset;

	/// Skinned Mesh ���� �߰�	
	float m_BW1, m_BW2, m_BW3; // weight 3��
		
	int m_Indices; // �ε���
	int* m_IndicesPointer;
	/// skinning���� �߰�
	int m_WeightIndex[4];
	float m_Weight[4];


};

/// �� ���� Texture - Vertex
/// �̰��� Raw�������̸� ����ȭ�� �̷����� �ᱹ��
/// OneVertex�� ���� �ȴ�.
struct OneTexVertex : public AlignedAllocationPolicy<16>
{
	OneTexVertex();
	~OneTexVertex();

	float m_u, m_v, m_w;
};


/// �� ���� Face
/// �Ľ� �ʱ⿡ Face�� ������ ������ ����
/// �ߺ��Ǵ� VertexNormal��, texture coordinate ����
/// ���� �� ���� - optimize�ʿ�
struct Face : public AlignedAllocationPolicy<16>
{
	Face();
	~Face();
	
	int m_VertexIndex[3]; // �� Face�� �̷�� ���ؽ��� �ε���
	Vector3 m_VertexNormal[3]; // Vertex�� Normal��
	
	Vector3 m_Normal; // Face�� Normal��	
	
	int m_TFace[3]; // Texture Coordinate
};


/// �� ���� Shape Vertex�� ������ �����ϱ� ���� ����ü
struct ShapeVertex : public AlignedAllocationPolicy<16>
{
	ShapeVertex();
	~ShapeVertex();

	int m_indexnum; // �ε��� �ѹ�. �׳� �Ϸù�ȣ��
	bool m_isknot; // KNOT�ΰ� INTERP�ΰ�?
	Vector3 m_pos; // ������ ������
};


/// �� ���� Shape Object�� ������ �����ϱ� ���� ����ü
struct ShapeLine : public AlignedAllocationPolicy<16>
{
	ShapeLine();
	~ShapeLine();
	
	int m_line_number; // ���° �����ΰ�?
	bool m_shape_closed; // ���� Shape�ΰ�?
	int m_shape_vertexcount; // �� ���� Vertex�� �ִ°�?
	vector<ShapeVertex*> m_shapevertex; // ���ؽ����� vector
};


/// �� ���� Scene�� �����ϱ� ���� ����ü
/// ASE���Ͽ��� ���, �ڸ�Ʈ ���� ������ SCENE�κ��� �б� ���� ����ü.
struct Scenedata : public AlignedAllocationPolicy<16>
{
	Scenedata();
	~Scenedata();

	string m_FileName; // ���� �̸�
	int m_FirstFrame;
	int m_LastFrame;
	int m_FrameSpeed;
	int m_TicksPerFrame;
	int m_MeshFrameStep;
	int m_KeyFrameStep;
	Vector3 m_SceneBackgroundStatic;
	Vector3 m_SceneAmbientStatic;
};


/// �� ���� Map�� �����ϱ� ���� Ŭ����
struct MaterialMap : public AlignedAllocationPolicy<16>
{
	MaterialMap();
	~MaterialMap();

	string		m_map_name;
	string		m_map_class;
	int		m_subno;
	float		m_map_amount;
	string		m_bitmap;
	int		m_map_type;
	float		m_uvw_u_offset;
	float		m_uvw_v_offset;
	float		m_u_tiling;
	float		m_v_tiling;
	float		m_uvw_angle;
	float		m_uvw_blur;
	float		m_uvw_blur_offset;
	float		m_uvw_noise_amt;
	float		m_uvw_noise_size;
	float		m_uvw_noise_level;
	float		m_uvw_noise_phase;
	int		m_bitmap_filter;
};


/// �� ���� Material�� �����ϱ� ���� Ŭ����
//struct ASEMaterial
//{
//public:
//	ASEMaterial();
//	~ASEMaterial();
//
//	// ������ *MATERIAL_COUNT 0 �ϳ��ۿ� ������
//	int m_materialnumber; // Material�� ��ȣ. �̰��� �������� ������Ʈ���� �����Ѵ�.
//	string m_material_name;
//	string m_material_class;
//	Vector3 m_material_ambient;
//	Vector3 m_material_diffuse;
//	Vector3 m_material_specular;
//	float m_material_shine;
//	float m_material_shinestrength;
//	float m_material_transparency;
//	float m_material_wiresize;
//	int m_material_shading;
//	float m_material_xp_falloff;
//	float m_material_selfillum;
//	float m_material_falloff;
//	int m_material_xp_type;
//
//	int m_numsubmtls;
//	ASEMaterial* m_submaterial; // �����Ʈ������ �����͸� ������ ����
//
//	MaterialMap* m_map_temp; // �ӽ÷� ���̴� ����
//	MaterialMap* m_map_diffuse; /// �ϴ� ������ map_diffuse�ۿ� ����
//	MaterialMap* m_map_generic;
//	//CMaterial_Map*			m_map_diffuse;
//	//CMaterial_Map*			m_map_diffuse;
//
//	bool m_istextureexist;	// �ؽ��İ� �����ϴ°�?
//};


// Light�� Animation ( Setting�� ��ȭ�� Animation )
struct Light_AnimationData : public AlignedAllocationPolicy<16>
{
	Light_AnimationData();
	~Light_AnimationData();

	float m_timevalue;		// ���� mesh�� �ִµ�.. �̰� ��� ���� �̷����̸�.
	Vector3 m_light_color;
	float m_light_intens;
	float m_light_aspect;
	float m_light_hotspot;		///
	float m_light_falloff;		/// �� �ΰ����� SpotLight�� ���
	float m_light_tdist;
	float m_light_mapbias;
	float m_light_maprange;
	int m_light_mapsize;
	float m_light_raybias;
};

// Bone �ϳ��� ����ü
struct Bone : public AlignedAllocationPolicy<16>
{
	Bone();
	~Bone();

	int m_BoneNumber;		// ������ ���ͷ� ���������� ���߿� ������ �ʿ�, �˻��� �ʿ�?
	string m_BoneName;
	
	Matrix* m_boneTM_NodeTM; // �̳༮�� ó���� NodeTM - offset�� ���ϱ� ���ؼ� ����	
	Matrix* m_boneTM_WorldTM; // �̳༮�� �ִϸ��̼��� �����, ������ WorldTM
};


// bone�ϳ��� ����� ����ġ �� ��
struct Weight : public AlignedAllocationPolicy<16>
{
	Weight();
	~Weight();

	int m_BoneNum;		// weight�� ����� Bone�� IndexNumber;
	float	m_BoneWeight;		// ����ġ
};


// Vertex �ϳ��� �־��� ����ġ���� �����ϴ� ����ü (����ü �ϳ��� ���ؽ� �ϳ��� �ϴ��� �����̴�)
struct VertexWeight : public AlignedAllocationPolicy<16>
{
	int m_NumWeightVertex;	// ���ؽ��� ��ȣ. ���� ���ͷ� �����ϹǷ� �� �� ������...
		
	Weight* m_TempBoneBlendWeight; // ���������� �Ʒ� ����Ʈ�� �ӽ� ����
	
	vector<Weight*> m_BoneBlendWeight_v;	// bone�ϳ��� ����� ����ġ ���ؽ� �ϳ��� ���� ���� Bone-Weight�� ���� �� ����
};


/// �� ���� �޽ø� �Ľ� - �����ϱ� ���� Ŭ����
/// ���� ���� �޽ð� ���� �� �ֱ� ������ ���� �� �κи� ����ü�� �������.
struct Mesh : public AlignedAllocationPolicy<16>
{
	Mesh();
	~Mesh();

	/// Ÿ��, �ִϸ��̼� ���
	int m_type; // Ÿ��
	bool m_IsAnimated; // �ִϸ��̼��� ������ �ִ°�? (������Ʈ�� ���� �ٸ��� �ؼ��� �� ����)
	Scenedata m_SceneData; // (3DS Max��) �� ������
	bool m_IsPosAnimated;
	bool m_IsRotAnimated;

	/// *NODE_NAME
	string m_NodeName;		// � �������� ������ �Ʒ��� m_meshname�� ���� �̸��� �ѹ� �� ����. �Ƹ��� �޽��� �̸� ����Ʈ�����... �� �׷��� �ƴұ� ��ư ������ ���� 1�̹Ƿ� �������� ������ ����.
	string m_ParentNodeName;	// �θ� ����� �̸�. �̰��� �ִٸ� �θ� �ִ� ���̴�.
	bool m_IsParentExist = false;		// �׷��� Ȯ���� bool������ ������ ����.

	/// *NODE_TM (Transform Matrix)
	Vector3 m_InheritPos;
	Vector3 m_InheritRot;
	Vector3 m_InheritScl;
	Vector3 m_TMRow0;
	Vector3 m_TMRow1;
	Vector3 m_TMRow2;
	Vector3 m_TMRow3;
	Vector3 m_TMPos;
	Vector3 m_TMRotAxis;
	float m_TMRotAngle;
	Vector3 m_TMScale;
	Vector3 m_TMScaleAxis;
	float m_TMScaleAxisAng;

	// ���� �߰� ����
	Matrix m_LocalTM;
	Matrix m_NodeTM;
	Mesh* m_pParentMesh;
	//vector<Mesh*> m_vpChildren; // �Ⱦ�

	/// *MESH
	float m_timevalue; // ���� ������ ���������� ���δ�. ��ư �ð���.
	int m_numbone; // ���� ����. �̰��� �ִٸ� ��Ű�� ������Ʈ�� �����Ѵ�.
	int m_numskinweight; // ��Ų ����ġ. ���Ҹ���?
	int m_Mesh_NumVertex; // (���ؽ��� ��ǥ������)
	int m_MeshNumFace; // (���ؽ��� �ε���)

	// �޽� ���� vertex �迭�� �����ϱ� ���� ������
	/// *MESH_VERTEX_LIST
	vector<Vertex*> m_MeshVertices_v; // ���ؽ�
	/// *MESH_FACE_LIST
	vector<Face*> m_MeshFaces_v; // �ε���

	// �ؽ��Ŀ� �����͵�
	/// *MESH_NUMTVERTEX
	bool m_IsTextureSet;
	int m_Mesh_NumTVertex; // �ؽ��Ŀ� ���ؽ� ����
	vector<OneTexVertex*> m_MeshTVertices_v;		// �ؽ��Ŀ� ���ؽ�
	/// *MESH_RVERTLIST
	int m_mesh_tvfaces; // �ؽ��Ŀ� ���̽� ���� - (������ ���� Face�� ���� �̹� ������ ��������Ƿ� ���븸 �־��ָ� �ȴ�)
	int m_mesh_numcvertex; // �� �̰� ��� ���°�?

	// �븻 ������
	//Vector3* m_mesh_facenormal;			// �� ���̽� �븻�� (���� �Ⱦ�)
	//Vector3* m_mesh_vertexnormal;		// ���ؽ� �븻�� 3���� (���� �Ⱦ�)

	vector<Vertex*> m_OptimizedVertices_v; /// ����ȭ ���� ���ؽ���
	IndexList* m_OptimizedIndices; /// ����ȭ ���� �ε�����

	/// Shape Object
	int m_shape_linecount; // ������ ��
	vector<ShapeLine*> m_vector_shape_line; // ������ ������ ������

	/// ������ ������
	int m_prop_motionblur; //*PROP_MOTIONBLUR 0
	int m_prop_castshadow; //*PROP_CASTSHADOW 1
	int m_prop_recvshadow; //*PROP_RECVSHADOW 1
	//int m_material_ref; //*MATERIAL_REF  �� �޽��� ��Ʈ������ ��ȣ
	Vector3 m_wireframe_color; //*WIREFRAME_COLOR 0.89803922	0.65098041	0.84313726

	/// Camera
	// �̷��� ������ �ѹ��� �о������ ���� ���α� �ѵ�..
	// �ϴ� �̷��� �����������ϱ�. �׸��� �ļ��� ��ͱ����� ����� �Ǹ� �̷��� �ϴ°� ���ϴ�
	// ��� D3D�� Ŭ������ ������ �� ��ȯ�� �Ǵϱ�
	string m_camera_type;
	// CAMERA_SETTINGS
	//float m_timevalue;		// mesh�� �̹� �ִ�.
	float m_camera_near;
	float m_camera_far;
	float m_camera_fov;
	float m_camera_tdist;
	bool m_camera_isloadTarget;	// ī�޶�� NodeTM�� �ΰ��� �ִµ�, ��ͱ��������� ������ ����� 
											// (�׻� ������ Target�� �о���� �� �ִ� �׷��� �װ��� ���ذ��� �Ѵ�)

	/// Light
	string m_light_type;
	// Omni�ΰ� SpotLight�ΰ��� ó���� �� ��� �Ѵ�.

	// Light Property
	string m_light_shadows;
	bool m_light_uselight;
	string m_light_spotshape;
	bool m_light_useglobal;
	bool m_light_absmapbias;
	bool m_light_overshoot;

	// Light Setting & Animation
	Light_AnimationData* m_lightanimation;		// Light Setting
	vector<Light_AnimationData*> m_list_lightanimation;	// �װ��� Animation

	bool m_light_isloadTarget;	// ���� NodeTM�� �� �� �ְ�, ī�޶�� ���� ������ �� ������ �ִ�.
	bool m_light_isAnimate;		// Animation�� �Ǵ°�?

	/// Skinning Object
	// SKIN_INITTM�� �����ϰ�
	// TM_ROW0~3
	// BONE_LIST
	bool m_IsSkinningObject;	// ��Ű�� ������Ʈ�ΰ�?

	bool m_IsBone;
	Bone* m_Bone;									// Bone �Ѱ��� ������ (�ļ� ���ο��� �ӽ� �������� ����)
	VertexWeight* m_WeightVertex;						// Weight_Vertex �Ѱ��� ������ (���������� �ӽð���)
	vector<Bone*> m_Bone_v;		// �̰� �׳� ��ȣ�� �̸� �����̴�. �� �ȿ� �����͸� �־���Ƶ� �����ڱ���(�����ϱ� ���ϰ� - �׷��� ������ �Ź� �̸����� �˻��� �ؾߵ�����. �ƴϸ� �ε����� ������ ��� �ð���� ������ �Ҽ���?)
	vector<VertexWeight*> m_WeightVertex_v;		// ���ؽ� �ϳ��� �޶�پ��ִ� ����ġ��
};




