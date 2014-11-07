#include "IceTurtle.h"

#include "CreateProjectileMessage.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "DestroyEntityMessage.h"

IceTurtle::IceTurtle()
{

	m_szSize = { 32, 32 };
	m_fFireTimer = 0.0f;

	AnimationEngine::GetInstance()->LoadAnimation("Assets/TurtleAnimations.xml");
	m_ts.SetCurrAnimation("turtleidle");
	m_bFacingRight = false;
	m_nDirection = 2;

	m_hShootSound = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/ShootSound.wav");
	m_hDeathSound = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/TurtleDeath.wav");
	m_hHitSound = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/TurtleHit.wav");

}


IceTurtle::~IceTurtle()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hDeathSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hShootSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hHitSound);

	//SetPlayer(nullptr);

}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void IceTurtle::Update(float elapsedTime)
{
	if(m_fFireTimer > 0.0f)
	{
		m_fFireTimer -= elapsedTime;
	}
	if(m_fFireTimer <= 0.0f)
	{
		m_fFireTimer = 0.0f;
		m_ts.SetCurrAnimation("turtlefire");
		m_ts.SetPlaying(true);
	}

	if(m_fFireTimer == 0.0f)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hShootSound);

		m_fFireTimer = 1.3f;
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


	}
	//down
	else if(m_nDirection == 3)
	{
		m_vtDirection = { 0, 1 };
		m_bFacingRight = false;
		m_fRotation = 30.0f;

	}
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
	Enemy::Update(elapsedTime);

	if(GetAlive() == false)
	{

		SGD::AudioManager::GetInstance()->PlayAudio(m_hHitSound);

		SGD::AudioManager::GetInstance()->PlayAudio(m_hDeathSound);
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}
void IceTurtle::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	float fRotX = rMyRect.left + m_szSize.width / 2;
	float fRotY = rMyRect.top + m_szSize.height / 2;

	//Render us with the camera
	if(m_bFacingRight == true)
		Camera::GetInstance()->DrawAnimation({ m_ptPosition.x + 16, m_ptPosition.y + m_szSize.height }, m_fRotation, m_ts, false, 1.0f, {});
	else
		Camera::GetInstance()->DrawAnimation({ m_ptPosition.x + 16, m_ptPosition.y + m_szSize.height }, m_fRotation, m_ts, true, 1.0f, {});

	//Camera::GetInstance()->Draw(rMyRect, { 255, 0, 255, 255 });


}

int IceTurtle::GetType(void) const
{

	return ENT_ICE_TURTLE;
}

SGD::Rectangle IceTurtle::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void IceTurtle::HandleCollision(const IEntity* pOther)
{

}