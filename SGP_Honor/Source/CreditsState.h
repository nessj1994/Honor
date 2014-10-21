#pragma once
#include "IGameState.h"
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
};

