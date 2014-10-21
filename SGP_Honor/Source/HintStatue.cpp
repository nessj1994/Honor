#include "HintStatue.h"


HintStatue::HintStatue()
{
}


HintStatue::~HintStatue()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void HintStatue::Update(float elapsedTime)
{


}
void HintStatue::Render(void)
{

}

int HintStatue::GetType(void) const
{
	return 1;

}

SGD::Rectangle HintStatue::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };


}

void HintStatue::HandleCollision(const IEntity* pOther)
{

}