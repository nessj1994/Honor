#include "BuzzSaw.h"
#include "Camera.h"
#include <cmath>
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#define rotationSpeed 60.0f

BuzzSaw::BuzzSaw()
{
	m_ptStartPosition = { 700, 150 };
	m_ptPosition = m_ptStartPosition;
	m_szSize = { 32, 32 };
	m_bMovingRight = false;
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/Graphics/Buzzsaw.png");
	m_hEffect = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/Buzzsaw.wav");
}


BuzzSaw::~BuzzSaw()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage); 
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hEffect);
}


/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void BuzzSaw::Update(float elapsedTime)
{



	if (m_unDistance != 0)
	{
		if (m_bHorizontal == true)
		{


			if (m_bMovingRight == true)
			{
				m_vtVelocity.x = 800;
			}
			else if (m_bMovingRight == false)
			{
				m_vtVelocity.x = -800;
			}
		}
		else
		{
			if (m_bMovingRight == true)
			{
				m_vtVelocity.x = 80;
			}
			else if (m_bMovingRight == false)
			{
				m_vtVelocity.x = -80;
			}
		}
	}

	if(m_bMovingRight)
	{
		if(m_ptPosition.x > m_ptStartPosition.x + m_unDistance)
		{
			m_bMovingRight = false;
		}
	}
	else if(m_bMovingRight == false)
	{
		if(m_ptPosition.x < m_ptStartPosition.x - m_unDistance)
		{
			m_bMovingRight = true;
		}
	}
	rotation += rotationSpeed * elapsedTime;
	if (rotation >= 30.0f)
		rotation = 0.0f;

	m_ptPosition += m_vtVelocity* elapsedTime;
}
void BuzzSaw::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Render us with the camera
	Camera::GetInstance()->DrawTexture(m_ptPosition, rotation, m_hImage, false, 1.0f, SGD::Color::Color(255, 255, 255, 255), m_szSize / 2.0f);
}

int BuzzSaw::GetType(void) const
{

	return ENT_BUZZSAW;
}

SGD::Rectangle BuzzSaw::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };


}

void BuzzSaw::HandleCollision(const IEntity* pOther)
{

	if(pOther->GetType() == ENT_PLAYER)
	{
		//if so move back up but kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
		if(!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hEffect))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
			m_bEffectPlay = true;
		}
	}
}