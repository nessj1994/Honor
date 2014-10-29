#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"

class PauseState :
	public IGameState
{
public:

	///////////////////////////////////////////////////////////
	///////////////////// Singleton Accessor /////////////////
	static PauseState* GetInstance(void);

	//////////////////////////////////////////////////////////////
	//////////////////// IGameState Interface ///////////////////
	virtual void Enter(void)			 override; //Load Resources
	virtual void Exit(void)				 override; //Unload Resources

	virtual bool Input(void)			 override; //Hanlde user Input
	virtual void Update(float elapsedTime)			 override; //update entities
	virtual void Render(void)			 override; //Render all entities

private:

	PauseState() = default;
	~PauseState() = default;

	PauseState(PauseState&) = delete;
	PauseState& operator= (PauseState&) = delete;

	//////////////////////////////////////////////////////////
	///////////////////// Cursor Index //////////////////////
	int m_nCursor = 0;

	//Mouse indices
	SGD::Rectangle m_rPlay = SGD::Rectangle(SGD::Point(384, 240.0f), SGD::Point(640.0f, 304.0f));
	SGD::Rectangle m_rOptions = SGD::Rectangle(SGD::Point(384, 310.0f), SGD::Point(640, 374.0f));
	SGD::Rectangle m_rInstructions = SGD::Rectangle(SGD::Point(384, 380.0f), SGD::Point(640, 444.0f));
	SGD::Rectangle m_rCredits = SGD::Rectangle(SGD::Point(384, 450.0f), SGD::Point(640, 514.0f));
	SGD::Rectangle m_rExit = SGD::Rectangle(SGD::Point(384, 520.0f), SGD::Point(640, 584.0f));
	SGD::Rectangle m_rSword = SGD::Rectangle(SGD::Point(216, 250), SGD::Point(344, 272));


	SGD::HTexture m_hSword = SGD::INVALID_HANDLE;
	SGD::HTexture m_hButton = SGD::INVALID_HANDLE;
};

