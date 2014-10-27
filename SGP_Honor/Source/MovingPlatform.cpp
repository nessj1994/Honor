#include "MovingPlatform.h"
#include "Camera.h"


MovingPlatform::MovingPlatform()
{
}


MovingPlatform::~MovingPlatform()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void MovingPlatform::Update(float elapsedTime)
{
	// Move horizontally or vertically
	if(m_bIsVertical)
	{
		// Moving backward or forward
		if(m_bReturning)
		{
			// Update speed and keep track of how far it has traveled
			m_fDistanceTraveled += m_fSpeed * elapsedTime;
			SetVelocity({ 0.0f, -m_fSpeed });
			// Move each entity
			for(unsigned int i = 0; i < m_vEntities.size(); ++i)
			{
				SGD::Point position = m_vEntities[i]->GetPosition();
				position.y -= m_fSpeed * elapsedTime;
				m_vEntities[i]->SetPosition(position);
			}
		}
		else
		{
			m_fDistanceTraveled += m_fSpeed * elapsedTime;
			SetVelocity({ 0.0f, m_fSpeed });
			// Move each entity
			for (unsigned int i = 0; i < m_vEntities.size(); ++i)
			{
				SGD::Point position = m_vEntities[i]->GetPosition();
				position.y += m_fSpeed * elapsedTime;
				m_vEntities[i]->SetPosition(position);
			}
		}
	}
	else
	{
		// Moving backward or forward
		if(m_bReturning)
		{
			m_fDistanceTraveled += m_fSpeed * elapsedTime;
			SetVelocity({ -m_fSpeed, 0.0f });
			// Move each entity
			for (unsigned int i = 0; i < m_vEntities.size(); ++i)
			{
				SGD::Point position = m_vEntities[i]->GetPosition();
				position.x -= m_fSpeed * elapsedTime;
				m_vEntities[i]->SetPosition(position);
				//m_vEntities[i]->SetVelocity(m_vtVelocity + m_vEntities[i]->GetVelocity());

			}
		}
		else
		{
			m_fDistanceTraveled += m_fSpeed * elapsedTime;
			SetVelocity({ m_fSpeed, 0.0f });
			// Move each entity
			for (unsigned int i = 0; i < m_vEntities.size(); ++i)
			{
				SGD::Point position = m_vEntities[i]->GetPosition();
				position.x += m_fSpeed * elapsedTime;
				m_vEntities[i]->SetPosition(position);

				//m_vEntities[i]->SetVelocity(m_vtVelocity + m_vEntities[i]->GetVelocity());

			}
		}
	}

	// Determine if it is time to move back
	if(m_fDistanceTraveled >= m_fTurnDistance)
	{
		m_fDistanceTraveled = 0;
		m_bReturning = !m_bReturning;
	}

	// Clear out entities
	m_vEntities.clear();

	Entity::Update(elapsedTime);
}
void MovingPlatform::Render(void)
{

	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Render us with the camera
	Camera::GetInstance()->Draw(rMyRect,
		SGD::Color::Color(255, 120, 80, 50));

}

int MovingPlatform::GetType(void) const
{

	return ENT_MOVING_PLATFORM;
}

SGD::Rectangle MovingPlatform::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void MovingPlatform::HandleCollision(const IEntity* pOther)
{
	m_vEntities.push_back((Entity*)pOther);
}