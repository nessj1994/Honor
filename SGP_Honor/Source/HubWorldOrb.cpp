#include "HubWorldOrb.h"
#include "Emitter.h"
#include "ParticleEngine.h"
#include "Camera.h"

HubWorldOrb::HubWorldOrb(SGD::Point _Pos)
{
	m_emEffect = ParticleEngine::GetInstance()->LoadEmitter("","",_Pos);
	m_hGlassOrb = SGD::GraphicsManager::GetInstance()->LoadTexture("");
}

HubWorldOrb::~HubWorldOrb()
{
	delete m_emEffect;
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hGlassOrb);
}

void HubWorldOrb::Update(float elapsedTime)
{
	m_emEffect->Update(elapsedTime);
}

void HubWorldOrb::Render()
{
	m_emEffect->Render();
	Camera::GetInstance()->DrawTexture(m_ptPosition, {}, m_hGlassOrb, false, 1, {}, {});
}