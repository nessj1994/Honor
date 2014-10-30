#pragma once
#include "Unit.h"
#include "Projectile.h"

class Emitter;

class Hawk :
	public Projectile
{
public:
	Hawk();
	virtual ~Hawk();

	virtual void Update(float elapsedTime);
	virtual void HandleCollision(const IEntity* pOther) override;
	int GetType(void) const override { return ENT_HAWK; }


	float GetCooldown(void) const { return m_fCooldown; }
	Entity* GetOwner(void) const { return m_pOwner; }
	float GetAirFriction(void) const { return m_fAirFriction; }

	void SetOwner(Entity* owner) { if (m_pOwner) m_pOwner->Release(); m_pOwner = owner; m_pOwner->AddRef(); }

private:
	
	Entity* m_pOwner = nullptr;
	float m_fCooldown = 0.0f;
	float m_fAirFriction = 1.5f;

};

