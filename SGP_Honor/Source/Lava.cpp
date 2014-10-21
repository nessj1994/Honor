#include "Lava.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"


Lava::Lava()
{

	m_szSize = { 64, 32 };
	m_bRising = true;
	m_ptOrigPos = m_ptPosition;
	m_szOrigSize = m_szSize;


}


Lava::~Lava()
{
}

void Lava::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();



	Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(255, 255,0, 0));
}

void Lava::Update(float elapsedTime)
{
	ChangePillar(300, elapsedTime);

}


void Lava::ChangePillar(int _height, float _delta)
{



		if ((m_ptPosition.y + m_szSize.height) < (m_ptOrigPos.y + m_szOrigSize.height) + _height
			&& m_bRising == true)
		{
			m_ptPosition.y -= 100 * _delta;
			m_szSize.height += 100 * _delta;


		}
		if ((m_ptPosition.y) <= m_ptOrigPos.y - _height
			&& m_bRising == true)
		{
			m_bRising = false;
			m_bApex = true;
		}
		if (m_bApex == true)
		{
			if (m_fApexTimer > 0)
			{
				m_fApexTimer -= _delta;
			}
			else
			{
				m_fApexTimer = 2.0f;
				m_bApex = false;
				m_bFalling = true;
			}
		}
		if (m_ptPosition.y <
			(m_ptOrigPos.y)
			&& m_bFalling == true)
		{
			m_ptPosition.y += 100 * _delta;
			m_szSize.height -= 100 * _delta;

		}

		if ((m_ptPosition.y) >= (m_ptOrigPos.y)
			&& m_bFalling == true)
		{
			m_bFalling = false;
			m_bResting = true;
		}

		if (m_bResting == true)
		{
			if (m_fApexTimer > 0)
			{
				m_fApexTimer -= _delta;
			}
			else
			{
				m_fApexTimer = 2.0f;
				m_bResting = false;
				m_bRising = true;

			}
		}


	

	


}