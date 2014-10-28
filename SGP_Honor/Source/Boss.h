#pragma once

#include "Unit.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Boss :
	public Unit
{
public:
	Boss();
	virtual ~Boss();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override { return ENT_BOSS; }

	void BasicCollision(const IEntity* pOther) override;
	void HandleCollision(const IEntity* pOther) override;

	 ////////////////////////////////////////////////
	////////////////////Accessors///////////////////
	Entity* GetPlayer(void) const { return m_pPlayer; }
	unsigned int GetHitPoints(void) const { return m_unHitPoints; }
	unsigned int GetCurrentState(void) const { return m_unCurrentState; }
	SGD::Point GetStartPosition(void) const { return m_ptPosition; }
	bool GetIsFalling(void)const { return is_Falling; }

	 /////////////////////////////////////////////
	/////////////////Mutators////////////////////
	void SetPlayer(Entity* player) { m_pPlayer = player; }
	void SetHitPoints(unsigned int hitPoints) { m_unHitPoints = hitPoints; }
	void SetCurrentState(unsigned int state) { m_unCurrentState = state; }
	void SetIsFalling(bool _fall) { is_Falling = _fall; }
	void SetStartPosition(SGD::Point startPos) { m_ptStartPosition = startPos; }

private:

	Entity* m_pPlayer;
	unsigned int m_unHitPoints;
	unsigned int m_unCurrentState;
	SGD::Point m_ptStartPosition;

	bool is_Falling = true;


};

