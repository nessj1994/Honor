#include "IceGolem.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "AnimationEngine.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "DestroyEntityMessage.h"

IceGolem::IceGolem() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	Listener::RegisterForEvent("ResetRoom");
	AnimationEngine::GetInstance()->LoadAnimation("Assets/YetiAnimations.xml");
	m_ts.ResetCurrFrame();
	m_ts.SetCurrAnimation("yetiidle");
	m_ts.SetPlaying(true);
	m_szSize = { 64, 64 };

	m_hSmashSound = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/YetiAttack.wav");
	m_hDeathSound = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/TurtleDeath.wav");
	m_hHitSound = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/TurtleHit.wav");
}


IceGolem::~IceGolem()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hDeathSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hSmashSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hHitSound);
	//SetPlayer(nullptr);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void IceGolem::Update(float elapsedTime)
{
	if (GetAlive() == false)
	{

		if (!m_bPlayedAudio)
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hHitSound);
			SGD::AudioManager::GetInstance()->PlayAudio(m_hDeathSound);
			m_bPlayedAudio = true;
		}

		SetAlive(false);
		/*DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;*/
		return;
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


	if(m_bInRange == true)
	{
		SGD::Vector vToTarget = GetPlayer()->GetPosition() - m_ptPosition;

		vToTarget.Normalize();

		SGD::Vector vUp = { 0, -1 };

		if(vUp.ComputeDotProduct(vToTarget) < 0.999)
		{
			if(vUp.ComputeSteering(vToTarget) >= 0)
			{
				m_nDirection = 0;
			}
			else
			{
				m_nDirection = 2;
			}
		}

		if(m_fDistance <= 84)
		{
			if(m_ts.IsPlaying() == false)
			{	

				m_ts.ResetCurrFrame();
				m_ts.SetPlaying(true);
			}
			if(!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hSmashSound))
			{
				SGD::AudioManager::GetInstance()->PlayAudio(m_hSmashSound);
			}
				m_vtVelocity.x = 0;
				m_ts.SetCurrAnimation("yetismash");
				m_szSize.width = 104;
		}
		else
		{
			if(m_ts.GetCurrAnimation() != "yetiwalking")
			{
				m_ts.ResetCurrFrame();
				m_ts.SetCurrAnimation("yetiwalking");
				m_szSize.width = 64;
			
			}
			
			if(m_nDirection == 2)
			{
			m_vtVelocity.x = -200;
			}
			else
			{
				m_vtVelocity.x = 200;
			}
		}
	}
	else
	{
		m_vtVelocity.x = 0;
		if(m_ts.GetCurrAnimation() != "yetiidle")
		{
			m_ts.ResetCurrFrame();
			m_ts.SetCurrAnimation("yetiidle");
			m_szSize.width = 64;

		}
	}
	m_ptPosition += m_vtVelocity * elapsedTime;


	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
	Enemy::Update(elapsedTime);

	
}
void IceGolem::Render(void)
{
	//Reset Room 
	if (!GetAlive())
	{
		return;
	}
	////Get the camera position for our offset
	//SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	////create a reference to our rectangle
	//SGD::Rectangle rMyRect = GetRect();

	////Offset our rectangle by the camera position for rendering
	//rMyRect.Offset({ -camPos.x, -camPos.y });

	//float fRotX = rMyRect.left + m_szSize.width / 2;
	//float fRotY = rMyRect.top + m_szSize.height / 2;

	//Camera::GetInstance()->Draw(rMyRect, { 255, 0, 255, 255 });

	//Render us with the camera
	if(m_bFacingRight == true)
		Camera::GetInstance()->DrawAnimation({ m_ptPosition.x + m_szSize.width / 2, m_ptPosition.y + m_szSize.height }, m_fRotation, m_ts, false, 1.0f, {});
	else
		Camera::GetInstance()->DrawAnimation({ m_ptPosition.x + m_szSize.width / 2, m_ptPosition.y + m_szSize.height }, m_fRotation, m_ts, true, 1.0f, {});

}
int IceGolem::GetType(void) const
{
	return ENT_ICE_GOLEM;
}

SGD::Rectangle IceGolem::GetRect(void) const
{
	return{ m_ptPosition, m_szSize };
}


