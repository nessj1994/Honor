#include "Wizard.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "GameplayState.h"
#include "Projectile.h"
#include "WizardDash.h"

Wizard::Wizard() : Listener(this)
{

	//m_bsCurrState = WZ_FLOATING;
	m_bsCurrState = WZ_BULL;

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

						clonesCasted = false;
						break;
	}
	case WZ_BULL:
	{	
					m_ts.SetCurrAnimation("Bull_Running");
					m_ts.SetPlaying(true);
					m_ts.SetSpeed(1.0f);

					m_fDashStateTimer -= elapsedTime;

					if (clonesCasted == false)
					{
						m_fDashStateTimer = 3.5f;
					}

					if (m_fDashStateTimer < 0)
					{
						m_fDashStateTimer = 0.0f;
						m_bsCurrState = WZ_FLOATING;

						ResetClones();

					}

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

					if (clonesCasted == false)
						CastClones();


					break;

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


void Wizard::CastClones()
{
	
	dashPtr1->SetPosition({ 100, 1400 });
	dashPtr1->SetTimer(0.5f);

	dashPtr2->SetPosition({ 400, 1400 });
	dashPtr2->SetTimer(1.0f);

	dashPtr3->SetPosition({ 100, 1700 });
	dashPtr3->SetTimer(1.5f);

	dashPtr4->SetPosition({ 400, 1700 });
	dashPtr4->SetTimer(2.0f);

	clonesCasted = true;

}

void Wizard::ResetClones()
{

	dashPtr1->SetPosition({ -200, -200 });
	dashPtr1->SetTimer(0.5f);

	dashPtr2->SetPosition({ -200, -200 });
	dashPtr2->SetTimer(1.0f);

	dashPtr3->SetPosition({ -200, -200 });
	dashPtr3->SetTimer(1.5f);

	dashPtr4->SetPosition({ -200, -200 });
	dashPtr4->SetTimer(2.0f);

	clonesCasted = true;

}