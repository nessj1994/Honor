#pragma once

#include "Entity.h"

class Emitter;

class Vomit : public Entity
{

	Emitter* m_eEffect;
	Emitter* m_eVomit;
	float m_fTimer;
	bool m_bFinished;
public:
	Vomit(SGD::Point _pos);
	~Vomit();
	Vomit(const Vomit& _Vomit);
	Vomit& operator= (const Vomit& _other);


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	bool Finished() { return m_bFinished; }
};

