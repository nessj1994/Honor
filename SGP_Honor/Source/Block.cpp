#include "Block.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"
#include "Unit.h"

Block::Block()
{
	
}


Block::~Block()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Block::Update(float elapsedTime)
{


}
void Block::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	//Camera::GetInstance()->Draw(SGD::Rectangle(10, 300, 20, 320), SGD::Color::Color(255, 0, 0, 255));

	Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(255, 0, 255, 0));

}

void Block::HandleCollision(const IEntity* pOther)
{
	//Unit::HandleCollision(pOther);

	if (pOther->GetType() == ENT_FROZEN)
	{
		SetFriction(0.1f);
	}
	else
	{
		SetFriction(1.0f);

	}

}