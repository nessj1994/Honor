#include "HubWorldOrb.h"
#include "Emitter.h"
#include "ParticleEngine.h"
#include "Camera.h"

HubWorldOrb::HubWorldOrb()
{
	m_emEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/GreenHonorOrbEmitter.xml", "GreenHonorOrb", { -1000, -1000 });
	m_emEffect2 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/OrangeHonorOrbEmitter.xml", "OrangeHonorOrb", { -1000, -1000 });
	m_emEffect3 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/SilverHonorOrbEmitter.xml", "SilverHonorOrb", { -1000, -1000 });
	m_hGlassOrb = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/Fallingspike.png");
}

HubWorldOrb::~HubWorldOrb()
{
	delete m_emEffect;
	delete m_emEffect2;
	delete m_emEffect3;
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hGlassOrb);
}

void HubWorldOrb::Update(float elapsedTime, int HonorCollected, SGD::Point _Pos)
{
	m_emEffect->SetPosition(_Pos);
	m_emEffect->Update(elapsedTime);
	m_emEffect2->SetPosition(_Pos);
	m_emEffect2->Update(elapsedTime);
	m_emEffect3->SetPosition(_Pos);
	m_emEffect3->Update(elapsedTime);
}

void HubWorldOrb::Render()
{
	m_emEffect->Render();
	m_emEffect2->Render();
	m_emEffect3->Render();
	Camera::GetInstance()->DrawTexture(m_ptPosition, {}, m_hGlassOrb, false, 1, {}, {});
}