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
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;


	///////////////////////////////////////////////
	/////////////////Accessors////////////////////
	Player* GetPlayer(void) const { return m_pPlayer; }
	bool IsWalker(void) const { return m_bWalker; }
	unsigned int GetCurrentState(void) const { return m_unCurrentState; }
	float GetAggroDistance(void) const { return m_fAggroDistance; }

	//////////////////////////////////////////////
	///////////////////Mutators//////////////////
	void SetPlayer(Player* player) { m_pPlayer = player; }
	void SetWalker(bool walker) { m_bWalker = walker; }
	void SetState(unsigned int state) { m_unCurrentState = state; }
	void SetAggroDistance(float distance) { m_fAggroDistance = distance; }


private:

	Player* m_pPlayer = nullptr;
	bool m_bWalker = true;
	unsigned int m_unCurrentState;
	float m_fAggroDistance = 0.0f;

};

