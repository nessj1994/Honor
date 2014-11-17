#include "Pendulum.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "AnimationEngine.h"
#include "Camera.h"


Pendulum::Pendulum()
{
	m_ptPosition = SGD::Point(900, 400);
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/Pendulum.png");
	AnimationEngine::GetInstance()->LoadAnimation("Assets/pendulum.xml");
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
	////Get the camera position for our offset
	//SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	////create a reference to our rectangle
	//SGD::Rectangle rMyRect = GetRect();

	////Offset our rectangle by the camera position for rendering
	//rMyRect.Offset({ -camPos.x, -camPos.y });

	////Render us with the camera
	//Camera::GetInstance()->Draw(rMyRect,
	//	SGD::Color::Color(255, 255, 0, 0));
	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, true, 1.0f);
}

SGD::Rectangle Pendulum::GetRect(void) const
{
	return AnimationEngine::GetInstance()->GetAttackRect(m_ts, true, 1, m_ptPosition);
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