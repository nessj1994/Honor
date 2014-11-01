#include "Laser.h"
#include "Camera.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

Laser::Laser() : Listener(this)
{
	m_szSize = { 32, 32 };

	Listener::RegisterForEvent("FLIP_LASER");

	m_ptPosition = { 200, 150 };
	m_ptOrigPos = m_ptPosition;
	//m_szSize = { 100, 200 };
	m_szOrigSize = m_szSize;
	m_bOn = true;

	}



Laser::~Laser()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Laser::Update(float elapsedTime)
{

	if (m_bOn == true)
	{

		if (m_bFull == false)
		{
			if (m_vtDirection.y == -1)
			{
				m_ptPosition.y -= 100 * elapsedTime;
				m_szSize.height += 100 * elapsedTime;
			}
			else if (m_vtDirection.y == 1)
			{
				//m_ptPosition.y -= 100 * elapsedTime;
				m_szSize.height += 100 * elapsedTime;
			}

			if (m_vtDirection.x == -1)
			{
				m_ptPosition.x -= 100 * elapsedTime;
				m_szSize.width += 100 * elapsedTime;
			}
			else if (m_vtDirection.x == 1)
			{
				//m_ptPosition.x -= 100 * elapsedTime;
				m_szSize.width += 100 * elapsedTime;
			}
		}

	}
	else
	{
		m_ptPosition.y = m_ptOrigPos.y;
		m_szSize.height = m_szOrigSize.height;

		m_bFull = false;
	}

}
void Laser::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();



	Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(255, 0, 255, 0));
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
	if (pOther->GetType() == ENT_SOLID_WALL)
	{
		m_bFull = true;
	}


}

void Laser::HandleEvent(const SGD::Event* pEvent)
{
	
	if (pEvent->GetEventID() == "FLIP_LASER")
	{
		Activator* pActivator = reinterpret_cast<Activator*>(pEvent->GetSender());
		if (pActivator->GetKeyID() == m_nFreq)
		{
			m_bOn = !m_bOn;
		}
	}
}