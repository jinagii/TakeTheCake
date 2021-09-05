#include "pch.h"
#include "Sound.h"


BOOL Sound::soundOpen(const char* FileName)
{
	DWORD dwReturn;
	MCI_OPEN_PARMS mciOpenParms;


	/// const char* ���ϰ�θ� LPCWSTR�� ��ȯ�ϴ� ����	
	int nLen = static_cast<int>(strlen(FileName) + 1);	// char ��� ����
	wchar_t str[200] = L"";								// �ӽ� ����

	size_t cn;

	/// <summary>
	/// ��Ƽ����Ʈ ������ �������� ���̵� ������ �������� ��ȯ
	/// </summary>
	/// <param name="pcon">��ȯ�� ������ ���� �޸� �ּ�</param>
	/// <param name="pwcs">��ȯ�� �����ڵ� ���ڿ��� ������ ����</param>
	/// <param name="sizet">���� ũ��</param>
	/// <param name="trans">���ڿ�</param>
	/// <param name="transColor">��ȯ�� �ִ� ����</param>
	/// <returns></returns>
	mbstowcs_s(&cn, str, 100, FileName, nLen);

	LPCWSTR convertedPath = str;					// LPCWSTR�� wc�� ����

	mciOpenParms.lpstrElementName = convertedPath;

	dwReturn = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms);
	if (dwReturn != 0)
	{
		return FALSE;
	}

	this->DeviceID = mciOpenParms.wDeviceID;
	this->isPause = FALSE;
	return TRUE;
}

BOOL Sound::soundClose()
{
	DWORD dwReturn = mciSendCommand(this->DeviceID, MCI_CLOSE, 0, NULL);
	if (dwReturn != 0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL Sound::soundPlay()
{
	DWORD dwReturn;
	MCI_PLAY_PARMS mciPlayParms;
	dwReturn = mciSendCommand(this->DeviceID, MCI_PLAY, NULL, (DWORD)(LPVOID)&mciPlayParms);
	if (dwReturn != 0)
	{
		soundClose();
		return FALSE;
	}
	this->isPause = FALSE;
	return TRUE;
}

BOOL Sound::soundPlayLooping()
{
	DWORD dwReturn;
	MCI_PLAY_PARMS mciPlayParms;
	dwReturn = mciSendCommand(this->DeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlayParms);
	if (dwReturn != 0)
	{
		soundClose();
		return FALSE;
	}
	this->isPause = FALSE;
	return TRUE;
}

BOOL Sound::soundPause()
{
	if (!this)
	{
		return FALSE;
	}

	if (!this->isPause)
	{
		DWORD dwReturn = mciSendCommand(this->DeviceID, MCI_PAUSE, NULL, NULL);
		if (dwReturn != 0)
		{
			return FALSE;
		}
		this->isPause = TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL Sound::soundResume()
{
	if (this->isPause)
	{
		DWORD dwReturn = mciSendCommand(this->DeviceID, MCI_RESUME, NULL, NULL);
		if (dwReturn != 0)
		{
			return FALSE;
		}
		this->isPause = FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

DWORD Sound::soundGetVolume()
{
	MCI_STATUS_PARMS mciStatusParams;
	mciStatusParams.dwItem = MCI_DGV_STATUS_VOLUME;
	DWORD dwReturn = mciSendCommand(this->DeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParams);
	if (dwReturn != 0)
	{
		return -1;
	}
	return mciStatusParams.dwReturn;;
}

BOOL Sound::soundSetVolume(DWORD volume)
{
	MCI_DGV_SETAUDIO_PARMS mciDgvSetaudioParams;
	DWORD currectVolume = min(1000, max(0, volume));

	mciDgvSetaudioParams.dwCallback = 0;
	mciDgvSetaudioParams.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciDgvSetaudioParams.dwValue = currectVolume;
	mciDgvSetaudioParams.dwOver = 0;
	mciDgvSetaudioParams.lpstrAlgorithm = NULL;
	mciDgvSetaudioParams.lpstrQuality = NULL;

	DWORD dwReturn = mciSendCommand(this->DeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciDgvSetaudioParams);
	if (dwReturn != 0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL Sound::soundSeek(DWORD seekTo)
{
	DWORD dwReturn;
	MCI_SEEK_PARMS mciSeekParams;
	mciSeekParams.dwTo = seekTo;

	dwReturn = mciSendCommand(this->DeviceID, MCI_SEEK, MCI_TO, (DWORD)(LPVOID)&mciSeekParams);
	if (dwReturn != 0)
	{
		soundClose();
		return FALSE;
	}

	this->isPause = TRUE;
	return TRUE;
}

BOOL Sound::soundPlayLoopingWav()
{
	DWORD dwReturn;
	DWORD length;
	DWORD position;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParams;

	dwReturn = mciSendCommand(this->DeviceID, MCI_STATUS, MCI_STATUS_LENGTH, (DWORD)(LPVOID)&mciStatusParams);
	if (dwReturn != 0)
	{
		soundClose();
		return FALSE;
	}
	length = mciStatusParams.dwReturn;

	dwReturn = mciSendCommand(this->DeviceID, MCI_STATUS, MCI_STATUS_POSITION, (DWORD)(LPVOID)&mciStatusParams);
	if (dwReturn != 0)
	{
		soundClose();
		return FALSE;
	}
	position = mciStatusParams.dwReturn;

	if (length <= position)
	{
		soundSeek(0);
	}

	dwReturn = mciSendCommand(this->DeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlayParms);
	if (dwReturn != 0)
	{
		soundClose();
		return FALSE;
	}
	return TRUE;
}
