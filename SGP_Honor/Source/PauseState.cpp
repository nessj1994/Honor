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

	m_hSword = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/SwordButton.png");
	m_hButton = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/Honor_Buttons.png");
	m_fstickYOff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).y;

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	rMouse = SGD::Rectangle({ pInput->GetMousePosition().x, pInput->GetMousePosition().y, pInput->GetMousePosition().x + 1, pInput->GetMousePosition().y + 1 });
	rLast = rMouse;

}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void PauseState::Exit(void)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSword);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hButton);
}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool PauseState::Input(void)
{

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	rLast = rMouse;

	rMouse = SGD::Rectangle({ pInput->GetMousePosition().x, pInput->GetMousePosition().y, pInput->GetMousePosition().x + 1, pInput->GetMousePosition().y + 1 });




	if((pInput->IsAnyKeyPressed() || isAnyButtonPressed()) && !pInput->IsKeyPressed(SGD::Key::MouseLeft))
	{
		m_bMouse = false;
	}
	else if(rMouse != rLast)
	{
		m_bMouse = true;
	}

	if(m_fInputTimer > .1f)
	{
		m_fstickYOff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).y;
		m_fInputTimer = 0;
	}

	m_fInputTimer += .0025f;

	//Move the cursor down
	if(pInput->IsKeyPressed(SGD::Key::Down)
		|| pInput->IsDPadPressed(0, SGD::DPad::Down) || m_fstickYOff > 0)
	{
		if(m_fstickYOff > 0)
		{
			m_fstickYOff = 0;
		}
		m_nCursor += 1;

		if(m_nCursor > 4)
		{
			m_nCursor = 4;
		}
	}
	//Move the cursor up
	else if(pInput->IsKeyPressed(SGD::Key::Up)
		|| pInput->IsDPadPressed(0, SGD::DPad::Up) || m_fstickYOff < 0)
	{
		if(m_fstickYOff < 0)
		{
			m_fstickYOff = 0;
		}
		m_nCursor -= 1;

		if(m_nCursor < 0)
		{
			m_nCursor = 0;
		}


	}

	if(m_bMouse)
	{
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
		if(m_rCredits.IsIntersecting(rMouse))
		{
			m_nCursor = 2;

			if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
			{
				Game::GetInstance()->AddState(CreditsState::GetInstance());

			}
		}
		if(m_rExit.IsIntersecting(rMouse))
		{
			m_nCursor = 3;

			if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
			{
				Game::GetInstance()->ClearStates();
				return true;

			}
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
			//Change state to the credits state
			Game::GetInstance()->AddState(CreditsState::GetInstance());
		}
		else if(m_nCursor == 3)
		{
			//return to main menu
			Game::GetInstance()->ClearStates();
		}
	}



	if(pInput->IsKeyPressed(SGD::Key::Escape)
		|| pInput->IsButtonPressed(0, 1 /*Button B on xbox controller*/))
	{

		m_nCursor = 3;


		//	GameplayState::GetInstance()->ResetAudio();
		//
		//	Game::GetInstance()->RemoveState();
	}


	if(m_nCursor == 0)
	{
		m_rSword.top = m_rPlay.top + 10;
	}
	else if(m_nCursor == 1)
	{
		m_rSword.top = m_rOptions.top + 10;
	}
	else if(m_nCursor == 2)
	{
		m_rSword.top = m_rCredits.top + 10;
	}
	else if(m_nCursor == 3)
	{
		m_rSword.top = m_rExit.top + 10;
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
	//Create a local reference to the input manager for ease of use
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	//Local refernce to the font
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");

	float fWidth = Game::GetInstance()->GetScreenWidth();

	GameplayState::GetInstance()->Render();

	float windowWidth = Game::GetInstance()->GetScreenWidth();
	float windowHeight = Game::GetInstance()->GetScreenHeight();

	SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle(0.0f, 0.0f, windowWidth, windowHeight), SGD::Color(150, 255, 255, 255), {}, {});
	pGraphics->DrawTexture(m_hSword, { m_rSword.left, m_rSword.top }, 0.0f, {}, {}, { 1.4f, 1.4f });
	//Change the highlighted options color to red dependent on the cursor
	if(m_nCursor == 0)
	{
		pGraphics->DrawRectangle(m_rPlay, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rPlay.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });

		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 240 }, 0.0f, {}, { 255, 255, 255, 255 });



		font.DrawString("Play", (int)((fWidth - (4 * 19)) / 2), 250, 1, SGD::Color{ 255, 255, 165, 0 });

	}
	else
	{
		pGraphics->DrawRectangle(m_rPlay, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 240 }, 0.0f, {}, { 255, 255, 255, 255 });

		//pGraphics->DrawTexture(m_hSword, { m_rPlay.left - 50, m_rPlay.top }, 0.0f, {}, {}, { 1.4f, 1.4f });

		font.DrawString("Play", (int)((fWidth - (4 * 19)) / 2), 250, 1, SGD::Color{ 255, 255, 165, 0 });
	}


	if(m_nCursor == 1)
	{
		pGraphics->DrawRectangle(m_rOptions, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rOptions.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 310 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Options", (int)(fWidth - (7 * 15)) / 2, 320, 1, SGD::Color{ 255, 255, 165, 0 });
	}
	else
	{
		pGraphics->DrawRectangle(m_rOptions, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 310 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Options", (int)(fWidth - (7 * 15)) / 2, 320, 1, SGD::Color{ 255, 255, 165, 0 });

	}

	if(m_nCursor == 2)
	{
		pGraphics->DrawRectangle(m_rCredits, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rCredits.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 380 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Credits", (int)((fWidth - (7 * 14)) / 2), 390, 1, SGD::Color{ 255, 255, 165, 0 });
	}
	else
	{
		pGraphics->DrawRectangle(m_rCredits, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 380 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Credits", (int)((fWidth - (7 * 14)) / 2), 390, 1, SGD::Color{ 255, 255, 165, 0 });

	}


	if(m_nCursor == 3)
	{
		pGraphics->DrawRectangle(m_rExit, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rExit.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 450 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Exit", (int)((fWidth - (4 * 18)) / 2), 460, 1, SGD::Color{ 255, 255, 165, 0 });
	}
	else
	{
		pGraphics->DrawRectangle(m_rExit, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 450 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Exit", (int)((fWidth - (4 * 18)) / 2), 460, 1, SGD::Color{ 255, 255, 165, 0 });

		//font.DrawString("Exit", 450, 450, 1, SGD::Color{ 255, 0, 0, 255 });
	}


}

bool PauseState::isAnyButtonPressed()
{

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	//Check for controller input
	if(pInput->IsButtonPressed(0, 0) ||
		pInput->IsButtonPressed(0, 1) ||
		pInput->IsButtonPressed(0, 2) ||
		pInput->IsButtonPressed(0, 3) ||
		pInput->IsButtonPressed(0, 4) ||
		pInput->IsButtonPressed(0, 5) ||
		pInput->IsButtonPressed(0, 6) ||
		pInput->IsButtonPressed(0, 7) ||
		pInput->IsButtonPressed(0, 8) ||
		pInput->IsButtonPressed(0, 9) ||
		pInput->IsButtonPressed(0, 10) ||
		pInput->IsButtonPressed(0, 11) ||
		pInput->IsDPadPressed(0, SGD::DPad::Up) ||
		pInput->IsDPadPressed(0, SGD::DPad::Left) ||
		pInput->IsDPadPressed(0, SGD::DPad::Right) ||
		pInput->IsDPadPressed(0, SGD::DPad::Down) ||
		pInput->GetLeftJoystick(0).x < -0.2 ||
		pInput->GetLeftJoystick(0).x > 0.2 ||
		pInput->GetRightJoystick(0).x < -0.2 ||
		pInput->GetRightJoystick(0).x > 0.2 ||
		pInput->GetLeftJoystick(0).y < -0.2 ||
		pInput->GetLeftJoystick(0).y > 0.2 ||
		pInput->GetRightJoystick(0).y < -0.2 ||
		pInput->GetRightJoystick(0).y > 0.2 ||
		pInput->GetTrigger(0) > 0 ||
		pInput->GetTrigger(0) < 0
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}