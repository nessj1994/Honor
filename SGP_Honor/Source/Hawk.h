#pragma once
#include "Unit.h"
#include "Projectile.h"



class Hawk :
	public Projectile
{
public:
	Hawk();
	virtual ~Hawk();

	virtual void Update(float elapsedTime);


	float GetCooldown(void) const { return m_fCooldown; }
	Entity* GetOwner(void) const { return m_pOwner; }

	void SetOwner(Entity* owner) { if (m_pOwner) m_pOwner->Release(); m_pOwner = owner; m_pOwner->AddRef(); }

private:

	Entity* m_pOwner = nullptr;
	float m_fCooldown = 0.0f;
};

