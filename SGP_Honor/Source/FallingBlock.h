#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class FallingBlock :
	public Entity, public SGD::Listener
{
public:
	FallingBlock();
	virtual ~FallingBlock();

	enum BlockStates {RESTING_STATE, FALLING_STATE, RISING_STATE};

	/////////////////////////////////////////////////
	////////////////// Interface ///////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);

	//////////////////////////////////////////////
	////////////////// Accessors ////////////////
	SGD::Point GetStartPosition(void) const { return m_ptStartPosition; }
	SGD::Point GetPosition(void) const { return m_ptPosition; }
	Entity* GetTarget(void) const { return m_pTarget; }
	unsigned int GetCurrentState(void) const { return m_unCurrentState; }
	float GetFallSpeed(void) const { return m_fFallSpeed; }
	float GetRiseSpeed(void) const { return m_fRiseSpeed; }

	////////////////////////////////////////////
	//////////////// Mutators /////////////////
	void SetStartPosition(SGD::Point startPos) { m_ptStartPosition = startPos; }
	void SetPosition(SGD::Point position) { m_ptPosition = position; }
	void SetTarget(Entity* newTarget){ if(m_pTarget)m_pTarget->Release(); m_pTarget = newTarget; if(m_pTarget)m_pTarget->AddRef(); }
	void SetFallSpeed(float fallSpeed) { m_fFallSpeed = fallSpeed; }
	void SetRiseSpeed(float riseSpeed) { m_fRiseSpeed = riseSpeed; }


private:

	SGD::Point m_ptStartPosition = { 0, 0 };
	SGD::Point m_ptPosition = m_ptStartPosition;
	SGD::Size m_szSize = { 32, 32 };
	Entity* m_pTarget = nullptr;
	unsigned int m_unCurrentState = 0;
	float m_fFallSpeed = 100.0f;
	float m_fRiseSpeed = 100.0f;


};

