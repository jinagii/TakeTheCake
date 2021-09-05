#pragma once

/// <summary>
/// SingletonBase Class
/// Writer : YoKing
/// 
/// 팩토리나 각종 매니저 같이 한 개의 인스턴스만 만들어서 사용할
/// 클래스들을 싱글톤 패턴으로 만들기위한 클래스
/// 
/// First Create	  : 2021.08.19
/// Last Modification : 2021.08.19
/// </summary>

template <typename T>
class SingletonBase
{
public:
	// 싱글톤 접근자
	static T* GetInstance();
	
	// 싱글톤 Release
	virtual void Release();

protected:
	// 싱글톤 인스턴스
	static T* sm_Singleton;

	SingletonBase() {};
	virtual ~SingletonBase() {};

	virtual HRESULT Initialize() { return S_OK; }

private:

};

// 싱글톤 초기화
template <typename T>
T* SingletonBase<T>::sm_Singleton = nullptr;

template<typename T>
inline T* SingletonBase<T>::GetInstance()
{
	// 생성되어 있지 않으면 생성
	if (!sm_Singleton)
	{
		sm_Singleton = new T;
	
	}

	// 인스턴스 반환
	return sm_Singleton;
}

template<typename T>
inline void SingletonBase<T>::Release()
{
	// 인스턴스가 있으면
	if (sm_Singleton)
	{
		// 메모리에서 해제
		delete sm_Singleton;

		sm_Singleton = nullptr;
	}
}
