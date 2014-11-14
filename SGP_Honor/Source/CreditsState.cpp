#include "CreditsState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "Font.h"
#include "BitmapFont.h"
#include "Game.h"
#include "GameplayState.h"

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

}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void CreditsState::Exit(void)
{

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
		Game::GetInstance()->RemoveState();
	}
	return true;
}

//////////////////////////////////////////////
// Update
// - Update all game entities
void CreditsState::Update(float elapsedTime)
{

}

/////////////////////////////////////////////
// Render
// - Render all game entities
void CreditsState::Render(void)
{
	////Create a local reference to the input manager for ease of use
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	pGraphics->DrawRectangle({ 0, 0, 7000, 7000 }, { 255, 0, 0, 0 });
	//Local refernce to the font
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");
	//Draw the background
	font.DrawString("HONOR", 200, 0, 2, SGD::Color{ 255, 255, 255, 255 });
	font.DrawString("Creators", 250, 100, 1, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Conor Maloney", 200, 150, 1, SGD::Color{ 255, 255, 255, 255 });
	font.DrawString("Michael Sciortino", 200, 200, 1, SGD::Color{ 255, 255, 255, 255 });
	font.DrawString("Jonathan Cox", 200, 250, 1, SGD::Color{ 255, 255, 255, 255 });
	font.DrawString("Jordan Ness", 200, 300, 1, SGD::Color{ 255, 255, 255, 255 });
	font.DrawString("Luis Garcia", 200, 350, 1, SGD::Color{ 255, 255, 255, 255 });

	font.DrawString("Art", 250, 400, 1, SGD::Color{ 255, 255, 0, 0 });
	font.DrawString("Caris Frazier", 200, 450, 1, SGD::Color{ 255, 255, 255, 255 });
}
