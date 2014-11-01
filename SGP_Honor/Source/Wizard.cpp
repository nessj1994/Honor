#include "Wizard.h"
#include "AnimationEngine.h"
#include "Camera.h"


Wizard::Wizard()
{
}


Wizard::~Wizard()
{
}


void Wizard::Update(float elapsedTime)
{
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);


	switch (m_bsCurrState)
	{
	case WZ_FLOATING:
	{

	}

	}
}

void Wizard::Render(void)
{
	Boss::Render();

}

void Wizard::HandleCollision(const IEntity* pOther)
{

}
