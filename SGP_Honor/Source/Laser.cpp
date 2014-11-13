#include "Laser.h"
#include "Camera.h"
#include <Windows.h>

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "ParticleEngine.h"
Laser::Laser() : Listener(this)
{
	m_szSize = { 32, 32 };

	Listener::RegisterForEvent("FLIP_LASER");

	m_ptPosition = { 200, 150 };
	m_ptOrigPos = m_ptPosition;
	//m_szSize = { 100, 200 };
	m_szOrigSize = m_szSize;
	m_bOn = true;

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/Laser.png");
	m_hEffect = SGD::AudioManager::GetInstance()->LoadAudio("assets/audio/Laser.wav");
	m_LaserEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/LaserEffect.xml", "Laser", m_ptPosition);
	m_LaserEffect->SetSize(m_szSize);
}



Laser::~Laser()
{
	delete m_LaserEffect;
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hEffect);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Laser::Update(float elapsedTime)
{
	//Emitter process for laser
	m_LaserEffect->SetPosition(m_ptPosition);
	m_LaserEffect->SetSize(m_szSize);
	m_LaserEffect->Update(elapsedTime);
	//
	if(m_bOn == true)
	{
		if(!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hEffect))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
		}
		if(m_bFull == false)
		{
			if(m_vtDirection.y == -1)
			{
				m_ptPosition.y -= 800 * elapsedTime;
				m_szSize.height += 800 * elapsedTime;
			}
			else if(m_vtDirection.y == 1)
			{
				//m_ptPosition.y -= 100 * elapsedTime;
				m_szSize.height += 1500 * elapsedTime;
			}

			if(m_vtDirection.x == -1)
			{
				m_ptPosition.x -= 1500 * elapsedTime;
				m_szSize.width += 1500 * elapsedTime;
			}
			else if(m_vtDirection.x == 1)
			{
				//m_ptPosition.x -= 100 * elapsedTime;
				m_szSize.width += 1500 * elapsedTime;
			}
		}

	}
	else
	{
		SGD::AudioManager::GetInstance()->StopAudio(m_hEffect);
		m_ptPosition.y = m_ptOrigPos.y;
		m_szSize.height = m_szOrigSize.height;

		m_bFull = false;
	}


	m_bFull = false;

	//Emitter process for laser
	m_LaserEffect->SetPosition(m_ptPosition);
	m_LaserEffect->SetSize(m_szSize);
	m_LaserEffect->Update(elapsedTime);
	//
}
void Laser::Render(void)
{
	m_LaserEffect->Render();

	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();



	Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(100, 155, 0, 0));

	m_LaserEffect->Render();

	Camera::GetInstance()->DrawTexture({ m_ptPosition.x + m_szSize.width, m_ptPosition.y },
		0.0f, m_hImage, true, 0.8f, {}, {});
}

int Laser::GetType(void) const
{
	return ENT_LASER;

}

SGD::Rectangle Laser::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void Laser::HandleCollision(const IEntity* pOther)
{
	RECT rPlayer;
	rPlayer.left = (LONG)GetRect().left;
	rPlayer.top = (LONG)GetRect().top;
	rPlayer.right = (LONG)GetRect().right;
	rPlayer.bottom = (LONG)GetRect().bottom;

	//Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	//Create a rectangle for the intersection
	RECT rIntersection = {};


	IntersectRect(&rIntersection, &rObject, &rPlayer);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	if (pOther->GetType() == ENT_SOLID_WALL)
	{
		m_bFull = true;
	}
	if (pOther->GetType() == ENT_DOOR)
	{
		m_bFull = true;
		
		if (nIntersectHeight > 16)
		{
			m_ptPosition.y = m_ptOrigPos.y;
			m_szSize.height = m_szOrigSize.height;
		}

		//m_vtDirection.x != 0

		if (nIntersectWidth > 16
			&& m_vtDirection.x != 0)
		{
			m_ptPosition.x = m_ptOrigPos.x;
			m_szSize.width = m_szOrigSize.width;
		}
		
		
		
	}
	if (pOther->GetType() == ENT_LASER)
	{
		int x = 0;
	}


}

void Laser::HandleEvent(const SGD::Event* pEvent)
{

	if(pEvent->GetEventID() == "FLIP_LASER")
	{
		Entity* pEntity = reinterpret_cast<Entity*>(pEvent->GetSender());
		if(pEntity->GetType() == ENT_SWITCH)
		{
			Activator* pActivator = reinterpret_cast<Activator*>(pEvent->GetSender());
			if(pActivator->GetKeyID() == m_nFreq)
			{
				m_bOn = !m_bOn;
			}
		}
		if(pEntity->GetType() == ENT_BOSS_CRAB)
		{
			m_bOn = !m_bOn;
		}
		if (pEntity->GetType() == ENT_DOOR)
		{
			//m_bOn = !m_bOn;
			
			m_ptPosition.y = m_ptOrigPos.y;
			m_szSize.height = m_szOrigSize.height;
		}
	}
}