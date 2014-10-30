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
	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, m_bFacingRight);
}