void IceGolem::HandleCollision(const IEntity* pOther)
{


	if(pOther->GetType() == Entity::ENT_SOLID_WALL)
	{

		BasicCollision(pOther);
		SetFriction(13000.0f);
	}
	if(pOther->GetType() == Entity::ENT_LEFT_RAMP)
	{


		LeftRampCollision(pOther);
		SetFriction(13000.0f);

	}

	if(pOther->GetType() == Entity::ENT_ICE_LEFT_RAMP)
	{
		LeftRampCollision(pOther);
		SetFriction(0.1f);
	}
	if(pOther->GetType() == Entity::ENT_RIGHT_RAMP)
	{


		RightRampCollision(pOther);
		SetFriction(13000.0f);

	}
	if(pOther->GetType() == Entity::ENT_ICE_RIGHT_RAMP)
	{
		RightRampCollision(pOther);
		SetFriction(0.1f);
	}



	if(pOther->GetType() == Entity::ENT_FROZEN)
	{

		BasicCollision(pOther);
		SetFriction(1.0f);
		if(GetVelocity().x > 0 && m_bFacingRight == false)
		{
			m_vtVelocity.x -= 400;
		}
		else if(GetVelocity().x < 0 && m_bFacingRight == true)
		{
			m_vtVelocity.x += 400;
		}
		SetVelocity(GetVelocity() * 1.2f);



	}

	if(pOther->GetType() == Entity::ENT_NOT_FROZEN)
	{
		BasicCollision(pOther);
		SetFriction(13000.0f);
	}

	if(pOther->GetType() == Entity::ENT_ICE)
	{

		BasicCollision(pOther);
		SetFriction(0.0f);
		m_vtVelocity *= 1.004f;
		if(GetVelocity().x > 0 && m_bFacingRight == false)
		{
			m_vtVelocity.x -= 50;
		}
		else if(GetVelocity().x < 0 && m_bFacingRight == true)
		{
			m_vtVelocity.x += 50;
		}


	}



}


void IceGolem::LeftRampCollision(const IEntity* pOther)
{
	///////for LEFT ramp RIGHT side of player

	float tempVal = 32.0f / 32.0f;


	SetGravity(GetGravity() * 4);
	SetVelocity({ GetVelocity().x, 0 });
	if(GetVelocity().x > 300)
	{
		m_vtVelocity.x = 300;

	}
	if(GetVelocity().x < -400)
	{
		m_vtVelocity.x = -400;
	}

	RECT rPlayer;
	rPlayer.left = (LONG)GetRect().left;
	rPlayer.top = (LONG)GetRect().top;
	rPlayer.right = (LONG)GetRect().right /*- 16*/;
	rPlayer.bottom = (LONG)GetRect().bottom /*- 10*/;

	////Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	////Create a rectangle for the intersection
	RECT rIntersection = {};


	IntersectRect(&rIntersection, &rPlayer, &rObject);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;


	//SGD::Rectangle rPlayer = GetRect();
	//SGD::Rectangle rOther = pOther->GetRect();
	//SGD::Rectangle rIntersecting = rPlayer.ComputeIntersection(rObject);

	//float rIntersectWidth = rIntersecting.ComputeWidth();
	//float rIntersectHeight = rIntersecting.ComputeHeight();


	//float tempInt = (/*(rObject.right - rObject.left) +*/ nIntersectWidth)* tempVal;

	//	if (is_Platform == false
	//		&& rPlayer.bottom < rObject.top)
	//	{

	//SetVelocity({ GetVelocity().x + 1000, GetVelocity().y - 1000 });






	if(/*nIntersectWidth*/ /*nIntersectWidth > 1  &&*/ nIntersectWidth < 31)
	{
		//SetPosition({ GetPosition().x, (float)rObject.bottom - tempVal - GetSize().height });
		m_ptPosition.y = (float)rObject.bottom - tempVal - GetSize().height;
		m_ptPosition.y = m_ptPosition.y - (nIntersectWidth * tempVal);
	}

	else if(nIntersectWidth == 31)
	{
		//m_ptPosition.y = (float)rObject.bottom - tempVal - GetSize().height;
		//m_ptPosition.y = m_ptPosition.y - (nIntersectWidth * tempVal);
		tempVal = 31 / 32;

		m_ptPosition.y = (float)rObject.bottom - GetSize().height - tempVal;
		m_ptPosition.y = m_ptPosition.y - (nIntersectWidth * 1);

		BasicCollision(pOther);


	}
	else if(nIntersectWidth == 32)
	{
		BasicCollision(pOther);

		m_ptPosition.x += 1;
	}


	//else
	//{
	//	SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 });
	//	m_ptPosition.y -= nIntersectHeight;
	//}



	//	}

	if(m_ptPosition.x + m_szSize.width > rObject.right)
		SetVelocity({ m_vtVelocity.x, 1000 });




}

