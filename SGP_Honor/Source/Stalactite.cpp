#include "Stalactite.h"


Stalactite::Stalactite()
{
}


Stalactite::~Stalactite()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Stalactite::Update(float elapsedTime)
{


}
void Stalactite::Render(void)
{

}

int Stalactite::GetType(void) const
{

	return 1;
}

SGD::Rectangle Stalactite::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Stalactite::HandleCollision(const IEntity* pOther)
{

}