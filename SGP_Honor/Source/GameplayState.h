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
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "AnimTimeStamp.h"
#include <map>



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
	void ResetAudio() { SGD::AudioManager::GetInstance()->PlayAudio(m_hBGM); }

	///////////////////////////////////////////////////////////
	///////////////////// Factory Methods  ////////////////////
	void CreateHonor(int x, int y, int _amount);
	void CreateActivator(int x, int y, bool _pressurePlate, bool _active, int _ID);
	void CreateLaser(int x, int y, SGD::Vector _direction, int _ID);
	void CreateTurret(int x, int y, int _direction, float _timer);
	void CreateDoor(int _x, int _y, bool _isHorizontal, int _ID);
	void CreateMovingPlatform(int _x, int _y, bool _vertical, float _turnDistance, float _speed);
	void CreateGeyser(int _x, int _y, float _speed, float _maxHeight, int _currState);
	void CreateLava(int _x, int _y, float _speed, float _maxHeight, int _currState);
	void CreateFallingBlock(int _x, int _y);
	void CreateBlock(int _x, int _y);
	void CreatePendulum(int _x, int _y);
	void CreateBuzzsaw(int _x, int _y, bool _horizontal, unsigned int _returnDistance);
	void CreateStalactite(int _x, int _y, float _timer);
	void CreateArmor(int _x, int _y);
	void CreateFreezableGround(int _x, int _y);
	void CreateFreezableLeftRamp(int _x, int _y);
	void CreateFreezableRightRamp(int _x, int _y);
	void CreateHintStatue(int _x, int _y, std::string _message);
	void CreateTeleporter(int _x, int _y, std::string _level);

	void LoadLevel(std::string _level);

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
	void LoadLevelMap();

	
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

	std::map<std::string, std::string> m_mLevels;

	AnimTimeStamp ts;
	AnimTimeStamp ts2;
	AnimTimeStamp ts3;

	float testtime = 0;
	
	//Test Entities to be removed later
	//FallingBlock* m_pFBlock = nullptr;
	//Door* m_pDoor = nullptr;
	//BossDoor* m_pBDoor = nullptr;
	//Activator* m_pSwitch = nullptr;
	//Activator* m_pPressurePlate = nullptr;
	//Stalactite* m_pStalactite = nullptr;
	//BuzzSaw* m_pBuzzSaw = nullptr;
	//Turret* m_pTurret = nullptr;

	//Pendulum * m_pPendulum = nullptr;
	//HintStatue * m_pStatue = nullptr;
	//Honor * m_pHonor = nullptr;
	//Armor * m_pArmor = nullptr;

	SGD::HAudio m_hBGM = SGD::INVALID_HANDLE;

};

