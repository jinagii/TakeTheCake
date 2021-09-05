#pragma once

/// <summary>
/// SoundManager Class
/// Writer : YoKing
/// 
/// ���� ���� Ŭ����
/// 
/// ���ӳ� ������� �߰��� ã�� ���� �Ѵ�.
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

	// ���� �߰� �Լ�
	class Sound* AddSound(const string soundName, const char* filePath);

	// Ű ������ ���� ã��
	class Sound* FindSound(std::string soundName);

	// ã�Ƽ� �ѹ��� ���
	void FindAndPlay(std::string soundName, bool IsLoop = false);

	// ��� ���� ����
	BOOL DeleteAll();

private:
	// �ʱ�ȭ ����
	bool m_IsInitialized;

	using  MapSoundList = std::unordered_map<std::string, Sound*>;
	using  MapSoundIter = std::unordered_map<std::string, Sound*>::iterator;

	MapSoundList m_SoundList;	// ���� ����Ʈ

};

