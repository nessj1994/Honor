#include "Armor.h"


Armor::Armor()
{
}


Armor::~Armor()
{
}


/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Armor::Update(float elapsedTime)
{


}
void Armor::Render(void)
{

}

int Armor::GetType(void) const
{

	return 1;
}

SGD::Rectangle Armor::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };


}

void Armor::HandleCollision(const IEntity* pOther)
{

}