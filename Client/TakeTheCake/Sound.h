#pragma once

/// <summary>
/// Sound Class
/// Writer : YoKing
/// 
/// ���� ������ ����� ���� Ŭ����
/// 
/// First Create	  : 2021.08.22
/// Last Modification : 2021.08.22
/// </summary>

class Sound
{
private:
	UINT DeviceID;
	BOOL isPause;

public:
	//���� ������ ���ϴ�.
	//������ ��� TRUE�� �����մϴ�.
	BOOL soundOpen(const char* FileName);

	//���� ������ �ݽ��ϴ�. 
	//������ ��� TRUE�� �����մϴ�.
	BOOL soundClose();

	//���带 ����մϴ�.
	//���� ����� �����ϸ� FALSE�� �����մϴ�.
	BOOL soundPlay();

	//���带 �ݺ�����մϴ�.(wav������ �ݺ����� �ʰ� �ѹ��� ����մϴ�.) 
	//���� ����� �����ϸ� FALSE�� �����մϴ�.
	BOOL soundPlayLooping();

	//���� ����� �Ͻ������մϴ�.
	//���尡 ������� �ƴѰ�� FALSE�� �����մϴ�.
	BOOL soundPause();

	//���� ����� �簳�մϴ�.
	//���尡 ������� ��� FALSE�� �����մϴ�.
	BOOL soundResume();

	//������ ������ �����ɴϴ�.
	//�����Ұ�� ������ �����ϰ� �����ϸ� -1�� ��ȯ�մϴ�.
	DWORD soundGetVolume();

	//������ ������ �����մϴ�. ������ 0 ~ 1000 ������ ���� ���� �� �ֽ��ϴ�.
	//���������� ������ ��� TRUE�� �����մϴ�.
	BOOL soundSetVolume(DWORD volume);

	//������ ��ġ�� Ž���մϴ�.
	//Ž�����ε��� ���尡 �����մϴ�.
	BOOL soundSeek(DWORD seekTo);

	//���带 �ݺ�����մϴ�.(wav���Ͽ�)
	//���� ����� �����ϸ� FALSE�� �����մϴ�.
	BOOL soundPlayLoopingWav();

};

