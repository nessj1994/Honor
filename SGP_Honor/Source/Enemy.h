#pragma once
#include "Unit.h"
#include "Player.h"




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
	bool GetFacingRight(void) const { return m_bFacingRight; }
	unsigned int GetCurrentState(void) const { return m_unCurrentState; }
	float GetAggroDistance(void) const { return m_fAggroDistance; }

	//////////////////////////////////////////////
	///////////////////Mutators//////////////////
	void SetPlayer(Player* player) { if(m_pPlayer)m_pPlayer->Release(); m_pPlayer = player; if(m_pPlayer)m_pPlayer->AddRef(); }
	void SetState(unsigned int state) { m_unCurrentState = state; }
	void SetAggroDistance(float distance) { m_fAggroDistance = distance; }
	void SetFacingRight(bool _right) { m_bFacingRight = _right; }

private:

	Player* m_pPlayer = nullptr;
	//bool m_bFacingRight = false;
	unsigned int m_unCurrentState;
	float m_fAggroDistance = 0.0f;
};

