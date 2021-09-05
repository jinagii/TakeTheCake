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
	/// �̹� �ʱ�ȭ �Ǿ� ������ �ٽ� ���� �ʴ´�.
	if (m_IsInitialized) return S_OK;

	// �ʱ�ȭ���µȷ� ����
	m_IsInitialized = true;

	// Sound ���� ���
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
	// ���� �ش��ϴ� ���尡 �ִ��� ã��
	Sound* sound = FindSound(soundName);

	// ���尡 ������ �� ���� ����
	if (sound) return sound;

	// ���尡 ������ ���� ����
	sound = new Sound();

	// ���� �ʱ�ȭ�� �����ϸ�
	if (!sound->soundOpen(filePath))
	{
		// �ٽ� �����ϰ� �����ϰ�
		SAFE_DELETE(filePath);

		// nullptr ����
		return nullptr;
	}

	// ���������� ��������� ����Ʈ�� �߰�
	m_SoundList.insert({ soundName, sound });

	return sound;
}

Sound* SoundManager::FindSound(std::string soundName)
{
	MapSoundIter iter = m_SoundList.find(soundName);

	// ã�Ҵٸ�
	if (iter != m_SoundList.end())
	{
		return iter->second;
	}

	// ������ nullptr ����
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
		// ���� ������ ������
		if (iter->second != nullptr)
		{
			// ���� �޸� ����
			SAFE_DELETE(iter->second);

			// ����Ʈ���� ����
			iter = m_SoundList.erase(iter);
		}

		else ++iter;	// ��������

	}

	m_SoundList.clear();

	return true;
}
