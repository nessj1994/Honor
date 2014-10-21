#include "Activator.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

Activator::Activator(bool isPressure)
{
	m_bPressurePlate = isPressure;

	m_ptPosition = { 68, 300 };
	m_szSize = { 32, 32 };

}


Activator::~Activator()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Activator::Update(float elapsedTime)
{
	if (m_fSwitchTimer > 0.0f)
	{
		m_fSwitchTimer -= elapsedTime;

	}


	if (m_fSwitchTimer < 0.0f && m_bPressurePlate == false)
	{
		m_fSwitchTimer = 0.0f;
	}
	else if (m_fSwitchTimer < 0.0f && m_bPressurePlate == true)
	{
		m_fSwitchTimer = 0.0f;
		//Close Door
		SGD::Event* pATEvent = new SGD::Event("CLOSE_DOOR", nullptr, this);
		SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
		pATEvent = nullptr;
	}

}

void Activator::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//Get our rectangle
	SGD::Rectangle rMyRect = GetRect();

	rMyRect.Offset({ -camPos.x, -camPos.y });

	Camera::GetInstance()->Draw(rMyRect, { 255, 255, 0, 0 });
}

int Activator::GetType(void) const
{

	if (m_bPressurePlate == false)
	{
		return ENT_SWITCH;
	}
	else
	{
		return ENT_PRESSURE_PLATE;
	}
}

SGD::Rectangle Activator::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void Activator::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		if (m_bPressurePlate == false && m_fSwitchTimer == 0.0f)
		{
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 3.0f;

		}
		else if (m_bPressurePlate == true)
		{
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("OPEN_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 0.25f;
		}

	}


	if (pOther->GetType() == ENT_HAWK)
	{
		if (m_bPressurePlate == false && m_fSwitchTimer == 0.0f)
		{

			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 3.0f;
		}

	}
}

