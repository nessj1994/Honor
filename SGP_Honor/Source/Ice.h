#pragma once
//#include "Entity.h"
#include "Projectile.h"
#include <vector>
#include "../SGD Wrappers/SGD_Handle.h"


class Unit;
//class Projectile;
//#include "Emitter.h"


class Emitter;

class Ice :
	public Projectile
{
public:
	Ice();
	virtual ~Ice();


	virtual void Update(float elapsedTime)  override;
	//void CastIce(Unit* pOther);

	virtual void Render(void) override;


	int GetType(void) const override { return ENT_SPRAY; }
	//Entity* GetOwner(void) const { return m_pOwner; }

	//void SetOwner(Entity* owner);

	virtual void HandleCollision(const IEntity* pOther) override;



	//std::vector< Projectile* >	ProjectileVector;



private:


	//Entity* m_pOwner = nullptr;
	Emitter* m_pEmitter = nullptr;
	float m_fTimer = 0;

};

