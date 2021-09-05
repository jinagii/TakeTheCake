#pragma once


/// <summary>
/// ���� ���� �����ϴ� ���� ������Ʈ���� ����
/// 
/// ����, ����, ã��, �� ���� �й�
/// 
/// 
/// </summary>

class ObjectManager
{
public:
	/// static ��� count
	static int sm_ObjectPoolSize;

	ObjectManager();
	virtual ~ObjectManager();

	void Initialize();
	void Release();
	void Update(class Camera* pCamera, float dTime);
	void Render();

	void AddObject(std::string name, class IObjectBase* object);	// ������Ʈ �߰�
	void DeleteObject(const std::string name);

	class IObjectBase* FindObject(const std::string name);

	int GetObjectPoolSize() const;


private:
	// ������Ʈ�� �ֻ��� �θ��� ����Ʈ�� ����
	std::unordered_map<std::string, class IObjectBase*> m_vecObjectPool;
};

