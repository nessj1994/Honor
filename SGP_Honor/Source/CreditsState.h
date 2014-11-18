#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"

class Emitter;

class CreditsState :
	public IGameState
{
public:

	///////////////////////////////////////////////////////////
	///////////////////// Singleton Accessor /////////////////
	static CreditsState* GetInstance(void);

	//////////////////////////////////////////////////////////////
	//////////////////// IGameState Interface ///////////////////
	virtual void Enter(void)			 override; //Load Resources
	virtual void Exit(void)				 override; //Unload Resources

	virtual bool Input(void)			 override; //Hanlde user Input
	virtual void Update(float elapsedTime)			 override; //update entities
	virtual void Render(void)			 override; //Render all entities

private:

	CreditsState() = default;
	virtual ~CreditsState() = default;

	CreditsState(CreditsState&) = delete;
	CreditsState& operator= (CreditsState&) = delete;

	float CreditTimer = 0.0f;
	float textMovement = 0.0f;
	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;

	//Emitter
	Emitter* m_emBackgroundEffect;
};

