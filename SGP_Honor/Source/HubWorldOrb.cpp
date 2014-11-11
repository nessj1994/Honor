#include "HubWorldOrb.h"
#include "Emitter.h"
#include "ParticleEngine.h"
#include "Camera.h"

HubWorldOrb::HubWorldOrb()
{
	m_hGlassOrb = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/CrystalBall.png");
	m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hGlassOrb);
}

HubWorldOrb::~HubWorldOrb()
{
	delete m_emEffect;
	delete m_emEffect2;
	delete m_emEffect3;
	delete m_emEffect4;
	delete m_emEffect5;
	delete m_emEffect6;
	delete m_emEffect7;
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hGlassOrb);
}

void HubWorldOrb::Update(float elapsedTime, int HonorCollected, SGD::Point _Pos)
{
	if (m_emEffect == nullptr &&  m_emEffect2 == nullptr && m_emEffect3 == nullptr)
	{
		m_emEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/GreenHonorOrbEmitter.xml", "GreenHonorOrb", _Pos);
		m_emEffect2 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/OrangeHonorOrbEmitter.xml", "OrangeHonorOrb", _Pos);
		m_emEffect3 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/SilverHonorOrbEmitter.xml", "SilverHonorOrb", _Pos);
		m_emEffect4 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/RedHonorOrbEmitter.xml", "RedHonorOrb", _Pos);
		m_emEffect5 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/BlueHonorOrbEmitter.xml", "BlueHonorOrb", _Pos);
		m_emEffect6 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/YellowHonorOrbEmitter.xml", "PurpleHonorOrb", _Pos);
		m_emEffect7 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/PurpleHonorOrbEmitter.xml", "YellowHonorOrb", _Pos);
		m_emEffect->SetMaxParticles(HonorCollected / 7);
		m_emEffect2->SetMaxParticles(HonorCollected / 7);
		m_emEffect3->SetMaxParticles(HonorCollected / 7);
		m_emEffect4->SetMaxParticles(HonorCollected / 7);
		m_emEffect5->SetMaxParticles(HonorCollected / 7);
		m_emEffect6->SetMaxParticles(HonorCollected / 7);
		m_emEffect7->SetMaxParticles(HonorCollected / 7);
		m_ptPosition = _Pos;
			m_emEffect->StartParticles(true);
			m_emEffect2->StartParticles(true);
			m_emEffect3->StartParticles(true);
			m_emEffect4->StartParticles(true);
			m_emEffect5->StartParticles(true);
			m_emEffect6->StartParticles(true);
			m_emEffect7->StartParticles(true);
			m_iHonorCollected = HonorCollected;		
	}
	m_emEffect->SetPosition(_Pos);
	m_emEffect->Update(elapsedTime);
	m_emEffect2->SetPosition(_Pos);
	m_emEffect2->Update(elapsedTime);
	m_emEffect3->SetPosition(_Pos);
	m_emEffect3->Update(elapsedTime);
	m_emEffect4->SetPosition(_Pos);
	m_emEffect4->Update(elapsedTime);
	m_emEffect5->SetPosition(_Pos);
	m_emEffect6->SetPosition(_Pos);
	m_emEffect6->Update(elapsedTime);
	m_emEffect7->SetPosition(_Pos);
	m_emEffect7->Update(elapsedTime);
	m_ptPosition = _Pos;
	if (m_iHonorCollected != HonorCollected)
	{
		m_emEffect->SetMaxParticles(HonorCollected / 7);
		m_emEffect2->SetMaxParticles(HonorCollected / 7);
		m_emEffect3->SetMaxParticles(HonorCollected / 7);
		m_emEffect4->SetMaxParticles(HonorCollected / 7);
		m_emEffect5->SetMaxParticles(HonorCollected / 7);
		m_emEffect6->SetMaxParticles(HonorCollected / 7);
		m_emEffect7->SetMaxParticles(HonorCollected / 7);
		m_emEffect->StartParticles(true);
		m_emEffect2->StartParticles(true);
		m_emEffect3->StartParticles(true);
		m_emEffect4->StartParticles(true);
		m_emEffect5->StartParticles(true);
		m_emEffect6->StartParticles(true);
		m_emEffect7->StartParticles(true);
		m_iHonorCollected = HonorCollected;
	}
}

void HubWorldOrb::Render()
{
	if (m_emEffect != nullptr)
	{
		m_emEffect->Render();
		m_emEffect2->Render();
		m_emEffect3->Render();
		m_emEffect4->Render();
		m_emEffect5->Render();
		m_emEffect6->Render();
		m_emEffect7->Render();
		Camera::GetInstance()->DrawTexture({ m_ptPosition.x - m_szSize.width / 2, m_ptPosition.y - m_szSize.height / 2 }, {}, m_hGlassOrb, false, 1, {}, {});
	}
}