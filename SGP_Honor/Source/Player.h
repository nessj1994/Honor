#pragma once


#include "Unit.h"
#include <vector>

#include "../SGD Wrappers/SGD_Listener.h"

class Hawk;
class Ice;
class Bounce;
class Dash;
class Emitter;
class SwordSwing;

class Player :
	public Unit, SGD::Listener
{
public:
	Player();
	virtual ~Player();

	enum BlockStates { RESTING_STATE, JUMPING_STATE, FALLING_STATE, LANDING_STATE, STICK_STATE };


	/////////////////////////////////////////////////
	/////////////////// Interface //////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_PLAYER; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;


	SGD::Rectangle GetSwingRect(void) const { return swingRect; }

	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);

	////////////////////////////////////////////////
	/////////////////////Update methods/////////////
	void UpdateDeath(float elapsedTime);
	void UpdateArmor(float elapsedTime);
	void UpdateEmitters(float elapsedTime);
	void UpdateTimers(float elapsedTime);
	void UpdateFriction(float elapsedTime, bool leftClamped);
	void UpdateBounce(float elapsedTime);
	void UpdateMovement(float elapsedTime, int stickFrame, bool leftClamped, float leftStickXOff);
	void UpdateDash(float elapsedTime);
	void UpdateJump(float elapsedTime);
	void UpdateHawk(float elapsedTime);
	void UpdateSpray(float elapsedTime);
	void UpdateConstants(float elapsedTime);
	void UpdateVelocity(float elapsedTime);
	void UpdatePlayerSwing(float elapsedTime);

	////////////////////////////////////////////////
	/////////////////////Methods///////////////////
	void CastDash();
	void CastHawk();
	void CastIce();
	void CastBounce();
	void BasicCollision(const IEntity* pOther) override;
	void LeftRampCollision(const IEntity* pOther);
	void RightRampCollision(const IEntity* pOther);
	void GeyserCollision(const IEntity* pOther);
	void LaserCollision(const IEntity* pOther);
	void SwingCollision(const IEntity* pOther);

	unsigned int GetCurrentState(void) const { return m_unCurrentState; }
	void JellyfishCollision(const IEntity* pOther);
	void KillPlayer();
	void ThrowPlayer(bool _right);


	//SGD::Rectangle GetWallRect(void) const;

	//////////////////////////////////////////////
	//////////////////Accessors//////////////////
	//Bools
	bool HasDash(void) const { return m_bHasDash; }
	bool HasHawk(void) const { return m_bHasHawk; }
	bool HasIce(void) const { return m_bHasIce; }
	bool HasBounce(void) const { return m_bHasBounce; }
	bool HasArmor(void) const { return m_bHasArmor; }
//	bool GetIsJumping(void)const { return is_Jumping; }
//	bool GetIsFalling(void)const { return is_Falling; }
	bool GetIsInputStuck(void)const { return is_Stuck; }
	bool GetDead(void) const { return m_bDead; }
	bool GetIsSwinging(void) const { return is_Swinging; }
	bool GetStunned(void) const { return m_bStunned; }

	//Floats
	float GetDashTime(void) const { return m_fDashTime; }
	//float GetJumpCapTime(void) const { return m_fJumpVelCap; }
	float GetJumpVelCur(void) const { return m_fJumpVelCur; }
	float GetDeathTimer(void) const { return m_fDeathTimer; }

	//Abilities
	Dash* GetDash(void) const { return m_pDash; }
	Ice* GetIce(void) const { return m_pIce; }
	Bounce* GetBounce(void) const { return m_pBounce; }
	SwordSwing* GetSword(void) const { return m_pSword; }

	//Unsigned ints
	unsigned int GetHonorCollected(void) const { return m_unHonorCollected; }

	////////////////////////////////////////////
	/////////////////Mutators//////////////////
	void SetStunnded(bool _stun) { m_bStunned = _stun; }
	void SetHasArmor(bool armor) { m_bHasArmor = armor; }
	void SetHasDash(bool dash) { m_bHasDash = dash; }
	void SetHasIce(bool ice) { m_bHasIce = ice; }
	void SetHasBounce(bool bounce) { m_bHasBounce = bounce; }
	void SetDashTimer(float timer) { m_fDashTime = timer; }
	void SetDash(Dash& dash) {};
	void SetIce(Ice& ice) {};
	void SetBounce(Bounce& bounce) {};
	//void SetIsJumping(bool _jump) { is_Jumping = _jump; }
	//void SetIsFalling(bool _fall) { is_Falling = _fall; }
	void SetIsInputStuck(bool _stuck) { is_Stuck = _stuck; }
	void SetHonorCollected(unsigned int honor) { m_unHonorCollected = honor; }
	void SetSword(SwordSwing* ptr) { m_pSword = ptr; }

	//void SetJumpCapTime(float _capTime) { m_fJumpVelCap = _capTime; }
	void SetJumpVelCur(float _velCur) { m_fJumpVelCur = _velCur; }
	void IncreaseHonorCollected(unsigned int amount) { m_unHonorCollected += amount; }

	//Hawk Explosion function
	void HawkExplode(SGD::Point _pos);
private:


	bool m_bHasDash = false;
	bool m_bHasHawk = false;
	bool m_bHasIce = false;
	bool m_bHasBounce = false;
	bool m_bHasArmor = false;
	bool m_bHawkCast = false;


	bool is_Jumping = false;
	bool is_Falling = true;
	bool has_landed = true;
	bool m_bSliding = false;


	bool is_Stuck = false;
	bool is_Left_Coll = false;
	bool is_Right_Coll = false;
	bool is_Platform = false;
	bool is_Ramp = false;
	bool m_bDead = false;
	bool is_Swinging = false;
	bool m_bStunned = false;

	float m_fDashTime = 0.0f;
	float m_fIceTimer = 0.0f;
	//float m_fJumpVelCap = 250;
	float m_fJumpVelCur = 0;
	float m_fJumpTimer = 0.4f;
	float m_fLandTimer = 0.0f;
	float m_fButtonTimer = 0.0f;
	float m_fDeathTimer = 0.0f;
	float m_fArmorTimer = 0.0f;
	float m_fStunTimer = 0.0f;

	float m_fShotTimer = 0.20f;
	float m_fInputTimer = 0.0f;
	float m_fHawkTimer = 1.0f;
	float m_fSwingTimer = 0.0f;

	unsigned int m_unCurrentState = 0;
	unsigned int m_unHonorCollected = 0;

	unsigned int m_unJumpCount = 0;

	//Hawk* m_pHawk;
	Dash* m_pDash;
	Ice* m_pIce;
	Bounce* m_pBounce;
	SwordSwing* m_pSword;

	SGD::HTexture m_hImage = SGD::INVALID_HANDLE;
	//Honor Particle Image for the HUD
	SGD::HTexture m_hHonorParticleHUD = SGD::INVALID_HANDLE;

	//Emitters
	//Honor emitter for HUD
	Emitter* m_emHonor;
	SGD::Rectangle swingRect = {0,0,0,0};
	//Hawk Explosion 
	Emitter* m_emFeatherExplosion;
	bool m_bHawkExplode = false;
	//Hawk Return
	Emitter* m_emHawkReturn;
	bool m_bReturningHawk = false;

	//Sound Effects
	SGD::HAudio m_hIceEffect = SGD::INVALID_HANDLE;
	SGD::HAudio m_hBounceEffect = SGD::INVALID_HANDLE;

};


