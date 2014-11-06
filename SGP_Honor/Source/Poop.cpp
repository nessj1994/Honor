#include "Poop.h"
#include "Emitter.h"
#include "ParticleEngine.h"
#include "DestroyEntityMessage.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cmath>
#include <Windows.h>

Poop::Poop(SGD::Point _Pos)
{
	m_ptPosition = _Pos;
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/PoopPic.png", {});
	m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/Poop.xml", "Poop", m_ptPosition);
	m_szSize = m_eEffect->GetSize();
	m_eEffect->SetPosition(_Pos);
	m_bFinished = false;
}


Poop::~Poop()
{
	delete m_eEffect;
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

Poop::Poop(const Poop& _Vomit)
{
	m_eEffect = _Vomit.m_eEffect;
	m_szSize = _Vomit.m_szSize;
}

Poop& Poop::operator= (const Poop& _other)
{
	if (&_other != this)
	{
		Poop(*_other);
	}
	return *this;
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Poop::Update(float elapsedTime)
{
	m_eEffect->SetPosition(m_ptPosition);
	SetVelocity({ GetVelocity().x, (400 - (-500) * elapsedTime) });
	if (!m_bFinished)
	{
		m_eEffect->Finish(false);
	}
	else
	{
		m_eEffect->Finish();
	}
	if (m_eEffect->Done())
	{
		DestroyEntityMessage* Temp = new DestroyEntityMessage(this);
		Temp->QueueMessage();
		Temp = nullptr;
	}
	else
	{
		m_eEffect->Update(elapsedTime);
		Entity::Update(elapsedTime);
	}
}

void Poop::Render(void)
{
	m_eEffect->Render();
	SGD::GraphicsManager::GetInstance()->DrawTexture(
		m_hImage, { m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y },
		(0.0f), m_szSize / 2, {}, m_szScale/2);
}

int Poop::GetType(void) const
{
	return ENT_POOP;
}

SGD::Rectangle Poop::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Poop::HandleCollision(const IEntity* pOther)
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
		m_bFinished = true;
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
		m_bFinished = true;
	}

}