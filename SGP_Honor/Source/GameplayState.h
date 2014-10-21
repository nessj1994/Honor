/*
|	File: GameplayState.h
|	Author: Jordan Ness
|	Purpose: The GameplayState class handles all of the 
			games entities and gets them running
|
*/

#pragma once
#include "IGameState.h"
#include "Camera.h"
#include "Animation.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "AnimTimeStamp.h"




////////////////////////////////////////////////////
//////////// Foward Class Declarations ////////////
class EntityManager;
class Entity;
class Emitter;
class Level;
class Player;
class FallingBlock;
class Door;
class BossDoor;
class Activator;
class Unit;
class Hawk;
class Stalactite;
class BuzzSaw;
class Turret;
class Pendulum;
class HintStatue;
class Honor;
class Armor;

//////////////////////////////////////////////////
// GameplayState class
// -runs game logic
// -SINGLETON (static allocation)
class GameplayState :
	public IGameState
{

public:

	///////////////////////////////////////////////////////////
	///////////////////// Singleton Accessor /////////////////
	static GameplayState* GetInstance(void);
	



	//////////////////////////////////////////////////////////////
	//////////////////// IGameState Interface ///////////////////
	virtual void Enter(void)						override; //Load Resources
	virtual void Exit(void)							override; //Unload Resources

	virtual bool Input(void)						override; //Handle user Input
	virtual void Update(float elapsedTime)			override; //update entities
	virtual void Render(void)						override; //Render all entities


	///////////////////////////////////////////////////////////
	/////////////////////// Accessors ////////////////////////
	Camera* GetCamera(void) const { return m_pCamera; }
	Level* GetCurrentLevel(void) const { return m_pLevel; }


	//////////////////////////////////////////////////////////
	////////////////////// Mutators /////////////////////////
	void SetCamera(Camera* camera) { m_pCamera = camera; }

	///////////////////////////////////////////////////////////
	///////////////////// Factory Methods  ////////////////////
	//void CreateHonor(int x, int y, int amount);
	//void CreateActivator(int x, int y);
	//void CreateTurret(int x, int y);
	//void CreateDoor(int x, int y);
	void CreateMovingPlatform(int _x, int _y, bool _vertical, float _turnDistance, float _speed);
	void CreateActivator(int _x, int _y, bool _isPressure, bool _currState, int _ID);
	void CreateDoor(int _x, int _y, bool _isHorizontal, int _ID, int _size);
	void CreateFallingBlock(int _x, int _y);
	void CreateGeyser(int x, int y);
	void CreateLava(int x, int y);
	void CreateLaser(int x, int y, SGD::Vector _direction, int _switchX, int _switchY);

	

	//void CreateGeyser(int x, int y);
	//void CreateLava(int x, int y);
	//void CreateBlock(int x, int y);
	//void CreatePendulum(int x, int y);
	//void CreateBuzzsaw(int x, int y);
	//void CreateStalactite(int x, int y);
	//void CreateArmor(int x, int y);
	//void CreateFreezableGround(int x, int y);
	//void CreateFreezableLeftRamp(int x, int y);
	//void CreateFreezableRightRamp(int x, int y);
	//void CreateHintStatue(int x, int y);

private:

	////////////////////////////////////////////////////////////
	///////////////////// Singleton ///////////////////////////
	GameplayState()										= default; //Default constructor
	virtual ~GameplayState()							= default; //Default destructor

	GameplayState(const GameplayState&)					= delete; //Delete copy constructor
	GameplayState& operator= (const GameplayState&)		= delete; //Delete assignment operator


	///////////////////////////////////////////////////////////
	///////////////////// Factory Methods  ////////////////////


	Player* CreatePlayer(void);
	Entity* CreateProjectile(Entity* pOwner) const;
	Entity* CreateSpray(Entity* pOwner) const;
	Hawk* CreateHawk(Entity* pOwner) const;


	void CreateBlocks(void);
	void CreatePermFrozenTiles(void);
	void CreateTempFrozenTiles(void);
	

	///////////////////////////////////////////////////////////
	//////////////////// Save and Load ///////////////////////
	void SaveGame();
	void LoadGame();

	
	//////////////////////////////////////////////////////////
	/////////////// Message Callback Funtion ////////////////
	static void MessageProc(const SGD::Message* pMsg);


	//////////////////////////////////////////////////////////
	/////////////////// Game Entities ///////////////////////

	EntityManager* m_pEntities						= nullptr;
	Camera* m_pCamera								= nullptr;
	Emitter* m_pEmitter								= nullptr;
	Emitter* m_pEmitter2						    = nullptr;
	Level* m_pLevel									= nullptr;
	Player* m_pPlayer								= nullptr;

	AnimTimeStamp ts;
	AnimTimeStamp ts2;
	AnimTimeStamp ts3;

	float testtime = 0;
	
	//Test Entities to be removed later
	FallingBlock* m_pFBlock = nullptr;
	Door* m_pDoor = nullptr;
	BossDoor* m_pBDoor = nullptr;
	Activator* m_pSwitch = nullptr;
	Activator* m_pPressurePlate = nullptr;
	Stalactite* m_pStalactite = nullptr;
	BuzzSaw* m_pBuzzSaw = nullptr;
	Turret* m_pTurret = nullptr;

	Pendulum * m_pPendulum = nullptr;
	HintStatue * m_pStatue = nullptr;
	Honor * m_pHonor = nullptr;
	Armor * m_pArmor = nullptr;

	SGD::HAudio m_hBGM = SGD::INVALID_HANDLE;

};

