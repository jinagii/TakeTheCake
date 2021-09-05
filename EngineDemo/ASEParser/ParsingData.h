#pragma once

#include "AlignedAllocationPolicy.h"
#include <vector>
#include <string>
#include <SimpleMath.h>

using namespace DirectX;
using namespace DirectX::PackedVector; // 사용하는것인가?
using namespace DirectX::SimpleMath;

using namespace std;


/// 3DSMAX의 ASE출력을 가져오기 위한 것
/// 

struct IndexList : public AlignedAllocationPolicy<16>
{
	int Index[3];
};

/// 한 개의 Vertex
struct Vertex : public AlignedAllocationPolicy<16>
{
	Vertex();
	~Vertex();

	Vector3 m_Pos; // 좌표상의 위치값
	Vector3 m_Normal; // 노말값

	bool m_isnormalset;
	float m_U, m_V; // 텍스쳐 좌표
		
	bool m_istextureset;

	/// Skinned Mesh 에서 추가	
	float m_BW1, m_BW2, m_BW3; // weight 3개
		
	int m_Indices; // 인덱스
	int* m_IndicesPointer;
	/// skinning에서 추가
	int m_WeightIndex[4];
	float m_Weight[4];


};

/// 한 개의 Texture - Vertex
/// 이것은 Raw데이터이며 최적화가 이뤄져서 결국은
/// OneVertex에 들어가게 된다.
struct OneTexVertex : public AlignedAllocationPolicy<16>
{
	OneTexVertex();
	~OneTexVertex();

	float m_u, m_v, m_w;
};


/// 한 개의 Face
/// 파싱 초기에 Face의 갯수는 정해져 있음
/// 중복되는 VertexNormal값, texture coordinate 값이
/// 있을 수 있음 - optimize필요
struct Face : public AlignedAllocationPolicy<16>
{
	Face();
	~Face();
	
	int m_VertexIndex[3]; // 이 Face를 이루는 버텍스의 인덱스
	Vector3 m_VertexNormal[3]; // Vertex의 Normal값
	
	Vector3 m_Normal; // Face의 Normal값	
	
	int m_TFace[3]; // Texture Coordinate
};


/// 한 개의 Shape Vertex의 정보를 저장하기 위한 구조체
struct ShapeVertex : public AlignedAllocationPolicy<16>
{
	ShapeVertex();
	~ShapeVertex();

	int m_indexnum; // 인덱스 넘버. 그냥 일련번호다
	bool m_isknot; // KNOT인가 INTERP인가?
	Vector3 m_pos; // 포지션 데이터
};


/// 한 개의 Shape Object의 정보를 저장하기 위한 구조체
struct ShapeLine : public AlignedAllocationPolicy<16>
{
	ShapeLine();
	~ShapeLine();
	
	int m_line_number; // 몇번째 라인인가?
	bool m_shape_closed; // 닫힌 Shape인가?
	int m_shape_vertexcount; // 몇 개의 Vertex가 있는가?
	vector<ShapeVertex*> m_shapevertex; // 버텍스들의 vector
};


/// 한 개의 Scene을 저장하기 위한 구조체
/// ASE파일에서 헤더, 코멘트 다음 나오는 SCENE부분을 읽기 위한 구조체.
struct Scenedata : public AlignedAllocationPolicy<16>
{
	Scenedata();
	~Scenedata();

	string m_FileName; // 파일 이름
	int m_FirstFrame;
	int m_LastFrame;
	int m_FrameSpeed;
	int m_TicksPerFrame;
	int m_MeshFrameStep;
	int m_KeyFrameStep;
	Vector3 m_SceneBackgroundStatic;
	Vector3 m_SceneAmbientStatic;
};


/// 한 개의 Map을 저장하기 위한 클래스
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


/// 한 개의 Material을 저장하기 위한 클래스
//struct ASEMaterial
//{
//public:
//	ASEMaterial();
//	~ASEMaterial();
//
//	// 지금은 *MATERIAL_COUNT 0 하나밖에 없더라
//	int m_materialnumber; // Material의 번호. 이것을 기준으로 오브젝트에서 참조한다.
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
//	ASEMaterial* m_submaterial; // 서브매트리얼은 포인터만 가지고 있자
//
//	MaterialMap* m_map_temp; // 임시로 쓰이는 변수
//	MaterialMap* m_map_diffuse; /// 일단 지금은 map_diffuse밖에 없다
//	MaterialMap* m_map_generic;
//	//CMaterial_Map*			m_map_diffuse;
//	//CMaterial_Map*			m_map_diffuse;
//
//	bool m_istextureexist;	// 텍스쳐가 존재하는가?
//};


