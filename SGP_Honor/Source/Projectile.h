#pragma once
#include "Entity.h"



class Projectile :
	public Entity
{
public:
	Projectile();
	virtual ~Projectile();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_PROJ; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	////////////////////////////////////////////////
	/////////////////Accessors/////////////////////
	//SGD::Vector GetDirection(void) const { return m_vtDirection; }
	Entity* GetOwner(void) const { return m_pOwner; }


	///////////////////////////////////////////////
	////////////////Mutators//////////////////////
	//void SetDirection(SGD::Vector Direction) { m_vtDirection = Direction; }
	void SetOwner(Entity* owner) { if(m_pOwner) m_pOwner->Release(); m_pOwner = owner; /*m_pOwner->AddRef();*/ }


private:

	//particle effect 
	//SGD::Vector m_vtDirection = { 0, 0 };
	Entity* m_pOwner = nullptr;




};

