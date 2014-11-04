#pragma once

#include "Entity.h"

class Emitter;

class Poop : public Entity
{
	Emitter* m_eEffect;

	bool m_bFinished;
public:
	Poop(SGD::Point _Pos);
	~Poop();

	Poop(const Poop& _Vomit);
	Poop& operator= (const Poop& _other);


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	bool Finished() { return m_bFinished; }
};

