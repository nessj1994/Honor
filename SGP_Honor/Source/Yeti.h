#pragma once
#include "Boss.h"
#include "../SGD Wrappers/SGD_Listener.h"
class Yeti :
	public Boss, public SGD::Listener
{
public:

	enum BossStates { CHASING_STATE, MELEE_STATE, RANGED_STATE, JUMPING_STATE, SPRAYING_STATE, BLIZZARD_STATE, DEAD_STATE };

	Yeti();
	virtual ~Yeti();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const;

	void BasicCollision(const IEntity* pOther) override;
	void HandleCollision(const IEntity* pOther) override;


	///////////////////////////////////////////////
	///////////// Listener Interface /////////////
	void HandleEvent(const SGD::Event* pEvent);


	///////////////////////////////////////////////
	///////////////// Accessors //////////////////





	/////////////////////////////////////////////
	///////////////// Mutators /////////////////


private:


	float m_fAttackCoolDown = 0.0f;
	float m_fBlizzardCooldown = 20.0f;
	float m_fAggroRange = 400.0f;
	float m_fMeleeRange = 70.0f;
	float m_fDistance = 0.0f;
	float m_fJumpTimer = 0.0f;
	bool m_bInRange = false;
	bool m_bSpraying = false;


};

