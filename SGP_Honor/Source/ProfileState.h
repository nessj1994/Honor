#pragma once
#include "IGameState.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"



class Profile;

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

	void LoadProfile(Profile* profile);

private:

	ProfileState() = default;
	virtual ~ProfileState() = default;

	ProfileState(ProfileState&) = delete;
	ProfileState& operator= (ProfileState&) = delete;

	unsigned int m_nCursor = 0;



	SGD::Rectangle m_rProfile1 = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 265, Game::GetInstance()->GetScreenHeight() / 2 - 60 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rProfile2 = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 265, Game::GetInstance()->GetScreenHeight() / 2 + 10 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rProfile3 = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 265, Game::GetInstance()->GetScreenHeight() / 2 + 80 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rCredits = SGD::Rectangle(SGD::Point(384, 450.0f), SGD::Point(640, 514.0f));
	SGD::Rectangle m_rExit = SGD::Rectangle(SGD::Point(384, 520.0f), SGD::Point(640, 584.0f));
	SGD::Rectangle m_rSword = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 265, Game::GetInstance()->GetScreenHeight() / 2 - 60 }, SGD::Size{ 260, 65 });


	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hSword = SGD::INVALID_HANDLE;
	SGD::HTexture m_hButton = SGD::INVALID_HANDLE;
};

