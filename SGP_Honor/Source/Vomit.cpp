#include "Vomit.h"
#include "Emitter.h"
#include "ParticleEngine.h"
#include "DestroyEntityMessage.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cmath>
#include <Windows.h>

Vomit::Vomit(SGD::Point _Pos)
{
	m_ptPosition = _Pos;
	m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/Vomit.xml", "Vomit", m_ptPosition);
	m_eVomit = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/VomitPile.xml", "VomitPile", m_ptPosition);
	m_szSize = m_eEffect->GetSize();
	m_eEffect->SetPosition(_Pos);
	m_bFinished = false;
}


Vomit::~Vomit()
{
	delete m_eEffect;
	delete m_eVomit;
}

Vomit::Vomit(const Vomit& _Vomit)
{
	m_eEffect = _Vomit.m_eEffect;
	m_eVomit = _Vomit.m_eVomit;
	m_szSize = _Vomit.m_szSize;
}

Vomit& Vomit::operator= (const Vomit& _other)
{
	if (&_other != this)
	{
		Vomit(*_other);
	}
	return *this;
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Vomit::Update(float elapsedTime)
{
	m_fTimer += elapsedTime;
	m_eEffect->SetPosition(m_ptPosition);
	m_eVomit->SetPosition(m_ptPosition);
	SetVelocity({ GetVelocity().x, (200 - (-500) * elapsedTime) });
	if (m_fTimer >= 5)
	{
		m_eVomit->Finish();
		m_eEffect->Finish();
		m_bFinished = true;
	}
	else
	{
		m_eVomit->Finish(false);
		m_eEffect->Finish(false);
	}
	if (m_eEffect->Done() && m_eVomit->Done())
	{
		DestroyEntityMessage* Temp = new DestroyEntityMessage(this);
		Temp->QueueMessage();
		Temp = nullptr;
	}
	else
	{
		m_eVomit->Update(elapsedTime);
		m_eEffect->Update(elapsedTime);
		Entity::Update(elapsedTime);
	}
}
void Vomit::Render(void)
{
	m_eVomit->Render();
	m_eEffect->Render();
	SGD::GraphicsManager::GetInstance()->DrawRectangle(GetRect(), { 255, 0, 128, 0 }, {}, 2);
}

int Vomit::GetType(void) const
{
	return ENT_VOMIT;
}

SGD::Rectangle Vomit::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Vomit::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		return;
	}
	RECT rMutant;
	rMutant.left = GetRect().left;
	rMutant.top = GetRect().top;
	rMutant.right = GetRect().right;
	rMutant.bottom = GetRect().bottom;

	RECT rObject;
	rObject.left = pOther->GetRect().left;
	rObject.top = pOther->GetRect().top;
	rObject.right = pOther->GetRect().right;
	rObject.bottom = pOther->GetRect().bottom;

	RECT rIntersection = {};

	IntersectRect(&rIntersection, &rObject, &rMutant);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;



	if (nIntersectHeight > nIntersectWidth)
	{
		if (rMutant.right == rIntersection.right)
		{
			SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
		if (rMutant.left == rIntersection.left)
		{
			SetPosition({ (float)rObject.right, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rMutant.bottom == rIntersection.bottom)
		{
			SetVelocity({ GetVelocity().x, 0 });
			SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });
		}
		if (rMutant.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}

}