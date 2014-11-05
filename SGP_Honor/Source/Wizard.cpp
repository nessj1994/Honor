#include "Wizard.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "GameplayState.h"
#include "Projectile.h"
#include "WizardDash.h"
#include "WizardHawk.h"

#include "CreateHorizontalBubble.h"
#include "CreateVerticalBubble.h"
#include "DestroyEntityMessage.h"

Wizard::Wizard() : Listener(this)
{

	//m_bsCurrState = WZ_FLOATING;
	m_bsCurrState = WZ_BULL;
	SetSize({ 160.0f, 96.0f });
	SetHitPoints(3);
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
	m_hVictory = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/BossDefeat.wav");

	m_fOrigStateTimer = 0.0f;
	m_fCurStateTimer = m_fOrigStateTimer;

	//m_nDamage = 1;

}


Wizard::~Wizard()
{
}


void Wizard::Update(float elapsedTime)
{
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);

	m_fCurStateTimer += elapsedTime;

	GetPlayer()->SetSlowed(false);
	GetPlayer()->SetFriction(25);

	switch (m_bsCurrState)
	{
	case WZ_FLOATING:
	{
						m_ts.SetCurrAnimation("Bull_Running");
						m_ts.SetPlaying(true);
						m_ts.SetSpeed(1.0f);

						//Floating velocity update
						//	if (floatingLeft == true)
						//	{
						//		SetVelocity({ GetVelocity().x - 50 * elapsedTime, GetVelocity().y - 50 * elapsedTime });
						//		m_fFloatTimer -= elapsedTime;
						//		if (m_fFloatTimer < -1.0f)
						//		{
						//			floatingLeft = false;
						//		}
						//	}
						//	else
						//	{
						//		SetVelocity({ GetVelocity().x + 50 * elapsedTime, GetVelocity().y + 50 * elapsedTime });
						//		m_fFloatTimer += elapsedTime;
						//
						//		if (m_fFloatTimer > 1.0f)
						//		{
						//			floatingLeft = true;
						//		}
						//
						//	}

						if (m_nDamage == 0)
						{
							if ((m_fCurStateTimer - m_fOrigStateTimer) > 1.0f)
							{

								if (m_nBullCount < 2)
								{

									m_bsCurrState = WZ_BULL;
									m_fOrigStateTimer = m_fCurStateTimer;
									m_nBullCount++;
								}
								else
								{
									m_bsCurrState = WZ_HAWK;
									m_fOrigStateTimer = m_fCurStateTimer;
								}
							}
						}
						if (m_nDamage == 1)
						{
							if ((m_fCurStateTimer - m_fOrigStateTimer) > 1.0f)
							{

								
									m_bsCurrState = WZ_BUBBLE;
									m_fOrigStateTimer = m_fCurStateTimer;
								
							}
						}
						if (m_nDamage == 2)
						{
							if ((m_fCurStateTimer - m_fOrigStateTimer) > 1.0f)
							{

								//if (m_nBullCount < 2)
								//{
								//
								//	m_bsCurrState = WZ_BUBBLE;
								//	m_fOrigStateTimer = m_fCurStateTimer;
								//	m_nBullCount++;
								//}
								//else
								//{
								m_bsCurrState = WZ_ICE;
								m_fOrigStateTimer = m_fCurStateTimer;
								//	}
							}
						}
						if (m_nDamage == 3)
						{

						}

						clonesCasted = false;
						hawksCasted = false;
						break;
	}
	case WZ_BULL:
	{

					if ((m_fCurStateTimer - m_fOrigStateTimer) > 3.0f)
					{

						ClonesUpdate(elapsedTime);

						if (clonesCasted == false)
							CastClones();
					}
					if ((m_fCurStateTimer - m_fOrigStateTimer) > 5.0f)
					{

						m_bsCurrState = WZ_FLOATING;

						m_fOrigStateTimer = m_fCurStateTimer;

					}

					break;

	}
	case WZ_HAWK:
	{

					HawkUpdate(elapsedTime);
					ClonesUpdate(elapsedTime);


					if ((m_fCurStateTimer - m_fOrigStateTimer) > 1.0f)
					{


						if (hawksCasted == false)
						{
							m_fHawkStateTimer = 5.5f;
							CastHawks();
						}

						if (clonesCasted == false)
							CastClones();

					}

					if ((m_fCurStateTimer - m_fOrigStateTimer) > 4.0f)
					{

						m_bsCurrState = WZ_FLOATING;


					}

					break;
	}
	case WZ_ICE:
	{

				   m_fBubbleSpawner -= elapsedTime;

				   if ((m_fCurStateTimer - m_fOrigStateTimer) > 1.0f)
				   {

					   if (hawksCasted == false)
					   {
						   m_fHawkStateTimer = 5.5f;
						   CastHawks();
					   }




					   if (m_fBubbleSpawner <= 0)
					   {
						   CastBubble();
						   m_fBubbleSpawner = 1.0f;
					   }
				   }
				   CastIce();

				   break;

	}
	case WZ_BUBBLE:
	{
					  HawkUpdate(elapsedTime);
					  ClonesUpdate(elapsedTime);

					  m_fBubbleSpawner -= elapsedTime;


					  if ((m_fCurStateTimer - m_fOrigStateTimer) > 1.0f)
					  {


						  if (hawksCasted == false)
						  {
							  m_fHawkStateTimer = 5.5f;
							  CastHawks();
						  }

						  if (clonesCasted == false)
							  CastClones();

						  //if (bubbleCasted == false)
						  //{

						  if (m_fBubbleSpawner <= 0)
						  {
							  CastBubble();
							  m_fBubbleSpawner = 1.0f;
						  }

						  // }

					  }

					  if ((m_fCurStateTimer - m_fOrigStateTimer) > 4.0f)
					  {

						  m_bsCurrState = WZ_FLOATING;


					  }

					  break;

	}
	case WZ_DAMAGED:
	{





	}
	case WZ_DEATH:
	{

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
	if (pOther->GetType() == ENT_HAWK
		&& m_bsCurrState == WZ_VUNERABLE)
	{
		m_bsCurrState = WZ_DAMAGED;
		//m_nDamage += 1;

	}

	m_nDamage += 1;

}


void Wizard::CastClones()
{

	if (m_bsCurrState == WZ_BULL)
	{


		dashPtr1->SetPosition({ 150, 800 });
		dashPtr1->SetTimer(0.5f);

		dashPtr2->SetPosition({ 800, 800 });
		dashPtr2->SetTimer(1.2f);

		dashPtr3->SetPosition({ 150, 900 });
		dashPtr3->SetTimer(1.5f);

		dashPtr4->SetPosition({ 800, 900 });
		dashPtr4->SetTimer(0.8f);

	}

	if (m_bsCurrState == WZ_HAWK)
	{

		dashPtr1->SetPosition({ 150, 800 });
		dashPtr1->SetTimer(0.5f);

		dashPtr2->SetPosition({ 800, 800 });
		dashPtr2->SetTimer(1.2f);

		dashPtr3->SetPosition({ 150, 900 });
		dashPtr3->SetTimer(1.5f);

		dashPtr4->SetPosition({ 800, 900 });
		dashPtr4->SetTimer(0.8f);


		//dashPtr1->SetPosition({ 100, 1850 });
		//dashPtr1->SetTimer(0.5f);
		//
		//dashPtr2->SetPosition({ 600, 1850 });
		//dashPtr2->SetTimer(1.5f);
		//
		//dashPtr3->SetPosition({ 100, 1950 });
		//dashPtr3->SetTimer(2.0f);
		//
		//dashPtr4->SetPosition({ 600, 1950 });
		//dashPtr4->SetTimer(1.0f);



	}

	if (clonesCasted == false)
	{
		m_fDashStateTimer = 3.5f;
	}


	clonesCasted = true;

}

void Wizard::ClonesUpdate(float elapsedTime)
{
	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
	m_ts.SetSpeed(1.0f);

	m_fDashStateTimer -= elapsedTime;



	if (m_fDashStateTimer < 0)
	{
		m_fDashStateTimer = 0.0f;
		//m_bsCurrState = WZ_FLOATING;
		//	m_bsCurrState = WZ_HAWK;



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

	clonesCasted = false;

}

void Wizard::CastHawks()
{

	if (m_bsCurrState == WZ_HAWK)
	{

		hawkPtr1->SetPosition({ 150, m_ptPosition.y });
		hawkPtr1->SetTimer(0.1f);

		hawkPtr2->SetPosition({ 350, m_ptPosition.y });
		hawkPtr2->SetTimer(0.2f);

		hawkPtr3->SetPosition({ 450, m_ptPosition.y });
		hawkPtr3->SetTimer(0.3f);

		hawkPtr4->SetPosition({ 650, m_ptPosition.y });
		hawkPtr4->SetTimer(0.4f);



		hawkPtr1->SetDestPos({ 100, 600 });
		hawkPtr2->SetDestPos({ 300, 600 });
		hawkPtr3->SetDestPos({ 500, 600 });
		hawkPtr4->SetDestPos({ 700, 600 });


		hawkPtr1->SetDestSize({ 70, 70 });
		hawkPtr2->SetDestSize({ 70, 70 });
		hawkPtr3->SetDestSize({ 70, 70 });
		hawkPtr4->SetDestSize({ 70, 70 });

	}

	if (m_bsCurrState == WZ_ICE)
	{
		if (rand() % 2 == 1)
		{

			hawkPtr1->SetPosition({ 250, m_ptPosition.y });
			hawkPtr1->SetTimer(0.1f);

			hawkPtr2->SetPosition({ 450, m_ptPosition.y });
			hawkPtr2->SetTimer(0.2f);

			hawkPtr3->SetPosition({ 550, m_ptPosition.y });
			hawkPtr3->SetTimer(0.3f);

			hawkPtr4->SetPosition({ 750, m_ptPosition.y });
			hawkPtr4->SetTimer(0.4f);


			hawkPtr1->SetDestPos({ 200, 600 });
			hawkPtr2->SetDestPos({ 400, 600 });
			hawkPtr3->SetDestPos({ 600, 600 });
			hawkPtr4->SetDestPos({ 800, 600 });
		}
		else
		{


			hawkPtr1->SetPosition({ 150, m_ptPosition.y });
			hawkPtr1->SetTimer(0.1f);

			hawkPtr2->SetPosition({ 350, m_ptPosition.y });
			hawkPtr2->SetTimer(0.2f);

			hawkPtr3->SetPosition({ 450, m_ptPosition.y });
			hawkPtr3->SetTimer(0.3f);

			hawkPtr4->SetPosition({ 650, m_ptPosition.y });
			hawkPtr4->SetTimer(0.4f);

			hawkPtr1->SetDestPos({ 100, 600 });
			hawkPtr2->SetDestPos({ 300, 600 });
			hawkPtr3->SetDestPos({ 500, 600 });
			hawkPtr4->SetDestPos({ 700, 600 });
		}


		hawkPtr1->SetDestSize({ 70, 70 });
		hawkPtr2->SetDestSize({ 70, 70 });
		hawkPtr3->SetDestSize({ 70, 70 });
		hawkPtr4->SetDestSize({ 70, 70 });
	}


	if (m_bsCurrState == WZ_BUBBLE)
	{
	//	hawkPtr1->SetPosition({ 150, m_ptPosition.y });
	//	hawkPtr1->SetTimer(0.1f);
	//
	//	hawkPtr2->SetPosition({ 350, m_ptPosition.y });
	//	hawkPtr2->SetTimer(0.2f);
	//
	//	hawkPtr3->SetPosition({ 450, m_ptPosition.y });
	//	hawkPtr3->SetTimer(0.3f);
	//
	//	hawkPtr4->SetPosition({ 650, m_ptPosition.y });
	//	hawkPtr4->SetTimer(0.4f);

		if (rand() % 2 == 1)
		{

			hawkPtr1->SetPosition({ 250, m_ptPosition.y });
			hawkPtr1->SetTimer(0.1f);

			hawkPtr2->SetPosition({ 450, m_ptPosition.y });
			hawkPtr2->SetTimer(0.2f);

			hawkPtr3->SetPosition({ 550, m_ptPosition.y });
			hawkPtr3->SetTimer(0.3f);

			hawkPtr4->SetPosition({ 750, m_ptPosition.y });
			hawkPtr4->SetTimer(0.4f);


			hawkPtr1->SetDestPos({ 200, 600 });
			hawkPtr2->SetDestPos({ 400, 600 });
			hawkPtr3->SetDestPos({ 600, 600 });
			hawkPtr4->SetDestPos({ 800, 600 });
		}
		else
		{


			hawkPtr1->SetPosition({ 150, m_ptPosition.y });
			hawkPtr1->SetTimer(0.1f);

			hawkPtr2->SetPosition({ 350, m_ptPosition.y });
			hawkPtr2->SetTimer(0.2f);

			hawkPtr3->SetPosition({ 450, m_ptPosition.y });
			hawkPtr3->SetTimer(0.3f);

			hawkPtr4->SetPosition({ 650, m_ptPosition.y });
			hawkPtr4->SetTimer(0.4f);

			hawkPtr1->SetDestPos({ 100, 600 });
			hawkPtr2->SetDestPos({ 300, 600 });
			hawkPtr3->SetDestPos({ 500, 600 });
			hawkPtr4->SetDestPos({ 700, 600 });
		}


		hawkPtr1->SetDestSize({ 70, 70 });
		hawkPtr2->SetDestSize({ 70, 70 });
		hawkPtr3->SetDestSize({ 70, 70 });
		hawkPtr4->SetDestSize({ 70, 70 });
	}


	hawksCasted = true;
}


void Wizard::HawkUpdate(float elapsedTime)
{

	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
	m_ts.SetSpeed(1.0f);

	m_fHawkStateTimer -= elapsedTime;

	if (hawkPtr4->GetPosition().x != -300)
	{


		if (hawkPtr1->GetPosition().y >= hawkPtr1->GetDestPos().y)
		{
			hawkPtr1->SetFall(true);
		}

		if (hawkPtr2->GetPosition().y >= hawkPtr2->GetDestPos().y)
		{
			hawkPtr2->SetFall(true);
		}

		if (hawkPtr3->GetPosition().y >= hawkPtr3->GetDestPos().y)
		{
			hawkPtr3->SetFall(true);
		}

		if (hawkPtr4->GetPosition().y >= hawkPtr4->GetDestPos().y)
		{
			hawkPtr4->SetFall(true);
		}


	}
	//Hawk 1
	if (hawkPtr1->GetFall() == true)
	{
		if (hawkPtr1->GetPosition().y < 950)
		{

			hawkPtr1->SetVelocity({ hawkPtr1->GetVelocity().x, hawkPtr1->GetVelocity().y + 1000 * elapsedTime });

		}
		else
		{
			hawkPtr1->SetDestPos({ -300, -300 });


		}
	}

	//hawk 2
	if (hawkPtr2->GetFall() == true)
	{
		if (hawkPtr2->GetPosition().y < 950)
		{
			hawkPtr2->SetVelocity({ hawkPtr2->GetVelocity().x, hawkPtr2->GetVelocity().y + 1000 * elapsedTime });

		}
		else
		{
			hawkPtr2->SetDestPos({ -300, -300 });


		}
	}

	//Hawk 3
	if (hawkPtr3->GetFall() == true)
	{
		if (hawkPtr3->GetPosition().y < 950)
		{
			hawkPtr3->SetVelocity({ hawkPtr3->GetVelocity().x, hawkPtr3->GetVelocity().y + 1000 * elapsedTime });

		}
		else
		{
			hawkPtr3->SetDestPos({ -300, -300 });


		}
	}

	//Hawk 4
	if (hawkPtr4->GetFall() == true)
	{
		if (hawkPtr4->GetPosition().y < 950)
		{
			hawkPtr4->SetVelocity({ hawkPtr4->GetVelocity().x, hawkPtr4->GetVelocity().y + 1000 * elapsedTime });

		}
		else
		{
			ResetHawks();

		}
	}












	if (m_fHawkStateTimer < 0)
	{
		m_fHawkStateTimer = 0.0f;
		//	m_bsCurrState = WZ_FLOATING;

		ResetHawks();

	}

}


void Wizard::ResetHawks()
{
	hawkPtr2->SetPosition({ -300, -300 });
	hawkPtr3->SetPosition({ -300, -300 });
	hawkPtr4->SetPosition({ -300, -300 });


	hawkPtr1->SetSize({ 0, 0 });
	hawkPtr2->SetSize({ 0, 0 });
	hawkPtr3->SetSize({ 0, 0 });
	hawkPtr4->SetSize({ 0, 0 });


	hawkPtr1->SetVelocity({ 0, 0 });
	hawkPtr2->SetVelocity({ 0, 0 });
	hawkPtr3->SetVelocity({ 0, 0 });
	hawkPtr4->SetVelocity({ 0, 0 });



	hawksCasted = false;


}

void Wizard::CastBubble()
{
	//if (rand() % 2 == 0)
	//{
	//	CreateHorizontalBubble* pMsg = new CreateHorizontalBubble(this);
	//	pMsg->QueueMessage();
	//	pMsg = nullptr;
	//}
	//else
	//{
	//	CreateVerticalBubble* pMsg = new CreateVerticalBubble(this);
	//	pMsg->QueueMessage();
	//	pMsg = nullptr;
	//}



	CreateHorizontalBubble* pMsg = new CreateHorizontalBubble(this);
	pMsg->QueueMessage();
	pMsg = nullptr;
}

void Wizard::CastIce()
{
	GetPlayer()->SetSlowed(true);
	GetPlayer()->SetFriction(40);
}

void Wizard::BubbleUpdate(float elapsedTime)
{

}