#include "Bull.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"


///////////////////////////////////////////////////
// Constructor
// -Setup the size
Bull::Bull() : Listener(this)
{
	Listener::RegisterForEvent("BullMarker");
	SetSize({ 160.0f, 96.0f });
	m_bFacingRight = false;
	m_bsCurrState = BS_WALKING;
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
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
	Boss::Update(elapsedTime);
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);

	// Move in the direction he is facing
	switch (m_bsCurrState)
	{
		case BS_WALKING:
		{
			m_ts.SetCurrAnimation("Bull_Running");			
			if (m_bFacingRight)
			{
				SetVelocity({ -128.0f, 0.0f });
			}
			else
			{
				SetVelocity({ 128.0f, 0.0f });
			}
		}
	}

	// Update turn timer
	if (m_fTurnTimer > 0.0f)
	{
		m_fTurnTimer -= elapsedTime;
	}
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
	Camera::GetInstance()->DrawAnimation(newPosition, 0, m_ts, m_bFacingRight);
}

///////////////////////////////////////////////////
// HandleCollision
// -Handle collisions
void Bull::HandleCollision(const IEntity* pOther)
{
	Boss::HandleCollision(pOther);
}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void Bull::HandleEvent(const SGD::Event* pEvent)
{
	//which event

	//Turn around
	if (pEvent->GetEventID() == "BullMarker" &&
		pEvent->GetSender() == this)
	{
		// Turn around if he can
		if (m_fTurnTimer <= 0.0f)
		{
			m_bFacingRight = !m_bFacingRight;
			m_fTurnTimer = 0.25f;
		}
	}
}