#include "Stalactite.h"
#include "LevelCollider.h"
#include "Camera.h"

Stalactite::Stalactite()
{
	m_szSize = { 16, 16 };
	m_ptPosition = { 900, 200 };
	m_ptStartPosition = { 900, 200 };
}


Stalactite::~Stalactite()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Stalactite::Update(float elapsedTime)
{
	if(m_bIsFalling)
	{
		m_vtVelocity.y = m_fFallSpeed;
	}

	if(m_fRespawnTimer > 0.0f)
	{
		m_fRespawnTimer -= elapsedTime;
	}


	if(m_fRespawnTimer < 0.0f)
	{
		m_fRespawnTimer = 0.0f;
		m_szSize = { 16, 16 };

	}

	m_ptPosition += m_vtVelocity * elapsedTime;

}
void Stalactite::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	Camera::GetInstance()->Draw(rMyRect, { 255, 0, 255, 255 });
}

int Stalactite::GetType(void) const
{

	return ENT_STALACTITE;
}

SGD::Rectangle Stalactite::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Stalactite::HandleCollision(const IEntity* pOther)
{
	if(pOther->GetType() == ENT_HAWK)
	{
		m_bIsFalling = true;
	}
				  if(pOther->GetType() == ENT_SOLID_WALL)
				  {
				  m_bIsFalling = false;
				  m_szSize = { 0, 0 };
				  m_fRespawnTimer = 3.0f;
				  m_vtVelocity.y = 0;
				  m_ptPosition = m_ptStartPosition;
				  }


}