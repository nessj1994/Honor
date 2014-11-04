#pragma once
#include "Boss.h"
#include <vector>
class Hawk;
class Stalactite;

class Caveman :
	public Boss, SGD::Listener
{
public:
	Caveman();
	virtual ~Caveman();

	enum CaveManStates { CM_RUNING, CM_JUMPING, CM_HIDING, CM_EMERGING, CM_FALLING, CM_SLASHING, CM_ATTACKING, CM_THINKING, CM_STUNNED };

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override { return ENT_BOSS_CAVEMAN; }
	void HandleCollision(const IEntity* pOther) override;

	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);
	
	bool GetAttacking() const;
	void ResetCaveMan();

	//For Hawk Events
	void HawkExplode(SGD::Point);

private:
	//Dont do this event for this time
	float m_fStopTimer;
	float m_fDoorTimer;
	//Data Memebers
	CaveManStates m_bsCurrState;

	//For the Thinking State
	float m_fThinkingTimer;

	//For Jumping State
	float m_fInTheAir;

	//For Hiding state
	float m_fHidingTimer;

	//For Door Events
	int m_iDoorSkip;

	//For Attakcing Events
	Hawk* m_pHawk;
		//FOr the Hawk
		Emitter* m_hHawkExplode;
	float m_fAttackingTimer;

	//For Falling Stalactites
	std::vector<Stalactite> m_vStalac;
	//check amount of Stalactites been made 
	float m_fStalacTimer;
};

