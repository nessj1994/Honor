#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Yeti.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "CreateSprayMessage.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include <cmath>
#include "Player.h"


Yeti::Yeti() : Listener(this)
{
	Listener::RegisterForEvent("JUMP_TIME");
	Listener::RegisterForEvent("LONG_JUMP");
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	Listener::RegisterForEvent("KILL_BOSS");
	Listener::RegisterForEvent("FROST_SPRAY");
	Listener::RegisterForEvent("END_SPRAY");
	Listener::RegisterForEvent("ResetRoom");

	AnimationEngine::GetInstance()->LoadAnimation("Assets/YetiAnimations.xml");
	m_ts.ResetCurrFrame();
	m_ts.SetCurrAnimation("yetiidle");
	m_ts.SetPlaying(true);
	m_szSize = { 128, 128 };
	SetStartPosition(m_ptPosition);
	SetCurrentState(CHASING_STATE);

	// Sound
	m_hGrunt1 = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Grunt4.wav");
	m_hGrunt2 = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Grunt5.wav");
	m_hGrunt3 = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Grunt6.wav");
	m_hJump = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Jump.wav");
	m_hLand = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Land.wav");
	m_hSlam = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Slam1.wav");
}


Yeti::~Yeti()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hGrunt1);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hGrunt2);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hGrunt3);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hJump);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hLand);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hSlam);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Yeti::Update(float elapsedTime)
{
	switch(GetCurrentState())
	{
	case CHASING_STATE:
	{
						  m_fStartTimer -= elapsedTime;
						  if(m_fStartTimer <= 0.0f)
						  {


							  if(GetPlayer()->GetRect().right < m_ptPosition.x)
							  {
								  m_bFacingRight = false;
							  }
							  else
							  {
								  m_bFacingRight = true;
							  }
							  if(!m_ts.IsPlaying() == false)
							  {
								  m_ts.SetPlaying(true);
							  }
							  //Use the walking animation
							  if(m_ts.GetCurrAnimation() != "yetiwalking")
							  {
								  m_ts.ResetCurrFrame();
								  m_ts.SetCurrAnimation("yetiwalking");
							  }

							  //is the player in range of an attack
							  if(m_bInRange)
							  {
								  //melee attack?
								  if(fabs(m_fDistance) <= m_fMeleeRange)
								  {
									  SGD::AudioManager::GetInstance()->PlayAudio(m_hSlam);
									  m_ts.ResetCurrFrame();
									  m_ts.SetCurrAnimation("yetismash");
									  SetCurrentState(MELEE_STATE);

								  }
								  //ranged attack?
								  else
								  {
									  //m_ts.ResetCurrFrame();
									  // m_ts.SetCurrAnimation("yetiranged");
									  //SetCurrentState(RANGED_STATE);

								  }
							  }



							  if(m_vtVelocity.x < 500)
							  {
								  m_vtVelocity.x = 500;
							  }

						  }
						  break;
	}
	case MELEE_STATE:
	{
						//Commence melee attack

						if(fabs(m_fDistance) > m_fMeleeRange)
						{
							SetCurrentState(CHASING_STATE);
						}

						if(!m_ts.IsPlaying())
						{
							m_ts.SetPlaying(true);
							SetCurrentState(CHASING_STATE);
						}

						break;
	}
	case RANGED_STATE:
	{
						 //commence ranged attack
						 break;
	}
	case JUMPING_STATE:
	{
						  m_ts.ResetCurrFrame();
						  m_ts.SetCurrAnimation("yetijump");

						  if(m_fJumpTimer > 0.2f)
						  {
							  SetVelocity({ GetVelocity().x, GetVelocity().y + (2200 * elapsedTime) });
							  /*	  if(GetVelocity().y > 0)
									{
									SetVelocity({ GetVelocity().x, 0 });

									}*/

						  }
						  else
							  SetVelocity({ GetVelocity().x, GetVelocity().y });

						  break;
	}
	case SPRAYING_STATE:
	{
						   //Spraying Blocks
						   m_fStartTimer -= elapsedTime;
						   if(m_fStartTimer <= 0.0f)
						   {
							   if(m_bSpraying == true)
							   {
								   CreateSprayMessage* pMsg = new CreateSprayMessage(this);
								   pMsg->QueueMessage();
								   pMsg = nullptr;
							   }

							   if(m_vtVelocity.x < 500)
							   {
								   m_vtVelocity.x = 500;
							   }
						   }
						   break;
	}
	case BLIZZARD_STATE:
	{
						   break;
	}
	case DEAD_STATE:
	{
					   m_ts.ResetCurrFrame();
					   m_ts.SetCurrAnimation("yetideath");
					   break;
	}
	default:
		break;
	}

	if(m_fJumpTimer < 0.2)
	{
		m_fJumpTimer += elapsedTime;
	}
	else
	{
		m_fJumpTimer = 0.3f;
	}

	if(GetCurrentState() != JUMPING_STATE)
	{
		SetGravity(-1500);
		SetVelocity({ GetVelocity().x, GetVelocity().y - GetGravity() * elapsedTime });
	}

	m_ptPosition += GetVelocity() * elapsedTime;
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
}
void Yeti::Render(void)
{
	////Get the camera position for our offset
	//SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	////create a reference to our rectangle
	//SGD::Rectangle rMyRect = GetRect();

	////Offset our rectangle by the camera position for rendering
	//rMyRect.Offset({ -camPos.x, -camPos.y });

	//Camera::GetInstance()->Draw(rMyRect, SGD::Color(255, 255, 0, 0));

	//Render us with the camera
	if(m_bFacingRight == true)
		Camera::GetInstance()->DrawAnimation({ m_ptPosition.x + 64, m_ptPosition.y + m_szSize.height }, 0.0f, m_ts, false, 1.0f, {});
	else
		Camera::GetInstance()->DrawAnimation({ m_ptPosition.x + 64, m_ptPosition.y + m_szSize.height }, 0.0f, m_ts, true, 1.0, {});



}
int Yeti::GetType(void) const
{
	return ENT_BOSS_YETI;
}

