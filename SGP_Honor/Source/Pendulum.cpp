#include "Pendulum.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "AnimationEngine.h"
#include "Camera.h"


Pendulum::Pendulum()
{
	m_ptPosition = SGD::Point(900, 400);
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("../Assets/graphics/Pendulum.png");
	AnimationEngine::GetInstance()->LoadAnimation("../Assets/pendulum.xml");
	m_ts.SetCurrAnimation("pendulum swing");
	m_ts.SetPlaying(true);
	m_ts.ResetCurrFrame();
}


Pendulum::~Pendulum()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Pendulum::Update(float elapsedTime)
{

	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
}
void Pendulum::Render(void)
{
	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, true);
}

SGD::Rectangle Pendulum::GetRect(void) const
{
	return AnimationEngine::GetInstance()->GetAttackRect(m_ts, false, 1, m_ptPosition);
	//return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void Pendulum::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		if (GetRect().IsIntersecting(pOther->GetRect()))
		{
			//kill the player
			SGD::Event Event = { "KILL_PLAYER", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);
		}

	}
}