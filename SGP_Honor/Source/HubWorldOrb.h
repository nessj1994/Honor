#pragma once
#include "../SGD Wrappers/SGD_GraphicsManager.h"

class Emitter;

class HubWorldOrb
{
	SGD::HTexture m_hGlassOrb = SGD::INVALID_HANDLE;
	Emitter* m_emEffect;
	Emitter* m_emEffect2;
	Emitter* m_emEffect3;
	SGD::Point m_ptPosition;
public:
	HubWorldOrb();
	~HubWorldOrb();
	void Update(float elapsedTime,int HonorCollected,SGD::Point _Pos);
	void Render();


};

