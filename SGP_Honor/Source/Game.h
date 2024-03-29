/**************************************************************************
|	File:	Game.h
|	Author: Jordan Ness
|	Purpose: The Game class handles all of the game logic and SGD Wrappers
**************************************************************************/
#pragma once


#include <vector>
#include "Profile.h"
////////////////////////////////////////////////////////
/////////////Forward Class Declarations////////////////
//Add class declarations here as needed
class IGameState;
class BitmapFont;




//////////////////////////////////////////////////////
////////////////////Game Class////////////////////////
// - runs the game logic
// - controls the SGD Wrappers
// - SINGLETON
//		- ONLY ONE INSTANCE
//		- Global Access method (GetInstance)
class Game
{
public:
	//////////////////////////////////////////////////////////////////////////
	/////////////////////// Singleton Accessors /////////////////////////////
	static Game* GetInstance(void);
	static void DeleteInstance(void);

	/////////////////////////////////////////////////////////////////////////
	//////////////////////// Setup, Play, Cleanup //////////////////////////
	bool Initialize(float width, float height);
	int Update();
	void Terminate(void);

	
	///////////////////////////////////////////////////////////////////////
	//////////////////// Screen Size Accessors ///////////////////////////
	float GetScreenWidth(void) const { return m_fScreenWidth; }
	float GetScreenHeight(void) const { return m_fScreenHeight; }
	bool GetWindowed(void) const { return m_bWindowed; }

	/////////////////////////////////////////////////////////////////////
	//////////////////// Screen Size Mutator ///////////////////////////
	void SetWindowed(bool windowed){ m_bWindowed = windowed; }

	/////////////////////////////////////////////////////////////////////
	//////////////////////// Font Accessor /////////////////////////////
	BitmapFont* GetFont(void) const { return m_pFont; }

	////////////////////////////////////////////////////////////////////
	////////////////////// Game State Machine /////////////////////////
	// - can only be called by the state's input, update, or render methods!!!!
	void AddState(IGameState* pNewState);
	void RemoveState(void);
	void ClearStates(void);

	//////////////////////////////////////////////////////////////////////
	////////////////////////// Game Time ////////////////////////////////
	unsigned long m_ulGameTime = 0;

	unsigned int GetStackSize() const { return m_vGameStates.size(); }

	//////////////////////////////////////////////////////////////////
	///////////////////////// Profiles //////////////////////////////
	Profile* GetProfile(int number)
	{
		if(number == 1)
		{
			return m_pProfile1;
		}
		else if(number == 2)
		{
			return m_pProfile2;
		}
		else if(number == 3)
		{
			return m_pProfile3;
		}
		return nullptr;
	}

	int GetSelectedNumber() { return m_nSelectedProfile; }
	void SetSelectedProfile(int number)
	{
		m_nSelectedProfile = number;
	}

	IGameState* GetCurrentGameState() const { return m_vGameStates[m_nCurrentState] /*m_CurrentGameState*/; }

private:
	/////////////////////////////////////////////////////////////////////////
	//////////////////////// Singleton Object //////////////////////////////
	static Game* s_pInstance;

	Game() = default;
	~Game() = default;

	Game(const Game&) = delete; //Copy Constructor
	Game& operator= (const Game&) = delete; //Assignment operator

	//////////////////////////////////////////////////////////////////////
	//////////////////////////Screen Size////////////////////////////////
	float m_fScreenWidth = 1;
	float m_fScreenHeight = 1;
	bool m_bWindowed = true;
	unsigned long m_GameTime;

	//GameStates
	
	IGameState* m_CurrentGameState;

	/////////////////////////////////////////////////////////////////////
	////////////////////////// Current Game State //////////////////////
	int m_nCurrentState = 0;

	std::vector<IGameState*> m_vGameStates;


	///////////////////////////////////////////////////////////////////
	//////////////////////// Bitmap Font /////////////////////////////
	BitmapFont* m_pFont;


	int m_nSelectedProfile;

	Profile* m_pProfile1 = new Profile(1);
	Profile* m_pProfile2 = new Profile(2);
	Profile* m_pProfile3 = new Profile(3);
};

