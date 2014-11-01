#include "Bull.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "ParticleEngine.h"


///////////////////////////////////////////////////
// Constructor
// -Setup the size
Bull::Bull() : Listener(this)
{
	Listener::RegisterForEvent("BullMarkerLeft");
	Listener::RegisterForEvent("BullMarkerRight");
	Listener::RegisterForEvent("ResetRoom");
	SetSize({ 160.0f, 96.0f });
	m_bFacingRight = false;
	m_bsCurrState = BS_WALKING;
	m_fWalkTimer = 1.0f;
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
	SetHitPoints(1);
	m_eFire1 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/FireEffect1.xml", "FireEffect1", m_ptPosition);
	m_eFire2 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/FireEffect2.xml", "FireEffect2", m_ptPosition);
	m_eFire3 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/FireEffect3.xml", "FireEffect3", m_ptPosition);
}

///////////////////////////////////////////////////
// Destructor
// -Setup the size
Bull::~Bull()
{
}

///////////////////////////////////////////////////
// Update
// -Main update loop for the bull
void Bull::Update(float elapsedTime)
{
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);

	switch (m_bsCurrState)
	{
		case BS_WALKING:
		{
			// Update animation
			m_ts.SetCurrAnimation("Bull_Running");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(1.0f);

			// Move in the direction he is facing
			if (m_bFacingRight)
			{
				SetVelocity({ 128.0f, 0.0f });
			}
			else
			{
				SetVelocity({ -128.0f, 0.0f });
			}

			// Charge every few seconds
			m_fWalkTimer -= elapsedTime;
			if (m_fWalkTimer <= 0.0f)
			{
				m_bCanCharge = true;
			}

			// Charge when the player is in sight
			if (m_bCanCharge)
			{
				float playerX = GetPlayer()->GetPosition().x;
				if (m_bFacingRight && playerX > m_ptPosition.x)
				{
					m_fChargeTimer = 3.0f;
					m_bsCurrState = BS_CHARGING;
					m_ts.ResetCurrFrame();
				}
				else if (!m_bFacingRight && playerX < m_ptPosition.x)
				{
					m_fChargeTimer = 3.0f;
					m_bsCurrState = BS_CHARGING;
					m_ts.ResetCurrFrame();
				}
			}

			break;
		}
		case BS_RUNNING:
		{
			// Update animation
			m_ts.SetCurrAnimation("Bull_Running");
			m_ts.SetSpeed(3.0f);

			// Move in the direction he is facing
			if (m_bFacingRight)
			{
				SetVelocity({8 * 128.0f, 0.0f });
			}
			else
			{
				SetVelocity({8 * -128.0f, 0.0f });
			}

			// Change states if a wall is hit
			if (GetHitWall())
			{
				m_fStunTimer = 3.0f;
				m_bsCurrState = BS_STUNNED;
				SetVelocity({ 0.0f, -500.0f });
				SetPosition({ m_ptPosition.x, m_ptPosition.y - 1 });
			}

			// Update timer to change to running
			m_fRunTimer -= elapsedTime;
			if (m_fRunTimer <= 0.0f)
			{
				m_fSlowTimer = 1.0f;
				m_bsCurrState = BS_SLOWING;
			}

			break;
		}
		case BS_CHARGING:
		{
			// Handle animation
			m_ts.SetCurrAnimation("Bull_Charging");
			m_ts.SetSpeed(1.0f);

			SetVelocity({ 0.0f, 0.0f });

			// Update timer to change to running
			m_fChargeTimer -= elapsedTime;
			if (m_fChargeTimer <= 0.0f)
			{
				m_fRunTimer = 3.0f;
				m_bsCurrState = BS_RUNNING;
				m_ts.ResetCurrFrame();
			}


			break;
		}
		case BS_SLOWING:
		{
			// Update animation
			m_ts.SetCurrAnimation("Bull_Running");
			m_ts.SetSpeed(2.0f * m_fSlowTimer + 1.0f);

			// Move in the direction he is facing
			if (m_bFacingRight)
			{
				SetVelocity({ (4 * m_fSlowTimer + 1.0f) * 128.0f, 0.0f });
			}
			else
			{
				SetVelocity({ (4 * m_fSlowTimer + 1.0f) * -128.0f, 0.0f });
			}

			// Update timer to change to running
			m_fSlowTimer -= elapsedTime;
			if (m_fSlowTimer <= 0.0f)
			{
				m_fWalkTimer = 5.0f;
				m_bCanCharge = false;
				m_bsCurrState = BS_WALKING;
			}

			break;
		}
		case BS_STUNNED:
		{
			// Update animation
			m_ts.SetCurrAnimation("Bull_Stunned");
			m_ts.SetSpeed(1.0f);

			// Bounce off of the wall
			if (m_bFacingRight)
			{
				SetVelocity({ -m_fStunSpeed, m_vtVelocity.y });
			}
			else
			{
				SetVelocity({ m_fStunSpeed, m_vtVelocity.y });
			}

			// Slow down recoil
			if (m_fStunTimer >= 2.0f)
			{
				m_fStunSpeed = 200.0f * ((m_fStunTimer - 2.0f) / 3.0f);
			}

			// Update timer to change to running
			m_fStunTimer -= elapsedTime;
			if (m_fStunTimer <= 0.0f)
			{
				m_bFacingRight = !m_bFacingRight;
				m_fWalkTimer = 5.0f;
				m_bCanCharge = false;
				m_bsCurrState = BS_WALKING;
				m_ts.ResetCurrFrame();
			}

			break;
		}
		case BS_RETURNING:
		{
			// Update animation
			m_ts.SetCurrAnimation("Bull_Running");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(2.0f);

			// Change back to walking after hitting the floor
			if (!GetIsFalling())
			{
				m_fSlowTimer = 1.0f;
				m_bsCurrState = BS_SLOWING;
				if (GetHitPoints() <= 0)
				{
					m_fDeathTimer = 13.0f;
					m_bsCurrState = BS_DEATH;
					m_ts.ResetCurrFrame();
				}
			}
			break;
		}
		case BS_DEATH:
		{
			// Update animation
			if (m_fDeathTimer > 10.0f)
			{
				// Slowly walk a little
				m_ts.SetCurrAnimation("Bull_Running");
				m_ts.SetPlaying(true);
				m_ts.SetSpeed(0.5f);
				if (m_bFacingRight)
				{
					SetVelocity({ 32.0f, 0.0f });
				}
				else
				{
					SetVelocity({ -32.0f, 0.0f });
				}
			}
			else if (m_fDeathTimer > 9.0f)
			{
				// Pass out and die
				if (!m_bDead)
				{
					m_ts.ResetCurrFrame();
					m_bDead = true;
				}

				m_ts.SetCurrAnimation("Bull_Stunned");
				m_ts.SetSpeed(1.0f);
				SetVelocity({ 0.0f, 0.0f });
			}
			else if (m_fDeathTimer > 5.0f)
			{
				// Fire effect
				m_eFire1->Update(elapsedTime);
				m_eFire2->Update(elapsedTime);
				m_eFire3->Update(elapsedTime);

			}
			else
			{
				// Alpha fade
			}

			// Update timer
			if (m_fDeathTimer > 0.0f)
			{
				m_fDeathTimer -= elapsedTime;
			}
			break;
		}
	}

	// Update turn timer
	if (m_fTurnTimer > 0.0f)
	{
		m_fTurnTimer -= elapsedTime;
	}

	// Reset hitting a wall
	SetHitWall(false);

	// Boss update
	Boss::Update(elapsedTime);
}

