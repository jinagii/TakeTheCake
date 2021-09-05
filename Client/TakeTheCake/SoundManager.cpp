#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

HRESULT SoundManager::Initialize()
{
	/// 이미 초기화 되어 있으면 다시 하지 않는다.
	if (m_IsInitialized) return S_OK;

	// 초기화상태된로 변경
	m_IsInitialized = true;

	// Sound 파일 경로
	std::string soundPath = "../../Library/Sounds/";

	AddSound(SOUND_TITLE_BGM,			(soundPath + SOUND_TITLE_BGM).c_str());
	AddSound(SOUND_INGAME_BGM,			(soundPath + SOUND_INGAME_BGM).c_str());
	AddSound(SOUND_ANT_BOID,			(soundPath + SOUND_ANT_BOID).c_str());
	AddSound(SOUND_TRAP,				(soundPath + SOUND_TRAP).c_str());
	AddSound(SOUND_GAME_CLEAR,			(soundPath + SOUND_GAME_CLEAR).c_str());
	AddSound(SOUND_GAME_OVER,			(soundPath + SOUND_GAME_OVER).c_str());
	AddSound(SOUND_ANT_MOVE,			(soundPath + SOUND_ANT_MOVE).c_str());
	AddSound(SOUND_ANT_DIE,				(soundPath + SOUND_ANT_DIE).c_str());
	AddSound(SOUND_CAKE_PICKUP,			(soundPath + SOUND_CAKE_PICKUP).c_str());
	//AddSound(SOUND_INTERACTION_TRAP,	(soundPath + SOUND_INTERACTION_TRAP).c_str());

	return S_OK;
}

void SoundManager::Release()
{
	DeleteAll();
}

Sound* SoundManager::AddSound(const string soundName, const char* filePath)
{
	// 먼저 해당하는 사운드가 있는지 찾음
	Sound* sound = FindSound(soundName);

	// 사운드가 있으면 그 사운드 리턴
	if (sound) return sound;

	// 사운드가 없으면 새로 만듬
	sound = new Sound();

	// 사운드 초기화가 실패하면
	if (!sound->soundOpen(filePath))
	{
		// 다시 안전하게 제거하고
		SAFE_DELETE(filePath);

		// nullptr 리턴
		return nullptr;
	}

	// 정상적으로 만들어지면 리스트에 추가
	m_SoundList.insert({ soundName, sound });

	return sound;
}

Sound* SoundManager::FindSound(std::string soundName)
{
	MapSoundIter iter = m_SoundList.find(soundName);

	// 찾았다면
	if (iter != m_SoundList.end())
	{
		return iter->second;
	}

	// 없으면 nullptr 리턴
	return nullptr;
}

void SoundManager::FindAndPlay(std::string soundName, bool IsLoop /*= false*/)
{
	Sound* _newSound = FindSound(soundName);
	_newSound->soundSeek(0);

	if (IsLoop == false)
	{
		_newSound->soundPlay();
	}
	else
	{
		_newSound->soundPlayLooping();
	}
}

BOOL SoundManager::DeleteAll()
{
	MapSoundIter iter = m_SoundList.begin();

	for (; iter != m_SoundList.end();)
	{
		// 사운드 정보가 있으면
		if (iter->second != nullptr)
		{
			// 사운드 메모리 해제
			SAFE_DELETE(iter->second);

			// 리스트에서 제거
			iter = m_SoundList.erase(iter);
		}

		else ++iter;	// 다음으로

	}

	m_SoundList.clear();

	return true;
}
