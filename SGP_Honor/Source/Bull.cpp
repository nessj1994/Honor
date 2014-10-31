#include "Bull.h"
#include "AnimationEngine.h"
#include "Camera.h"


///////////////////////////////////////////////////
// Constructor
// -Setup the size
Bull::Bull()
{
	SetSize({ 160.0f, 96.0f });
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Charging");
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