#include "Laser.h"


Laser::Laser()
{
}


Laser::~Laser()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Laser::Update(float elapsedTime)
{


}
void Laser::Render(void)
{

}

int Laser::GetType(void) const
{
	return 1;

}

SGD::Rectangle Laser::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void Laser::HandleCollision(const IEntity* pOther)
{

}