#include "BullEnemy.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "Player.h"

///////////////////////////////
// Ctor/Dtor
BullEnemy::BullEnemy() : Listener(this)
{
	Listener::RegisterForEvent("TurnMarker");
	m_szSize = { 64, 64 };
	m_bsCurrState = BS_RUNNING;
	m_fChangeTimer = 0.0f;
	SetFacingRight(true);
}

BullEnemy::~BullEnemy()
{

}

///////////////////////////////
// Update
// -Main update loop
void BullEnemy::Update(float elapsedTime)
{
	// Update timer
	if (m_fTurnTimer > 0.0f)
	{
		m_fTurnTimer -= elapsedTime;
	}

	// Every few seconds randomly change direction or switch states
	// Only if the bull is not in the middle of charging
	if (m_bsCurrState != BS_RUNNING)
	{
		m_fChangeTimer -= elapsedTime;
		if (m_fChangeTimer <= 0.0f)
		{
			m_fChangeTimer = rand() % 3 + 2.0f;
			switch (rand() % 3)
			{
				case 0:
				{
					m_bsCurrState = BS_IDLE;
					break;
				}
				case 1:
				{
					m_bsCurrState = BS_WALKING;
				}
				case 2:
				{
					SetFacingRight(!GetFacingRight());
				}
			}
		}
	}

	// Switch case for each state
	switch (m_bsCurrState)
	{
		case BS_IDLE:
		{
			// Stand still
			SetVelocity({ 0.0f, m_vtVelocity.y });

			// Charge at the player if he is in site
			float playerX = GetPlayer()->GetPosition().x;
			if (abs(playerX - m_ptPosition.x + 32) < 256.0f)
			{
				if (GetFacingRight() && playerX > m_ptPosition.x + 32)
				{
					m_bsCurrState = BS_RUNNING;
				}
				else if (!GetFacingRight() && playerX < m_ptPosition.x + 32)
				{
					m_bsCurrState = BS_RUNNING;
				}
			}

			break;
		}
		case BS_WALKING:
		{
			// walk left or right
			if (GetFacingRight())
			{
				// Update velocity
				SetVelocity({ m_fRunSpeed, m_vtVelocity.y });
			}
			else
			{
				// Update velocity
				SetVelocity({ -m_fRunSpeed, m_vtVelocity.y });
			}

			// Charge at the player if he is in site
			float playerX = GetPlayer()->GetPosition().x;
			if (abs(playerX - m_ptPosition.x + 32) < 256.0f)
			{
				if (GetFacingRight() && playerX > m_ptPosition.x + 32)
				{
					m_bsCurrState = BS_RUNNING;
				}
				else if (!GetFacingRight() && playerX < m_ptPosition.x + 32)
				{
					m_bsCurrState = BS_RUNNING;
				}
			}

			break;
		}
		case BS_RUNNING:
		{
			// Run left or right
			if (GetFacingRight())
			{
				// Update velocity
				SetVelocity({ m_fRunSpeed * 2, m_vtVelocity.y });
			}
			else
			{
				// Update velocity
				SetVelocity({ -m_fRunSpeed * 2, m_vtVelocity.y });
			}

			// Stop charging at the player if he is not in site
			float playerX = GetPlayer()->GetPosition().x;
			if (abs(playerX - m_ptPosition.x + 32) > 256.0f ||
				GetFacingRight() && playerX < m_ptPosition.x + 32 ||
				!GetFacingRight() && playerX > m_ptPosition.x + 32)
			{
				m_bsCurrState = BS_WALKING;
			}

			break;
		}
		case BS_DEATH:
		{
			// Stand still
			SetVelocity({ 0.0f, m_vtVelocity.y });

			break;
		}
	}

	Enemy::Update(elapsedTime);
}

///////////////////////////////
// Render
// -Main render loop
void BullEnemy::Render()
{
	Enemy::Render();
}

///////////////////////////////
// HandleCollision
// -Handles collision between entities
void BullEnemy::HandleCollision(const IEntity * pOther)
{
	if (pOther->GetType() == ENT_SOLID_WALL)
	{
		BasicCollision(pOther);
	}

	if (pOther->GetType() == ENT_DOOR)
	{
		BasicCollision(pOther);
	}
}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void BullEnemy::HandleEvent(const SGD::Event* pEvent)
{
	//which event

	//Turn around
	if (pEvent->GetEventID() == "TurnMarker" &&
		pEvent->GetSender() == this)
	{
		if (m_fTurnTimer <= 0.0f)
		{
			SetFacingRight(!GetFacingRight());
			m_fTurnTimer = 1.0f;
		}
	}
}