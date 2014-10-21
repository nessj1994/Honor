#include "Boss.h"


Boss::Boss()
{
}


Boss::~Boss()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Boss::Update(float elapsedTime)
{


}

void Boss::Render(void)
{


}

int Boss::GetType(void) const
{

	return 1;
}


SGD::Rectangle Boss::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };
}


void Boss::HandleCollision(const IEntity* pOther)
{


}