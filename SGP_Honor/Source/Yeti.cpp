#include "Yeti.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "CreateSprayMessage.h"
#include "../SGD Wrappers/SGD_Event.h"
#include <cmath>

Yeti::Yeti() : Listener(this)
{
	Listener::RegisterForEvent("JUMP_TIME");
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	AnimationEngine::GetInstance()->LoadAnimation("Assets/YetiAnimations.xml");
	m_ts.ResetCurrFrame();
	m_ts.SetCurrAnimation("yetispecial");
	m_ts.SetPlaying(true);
	m_szSize = { 64, 128 };
	SetStartPosition(m_ptPosition);
	SetCurrentState(CHASING_STATE);

}


Yeti::~Yeti()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Yeti::Update(float elapsedTime)
{
	switch(GetCurrentState())
	{
	case CHASING_STATE:
	{
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
						  break;
	}
	case MELEE_STATE:
	{
						//Commence melee attack
						m_ts.SetCurrAnimation("yetismash");

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
			

						  if(m_fJumpTimer > 0.2f)
						  {
							  SetVelocity({ GetVelocity().x, GetVelocity().y + (2200 * elapsedTime) });
						/*	  if(GetVelocity().y > 0)
							  {
								  SetVelocity({ GetVelocity().x, 0 });

							  }*/

						  }
						  else
							  SetVelocity({ GetVelocity().x, -600 });
							
						  break;
	}
	case SPRAYING_STATE:
	{
						   //Spraying Blocks
						   CreateSprayMessage* pMsg = new CreateSprayMessage(this);
						   pMsg->QueueMessage();
						   pMsg = nullptr;
						   break;
	}
	case BLIZZARD_STATE:
	{
						   break;
	}
	case DEAD_STATE:
	{
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
	m_ptPosition += GetVelocity() * elapsedTime;
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
}
void Yeti::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	Camera::GetInstance()->Draw(rMyRect, SGD::Color(255, 255, 0, 0));

	//Render us with the camera
	if(m_bFacingRight == true)
		Camera::GetInstance()->DrawAnimation({ m_ptPosition.x, m_ptPosition.y + m_szSize.height}, 0.0f, m_ts, false, 1.0f, {});
	else
		Camera::GetInstance()->DrawAnimation(m_ptPosition, 0.0f, m_ts, true, 1.0f, {});



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

}
void Yeti::HandleCollision(const IEntity* pOther)
{
	SGD::Rectangle rMyRect = GetRect();
	SGD::Rectangle rOther = pOther->GetRect();

	SGD::Rectangle rIntersect = rMyRect.ComputeIntersection(rOther);

	//Colliding with top or bottom
	if(rIntersect.ComputeWidth() > rIntersect.ComputeHeight())
	{
		if(rMyRect.right == rIntersect.right)
		{
			SetVelocity({ GetVelocity().x, 0 });
			SetPosition({ GetPosition().x, rOther.top - m_szSize.height + 1 });
		}
	}
	//Colliding with side
	else
	{

	}
}

void Yeti::HandleEvent(const SGD::Event* pEvent)
{
	if(pEvent->GetEventID() == "JUMP_TIME")
	{
		SetCurrentState(JUMPING_STATE);
		m_fJumpTimer = 0.0f;
	}
	if(pEvent->GetEventID() == "ASSESS_PLAYER_RANGE")
	{
		Entity* pPlayer = reinterpret_cast<Entity*>(pEvent->GetSender());
		
		if(GetPosition().x - (pPlayer->GetPosition().x + pPlayer->GetSize().width) <= m_fAggroRange || (GetPosition().x + m_szSize.width) - pPlayer->GetPosition().x <= m_fAggroRange)
		{
			m_bInRange = true;
			m_fDistance = GetPosition().x - pPlayer->GetPosition().x;
		}
		else
		{
			m_fDistance = GetPosition().x - pPlayer->GetPosition().x;

			m_bInRange = false;
		}
	}
}
