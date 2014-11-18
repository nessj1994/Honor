#pragma once
#include "IGameState.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

class Emitter;

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

	void StartAudio() { SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic, true); }
	void StopAudio() { SGD::AudioManager::GetInstance()->StopAudio(m_hMusic); }

private:

	MainMenuState() = default;
	virtual ~MainMenuState() = default;

	MainMenuState(MainMenuState&) = delete;
	MainMenuState& operator= (MainMenuState&) = delete;


	//////////////////////////////////////////////////////////
	///////////////////// Cursor Index //////////////////////
	int m_nCursor = 0;
	
	SGD::HAudio m_hSelection = SGD::INVALID_HANDLE;
	SGD::HAudio m_hMusic = SGD::INVALID_HANDLE;

	//SGD::Rectangle m_rPlay = SGD::Rectangle (100.0f, 400.0f, 520.0f, 600.0f );
	SGD::Rectangle m_rPlay = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 - 60}, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rOptions = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 10 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rInstructions = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 80 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rCredits = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 150 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rExit = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 220 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rSword = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 300, Game::GetInstance()->GetScreenHeight() / 2 - 60 }, SGD::Size{ 260, 65 });


	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hSword = SGD::INVALID_HANDLE;
	SGD::HTexture m_hButton = SGD::INVALID_HANDLE;


	bool m_bMouse = false;

	SGD::Rectangle rLast;
	SGD::Rectangle rMouse;

	//Arcade input Timer
	float m_fstickYOff;
	float m_fInputTimer;

	//Emitter Effects

	Emitter* m_emSelect;
	Emitter* m_emTitle;
	bool m_bContinue;

	bool isAnyButtonPressed();
};

