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
	void DeleteProfile(Profile* profile);


private:

	ProfileState() = default;
	virtual ~ProfileState() = default;

	ProfileState(ProfileState&) = delete;
	ProfileState& operator= (ProfileState&) = delete;

	int m_nCursor = 0;
	int m_nOptionCursor = 0;


	SGD::Rectangle m_rProfile1 = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 265, Game::GetInstance()->GetScreenHeight() / 2 - 170 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rProfile2 = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 265, Game::GetInstance()->GetScreenHeight() / 2 - 100 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rProfile3 = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 265, Game::GetInstance()->GetScreenHeight() / 2 - 30 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rSelect = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 - 290, Game::GetInstance()->GetScreenHeight() / 2 + 160 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rDelete = SGD::Rectangle({ Game::GetInstance()->GetScreenWidth() / 2 + 50 , Game::GetInstance()->GetScreenHeight() / 2 + 160 }, SGD::Size{ 260, 65 });
	SGD::Rectangle m_rSword = SGD::Rectangle(SGD::Point{ 20, 140 }, SGD::Size{ 280, 305 });


	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hSword = SGD::INVALID_HANDLE;
	SGD::HTexture m_hButton = SGD::INVALID_HANDLE;


};

