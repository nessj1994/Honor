#pragma once
#include "../SGD Wrappers/SGD_GraphicsManager.h"

class Emitter;

class HubWorldOrb
{
	SGD::HTexture m_hGlassOrb = SGD::INVALID_HANDLE;
	Emitter* m_emEffect = nullptr;
	Emitter* m_emEffect2 =  nullptr;
	Emitter* m_emEffect3 = nullptr;
	Emitter* m_emEffect4 = nullptr;
	Emitter* m_emEffect5 = nullptr;
	Emitter* m_emEffect6 = nullptr;
	Emitter* m_emEffect7 = nullptr;
	SGD::Point m_ptPosition;
	SGD::Size m_szSize;
	int m_iHonorCollected;
public:
	HubWorldOrb();
	~HubWorldOrb();
	void Update(float elapsedTime,int HonorCollected,SGD::Point _Pos);
	void Render();


};

