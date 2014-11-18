#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Game.h"

class Emitter;

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


	int m_unCursor = 0;

	SGD::HAudio m_hSelection = SGD::INVALID_HANDLE;

	SGD::HAudio m_hWorld1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWorld2 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWorld3 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWorld4 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWorld5 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hFinalBoss = SGD::INVALID_HANDLE;

	SGD::HAudio m_hBGM = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hSword = SGD::INVALID_HANDLE;
	SGD::HTexture m_hButton = SGD::INVALID_HANDLE;
	SGD::HTexture m_hEsc = SGD::INVALID_HANDLE;
	SGD::HTexture m_hCircle = SGD::INVALID_HANDLE;
	

	SGD::Rectangle m_rPlay = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 - 60 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rOptions = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 10 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rInstructions = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 130, Game::GetInstance()->GetScreenHeight() / 2 + 80 }, SGD::Size{ 260, 65 });


	unsigned int m_unMusicVol = 100;
	unsigned int m_unEffectsVol = 100;
	bool m_bFullScreen = !Game::GetInstance()->GetWindowed();

	//Arcade input Timer
	float m_fstickYOff;
	float m_fstickXoff;
	float m_fInputTimer;

	//Emitter
	Emitter* m_emBackgroundEffect;
	Emitter* m_emTitle;
};

