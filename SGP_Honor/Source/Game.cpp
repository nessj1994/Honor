/**************************************************************************
|	File:	Game.cpp
|	Author: Jordan Ness
|	Purpose: The Game class handles all of the game logic and SGD Wrappers
**************************************************************************/

#include "Game.h"
#include "GameplayState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "IGameState.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "BitmapFont.h"

#include <ctime>
#include <cstdlib>


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


///////////////////////////////////////////
/////////// singleton methods ////////////

//Instantiate the static member for the games instance
Game* Game::s_pInstance = nullptr;

//Allocate ONE instance and return it
Game* Game::GetInstance(void)
{
	if(s_pInstance == nullptr)
	{
		s_pInstance = new Game;
	}

	return s_pInstance;
}

//Deallocate ONE instance
void Game::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

//Initialize the SGD Wrappers
//Enter the initial state (Main Menu State)
bool Game::Initialize(float width, float height)
{
	//Seed random first
	srand((unsigned int)time(nullptr));
	rand();

	m_fScreenHeight = height;
	m_fScreenWidth = width;

	//Initialize the wrappers
	if(SGD::AudioManager::GetInstance()->Initialize() == false
		|| SGD::GraphicsManager::GetInstance()->Initialize(true) == false
		|| SGD::InputManager::GetInstance()->Initialize() == false)
	{
		return false;
	}


	m_fScreenWidth = width ;
	m_fScreenHeight = height;

	//Initialize the font
	m_pFont = new BitmapFont();
	m_pFont->LoadFont("Assets/HonorFont.fnt");

	//Start the game in the proper state
	AddState(MainMenuState::GetInstance());

	m_ulGameTime = GetTickCount();

	return true;
}


int Game::Update()
{
	//Update the SGD Wrappers
	if(SGD::AudioManager::GetInstance()->Update() == false
		|| SGD::GraphicsManager::GetInstance()->Update() == false
		|| SGD::InputManager::GetInstance()->Update() == false)
	{
		return -10; //Exit the game due to FAILURE
	}

	//Calculate the elapsed time between frames
	unsigned long now = GetTickCount();						//Current time in milliseconds
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;		//Convert to a fraction of a second
	m_ulGameTime = now;										//Store the current time

	//Cap the elapsed time to 1/8th of a second
	if(elapsedTime > 0.0125f)
	{
		elapsedTime = 0.0125f;
	}

	//Let the current state handle input
	int pCurrent = m_nCurrentState;

	if(m_vGameStates[m_nCurrentState]->Input() == false)
	{
		return 1; //exit success
	}

	//Update and render the current state if it wasnt changed
	if(m_nCurrentState == pCurrent)
	{
		m_vGameStates[m_nCurrentState]->Update(elapsedTime);
		m_vGameStates[m_nCurrentState]->Render();

	}


	return 0;
}

void Game::Terminate(void)
{
	//Exit the current state
	for(unsigned int i = 0; i < m_vGameStates.size(); i++)
	{
		m_vGameStates[i]->Exit();
	}
	m_vGameStates.clear();
	delete m_pFont;

	delete m_pProfile1;
	delete m_pProfile2;
	delete m_pProfile3;



	//Terminate the core SGD Wrappers
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::DeleteInstance();

	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::DeleteInstance();

	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::DeleteInstance();
}

void Game::AddState(IGameState* pNewState)
{
	//Store the new State
	m_vGameStates.push_back(pNewState);

	//Set the current state
	m_nCurrentState = m_vGameStates.size() - 1;

	//If new state isnt nullptr enter the state
	if(pNewState != nullptr)
	{
		m_vGameStates[m_nCurrentState]->Enter();
	}
}

void Game::RemoveState(void)
{
	//Exit the state
	m_vGameStates[m_nCurrentState]->Exit();

	//Pop it off the stack
	m_vGameStates.pop_back();

	//Decrement the current state
	m_nCurrentState--;
}

void Game::ClearStates(void)
{
	for(unsigned int i = 0; i < m_vGameStates.size(); i++)
	{
		m_vGameStates[i]->Exit();
	}
	m_vGameStates.clear();
	AddState(MainMenuState::GetInstance());
}

