#pragma once


/// <summary>
/// 게임 내에 등장하는 게임 오브젝트들을 관리
/// 
/// 생성, 삭제, 찾기, 씬 별로 분배
/// 
/// 
/// </summary>

class ObjectManager
{
public:
	/// static 멤버 count
	static int sm_ObjectPoolSize;

	ObjectManager();
	virtual ~ObjectManager();

	void Initialize();
	void Release();
	void Update(class Camera* pCamera, float dTime);
	void Render();

	void AddObject(std::string name, class IObjectBase* object);	// 오브젝트 추가
	void DeleteObject(const std::string name);

	class IObjectBase* FindObject(const std::string name);

	int GetObjectPoolSize() const;


private:
	// 오브젝트의 최상위 부모의 리스트로 관리
	std::unordered_map<std::string, class IObjectBase*> m_vecObjectPool;
};

