#include "InstructionsState.h"

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
InstructionsState* InstructionsState::GetInstance(void)
{
	static InstructionsState s_Instance; //This is stored ONCE in global memory
	return &s_Instance;
}




//////////////////////////////////////////////////////////////
//////////////////// IGameState Interface ///////////////////



///////////////////////////////////////////////////
// Enter
// - Reset the game
// - Load the resources
// - set up entities
void InstructionsState::Enter(void) //Load Resources
{

}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void InstructionsState::Exit(void)
{

}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool InstructionsState::Input(void) //Hanlde user Input
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

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
void InstructionsState::Update(float elapsedTime)
{

}

/////////////////////////////////////////////
// Render
// - Render all game entities
void InstructionsState::Render(void)
{


}