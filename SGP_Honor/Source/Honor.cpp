#include "Honor.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"
#include "ParticleEngine.h"
#include "Player.h"

Honor::Honor()
{
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/HonorPiece.png");
	m_ptPosition = SGD::Point(200, 300);
	SetSize(SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage));
	m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/GreenHonor.xml", "GreenHonor", m_ptPosition);
}


Honor::~Honor()
{
	delete m_eEffect;
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Honor::Update(float elapsedTime)
{
	m_eEffect->Update(elapsedTime);
}
void Honor::Render(void)
{
	m_eEffect->Render(m_ptPosition);
	if (isCollected == false)
		Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false);
	//SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, SGD::Point(200, 400));
}

SGD::Rectangle Honor::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Honor::SetEmitter()
{
	switch (m_HonorAmount)
	{
		default:
			m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/GreenHonor.xml", "GreenHonor", m_ptPosition);
			break;
	}
}

void Honor::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == Entity::ENT_PLAYER)
	{
		if (GetRect().IsIntersecting(pOther->GetRect()) == true)
			isCollected = true;

	}
}