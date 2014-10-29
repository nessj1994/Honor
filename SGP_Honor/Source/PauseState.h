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
	SGD::Rectangle m_rPlay = SGD::Rectangle(SGD::Point(450.0f, 255.0f), SGD::Point(520.0f, 287.0f));
	SGD::Rectangle m_rOptions = SGD::Rectangle(SGD::Point(450.0f, 305.0f), SGD::Point(562.0f, 337.0f));
	SGD::Rectangle m_rInstructions = SGD::Rectangle(SGD::Point(450.0f, 355.0f), SGD::Point(622.0f, 387.0f));
	SGD::Rectangle m_rCredits = SGD::Rectangle(SGD::Point(450.0f, 405.0f), SGD::Point(562.0f, 437.0f));
	SGD::Rectangle m_rExit = SGD::Rectangle(SGD::Point(450.0f, 455.0f), SGD::Point(520.0f, 487.0f));


	SGD::HTexture m_hSword = SGD::INVALID_HANDLE;
	SGD::HTexture m_hButton = SGD::INVALID_HANDLE;
};

