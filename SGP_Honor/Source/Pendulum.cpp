#include "Pendulum.h"


Pendulum::Pendulum()
{
}


Pendulum::~Pendulum()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Pendulum::Update(float elapsedTime)
{


}
void Pendulum::Render(void)
{

}

int Pendulum::GetType(void) const
{

	return 1;
}

SGD::Rectangle Pendulum::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void Pendulum::HandleCollision(const IEntity* pOther)
{

}