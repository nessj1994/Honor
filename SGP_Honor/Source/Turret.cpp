#include "Turret.h"
#include "CreateProjectileMessage.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "AnimationEngine.h"
#include "Camera.h"
Turret::Turret()
{
	m_ptPosition = { 800, 150 };
	m_szSize = { 32, 32 };
	m_fFireTimer = 0.0f;
	m_vtDirection = { 1, 0 };
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"Assets/Graphics/Turret.png");
	AnimationEngine::GetInstance()->LoadAnimation("Assets/TurretAnimations.xml");
	m_ts.SetCurrAnimation("turretidle");
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
		m_ts.SetCurrAnimation("turretfire");
		m_ts.SetPlaying(true);
	}

	if(m_fFireTimer == 0.0f)
	{

		m_fFireTimer = 1.65f;
		CreateProjectileMessage* pMsg = new CreateProjectileMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;

	
	}
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
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
	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0.0f, m_ts, false);
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