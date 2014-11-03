#pragma once

#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Emitter;

class FreezeableGround :
	public Entity
{
public:
	FreezeableGround();
	virtual ~FreezeableGround();



	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	bool GetIsFrozen(void) const { return m_bIsFrozen; }
	bool GetIsPerm(void) const { return m_bPermFrozen; }

	int GetType(void) const  { return frozenTYPE; }

	void SetIsFrozen(bool _frozen) { m_bIsFrozen = _frozen; }
	void SetIsPerm(bool _perm) { m_bPermFrozen = _perm; }

	void SetType(int _type) { frozenTYPE = _type; }



private:
	Emitter* m_eEffect;

	int frozenTYPE = 0;
	float m_fFreezeTimer = 0.0f;

	bool m_bPermFrozen = false;
	bool m_bIsFrozen = false;


};

