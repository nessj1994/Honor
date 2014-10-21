#include "Geyser.h"


Geyser::Geyser()
{
}


Geyser::~Geyser()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Geyser::Update(float elapsedTime)
{


}
void Geyser::Render(void)
{

}

int Geyser::GetType(void) const
{

	return 1;
}

SGD::Rectangle Geyser::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };


}

void Geyser::HandleCollision(const IEntity* pOther)
{

}