void IceGolem::RightRampCollision(const IEntity* pOther)
{
	///////for RIGHT ramp LEFT side of player

	float tempVal = 32.0f / 32.0f;


	SetGravity(GetGravity() * 4);
	SetVelocity({ GetVelocity().x, 0 });
	if(GetVelocity().x > 300)
	{
		m_vtVelocity.x = 300;

	}
	if(GetVelocity().x < -400)
	{
		m_vtVelocity.x = -400;
	}


	//SetGravity(0);

	RECT rPlayer;
	rPlayer.left = (LONG)GetRect().left;
	rPlayer.top = (LONG)GetRect().top;
	rPlayer.right = (LONG)GetRect().right /*- 16*/;
	rPlayer.bottom = (LONG)GetRect().bottom /*- 10*/;

	//Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	//Create a rectangle for the intersection
	RECT rIntersection = {};


	IntersectRect(&rIntersection, &rPlayer, &rObject);


	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;



	float tempInt = nIntersectWidth * tempVal;



	/*SGD::Rectangle rPlayer = GetRect();
	SGD::Rectangle rOther = pOther->GetRect();
	SGD::Rectangle rIntersecting = rPlayer.ComputeIntersection(rOther);*/

	//float rIntersectWidth = rIntersection.ComputeWidth();
	//float rIntersectHeight = rIntersection.ComputeHeight();


	if(/*nIntersectWidth*/ nIntersectWidth > -31)
	{
		tempVal = 31 / 32;

		m_ptPosition.y = (float)rObject.bottom - GetSize().height - tempVal;
		m_ptPosition.y = m_ptPosition.y - (nIntersectWidth * 1);
	}
	//else
	//{
	//	//SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 });
	//	m_ptPosition.y -= rIntersectHeight;
	//}


	if(m_ptPosition.x + m_szSize.width < rObject.left)
		SetVelocity({ m_vtVelocity.x, 1000 });








}

void IceGolem::BasicCollision(const IEntity* pOther)
{
	SGD::Rectangle rMyRect = GetRect();
	SGD::Rectangle rOther = pOther->GetRect();

	SGD::Rectangle rIntersect = rMyRect.ComputeIntersection(rOther);

	SetGravity(-3000);


	//Colliding with top or bottom
	if(rIntersect.ComputeWidth() > rIntersect.ComputeHeight())
	{
		if(rMyRect.bottom == rIntersect.bottom)
		{

			SetVelocity({ GetVelocity().x, 0 });
			SetPosition({ GetPosition().x, rOther.top - m_szSize.height + 1 });
		}
	}
	//Colliding with side
	else if(rIntersect.ComputeHeight() > rIntersect.ComputeWidth())
	{
		if(rMyRect.right == rIntersect.right)
		{
			SetVelocity({ 0, GetVelocity().y });
			SetPosition({ rOther.left - m_szSize.width, GetPosition().y });

		}
		else
		{
			SetVelocity({ 0, GetVelocity().y });
			SetPosition({ rOther.right, GetPosition().y });
		}
	}
}
////////////////////////////////////////////////
////////////// Listener Interface //////////////
void IceGolem::HandleEvent(const SGD::Event* pEvent)
{
	if(pEvent->GetEventID() == "ASSESS_PLAYER_RANGE")
	{
		Entity* pPlayer = reinterpret_cast<Entity*>(pEvent->GetSender());

		SGD::Vector vToThreat = pPlayer->GetPosition() - m_ptPosition;
		if(vToThreat.ComputeLength() < 200 && vToThreat.ComputeLength() > 30)
		{
			m_bInRange = true;
			m_fDistance = vToThreat.ComputeLength();

		
		}
		else
		{
			m_bInRange = false;
		}
	}
	if (pEvent->GetEventID() == "ResetRoom")
	{
		SetAlive(true);
		SetPosition(GetOriginalPos());
		m_bPlayedAudio = false;
	}

}


