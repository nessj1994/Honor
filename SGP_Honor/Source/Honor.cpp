#include "Honor.h"


Honor::Honor()
{
}


Honor::~Honor()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Honor::Update(float elapsedTime)
{


}
void Honor::Render(void)
{

}

int Honor::GetType(void) const
{
	return 1;

}

SGD::Rectangle Honor::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };


}

void Honor::HandleCollision(const IEntity* pOther)
{

}