#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"

class OptionsState :
	public IGameState
{
public:
	///////////////////////////////////////////////////////////
	///////////////////// Singleton Accessor /////////////////
	static OptionsState* GetInstance(void);

	//////////////////////////////////////////////////////////////
	//////////////////// IGameState Interface ///////////////////
	virtual void Enter(void)			 override; //Load Resources
	virtual void Exit(void)				 override; //Unload Resources

	virtual bool Input(void)			 override; //Hanlde user Input
	virtual void Update(float elapsedTime)			 override; //update entities
	virtual void Render(void)			 override; //Render all entities


private:
	OptionsState() = default;
	virtual ~OptionsState() = default;

	OptionsState(const OptionsState&)				= delete;
	OptionsState& operator= (const OptionsState&)	= delete;


	unsigned int m_unCursor = 0;

	SGD::HAudio m_hBGM = SGD::INVALID_HANDLE;
	
	unsigned int m_unMusicVol = 100;
	unsigned int m_unEffectsVol = 100;

};

