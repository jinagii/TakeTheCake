#pragma once

class TestScene;
class DXObjectFactory;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Initialize(DXObjectFactory* pFactory);
	void Update(float dTime);
	void Render();

	void Finalize();

public:
	
	// Key입력 관련
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	void OnMouseMove(int btnState, int x, int y);

private:
	TestScene* m_pTestScene;

};

