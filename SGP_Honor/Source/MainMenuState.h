#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class MainMenuState :
	public IGameState
{
public:

	///////////////////////////////////////////////////////////
	///////////////////// Singleton Accessor /////////////////
	static MainMenuState* GetInstance(void);

	//////////////////////////////////////////////////////////////
	//////////////////// IGameState Interface ///////////////////
	virtual void Enter(void)			 override; //Load Resources
	virtual void Exit(void)				 override; //Unload Resources

	virtual bool Input(void)			 override; //Hanlde user Input
	virtual void Update(float elapsedTime)			 override; //update entities
	virtual void Render(void)			 override; //Render all entities

private:

	MainMenuState() = default;
	virtual ~MainMenuState() = default;

	MainMenuState(MainMenuState&) = delete;
	MainMenuState& operator= (MainMenuState&) = delete;


	//////////////////////////////////////////////////////////
	///////////////////// Cursor Index //////////////////////
	int m_nCursor = 0;
	

	//SGD::Rectangle m_rPlay = SGD::Rectangle (100.0f, 400.0f, 520.0f, 600.0f );
	SGD::Rectangle m_rPlay = SGD::Rectangle(SGD::Point(450.0f, 255.0f), SGD::Point(520.0f, 287.0f));
	SGD::Rectangle m_rOptions = SGD::Rectangle(SGD::Point(450.0f, 305.0f), SGD::Point(562.0f, 337.0f));
	SGD::Rectangle m_rInstructions = SGD::Rectangle(SGD::Point(450.0f, 355.0f), SGD::Point(622.0f, 387.0f));
	SGD::Rectangle m_rCredits = SGD::Rectangle(SGD::Point(450.0f, 405.0f), SGD::Point(562.0f, 437.0f));
	SGD::Rectangle m_rExit = SGD::Rectangle(SGD::Point(450.0f, 455.0f), SGD::Point(520.0f, 487.0f));




};

