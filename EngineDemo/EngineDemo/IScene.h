#pragma once

class DXObjectFactory;

class IScene
{
public:
	IScene() {}
	virtual ~IScene() {}

	virtual void Initialize(DXObjectFactory* pFactory) {}
	virtual void Update(float dTime) {}
	virtual void Render() {}
};