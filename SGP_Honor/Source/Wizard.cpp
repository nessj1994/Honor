#include "Wizard.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "GameplayState.h"


Wizard::Wizard() : Listener(this)
{

	m_bsCurrState = WZ_FLOATING;

	SetHitPoints(3);
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
	m_hVictory = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/BossDefeat.wav");



}


Wizard::~Wizard()
{
}


void Wizard::Update(float elapsedTime)
{
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);


	switch (m_bsCurrState)
	{
	case WZ_FLOATING:
	{
						m_ts.SetCurrAnimation("Bull_Running");
						m_ts.SetPlaying(true);
						m_ts.SetSpeed(1.0f);

						//Floating velocity update
						if (floatingLeft == true)
						{
							SetVelocity({ GetVelocity().x - 50 * elapsedTime, GetVelocity().y - 50 * elapsedTime });
							m_fFloatTimer -= elapsedTime;
							if (m_fFloatTimer < -1.0f)
							{
								floatingLeft = false;
							}
						}
						else
						{
							SetVelocity({ GetVelocity().x + 50 * elapsedTime, GetVelocity().y + 50 * elapsedTime });
							m_fFloatTimer += elapsedTime;

							if (m_fFloatTimer > 1.0f)
							{
								floatingLeft = true;
							}

						}

						

	}

	}

	Boss::Update(elapsedTime);
}

void Wizard::Render(void)
{
	Boss::Render();

	SGD::Point newPosition = m_ptPosition;
	newPosition.y += 38;
	newPosition.x += 50;
	Camera::GetInstance()->DrawAnimation(newPosition, 0, m_ts, !m_bFacingRight, 1.0f);

}

void Wizard::HandleCollision(const IEntity* pOther)
{

}
