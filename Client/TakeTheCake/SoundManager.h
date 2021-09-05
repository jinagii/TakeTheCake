#pragma once

/// <summary>
/// SoundManager Class
/// Writer : YoKing
/// 
/// 사운드 관리 클래스
/// 
/// 게임내 사운드들의 추가와 찾기 등을 한다.
/// 
/// First Create	  : 2021.08.22
/// Last Modification : 2021.08.22
/// </summary>

#include "SingletonBase.h"

class SoundManager : public SingletonBase<SoundManager>
{
public:
	SoundManager();
	~SoundManager();

	HRESULT Initialize();
	void Release();

	// 사운드 추가 함수
	class Sound* AddSound(const string soundName, const char* filePath);

	// 키 값으로 사운드 찾기
	class Sound* FindSound(std::string soundName);

	// 찾아서 한번에 출력
	void FindAndPlay(std::string soundName, bool IsLoop = false);

	// 모든 사운드 제거
	BOOL DeleteAll();

private:
	// 초기화 여부
	bool m_IsInitialized;

	using  MapSoundList = std::unordered_map<std::string, Sound*>;
	using  MapSoundIter = std::unordered_map<std::string, Sound*>::iterator;

	MapSoundList m_SoundList;	// 사운드 리스트

};

