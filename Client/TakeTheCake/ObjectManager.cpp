#include "pch.h"
#include "IObjectBase.h"
#include "ObjectManager.h"

int ObjectManager::sm_ObjectPoolSize = 0;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	Release();
}

void ObjectManager::Initialize()
{
	std::unordered_map<std::string, IObjectBase*>::iterator iter;

	for (iter = m_vecObjectPool.begin();
		iter != m_vecObjectPool.end();
		++iter)
	{
		iter->second->Initialize();
	}
}

void ObjectManager::Release()
{
	std::unordered_map<std::string, IObjectBase*>::iterator iter;
	for (iter = m_vecObjectPool.begin();
		iter != m_vecObjectPool.end();
		++iter)
	{
		ReleaseCOM(iter->second);
	}
}

void ObjectManager::Update(Camera* pCamera, float dTime)
{
	std::unordered_map<std::string, IObjectBase*>::iterator iter;

	for (iter = m_vecObjectPool.begin();
		iter != m_vecObjectPool.end();
		++iter) 
	{
		iter->second->Update(pCamera, dTime);
	}
}

void ObjectManager::Render()
{
	std::unordered_map<std::string, IObjectBase*>::iterator iter;

	for (iter = m_vecObjectPool.begin();
		iter != m_vecObjectPool.end();
		++iter)
	{
		iter->second->Render();
	}
}

void ObjectManager::AddObject(std::string name, IObjectBase* object)
{
	m_vecObjectPool.insert({name, object});

	sm_ObjectPoolSize++;
}

void ObjectManager::DeleteObject(const std::string name)
{

	m_vecObjectPool.find(name)->second->Release();	// 릴리즈 후
	m_vecObjectPool.erase(name);					// 제거
}

IObjectBase* ObjectManager::FindObject(const std::string name)
{
	std::unordered_map<std::string, IObjectBase*>::iterator iter;
	iter = m_vecObjectPool.find(name);

	if (iter == m_vecObjectPool.end())
		return nullptr;

	return iter->second;
}

int ObjectManager::GetObjectPoolSize() const
{
	return m_vecObjectPool.size();
}

