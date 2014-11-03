#include "WizardHawk.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "Wizard.h"

WizardHawk::WizardHawk()
{
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
	m_szSize = { 70, 70 };

}


WizardHawk::~WizardHawk()
{
}


void WizardHawk::Update(float elapsedTime)
{
	

	m_fTimer -= elapsedTime;

	if (m_fTimer < 0)
	{

		if (m_ptPosition.x != -300)
		{
			if (m_ptPosition.x < m_ptDestPos.x)
			{
				m_ptPosition.x += elapsedTime * 20;

				if ((m_ptDestPos.x - m_ptPosition.x) < 5)
				{
					m_ptPosition.x = m_ptDestPos.x;
				}
			}

			if (m_ptPosition.x > m_ptDestPos.x)
			{
				m_ptPosition.x -= elapsedTime * 20;


				if ((m_ptDestPos.x - m_ptPosition.x) < 5)
				{
					m_ptPosition.x = m_ptDestPos.x;
				}


			}

			if (m_ptPosition.y < m_ptDestPos.y)
			{
				m_ptPosition.y += elapsedTime * 200;
			}

			if (m_szSize.width < m_szDestSize.width)
			{
				m_szSize.width += elapsedTime * 200;
				m_szSize.height += elapsedTime * 200;
			}
		}

		//Then do size expansion

		

	}




}

void WizardHawk::Render(void)
{
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Render us with the camera
	Camera::GetInstance()->Draw(rMyRect, SGD::Color::Color(255, 255, 0, 0));


}


void WizardHawk::HandleCollision(const IEntity* pOther)
{

}