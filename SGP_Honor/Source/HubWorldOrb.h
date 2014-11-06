#pragma once
#include "../SGD Wrappers/SGD_GraphicsManager.h"

class Emitter;

class HubWorldOrb
{
	SGD::HTexture m_hGlassOrb = SGD::INVALID_HANDLE;
	Emitter* m_emEffect;
	SGD::Point m_ptPosition;
public:
	HubWorldOrb(SGD::Point _Pos);
	~HubWorldOrb();
	void Update(float elapsedTime);
	void Render();


};

