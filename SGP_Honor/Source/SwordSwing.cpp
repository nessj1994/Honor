#include "SwordSwing.h"


SwordSwing::SwordSwing()
{
}


SwordSwing::~SwordSwing()
{
}

void SwordSwing::Update(float elapsedTime) 
{
	m_ptPosition.x = GetRect().left;
	m_ptPosition.y = GetRect().top;

	m_szSize.width = GetRect().right - GetRect().left;
	m_szSize.height = GetRect().bottom - GetRect().top;


}
void SwordSwing::Render(void)
{

}

void SwordSwing::HandleCollision(const IEntity* pOther)
{

}