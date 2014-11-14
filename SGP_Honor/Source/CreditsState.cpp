#include "CreditsState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "Font.h"
#include "BitmapFont.h"
#include "Game.h"
#include "GameplayState.h"
#include "Font.h"
#include "BitmapFont.h"

///////////////////////////////////////////////////////////
///////////////////// Singleton Accessor /////////////////

/////////////////////////////////////////////////////////
// GetInstance
// - allocate the static global instance
// - return THE instance
CreditsState* CreditsState::GetInstance(void)
{
	static CreditsState s_Instance; //This is stored ONCE in global memory
	return &s_Instance;
}




//////////////////////////////////////////////////////////////
//////////////////// IGameState Interface ///////////////////



///////////////////////////////////////////////////
// Enter
// - Reset the game
// - Load the resources
// - set up entities
void CreditsState::Enter(void) //Load Resources
{
	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/Honor_Castle.png");
}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void CreditsState::Exit(void)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool CreditsState::Input(void) //Hanlde user Input
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	//Change between windowed and full screen modes
	if(pInput->IsKeyDown(SGD::Key::Alt) && pInput->IsKeyReleased(SGD::Key::Enter))
	{
		SGD::GraphicsManager::GetInstance()->Resize({ Game::GetInstance()->GetScreenWidth(),
			Game::GetInstance()->GetScreenHeight() }, !(Game::GetInstance()->GetWindowed()));
		Game::GetInstance()->SetWindowed(!(Game::GetInstance()->GetWindowed()));
	}

	if(pInput->IsKeyPressed(SGD::Key::Escape)
		|| pInput->IsButtonPressed(0, 1 /*Button B on xbox controller*/))
	{
		Game::GetInstance()->ClearStates();
		//Game::GetInstance()->RemoveState();
	}
	return true;
}

//////////////////////////////////////////////
// Update
// - Update all game entities
void CreditsState::Update(float elapsedTime)
{
	if (CreditTimer < 33.0f)
	{
		CreditTimer += elapsedTime;
		textMovement -= 50 * elapsedTime;
	}
	else
	{
		CreditTimer = 0.0f;
		textMovement = 0.0f;
		Game::GetInstance()->ClearStates();
		//Game::GetInstance()->RemoveState();
	}
}

/////////////////////////////////////////////
// Render
// - Render all game entities
void CreditsState::Render(void)
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackground, { 0, 0 }, 0.0f, {}, {}, { 1.6f, 1.2f });

	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");

	font.DrawString("Developers:", 50, (int)(650 + textMovement), 2, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Jordan Ness", 100, (int)(750 + textMovement), 1, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Jonathan Cox", 100, (int)(800 + textMovement), 1, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Conor Maloney", 100, (int)(850 + textMovement), 1, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Luis Garcia", 100, (int)(900 + textMovement), 1, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Michael Sciortino", 100, (int)(950 + textMovement), 1, SGD::Color{ 255, 255, 0, 0 });

	font.DrawString("EP:", 50, (int)(1050 + textMovement), 2, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("John Oleske", 100, (int)(1150 + textMovement), 1, SGD::Color{ 255, 255, 0, 0 });

	font.DrawString("AP:", 50, (int)(1250 + textMovement), 2, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Sean Hathaway", 100, (int)(1350 + textMovement), 1, SGD::Color{ 255, 255, 0, 0 });

	font.DrawString("Artists:", 50, (int)(1450 + textMovement), 2, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Caris Frazier", 100, (int)(1550 + textMovement), 1, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Gregory Bey", 100, (int)(1600 + textMovement), 1, SGD::Color{ 255, 255, 0, 0 });
}
