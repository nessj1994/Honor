#pragma once
#include "IGameState.h"
class ProfileState :
	public IGameState
{
public:

	///////////////////////////////////////////////////////////
	///////////////////// Singleton Accessor /////////////////
	static ProfileState* GetInstance(void);

	//////////////////////////////////////////////////////////////
	//////////////////// IGameState Interface ///////////////////
	virtual void Enter(void)			 override; //Load Resources
	virtual void Exit(void)				 override; //Unload Resources

	virtual bool Input(void)			 override; //Hanlde user Input
	virtual void Update(float elapsedTime)			 override; //update entities
	virtual void Render(void)			 override; //Render all entities

private:

	ProfileState() = default;
	virtual ~ProfileState() = default;

	ProfileState(ProfileState&) = delete;
	ProfileState& operator= (ProfileState&) = delete;
};

