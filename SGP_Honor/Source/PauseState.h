#pragma once
#include "IGameState.h"
#include "Game.h"
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
	SGD::Rectangle m_rPlay = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 - 60 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rOptions = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 10 }, SGD::Size{ 260, 65 });
	//SGD::Rectangle m_rInstructions = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 80 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rCredits = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 80 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rExit = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 150 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rSword = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 300, Game::GetInstance()->GetScreenHeight() / 2 - 60 }, SGD::Size{ 260, 65 });



	bool m_bMouse = false;

	SGD::Rectangle rLast;
	SGD::Rectangle rMouse;

	bool isAnyButtonPressed();

	SGD::HTexture m_hSword = SGD::INVALID_HANDLE;
	SGD::HTexture m_hButton = SGD::INVALID_HANDLE;

	//For Arcade Machine
	float m_fInputTimer;
	float m_fstickYOff;

};

