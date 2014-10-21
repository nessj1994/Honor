/*
|	File: IGameState.h
|	Author: Jordan Ness
|	Purpose: The IGameState class creates the interface
|			that all gamestate child classes must implement
|
*/


#pragma once




////////////////////////////////////////////////////
// IGameState class
// - This is an abstract base class
// - This class only declares the interface for the game states
class IGameState
{
protected:
	IGameState() = default;
	virtual ~IGameState() = default;


public:

	//////////////////////////////////////////////////////////////
	/////////////////////IGameState Interface////////////////////
	virtual void Enter(void) = 0; //Load Resources
	virtual void Exit(void) = 0; //Unload Resources

	virtual bool Input(void) = 0; //Hanlde user Input
	virtual void Update(float elapsedTime) = 0; //update entities
	virtual void Render(void) = 0; //Render all entities


};