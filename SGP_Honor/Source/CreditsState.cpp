#include "CreditsState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Color.h"

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


}