// Light의 Animation ( Setting의 변화의 Animation )
struct Light_AnimationData : public AlignedAllocationPolicy<16>
{
	Light_AnimationData();
	~Light_AnimationData();

	float m_timevalue;		// 역시 mesh에 있는데.. 이거 어떻게 하지 이런식이면.
	Vector3 m_light_color;
	float m_light_intens;
	float m_light_aspect;
	float m_light_hotspot;		///
	float m_light_falloff;		/// 이 두가지는 SpotLight인 경우
	float m_light_tdist;
	float m_light_mapbias;
	float m_light_maprange;
	int m_light_mapsize;
	float m_light_raybias;
};

// Bone 하나의 구조체
struct Bone : public AlignedAllocationPolicy<16>
{
	Bone();
	~Bone();

	int m_BoneNumber;		// 어차피 벡터로 잡을거지만 나중에 참조시 필요, 검색시 필요?
	string m_BoneName;
	
	Matrix* m_boneTM_NodeTM; // 이녀석은 처음의 NodeTM - offset을 구하기 위해서 존재	
	Matrix* m_boneTM_WorldTM; // 이녀석은 애니메이션이 적용된, 마지막 WorldTM
};


// bone하나와 연결된 가중치 한 개
struct Weight : public AlignedAllocationPolicy<16>
{
	Weight();
	~Weight();

	int m_BoneNum;		// weight가 적용된 Bone의 IndexNumber;
	float	m_BoneWeight;		// 가중치
};


// Vertex 하나에 주어진 가중치들을 포함하는 구조체 (구조체 하나가 버텍스 하나와 일대일 대응이다)
struct VertexWeight : public AlignedAllocationPolicy<16>
{
	int m_NumWeightVertex;	// 버텍스의 번호. 역시 벡터로 관리하므로 알 수 있지만...
		
	Weight* m_TempBoneBlendWeight; // 마찬가지로 아래 리스트의 임시 공간
	
	vector<Weight*> m_BoneBlendWeight_v;	// bone하나와 연결된 가중치 버텍스 하나는 여러 개의 Bone-Weight를 가질 수 있음
};


/// 한 개의 메시를 파싱 - 저장하기 위한 클래스
/// 복수 개의 메시가 있을 수 있기 때문에 따로 이 부분만 구조체로 만들었다.
struct Mesh : public AlignedAllocationPolicy<16>
{
	Mesh();
	~Mesh();

	/// 타입, 애니메이션 등등
	int m_type; // 타입
	bool m_IsAnimated; // 애니메이션을 가지고 있는가? (오브젝트에 따라 다르게 해석될 수 있음)
	Scenedata m_SceneData; // (3DS Max의) 씬 데이터
	bool m_IsPosAnimated;
	bool m_IsRotAnimated;

	/// *NODE_NAME
	string m_NodeName;		// 어떤 식인지는 모르지만 아래의 m_meshname과 같은 이름이 한번 더 들어간다. 아마도 메쉬의 이름 리스트라던지... 뭐 그런게 아닐까 여튼 지금은 버전 1이므로 이정도로 만족을 하자.
	string m_ParentNodeName;	// 부모 노드의 이름. 이것이 있다면 부모가 있는 것이다.
	bool m_IsParentExist = false;		// 그래도 확실히 bool형으로 가지고 있자.

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

	// 새로 추가 사항
	Matrix m_LocalTM;
	Matrix m_NodeTM;
	Mesh* m_pParentMesh;
	//vector<Mesh*> m_vpChildren; // 안씀

	/// *MESH
	float m_timevalue; // 여러 곳에서 여러가지로 쓰인다. 여튼 시간값.
	int m_numbone; // 본의 갯수. 이것이 있다면 스키닝 오브젝트로 간주한다.
	int m_numskinweight; // 스킨 가중치. 뭔소린지?
	int m_Mesh_NumVertex; // (버텍스의 좌표데이터)
	int m_MeshNumFace; // (버텍스의 인덱스)

