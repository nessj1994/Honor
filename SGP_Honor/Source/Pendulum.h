#pragma once
#include "Entity.h"
#include "AnimTimeStamp.h"
class Pendulum :
	public Entity
{
public:
	Pendulum();
	virtual ~Pendulum();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return Entity::ENT_PENDULUM; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	AnimTimeStamp GetTS() const { return m_ts; }

private:
	AnimTimeStamp m_ts;
};

