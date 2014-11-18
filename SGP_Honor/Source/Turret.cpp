#include "Turret.h"
#include "CreateProjectileMessage.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
Turret::Turret()
{
	//m_ptPosition = { 800, 150 };
	m_szSize = { 32, 32 };
	m_fFireTimer = 0.0f;
	
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"Assets/Graphics/Turret.png");
	AnimationEngine::GetInstance()->LoadAnimation("Assets/TurretAnimations.xml");
	m_ts.SetCurrAnimation("turretidle");
	m_hShootSound = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/ShootSound.wav");
}


Turret::~Turret()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hShootSound);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Turret::Update(float elapsedTime)
{
	if (m_fShootTimer > 0.0f)
	{
		m_fShootTimer -= elapsedTime;
	}

	if (m_fShootTimer <= 0.0f)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hShootSound);
		m_ts.SetCurrAnimation("turretfire");
		m_ts.SetPlaying(true);
		m_fShootTimer = m_fFireTimer;
		CreateProjectileMessage* pMsg = new CreateProjectileMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;
	}

	//Right
	if(m_nDirection == 0)
	{
		m_vtDirection = { 1, 0 };
		//m_fRotation = 45.0f;
		m_bFacingRight = true;
	}
	//Left
	else if(m_nDirection == 2)
	{
		m_vtDirection = { -1, 0 };
		m_bFacingRight = false;
	}
	//up
	else if(m_nDirection == 1)
	{
		m_vtDirection = { 0, -1 };
		m_bFacingRight = true;
		m_fRotation = 30.0f;

	}
	//down
	else if(m_nDirection == 3)
	{
		m_vtDirection = { 0, 1 };
		SetDirection({ 0, 1 });
		m_bFacingRight = false;
		m_fRotation = 30.0f;
		
	}
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
}
void Turret::Render(void)
{
	////Get the camera position for our offset
	//SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	////create a reference to our rectangle
	//SGD::Rectangle rMyRect = GetRect();

	////Offset our rectangle by the camera position for rendering
	//rMyRect.Offset({ -camPos.x, -camPos.y });

	//float fRotX = rMyRect.left + m_szSize.width / 2;
	//float fRotY = rMyRect.top + m_szSize.height / 2;

	//Render us with the camera
	if(m_bFacingRight == true)
		Camera::GetInstance()->DrawAnimation(m_ptPosition, m_fRotation, m_ts, false, 1.0f, {});
	else
		Camera::GetInstance()->DrawAnimation(m_ptPosition, m_fRotation, m_ts, true, 1.0f, {});

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