	// 메시 안의 vertex 배열을 선언하기 위한 포인터
	/// *MESH_VERTEX_LIST
	vector<Vertex*> m_MeshVertices_v; // 버텍스
	/// *MESH_FACE_LIST
	vector<Face*> m_MeshFaces_v; // 인덱스

	// 텍스쳐용 데이터들
	/// *MESH_NUMTVERTEX
	bool m_IsTextureSet;
	int m_Mesh_NumTVertex; // 텍스쳐용 버텍스 갯수
	vector<OneTexVertex*> m_MeshTVertices_v;		// 텍스쳐용 버텍스
	/// *MESH_RVERTLIST
	int m_mesh_tvfaces; // 텍스쳐용 페이스 갯수 - (갯수는 위의 Face와 같고 이미 위에서 만들었으므로 내용만 넣어주면 된다)
	int m_mesh_numcvertex; // ※ 이건 어디에 쓰는것?

	// 노말 정보들
	//Vector3* m_mesh_facenormal;			// 각 페이스 노말에 (아직 안씁)
	//Vector3* m_mesh_vertexnormal;		// 버텍스 노말이 3개씩 (아직 안씁)

	vector<Vertex*> m_OptimizedVertices_v; /// 최적화 후의 버텍스들
	IndexList* m_OptimizedIndices; /// 최적화 후의 인덱스들

	/// Shape Object
	int m_shape_linecount; // 라인의 수
	vector<ShapeLine*> m_vector_shape_line; // 쉐이프 라인의 데이터

	/// 나머지 정보들
	int m_prop_motionblur; //*PROP_MOTIONBLUR 0
	int m_prop_castshadow; //*PROP_CASTSHADOW 1
	int m_prop_recvshadow; //*PROP_RECVSHADOW 1
	//int m_material_ref; //*MATERIAL_REF  이 메쉬의 매트리얼의 번호
	Vector3 m_wireframe_color; //*WIREFRAME_COLOR 0.89803922	0.65098041	0.84313726

	/// Camera
	// 이렇게 모든것을 한번에 읽어버리는 것은 별로긴 한데..
	// 일단 이렇게 만들어버렸으니까. 그리고 파서를 재귀구조로 만들게 되면 이렇게 하는게 편하다
	// 어쩄든 D3D용 클래스로 전달할 때 변환이 되니까
	string m_camera_type;
	// CAMERA_SETTINGS
	//float m_timevalue;		// mesh에 이미 있다.
	float m_camera_near;
	float m_camera_far;
	float m_camera_fov;
	float m_camera_tdist;
	bool m_camera_isloadTarget;	// 카메라는 NodeTM이 두개가 있는데, 재귀구조내에서 문제가 생긴다 
											// (항상 뒤쪽의 Target을 읽어버릴 수 있다 그래서 그것을 피해가야 한다)

	/// Light
	string m_light_type;
	// Omni인가 SpotLight인가의 처리를 해 줘야 한다.

	// Light Property
	string m_light_shadows;
	bool m_light_uselight;
	string m_light_spotshape;
	bool m_light_useglobal;
	bool m_light_absmapbias;
	bool m_light_overshoot;

	// Light Setting & Animation
	Light_AnimationData* m_lightanimation;		// Light Setting
	vector<Light_AnimationData*> m_list_lightanimation;	// 그것의 Animation

	bool m_light_isloadTarget;	// 빛도 NodeTM이 두 개 있고, 카메라와 같은 이유로 이 변수가 있다.
	bool m_light_isAnimate;		// Animation이 되는가?

	/// Skinning Object
	// SKIN_INITTM은 무시하고
	// TM_ROW0~3
	// BONE_LIST
	bool m_IsSkinningObject;	// 스키닝 오브젝트인가?

	bool m_IsBone;
	Bone* m_Bone;									// Bone 한개의 포인터 (파서 내부에서 임시 공간으로 쓰임)
	VertexWeight* m_WeightVertex;						// Weight_Vertex 한개의 포인터 (마찬가지로 임시공간)
	vector<Bone*> m_Bone_v;		// 이건 그냥 번호와 이름 관계이다. 이 안에 포인터를 넣어놓아도 괜찮겠구나(참조하기 편하게 - 그렇지 않으면 매번 이름으로 검색을 해야될지도. 아니면 인덱스가 있으니 상수 시간대로 접근을 할수도?)
	vector<VertexWeight*> m_WeightVertex_v;		// 버텍스 하나에 달라붙어있는 가중치들
};




