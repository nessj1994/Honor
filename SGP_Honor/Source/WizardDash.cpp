#include "WizardDash.h"
#include "AnimationEngine.h"
#include "Camera.h"


WizardDash::WizardDash()
{
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
	m_szSize = { 70, 70 };

}


WizardDash::~WizardDash()
{
}


void WizardDash::Update(float elapsedTime)
{
	//Projectile::Update(elapsedTime);
	if (m_ptPosition.x != -200
		&& m_fTimer < 0)
	{
		if (IsFacingRight() == true)
		{
			SetVelocity({ 1000, 0 });
		}
		else
		{
			SetVelocity({ -1000, 0 });

		}
	}

	m_fTimer -= elapsedTime;

	if (m_fTimer < -0.1f)
	{
		m_fTimer = -0.1f;
	}

	if (GetVelocity().x < 0
		&& GetPosition().x < 50)
	{
		SetVelocity({ 0, 0 });
	}

	if (GetVelocity().x > 0
		&& GetPosition().x > 450)
	{
		SetVelocity({ 0, 0 });
	}

	Entity::Update(elapsedTime);
}
void WizardDash::Render(void)
{
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Render us with the camera
	Camera::GetInstance()->Draw(rMyRect, SGD::Color::Color(255, 255, 0, 0));



}



void WizardDash::HandleCollision(const IEntity* pOther)
{

}