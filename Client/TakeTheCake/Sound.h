#pragma once

/// <summary>
/// Sound Class
/// Writer : YoKing
/// 
/// 게임 내에서 사용할 사운드 클래스
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
	//사운드 파일을 엽니다.
	//성공할 경우 TRUE를 리턴합니다.
	BOOL soundOpen(const char* FileName);

	//사운드 파일을 닫습니다. 
	//성공할 경우 TRUE를 리턴합니다.
	BOOL soundClose();

	//사운드를 재생합니다.
	//사운드 재생을 실패하면 FALSE를 리턴합니다.
	BOOL soundPlay();

	//사운드를 반복재생합니다.(wav파일은 반복되지 않고 한번만 재생합니다.) 
	//사운드 재생을 실패하면 FALSE를 리턴합니다.
	BOOL soundPlayLooping();

	//사운드 재생을 일시정지합니다.
	//사운드가 재생중이 아닌경우 FALSE를 리턴합니다.
	BOOL soundPause();

	//사운드 재생을 재개합니다.
	//사운드가 재생중일 경우 FALSE를 리턴합니다.
	BOOL soundResume();

	//사운드의 볼륨을 가져옵니다.
	//성공할경우 볼륨을 리턴하고 실패하면 -1을 반환합니다.
	DWORD soundGetVolume();

	//사운드의 볼륨을 설정합니다. 볼륨은 0 ~ 1000 사이의 값을 가질 수 있습니다.
	//볼륨설정에 성공한 경우 TRUE를 리턴합니다.
	BOOL soundSetVolume(DWORD volume);

	//사운드의 위치를 탐색합니다.
	//탐색중인동안 사운드가 정지합니다.
	BOOL soundSeek(DWORD seekTo);

	//사운드를 반복재생합니다.(wav파일용)
	//사운드 재생을 실패하면 FALSE를 리턴합니다.
	BOOL soundPlayLoopingWav();

};

