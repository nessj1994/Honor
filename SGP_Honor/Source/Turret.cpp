#include "Turret.h"
#include "CreateProjectileMessage.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "Camera.h"
Turret::Turret()
{
	m_ptPosition = { 800, 150 };
	m_szSize = { 32, 32 };
	m_fFireTimer = 0.0f;
	m_vtDirection = { 1, 0 };
}


Turret::~Turret()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Turret::Update(float elapsedTime)
{
	if(m_fFireTimer > 0.0f)
	{
		m_fFireTimer -= elapsedTime;
	}
	if(m_fFireTimer <= 0.0f)
	{
		m_fFireTimer = 0.0f;
	}

	if(m_fFireTimer == 0.0f)
	{
		m_fFireTimer = 1.0f;
		CreateProjectileMessage* pMsg = new CreateProjectileMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;
	}

}
void Turret::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Render us with the camera
	Camera::GetInstance()->Draw(rMyRect,
		SGD::Color::Color(255, 255, 255, 0));
}

int Turret::GetType(void) const
{

	return ENT_TURRET;
}

SGD::Rectangle Turret::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void Turret::HandleCollision(const IEntity* pOther)
{

}