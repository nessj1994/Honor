#include "BullEnemy.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "Player.h"

///////////////////////////////
// Ctor/Dtor
BullEnemy::BullEnemy() : Listener(this)
{
	Listener::RegisterForEvent("TurnMarker");
	m_szSize = { 64, 48 };
	m_bsCurrState = BS_RUNNING;
	m_fChangeTimer = 0.0f;
	SetFacingRight(true);
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Enemy_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Enemy_Running");
}

BullEnemy::~BullEnemy()
{

}

///////////////////////////////
// Update
// -Main update loop
void BullEnemy::Update(float elapsedTime)
{
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);

	// Update timer
	if (m_fTurnTimer > 0.0f)
	{
		m_fTurnTimer -= elapsedTime;
	}

	// Check if dying
	if (GetDead() && !m_bDying)
	{
		m_bDying = true;
		m_fDeathTimer = 0.5f;
		m_bsCurrState = BS_DEATH;
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
			// Update animation
			m_ts.SetCurrAnimation("Bull_Enemy_Idle");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(1.0f);

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
			// Update animation
			m_ts.SetCurrAnimation("Bull_Enemy_Running");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(1.0f);

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
			// Update animation
			m_ts.SetCurrAnimation("Bull_Enemy_Running");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(2.0f);

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
			// Update animation
			m_ts.SetCurrAnimation("Bull_Enemy_Death");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(2.0f);

			// Stand still
			SetVelocity({ 0.0f, m_vtVelocity.y });

			// Timer to death
			m_fDeathTimer -= elapsedTime;
			if (m_fDeathTimer <= 0.0f)
			{
				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}

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
	SGD::Point newPosition = m_ptPosition;
	newPosition.y -= 12;
	newPosition.x += 16;
	Camera::GetInstance()->DrawAnimation(newPosition, 0, m_ts, !GetFacingRight(), 1.0f);
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

///////////////////////////////
// GetAttacking
// -Returns if this bull is attacking
bool BullEnemy::GetAttacking()
{
	return m_bsCurrState == BS_RUNNING;
}