///////////////////////////////////////////////////
// Render
// -Draw the bull and his animations
void Bull::Render(void)
{
	Boss::Render();
	SGD::Point newPosition = m_ptPosition;
	newPosition.y += 38;
	newPosition.x += 50;
	Camera::GetInstance()->DrawAnimation(newPosition, 0, m_ts, !m_bFacingRight, 1.0f);
	if (m_bDead)
	{
		SGD::Point renderPosition = m_ptPosition;
		renderPosition.x -= 32;
		renderPosition.y += 32;
		m_eFire1->Render(renderPosition);
		m_eFire2->Render(renderPosition);
		//m_eFire3->Render(renderPosition);
	}
}

///////////////////////////////////////////////////
// HandleCollision
// -Handle collisions
void Bull::HandleCollision(const IEntity* pOther)
{
	Boss::HandleCollision(pOther);

	// Death
	if (pOther->GetType() == ENT_DEATH)
	{
		// Set returning to true and start moving back towards the platform
		if (m_bsCurrState != BS_RETURNING)
		{
			m_bsCurrState = BS_RETURNING;
			SetHitPoints(GetHitPoints() - 1);
			if (m_bFacingRight)
			{
				SetVelocity({ -800.0f, -1300.0f });
				m_bFacingRight = false;
			}
			else
			{
				SetVelocity({ 800.0f, -1300.0f });
				m_bFacingRight = true;
			}
		}
	}
}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void Bull::HandleEvent(const SGD::Event* pEvent)
{
	//which event

	//Turn to the right
	if (pEvent->GetEventID() == "BullMarkerLeft" &&
		pEvent->GetSender() == this)
	{
		// Turn around if he can
		if (m_fTurnTimer <= 0.0f &&
			m_bsCurrState == BS_WALKING)
		{
			m_bFacingRight = true;
			m_fTurnTimer = 0.25f;
		}
	}
	// Turn to the left
	else if (pEvent->GetEventID() == "BullMarkerRight" &&
		pEvent->GetSender() == this)
	{
		// Turn around if he can
		if (m_fTurnTimer <= 0.0f &&
			m_bsCurrState == BS_WALKING)
		{
			m_bFacingRight = false;
			m_fTurnTimer = 0.25f;
		}
	}
	// Player death
	else if (pEvent->GetEventID() == "ResetRoom")
	{
		ResetBull();
	}
}

///////////////////////////////////////////////////
// ResetBull
// -Helper method to get the bull ready when the player dies
void Bull::ResetBull()
{
	SetHitPoints(3);
	SetPosition(GetStartPosition());
	m_bFacingRight = false;
	m_bCanCharge = false;
	m_bsCurrState = BS_WALKING;
	m_fAlphaFade = 0;
	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
	m_ts.ResetCurrFrame();

	// Reset timers
	m_fWalkTimer = 5.0f;
	m_fTurnTimer = 0.0f;
	m_fChargeTimer = 0.0f;
	m_fRunTimer = 0.0f;
	m_fSlowTimer = 0.0f;
	m_fStunTimer = 0.0f;
	m_fDeathTimer = 0.0f;
}