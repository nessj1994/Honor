#include "Laser.h"
#include "Camera.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

Laser::Laser()
{
	m_szSize = { 32, 32 };

}


Laser::~Laser()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Laser::Update(float elapsedTime)
{
	if (m_bFull == false)
	{
		m_ptPosition.y -= 100 *  elapsedTime;
		m_szSize.height += 100 * elapsedTime;
	}

}
void Laser::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();



	Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(255, 0, 255, 0));
}

int Laser::GetType(void) const
{
	return ENT_LASER;

}

SGD::Rectangle Laser::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void Laser::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_SOLID_WALL)
	{
		m_bFull = true;
	}


}