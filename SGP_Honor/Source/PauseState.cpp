#include "PauseState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "GameplayState.h"
#include "OptionsState.h"
#include "InstructionsState.h"
#include "CreditsState.h"
#include "Font.h"
#include "BitmapFont.h"

///////////////////////////////////////////////////////////
///////////////////// Singleton Accessor /////////////////

/////////////////////////////////////////////////////////
// GetInstance
// - allocate the static global instance
// - return THE instance
PauseState* PauseState::GetInstance(void)
{
	static PauseState s_Instance;
	return &s_Instance;
}

//////////////////////////////////////////////////////////////
//////////////////// IGameState Interface ///////////////////


///////////////////////////////////////////////////
// Enter
// - Reset the game
// - Load the resources
// - set up entities
void PauseState::Enter(void)
{
	//Set the background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 50, 50, 50, 50 });	// dark gray
	m_nCursor = 0;
}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void PauseState::Exit(void)
{

}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool PauseState::Input(void)
{

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	SGD::Rectangle rMouse = SGD::Rectangle({ pInput->GetMousePosition().x, pInput->GetMousePosition().y, pInput->GetMousePosition().x + 1, pInput->GetMousePosition().y + 1 });

	//Move the cursor down
	if(pInput->IsKeyPressed(SGD::Key::Down)
		|| pInput->IsDPadPressed(0,SGD::DPad::Down))
	{
		m_nCursor += 1;

		if(m_nCursor > 4)
		{
			m_nCursor = 4;
		}
	}
	//Move the cursor up
	else if(pInput->IsKeyPressed(SGD::Key::Up)
		|| pInput->IsDPadPressed(0, SGD::DPad::Up))
	{
		m_nCursor -= 1;

		if(m_nCursor < 0)
		{
			m_nCursor = 0;
		}


	}


	if(m_rPlay.IsIntersecting(rMouse))
	{
		m_nCursor = 0;

		if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			Game::GetInstance()->RemoveState();


		}
	}
	if(m_rOptions.IsIntersecting(rMouse))
	{
		m_nCursor = 1;

		if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			Game::GetInstance()->AddState(OptionsState::GetInstance());

		}
	}
	if(m_rInstructions.IsIntersecting(rMouse))
	{
		m_nCursor = 2;

		if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			Game::GetInstance()->AddState(InstructionsState::GetInstance());

		}
	}
	if(m_rCredits.IsIntersecting(rMouse))
	{
		m_nCursor = 3;

		if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			Game::GetInstance()->AddState(CreditsState::GetInstance());

		}
	}
	if(m_rExit.IsIntersecting(rMouse))
	{
		m_nCursor = 4;

		if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			Game::GetInstance()->ClearStates();


		}
	}


	//Change between windowed and full screen modes
	if(pInput->IsKeyDown(SGD::Key::Alt) && pInput->IsKeyReleased(SGD::Key::Enter))
	{
		SGD::GraphicsManager::GetInstance()->Resize({ Game::GetInstance()->GetScreenWidth(),
			Game::GetInstance()->GetScreenHeight() }, !(Game::GetInstance()->GetWindowed()));
		Game::GetInstance()->SetWindowed(!(Game::GetInstance()->GetWindowed()));
	}
	//Select the current menu option
	else if((pInput->IsKeyPressed(SGD::Key::Enter) && !(pInput->IsKeyDown(SGD::Key::Alt)))
		|| pInput->IsButtonPressed(0, 0 /*Button A on xbox controller */))
	{
		if(m_nCursor == 0)
		{
			//Changestate to gameplay state
			GameplayState::GetInstance()->ResetAudio();
			Game::GetInstance()->RemoveState();
		}
		else if(m_nCursor == 1)
		{
			//Change State to options state
			Game::GetInstance()->AddState(OptionsState::GetInstance());
		}
		else if(m_nCursor == 2)
		{
			//change state to instructions state
			Game::GetInstance()->AddState(InstructionsState::GetInstance());
		}
		else if(m_nCursor == 3)
		{
			//Change state to the credits state
			Game::GetInstance()->AddState(CreditsState::GetInstance());
		}
		else if(m_nCursor == 4)
		{
			//return to main menu
			Game::GetInstance()->ClearStates();
		}
	}

	

	if(pInput->IsKeyPressed(SGD::Key::Escape)
		|| pInput->IsButtonPressed(0, 1 /*Button B on xbox controller*/))
	{
		GameplayState::GetInstance()->ResetAudio();

		Game::GetInstance()->RemoveState();
	}

	return true;
}


//////////////////////////////////////////////
// Update
// - Update all game entities
void PauseState::Update(float elapsedTime)
{

}

/////////////////////////////////////////////
// Render
// - Render all game entities
void PauseState::Render(void)
{
	//Local refernce to the font
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");

	GameplayState::GetInstance()->Render();

	float windowWidth = Game::GetInstance()->GetScreenWidth();
	float windowHeight = Game::GetInstance()->GetScreenHeight();

	SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle(0.0f, 0.0f, windowWidth, windowHeight), SGD::Color(150, 255, 255, 255), {}, {});

	//Draw Pause
	font.DrawString("Pause", 370, 100, 3, SGD::Color{ 255, 255, 0, 0 });

	//Change the highlighted options color to red dependent on the cursor
	if(m_nCursor == 0)
	{
		font.DrawString("Play", 450, 250, 1, SGD::Color{ 255, 255, 0, 0 });
	}
	else
	{
		font.DrawString("Play", 450, 250, 1, SGD::Color{ 255, 0, 0, 255 });
	}


	if(m_nCursor == 1)
	{
		font.DrawString("Options", 450, 300, 1, SGD::Color{ 255, 255, 0, 0 });
	}
	else
	{
		font.DrawString("Options", 450, 300, 1, SGD::Color{ 255, 0, 0, 255 });
	}


	if(m_nCursor == 2)
	{
		font.DrawString("Instructions", 450, 350, 1, SGD::Color{ 255, 255, 0, 0 });
	}
	else
	{
		font.DrawString("Instructions", 450, 350, 1, SGD::Color{ 255, 0, 0, 255 });
	}


	if(m_nCursor == 3)
	{
		font.DrawString("Credits", 450, 400, 1, SGD::Color{ 255, 255, 0, 0 });
	}
	else
	{
		font.DrawString("Credits", 450, 400, 1, SGD::Color{ 255, 0, 0, 255 });
	}


	if(m_nCursor == 4)
	{
		font.DrawString("Exit", 450, 450, 1, SGD::Color{ 255, 255, 0, 0 });
	}
	else
	{
		font.DrawString("Exit", 450, 450, 1, SGD::Color{ 255, 0, 0, 255 });
	}
}