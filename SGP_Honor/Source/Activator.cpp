#include "Activator.h"
#include "Camera.h"

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
	if(m_fSwitchTimer > 0.0f)
	{
		m_fSwitchTimer -= elapsedTime;

	}
	

	if(m_fSwitchTimer < 0.0f)
	{
		m_fSwitchTimer = 0.0f;
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

	if(m_bPressurePlate == false)
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
	if(pOther->GetType() == ENT_PLAYER)
	{
		if(m_fSwitchTimer == 0.0f)
		{
			m_bIsOn = !m_bIsOn;
			m_fSwitchTimer = 3.0f;

		}

	}
}