SGD::Rectangle Yeti::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void Yeti::BasicCollision(const IEntity* pOther)
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
			if(GetVelocity().y != 0 && GetCurrentState() != MELEE_STATE)
			{
				SetCurrentState(CHASING_STATE);
			}
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
void Yeti::HandleCollision(const IEntity* pOther)
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

void Yeti::HandleEvent(const SGD::Event* pEvent)
{
	if(pEvent->GetSender() == this)
	{
		if(pEvent->GetEventID() == "JUMP_TIME")
		{
			if(GetCurrentState() != JUMPING_STATE)
			{
				SetCurrentState(JUMPING_STATE);
				m_vtVelocity = { 400, -700 };
				m_fJumpTimer = 0.0f;
			}
		}
		if(pEvent->GetEventID() == "LONG_JUMP")
		{

			SetCurrentState(JUMPING_STATE);
			m_vtVelocity = { 700, -1000 };
			m_fJumpTimer = 0.0f;

		}

		if(pEvent->GetEventID() == "KILL_BOSS")
		{
			SetCurrentState(DEAD_STATE);
		}

		if(pEvent->GetEventID() == "FROST_SPRAY")
		{
			m_bSpraying = true;
			SetCurrentState(SPRAYING_STATE);

		}

		if(pEvent->GetEventID() == "END_SPRAY")
		{
			m_bSpraying = false;
			SetCurrentState(CHASING_STATE);
		}
	}
	//if(pEvent->GetSender() == this)
	//{
	//	if(pEvent->GetEventID() == "LONG_JUMP")
	//	{

	//			SetCurrentState(JUMPING_STATE);
	//			m_vtVelocity = { 700, -1000 };
	//			m_fJumpTimer = 0.0f;

	//	}
	//}
	if(pEvent->GetEventID() == "ASSESS_PLAYER_RANGE")
	{
		Entity* pPlayer = reinterpret_cast<Entity*>(pEvent->GetSender());

		if(fabs(pPlayer->GetPosition().x - (GetPosition().x + m_szSize.width)) <= m_fAggroRange)
		{
			m_bInRange = true;
			m_fDistance = pPlayer->GetPosition().x - (GetPosition().x + m_szSize.width);
		}
		else if((pPlayer->GetPosition().x + pPlayer->GetSize().width) < GetPosition().x)
		{
			SGD::Event Event = { "KILL_PLAYER", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);
		}

		else
		{
			m_fDistance = GetPosition().x - pPlayer->GetPosition().x;

			m_bInRange = false;
		}
	}

	if(pEvent->GetEventID() == "ResetRoom")
	{
		m_ptPosition = GetStartPosition();
		SetCurrentState(CHASING_STATE);
		m_fStartTimer = 2.0f;
		m_bFacingRight = true;

	}


}


void Yeti::LeftRampCollision(const IEntity* pOther)
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

void Yeti::RightRampCollision(const IEntity* pOther)
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