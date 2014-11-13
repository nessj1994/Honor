#include "Honor.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
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
	//m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/GreenHonor.xml", "GreenHonor", midPoint);

	// Sounds
	m_hPiano1 = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/Piano1.wav");
	m_hPiano2 = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/Piano2.wav");
	m_hPiano3 = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/Piano3.wav");
	m_hPiano4 = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/Piano4.wav");
	m_hPiano5 = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/Piano5.wav");
	m_hPiano6 = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/Piano6.wav");
	m_hPiano7 = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/Piano7.wav");
}


Honor::~Honor()
{
	delete m_eEffect;
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);

	// Unload sounds
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hPiano1);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hPiano2);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hPiano3);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hPiano4);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hPiano5);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hPiano6);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hPiano7);
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
			if (!m_bIsCollected)
			{
				PlayAudio();
			}
			GameplayState::GetInstance()->IncreaseHonorBeforeDeath(m_HonorAmount);
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

	// Reset honor
	if (pEvent->GetEventID() == "ResetRoom" && !m_bStartedCollected)
	{
		SetIsCollected(false);
		GameplayState::GetInstance()->GetCurrentLevel()->UpdateHonorVector(m_unVectorID, m_bIsCollected);
	}
}

//////////////////////////
// PlayAudio
// -Plays the correct audio based on how much honor this piece has
void Honor::PlayAudio()
{
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();
	switch (m_HonorAmount)
	{
		case 1: // Green 1
			pAudio->PlayAudio(m_hPiano1);
			break;
		case 5: // Blue 5
			pAudio->PlayAudio(m_hPiano2);
			break;
		case 10: // Yellow 10
			pAudio->PlayAudio(m_hPiano3);
			break;
		case 20: // Red 20
			pAudio->PlayAudio(m_hPiano4);
			break;
		case 50: // Purple 50
			pAudio->PlayAudio(m_hPiano5);
			break;
		case 100: // Orange 100
			pAudio->PlayAudio(m_hPiano6);
			break;
		case 200: // Silver 200
			pAudio->PlayAudio(m_hPiano7);
			break;
	}
}