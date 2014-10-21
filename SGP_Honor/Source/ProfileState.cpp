#include "ProfileState.h"

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
ProfileState* ProfileState::GetInstance(void)
{
	static ProfileState s_Instance; //This is stored ONCE in global memory
	return &s_Instance;
}




//////////////////////////////////////////////////////////////
//////////////////// IGameState Interface ///////////////////



///////////////////////////////////////////////////
// Enter
// - Reset the game
// - Load the resources
// - set up entities
void ProfileState::Enter(void) //Load Resources
{

}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void ProfileState::Exit(void)
{

}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool ProfileState::Input(void) //Hanlde user Input
{

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if(pInput->IsKeyPressed(SGD::Key::Escape))
	{
		Game::GetInstance()->RemoveState();
	}

	return true;
}

//////////////////////////////////////////////
// Update
// - Update all game entities
void ProfileState::Update(float elapsedTime)
{

}

/////////////////////////////////////////////
// Render
// - Render all game entities
void ProfileState::Render(void)
{


}
