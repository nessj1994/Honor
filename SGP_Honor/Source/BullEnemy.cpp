#include "BullEnemy.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Player.h"

///////////////////////////////
// Ctor/Dtor
BullEnemy::BullEnemy() : Listener(this)
{
	Listener::RegisterForEvent("TurnMarker");
	Listener::RegisterForEvent("ResetRoom");
	m_szSize = { 64, 48 };
	m_bsCurrState = BS_RUNNING;
	m_fChangeTimer = 0.0f;
	SetFacingRight(true);
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Enemy_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Enemy_Running");
	m_hWalking = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Bull_Walking.wav");
	m_hRunning = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Bull_Running.wav");
	m_hRoar1 = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Bull_Roar1.wav");
	m_hRoar2 = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Bull_Roar3.wav");
	m_fResetTimer = 1;
}

BullEnemy::~BullEnemy()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hWalking);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hRunning);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hRoar1);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hRoar2);
}

///////////////////////////////
// Update
// -Main update loop
void BullEnemy::Update(float elapsedTime)
{
	//For Reset Bug
	if (m_fResetTimer > .5f)
	{
		m_ptPosition = GetOriginalPos();
	}
	m_fResetTimer -= elapsedTime;
	//
	m_unPrevFrame = m_ts.GetCurrFrame();
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	// only play audio if the player is close
	float playerDeltaX = abs(GetPlayer()->GetPosition().x - m_ptPosition.x);
	float playerDeltaY = abs(GetPlayer()->GetPosition().y - m_ptPosition.y);
	m_bPlayAudio = (playerDeltaX < 600.0f && playerDeltaY < 400.0f);

	// Update timer
	if (m_fTurnTimer > 0.0f)
	{
		m_fTurnTimer -= elapsedTime;
	}

	// Check if dying
	if (!GetAlive() && !m_bDying)
	{
		m_bDying = true;
		m_fDeathTimer = 0.5f;
		m_bsCurrState = BS_DEATH;
		m_ts.ResetCurrFrame();
		if (m_bPlayAudio)
		{
			pAudio->PlayAudio(m_hRoar2);
		}
	}
	// Every few seconds randomly change direction or switch states
	// Only if the bull is not in the middle of charging
	if (m_bsCurrState != BS_RUNNING && m_bsCurrState != BS_DEATH)
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
					break;
				}
				case 2:
				{
					if (m_fTurnTimer > 0.0f)
					{
						SetFacingRight(!GetFacingRight());
					}
					break;
				}
			}
		}
	}

	// Switch case for each state
	switch (m_bsCurrState)
	{
		case BS_IDLE:
		{
						//For Reset Room
						if (!GetAlive())
						{
							return;
						}
			// Update animation
			m_ts.SetCurrAnimation("Bull_Enemy_Idle");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(1.0f);

			// Play sounds
			if (m_bPlayAudio && m_ts.GetCurrFrame() == 0 && m_unPrevFrame != 0 &&
				!pAudio->IsAudioPlaying(m_hRoar1))
			{
				pAudio->PlayAudio(m_hRoar1);
			}

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
						   //For Reset Room
						   if (!GetAlive())
						   {
							   return;
						   }
			// Update animation
			m_ts.SetCurrAnimation("Bull_Enemy_Running");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(1.0f);

			// Play sounds
			if (m_bPlayAudio && m_ts.GetCurrFrame() == 0 && m_unPrevFrame != 0)
			{
				pAudio->PlayAudio(m_hWalking);
			}

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
			//For Reset Room
			 if (!GetAlive())
			 {
			   return;
			 }
			// Update animation
			m_ts.SetCurrAnimation("Bull_Enemy_Running");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(2.0f);

			// Play sounds
			if (m_bPlayAudio && m_ts.GetCurrFrame() == 0 && m_unPrevFrame != 0)
			{
				pAudio->PlayAudio(m_hRunning);
			}

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
			m_ts.SetSpeed(1.0f);

			// Stand still
			SetVelocity({ 0.0f, m_vtVelocity.y });

			// Timer to death
			m_fDeathTimer -= elapsedTime;
			if (m_fDeathTimer <= 0.0f)
			{
				//Reseting Enemys
				SetAlive(false);
				/*DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;*/
			}

			break;
		}
	}

	// Apply gravity
	if (GetFalling() == true)
	{
		SetGravity(-3000);
		SetVelocity({ GetVelocity().x, GetVelocity().y - GetGravity() * elapsedTime });
	}

	Enemy::Update(elapsedTime);
}

///////////////////////////////
// Render
// -Main render loop
void BullEnemy::Render()
{
	if (!GetAlive() && m_fDeathTimer <= 0.0f)
	{
		return;
	}
	Enemy::Render();
	SGD::Point newPosition = m_ptPosition;
	newPosition.y += 48;
	newPosition.x += 32;
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
	if (pOther->GetType() == ENT_LASER)
	{
		if (m_bsCurrState == BS_DEATH)
		{
			return;
		}
		m_bDying = true;
		m_fDeathTimer = 0.5f;
		m_bsCurrState = BS_DEATH;
		m_ts.ResetCurrFrame();
		
	}
	Enemy::HandleCollision(pOther);
}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void BullEnemy::HandleEvent(const SGD::Event* pEvent)
{
	//which event
	if (pEvent->GetEventID() == "ResetRoom")
	{
		m_bDying = false;
		m_bsCurrState = BS_IDLE;
		SetAlive(true);
		SetPosition(GetOriginalPos());
		m_fResetTimer = 2;
	}
	//Turn around
	if (pEvent->GetEventID() == "TurnMarker" &&
		pEvent->GetSender() == this)
	{
		if (m_fTurnTimer <= 0.0f)
		{
			SetFacingRight(!GetFacingRight());
			m_fTurnTimer = .25f;
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