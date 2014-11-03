#pragma once
#include "Unit.h"
class Turret :
	public Unit
{
public:
	Turret();
	virtual ~Turret();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;
	

	/////////////////////////////////////////////////
	/////////////////Mutators//////////////////////
	void SetFireTimer(float _timer) { m_fFireTimer = _timer; }
	void SetDirection(int _direction) { m_nDirection = _direction; }

private:

	float m_fFireTimer = 1.65f;
	int m_nDirection = 0;
	float m_fRotation = 0.0f;
};


