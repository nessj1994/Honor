#include "Honor.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"
#include "ParticleEngine.h"
#include "Player.h"
#include "GameplayState.h"
#include "Level.h"

Honor::Honor() : Listener(this)
{
	Listener::RegisterForEvent("ResetRoom");
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/HonorPiece.png");
	m_ptPosition = SGD::Point(200, 300);
	SetSize(SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage));
	SGD::Point midPoint = SGD::Point(m_ptPosition.x + m_szSize.width / 4, m_ptPosition.y + m_szSize.height / 4);
	m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/GreenHonor.xml", "GreenHonor", midPoint);
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
	if (m_bIsCollected == false)
	{
		Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 1.0f, {}, {});
		//SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, SGD::Point(200, 400));
		SGD::Point midPoint = SGD::Point(m_ptPosition.x + m_szSize.width / 4, m_ptPosition.y + m_szSize.height / 4);
		m_eEffect->Render(midPoint);
	}
}

SGD::Rectangle Honor::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Honor::SetEmitter()
{
	switch (m_HonorAmount)
	{
		case 1: // Green 1
			m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/GreenHonor.xml", "GreenHonor", m_ptPosition);
			break;
		case 5: // Blue 5
			m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/BlueHonor.xml", "BlueHonor", m_ptPosition);
			break;
		case 10: // Yellow 10
			m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/YellowHonor.xml", "YellowHonor", m_ptPosition);
			break;
		case 20: // Red 20
			m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/RedHonor.xml", "RedHonor", m_ptPosition);
			break;
		case 50: // Purple 50
			m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/PurpleHonor.xml", "PurpleHonor", m_ptPosition);
			break;
		case 100: // Orange 100
			m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/OrangeHonor.xml", "OrangeHonor", m_ptPosition);
			break;
		case 200: // Silver 200
			m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/SilverHonor.xml", "SilverHonor", m_ptPosition);
			break;
	}
}

void Honor::HandleCollision(const IEntity* pOther)
{
	if (!m_bIsCollected && pOther->GetType() == Entity::ENT_PLAYER)
	{
		if (GetRect().IsIntersecting(pOther->GetRect()) == true)
		{
			m_bIsCollected = true;
			GameplayState::GetInstance()->GetCurrentLevel()->UpdateHonorVector(m_unVectorID, m_bIsCollected);
		}
	}
}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void Honor::HandleEvent(const SGD::Event* pEvent)
{
	//which event

	//Turn around
	//if (pEvent->GetEventID() == "ResetRoom")
	//{
	//	SetIsCollected(false);
	//}
}