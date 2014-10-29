#include "FallingBlock.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"

FallingBlock::FallingBlock() : Listener(this)
{
	//Listen for the player to be in range
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"Assets/Graphics/FallingBlock.png");
}


FallingBlock::~FallingBlock()
{
	SetTarget(nullptr);
}

/////////////////////////////////////////////////
///////////////// Interface ////////////////////
void FallingBlock::Update(float elapsedTime)
{
	//If we have a target and were in resting state
	if(m_pTarget != nullptr && m_unCurrentState == RESTING_STATE)
	{
		//Set our state to falling
		m_unCurrentState = FALLING_STATE;
	}
	//if we have a target
	else if(m_pTarget != nullptr)
	{
		//is out target still under us
		if(m_pTarget->GetPosition().x > m_ptPosition.x + m_szSize.width || m_pTarget->GetPosition().x + m_pTarget->GetSize().width < m_ptPosition.x)
		{
			//If not remove the target
			SetTarget(nullptr);
		}
	}

	//If were falling
	if(m_unCurrentState == FALLING_STATE)
	{
		//set our velocity's y value to our falling speed
		m_vtVelocity.y = m_fFallSpeed;
	}
	//If were rising
	else if(m_unCurrentState == RISING_STATE)
	{
		//Set our velocity's y value to our rising speed
		m_vtVelocity.y -= m_fRiseSpeed;
	}
	//If we are resting
	else if(m_unCurrentState == RESTING_STATE)
	{
		//Set our velocity's y value back to 0
		m_vtVelocity.y = 0;
	}
	//Is our y position is less than or equal to our starting y 
	if(m_ptPosition.y <= m_ptStartPosition.y)
	{
		//Lock our position back to the starting position
		m_ptPosition = m_ptStartPosition;

		//Set our state to resting
		m_unCurrentState = RESTING_STATE;

	}

	//Move based on velocity and elapsed time
	m_ptPosition += m_vtVelocity  * elapsedTime;

}
void FallingBlock::Render(void)
{
	
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();
	
	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({-camPos.x, -camPos.y});

	//Render us with the camera
	//Camera::GetInstance()->Draw(rMyRect,
	//	SGD::Color::Color(255, 0, 255, 0));
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0.0f, m_hImage, false, 1.0f, {}, {});


}

int FallingBlock::GetType(void) const
{
	//Return our type
	return ENT_FALLING_BLOCK;
}

SGD::Rectangle FallingBlock::GetRect(void) const
{
	//Return the rectangle of the object
	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void FallingBlock::HandleCollision(const IEntity* pOther)
{

	//Did the block collide with the player
	if(pOther->GetType() == ENT_PLAYER)
	{
		//if so move back up but kill the player
		m_unCurrentState = RISING_STATE;
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
		
	}
	else if(pOther->GetType() == Entity::ENT_SOLID_WALL)
	{
		m_unCurrentState = RISING_STATE;
	}


}

void FallingBlock::HandleEvent(const SGD::Event* pEvent)
{
	//Is the player beneath us
	if(pEvent->GetEventID() == "ASSESS_PLAYER_RANGE")
	{
		//If our target is null
		if(m_pTarget == nullptr)
		{
			//create a local reference to the events owner so we can target it
			Entity* pPlayer = reinterpret_cast<Entity*>(pEvent->GetSender());

			//Is the player beneath us
			if(pPlayer->GetPosition().x + pPlayer->GetSize().width >= m_ptPosition.x && pPlayer->GetPosition().x <= m_ptPosition.x + m_szSize.width)
			{
				//if so target them
				SetTarget(pPlayer);
			}
		}
	}
}
