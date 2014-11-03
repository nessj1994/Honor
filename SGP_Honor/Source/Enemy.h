#pragma once
#include "Unit.h"
//#include "Player.h"


class Player;

class Enemy :
	public Unit
{
public:
	Enemy();
	virtual ~Enemy();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;

	void BasicCollision(const IEntity* pOther) override;
	virtual void HandleCollision(const IEntity* pOther) override;


	///////////////////////////////////////////////
	/////////////////Accessors////////////////////
	Player* GetPlayer(void) const { return m_pPlayer; }
	bool GetFalling(void) const { return m_bFalling; }
	bool GetFacingRight(void) const { return m_bFacingRight; }
	unsigned int GetCurrentState(void) const { return m_unCurrentState; }
	float GetAggroDistance(void) const { return m_fAggroDistance; }
	bool GetDead(void) const { return m_bDead; }

	//////////////////////////////////////////////
	///////////////////Mutators//////////////////
	void SetPlayer(Player* player) { m_pPlayer = player; }
	void SetFalling(bool walker) { m_bFalling = walker; }
	void SetState(unsigned int state) { m_unCurrentState = state; }
	void SetAggroDistance(float distance) { m_fAggroDistance = distance; }
	void SetFacingRight(bool _right) { m_bFacingRight = _right; }
	void SetDead(bool _dead) { m_bDead = _dead; }

private:

	Player* m_pPlayer = nullptr;
	bool m_bFalling = true;
	bool m_bFacingRight = false;
	bool m_bDead = false;
	unsigned int m_unCurrentState;
	float m_fAggroDistance = 0.0f;

};

