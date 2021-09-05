#pragma once

/// <summary>
/// SingletonBase Class
/// Writer : YoKing
/// 
/// ���丮�� ���� �Ŵ��� ���� �� ���� �ν��Ͻ��� ���� �����
/// Ŭ�������� �̱��� �������� ��������� Ŭ����
/// 
/// First Create	  : 2021.08.19
/// Last Modification : 2021.08.19
/// </summary>

template <typename T>
class SingletonBase
{
public:
	// �̱��� ������
	static T* GetInstance();
	
	// �̱��� Release
	virtual void Release();

protected:
	// �̱��� �ν��Ͻ�
	static T* sm_Singleton;

	SingletonBase() {};
	virtual ~SingletonBase() {};

	virtual HRESULT Initialize() { return S_OK; }

private:

};

// �̱��� �ʱ�ȭ
template <typename T>
T* SingletonBase<T>::sm_Singleton = nullptr;

template<typename T>
inline T* SingletonBase<T>::GetInstance()
{
	// �����Ǿ� ���� ������ ����
	if (!sm_Singleton)
	{
		sm_Singleton = new T;
	
	}

	// �ν��Ͻ� ��ȯ
	return sm_Singleton;
}

template<typename T>
inline void SingletonBase<T>::Release()
{
	// �ν��Ͻ��� ������
	if (sm_Singleton)
	{
		// �޸𸮿��� ����
		delete sm_Singleton;

		sm_Singleton = nullptr;
	}
}
