#include "WizardHawk.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include "Wizard.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

WizardHawk::WizardHawk()
{
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Bull_Animation.xml");
	m_ts.SetCurrAnimation("Bull_Running");
	m_ts.SetPlaying(true);
	m_szSize = { 32, 32 };

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/WizardHawk.png");
}


WizardHawk::~WizardHawk()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	delete m_pWizard;
}


void WizardHawk::Update(float elapsedTime)
{
	

	m_ptPosition.x = m_ptDestPos.x;
	m_ptPosition.y = m_ptDestPos.y;
	SetGravity(0);

	m_fTimer -= elapsedTime;

	//if (m_fTimer < 0)
	//{
	//	if (m_ptPosition.x != -300)
	//	{
	//		if (m_ptPosition.x < m_ptDestPos.x)
	//		{
	//			m_ptPosition.x += elapsedTime * 20;
	//
	//			if ((m_ptDestPos.x - m_ptPosition.x) < 1)
	//			{
	//				m_ptPosition.x = m_ptDestPos.x;
	//			}
	//		}
	//
	//		if (m_ptPosition.x > m_ptDestPos.x)
	//		{
	//			m_ptPosition.x -= elapsedTime * 20;
	//
	//
	//			if ((m_ptDestPos.x - m_ptPosition.x) < 1)
	//			{
	//				m_ptPosition.x = m_ptDestPos.x;
	//			}
	//
	//
	//		}
	//
	//		if (m_ptPosition.y < m_ptDestPos.y)
	//		{
	//			m_ptPosition.y += elapsedTime * 200;
	//
	//			if ((m_ptDestPos.y - m_ptPosition.y) < 1)
	//			{
	//				m_ptPosition.y = m_ptDestPos.y;
	//
	//			}
	//
	//		}
	//
	//		if (m_szSize.width < m_szDestSize.width)
	//		{
	//			m_szSize.width += elapsedTime * 200;
	//			m_szSize.height += elapsedTime * 200;
	//
	//
	//			if ((m_ptDestPos.y - m_ptPosition.y) < 1)
	//			{
	//				m_ptPosition.y = m_ptDestPos.y;
	//
	//			}
	//
	//		}
	//	}
	//
	//	//Then do size expansion
	//
	//	
	//
	//}

	Unit::Update(elapsedTime);


}

void WizardHawk::Render(void)
{
	//SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	////create a reference to our rectangle
	//SGD::Rectangle rMyRect = GetRect();

	////Offset our rectangle by the camera position for rendering
	//rMyRect.Offset({ -camPos.x, -camPos.y });

	////Render us with the camera
	//Camera::GetInstance()->Draw(rMyRect, SGD::Color::Color(255, 255, 0, 0));
	Camera::GetInstance()->DrawTexture({ m_ptPosition.x + m_szSize.width, m_ptPosition.y },
		0.0f, m_hImage, true, 0.8f, {}, {});

}


void WizardHawk::HandleCollision(const IEntity* pOther)
{

}