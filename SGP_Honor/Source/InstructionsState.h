#pragma once
#include "IGameState.h"
class InstructionsState :
	public IGameState
{
public:
	///////////////////////////////////////////////////////////
	///////////////////// Singleton Accessor /////////////////
	static InstructionsState* GetInstance(void);

	//////////////////////////////////////////////////////////////
	//////////////////// IGameState Interface ///////////////////
	virtual void Enter(void)			 override; //Load Resources
	virtual void Exit(void)				 override; //Unload Resources

	virtual bool Input(void)			 override; //Hanlde user Input
	virtual void Update(float elapsedTime)			 override; //update entities
	virtual void Render(void)			 override; //Render all entities

private:


	InstructionsState() = default;
	virtual ~InstructionsState() = default;

	InstructionsState(InstructionsState&) = delete;
	InstructionsState& operator= (InstructionsState&) = delete;
};

