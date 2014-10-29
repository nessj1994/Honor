#include "GameplayState.h"


#include "Game.h"
#include "MainMenuState.h"
#include "Emitter.h"
#include "Level.h"
#include "AnimationEngine.h"
#include "MessageID.h"
#include "DestroyEntityMessage.h"
#include "CreateProjectileMessage.h"
#include "CreateGravProjectileMessage.h"
#include "CreateSprayMessage.h"
#include "CreateHawkMessage.h"
#include "ChangeLevelMessage.h"
#include "MovingPlatform.h"

#include "Entity.h"
#include "Projectile.h"
#include "GravProjectile.h"
#include "Player.h"
#include "EntityManager.h"
#include "Camera.h"
#include "ParticleEngine.h"
#include "Block.h"
#include "FreezeableGround.h"
#include "PauseState.h"
#include "FallingBlock.h"
#include "Door.h"
#include "BossDoor.h"
#include "Activator.h"
#include "Stalactite.h"
#include "Ice.h"
#include "Hawk.h"
#include "BuzzSaw.h"
#include "Turret.h"
#include "Stalactite.h"
#include "Geyser.h"
#include "laser.h"
#include "Lava.h"
#include "Pendulum.h"
#include "Armor.h"
#include "Honor.h"
#include "HintStatue.h"
#include "Squid.h"
#include "Pouncer.h"
#include "Jellyfish.h"
#include "Teleporter.h"
#include "Bull.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../TinyXML/tinyxml.h"


#include <cassert>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

///////////////////////////////////////////////////////////
///////////////////// Singleton Accessor /////////////////

/////////////////////////////////////////////////////////
// GetInstance
// - allocate the static global instance
// - return THE instance
GameplayState* GameplayState::GetInstance(void)
{
	static GameplayState s_Instance; //This is stored ONCE in global memory
	return &s_Instance;
}




//////////////////////////////////////////////////////////////
//////////////////// IGameState Interface ///////////////////



///////////////////////////////////////////////////
// Enter
// - Reset the game
// - Load the resources
// - set up entities
void GameplayState::Enter(void) //Load Resources
{
	//Create Local references to the SGD Wrappers
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	//Set the background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 }); //Black

	//Allocate the Entity Manager
	m_pEntities = new EntityManager;

	//Initialize the Event and Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);




	//Load Audio
	m_hBGM = pAudio->LoadAudio(L"Assets/Audio/HonorBGM.xwm");
	pAudio->PlayAudio(m_hBGM, true);

	//These are only for testing and will be removed later
	//m_pDoor = new Door();
	//m_pBDoor = new BossDoor();
	//m_pFBlock = new FallingBlock();
	//m_pSwitch = new Activator(false);
	//m_pPressurePlate = new Activator(true);
	//m_pStalactite = new Stalactite();
	//m_pBuzzSaw = new BuzzSaw();
	//m_pTurret = new Turret();

	//m_pArmor = new Armor();
	//m_pHonor = new Honor();
	//m_pPendulum = new Pendulum();
	//m_pStatue = new HintStatue();
	//m_pStatue->SetMessageString("This is a test string");

	m_pSquid = new Squid();
	m_pPouncer = new Pouncer();
	m_pJellyfish = new Jellyfish();
	m_pJellyfish2 = new Jellyfish();
	m_pJellyfish2->SetPosition({100, 600});



	//Create player with factory method
	m_pPlayer = CreatePlayer();

	Camera::GetInstance()->SetPlayer(m_pPlayer);

	//Add factory methods to put entities in the manager
	/*CreateBlocks();
	CreatePermFrozenTiles();
	CreateTempFrozenTiles();
	CreateGeyser(1000, 700);
	CreateLaser(1500, 500, { 1, 1 }, 1500, 700);
	CreateLava(50, 700);*/

	//CreateMovingPlatform(1000, 500, false, 200, 100);

	// Add Entities to the entity manager

	m_pEntities->AddEntity(m_pPlayer, Entity::ENT_PLAYER);

	//Remove this test code later
	/*m_pEntities->AddEntity(m_pFBlock, Entity::ENT_FALLING_BLOCK);
	m_pEntities->AddEntity(m_pDoor, Entity::ENT_DOOR);
	m_pEntities->AddEntity(m_pBDoor, Entity::ENT_BOSS_DOOR);
	m_pEntities->AddEntity(m_pSwitch, Entity::ENT_SWITCH);
	m_pEntities->AddEntity(m_pStalactite, Entity::ENT_STALACTITE);
	m_pEntities->AddEntity(m_pBuzzSaw, Entity::ENT_BUZZSAW);
	m_pEntities->AddEntity(m_pTurret, Entity::ENT_TURRET);
	m_pEntities->AddEntity(m_pStalactite, Entity::ENT_STALACTITE);


	m_pEntities->AddEntity(m_pArmor, Entity::ENT_ARMOR);
	m_pEntities->AddEntity(m_pHonor, Entity::ENT_HONOR);
	m_pEntities->AddEntity(m_pPendulum, Entity::ENT_PENDULUM);
	m_pEntities->AddEntity(m_pStatue, Entity::ENT_STATUE);
	m_pDoor->SetActivator(m_pSwitch);




	//For Particle Testing*/
	m_pEmitter2 = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/C++Test.xml", "Test", { 96, 672 });

	// Load in map for the levels and start the first level
	LoadLevelMap();
	LoadLevel("HubLevel");

	m_pEntities->AddEntity(m_pSquid, Entity::ENT_ENEMY);
	//m_pEntities->AddEntity(m_pPouncer, Entity::ENT_ENEMY);
	m_pEntities->AddEntity(m_pJellyfish, Entity::ENT_JELLYFISH);
	m_pEntities->AddEntity(m_pJellyfish2, Entity::ENT_JELLYFISH);

	// Temporary
	//CreateBullBoss(500, 400);
}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void GameplayState::Exit(void)
{

	//Save the game
	SaveGame();


	if (m_pEntities != nullptr)
	{
		//Deallocate the Entity Manager
		m_pEntities->RemoveAll();
		delete m_pEntities;
		m_pEntities = nullptr;
	}

	//delete m_pBDoor;
	//delete m_pDoor;
	//delete m_pSwitch;
	//delete m_pPressurePlate;
	//delete m_pBuzzSaw;
	//delete m_pTurret;
	//delete m_pStalactite;

	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Release();
	}
	
	//if (m_pStatue != nullptr)
	//	m_pStatue->Release();

	//if (m_pHonor != nullptr)
	//	m_pHonor->Release();

	//if (m_pArmor != nullptr)
	//	m_pArmor->Release();

	//if (m_pPendulum != nullptr)
	//	m_pPendulum->Release();

	delete m_pSquid;

	
	delete m_pPouncer;

	if (m_pJellyfish != nullptr)
		m_pJellyfish->Release();

	if (m_pJellyfish2 != nullptr)
		m_pJellyfish2->Release();
	//Create local references to the SGD Wrappers

	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	ParticleEngine::GetInstance()->Terminate();
	ParticleEngine::GetInstance()->DeleteInstance();
	delete m_pEmitter2;



	//Unload Assets
	//Level
	delete m_pLevel;

	AnimationEngine::GetInstance()->Terminate();
	AnimationEngine::GetInstance()->DeleteInstance();

	//Graphics


	//Audio
	pAudio->StopAudio(m_hBGM);
	pAudio->UnloadAudio(m_hBGM);



	//Terminate and deallocate the SGD Wrappers
	//event manager
	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::GetInstance()->DeleteInstance();

	//message manager
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::GetInstance()->DeleteInstance();
}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool GameplayState::Input(void) //Hanlde user Input
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	//DOOR TEST This will be removed later
	//if(pInput->IsKeyPressed(SGD::Key::X))
	//{
	//	m_pDoor->SetOpen(!(m_pDoor->GetOpen()));
	//}
	if (pInput->IsKeyPressed(SGD::Key::H))
	{
		m_pPlayer->SetHonorCollected(m_pPlayer->GetHonorCollected() + 50);
		//Gained Honor event for certain doors
		SGD::Event* pATEvent = new SGD::Event("HONOR_GAINED", nullptr, m_pPlayer);
		SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
		pATEvent = nullptr;

	}

	// Temporary test for level changing
	if (pInput->IsKeyPressed(SGD::Key::T))
	{
		LoadLevel("Level1_1");
	}

	if (pInput->IsKeyPressed(SGD::Key::Escape)
		|| pInput->IsButtonPressed(0, 7 /*Button start on xbox controller*/))
	{
		Game::GetInstance()->AddState(PauseState::GetInstance());
		pAudio->StopAudio(m_hBGM);
	}

	return true;
}

//////////////////////////////////////////////
// Update
// - Update all game entities
void GameplayState::Update(float elapsedTime)
{
	//if (m_pHonor->GetIsCollected() == true)
	//	m_pEntities->RemoveEntity(m_pHonor);

	//if (m_pArmor->GetIsCollected() == true)
	//	m_pEntities->RemoveEntity(m_pArmor);

	//	m_pCamera->Update(elapsedTime);

	m_pEmitter2->Update(elapsedTime);
	float x = elapsedTime;

	m_pEntities->UpdateAll(elapsedTime);
	Camera::GetInstance()->Update(elapsedTime);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_HONOR);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_BLOCK);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_PERM_FREEZE);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_TEMP_FREEZE);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_DOOR);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_BOSS_DOOR);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_SWITCH);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_BUZZSAW);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_PROJ);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_GEYSER);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_LASER);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_LAVA);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_MOVING_PLATFORM);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_TELEPORTER);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_ENEMY);
	m_pEntities->CheckCollisions(Entity::ENT_JELLYFISH, Entity::ENT_PLAYER);




	m_pEntities->CheckCollisions(Entity::ENT_TEMP_FREEZE, Entity::ENT_SPRAY);
	m_pEntities->CheckCollisions(Entity::ENT_GEYSER, Entity::ENT_SPRAY);



	m_pEntities->CheckCollisions(Entity::ENT_PROJ, Entity::ENT_BLOCK);
	m_pEntities->CheckCollisions(Entity::ENT_FALLING_BLOCK, Entity::ENT_BLOCK);
	m_pEntities->CheckCollisions(Entity::ENT_FALLING_BLOCK, Entity::ENT_PLAYER);
	m_pEntities->CheckCollisions(Entity::ENT_HAWK, Entity::ENT_STALACTITE);
	m_pEntities->CheckCollisions(Entity::ENT_HAWK, Entity::ENT_SWITCH);
	m_pEntities->CheckCollisions(Entity::ENT_HAWK, Entity::ENT_GEYSER);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_STATUE);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_PENDULUM);

	//if (m_pArmor != nullptr)
	//	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_ARMOR);

	//if (m_pHonor != nullptr)
	//	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_HONOR);

	m_pEntities->CheckWorldCollision(Entity::ENT_PLAYER);
	m_pEntities->CheckWorldCollision(Entity::ENT_FALLING_BLOCK);
	m_pEntities->CheckWorldCollision(Entity::ENT_PROJ);
	m_pEntities->CheckWorldCollision(Entity::ENT_HAWK);
	m_pEntities->CheckWorldCollision(Entity::ENT_STALACTITE);
	m_pEntities->CheckWorldCollision(Entity::ENT_LASER);
	m_pEntities->CheckWorldCollision(Entity::ENT_BOSS_BULL);

	m_pEntities->CheckWorldCollision(Entity::ENT_ENEMY);

	m_pEntities->CheckWorldEvent(Entity::ENT_PLAYER);


	//Process messages and events
	SGD::EventManager::GetInstance()->Update();
	SGD::MessageManager::GetInstance()->Update();
}

/////////////////////////////////////////////
// Render
// - Render all game entities
void GameplayState::Render(void)
{
	m_pLevel->RenderImageLayer(true);
	m_pLevel->Render();



	//m_pEmitter2->Render();
	m_pEntities->RenderAll();
	m_pLevel->RenderImageLayer(false);


}

//Static Message callback function
void GameplayState::MessageProc(const SGD::Message* pMsg)
{

	//Show warning when a message ID enumerator is not handled
#pragma warning (push)
#pragma warning (1 : 4061)

	//What type of message is this
	switch (pMsg->GetMessageID())
	{
		case MessageID::MSG_DESTROY_ENTITY:
		{
			//Downcast to the real message type
			const DestroyEntityMessage* pDestroyMsg =
				dynamic_cast<const DestroyEntityMessage*>(pMsg);

			//Make sure the message isnt a nullptr
			assert(pDestroyMsg != nullptr
				   && "GameplayState::MessageProc - MSG_DESTROY_ENTITY is not actually a DestroyEntityMessage");

			//get the message's entity
			Entity* ptr = pDestroyMsg->GetEntity();

			//Use the singleton to access the entity manager
			if (ptr != nullptr)
			{
				GameplayState::GetInstance()->m_pEntities->RemoveEntity(ptr);
			}

			break;
		}
		case MessageID::MSG_CREATE_PROJECTILE:
		{
			//Downcast to the real message type
			const CreateProjectileMessage* pCreateMsg =
				dynamic_cast<const CreateProjectileMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_PROJECTILE is not actually a CreateProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound

			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateProjectile(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);
			}
			else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_TURRET)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);
			}

			// if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			// {
			//	 pSelf->m_pEntities->AddEntity(pProj, EntityManager::BUCKET_PLAYER_PROJ);
			// }
			// else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_ENEMY)
			// {
			//	 pSelf->m_pEntities->AddEntity(pProj, EntityManager::BUCKET_ENEMY_PROJ);
			// }

			pProj->Release();
			pProj = nullptr;


			break;
		}
		case MessageID::MSG_CREATE_GRAVPROJECTILE:
		{
			//Downcast to the real message type
			const CreateGravProjectileMessage* pCreateMsg = dynamic_cast<const CreateGravProjectileMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				 && "GameplayState::MessageProc - MSG_CREATE_GRAVPROJECTILE is not actually a CreateGravProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound

			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateGravProjectile(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_ENEMY)
			{
				 pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);
			}

			pProj->Release();
			pProj = nullptr;


			break;
		}
		case MessageID::MSG_CREATE_SPRAY:
		{

			//Downcast to the real message type
			const CreateSprayMessage* pCreateMsg =
				dynamic_cast<const CreateSprayMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_PROJECTILE is not actually a CreateProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound

			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateSpray(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_SPRAY);
			}
			else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_ENEMY)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_SPRAY);
			}

			// if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			// {
			//	 pSelf->m_pEntities->AddEntity(pProj, EntityManager::BUCKET_PLAYER_PROJ);
			// }
			// else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_ENEMY)
			// {
			//	 pSelf->m_pEntities->AddEntity(pProj, EntityManager::BUCKET_ENEMY_PROJ);
			// }

			pProj->Release();
			pProj = nullptr;


			break;

		}
		case MessageID::MSG_CREATE_HAWK:
		{

			//Downcast to the real message type
			const CreateHawkMessage* pCreateMsg =
				dynamic_cast<const CreateHawkMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_PROJECTILE is not actually a CreateProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound


			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateHawk(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_HAWK);


			}
			else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_ENEMY)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_HAWK);
			}

			pProj->Release();
			pProj = nullptr;


			break;

		}
		case MessageID::MSG_CHANGE_LEVEL:
		{
			//Downcast to the real message type
			const ChangeLevelMessage* pCreateMsg =
				dynamic_cast<const ChangeLevelMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CHANGE_LEVEL is not actually a CreateProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();

			// Reference to the teleporter entity
			Teleporter * teleporter = dynamic_cast<Teleporter*>(pCreateMsg->GetOwner());

			pSelf->LoadLevel(teleporter->GetLevel());

		}
		case MessageID::MSG_UNKNOWN:
		{
			OutputDebugStringW(L"Game::MessageProc - unknown message id\n");
			break;
		}
		default:
		{

			break;
		}
	}


}


///////////////////////////////////////////////////////////
//////////////////// Factory Methods /////////////////////

//Creates a new projectile to be added to the entity manager
Entity* GameplayState::CreateProjectile(Entity* pOwner) const
{
	Projectile* proj = new Projectile();
	if (pOwner->GetDirection().x == 1)
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x + pOwner->GetSize().width, pOwner->GetPosition().y + +pOwner->GetSize().height / 2));
	else
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y + pOwner->GetSize().height / 2));

	proj->SetSize({ 40, 40 });
	proj->SetDirection({ pOwner->GetDirection() });
	//proj->SetVelocity({ pOwner->GetDirection() });
	proj->SetOwner(pOwner);

	return proj;

}

Entity* GameplayState::CreateGravProjectile(Entity* pOwner) const
{
	GravProjectile* proj = new GravProjectile();
	if (pOwner->GetDirection().x == 1)
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x + pOwner->GetSize().width / 2, pOwner->GetPosition().y - pOwner->GetSize().height / 2));
	else
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x - pOwner->GetSize().width, pOwner->GetPosition().y - pOwner->GetSize().height / 2));

	proj->SetDirection({ pOwner->GetDirection() });
	proj->SetOwner(pOwner);

	return proj;
}

//Creates a new projectile to be added to the entity manager
Entity* GameplayState::CreateSpray(Entity* pOwner) const
{
	Ice* proj = new Ice;
	if (pOwner->GetDirection().x == 1)
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x + pOwner->GetSize().width, pOwner->GetPosition().y + pOwner->GetSize().height / 2));
	else
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y + pOwner->GetSize().height / 2));

	proj->SetSize({ 4, 4 });
	proj->SetDirection({ pOwner->GetDirection().x, -1 });
	//proj->SetVelocity({ pOwner->GetDirection() });
	proj->SetOwner(pOwner);

	return proj;

}


Hawk* GameplayState::CreateHawk(Entity* pOwner) const
{
	Hawk* proj = new Hawk;
	if (pOwner->GetDirection().x == 1)
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x + pOwner->GetSize().width, pOwner->GetPosition().y + pOwner->GetSize().height / 2));
	else
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y + pOwner->GetSize().height / 2));

	proj->SetSize({ 10, 10 });
	proj->SetDirection({ pOwner->GetDirection() });
	proj->SetOwner(pOwner);
	//pOwner->SetHawkPtr(proj);
	m_pPlayer->SetHawkPtr(proj);

	return proj;

}



/////////////////////////////////////////////
// Factory Methods
// - Creates character class
Player* GameplayState::CreatePlayer(void)
{
	Player* pPlayer = new Player;

	pPlayer->SetPosition(SGD::Point(100, 100));
	pPlayer->SetSize(SGD::Size(32, 64));

	return pPlayer;



}

void GameplayState::CreateBlocks(void)
{


	for (unsigned int i = 0; i < 10; i++)
	{
		Block* pBlock_1 = new Block;

		pBlock_1->SetPosition(SGD::Point(100 + (i * 20.0f), 400.0f));
		pBlock_1->SetSize(SGD::Size(20, 20));

		m_pEntities->AddEntity(pBlock_1, Entity::ENT_BLOCK);

		pBlock_1->Release();
	}

	//for (unsigned int i = 0; i < 40; i++)
	//{
	//	Block* pBlock_2 = new Block;

	//	pBlock_2->SetPosition(SGD::Point(600, 380 - (i * 20)));
	//	pBlock_2->SetSize(SGD::Size(20, 20));

	//	m_pEntities->AddEntity(pBlock_2, Entity::ENT_BLOCK);

	//	pBlock_2->Release();
	//}

}

void GameplayState::CreatePermFrozenTiles(void)
{

	for (unsigned int i = 0; i < 10; i++)
	{
		FreezeableGround* pFreeze = new FreezeableGround;


		//// TYPE STATE WILL CHANGE ( Needs to be done through TYPE with enttiyManger) Two types for each frozen tile === It's Current state and it's Bucket type (temp or perm tile)
		pFreeze->SetType(Entity::ENT_FROZEN);

		pFreeze->SetSize(SGD::Size(20, 20));
		pFreeze->SetPosition(SGD::Point(300 + (i * 20.0f), 400.0f));
		pFreeze->SetIsFrozen(true);
		pFreeze->SetIsPerm(true);

		////// TYPE OF TILE Bucket
		m_pEntities->AddEntity(pFreeze, Entity::ENT_PERM_FREEZE);

		pFreeze->Release();
	}


}


void GameplayState::CreateTempFrozenTiles(void)
{
	for (unsigned int i = 0; i < 10; i++)
	{
		FreezeableGround* pFreeze = new FreezeableGround;

		//// TYPE STATE WILL CHANGE ( Needs to be done through TYPE with enttiyManger) Two types for each frozen tile === It's Current state and it's Bucket type (temp or perm tile)
		pFreeze->SetType(Entity::ENT_NOT_FROZEN);

		pFreeze->SetSize(SGD::Size(20, 20));
		pFreeze->SetPosition(SGD::Point(500 + (i * 20.0f), 400.0f));
		pFreeze->SetIsFrozen(false);
		pFreeze->SetIsPerm(false);

		////// TYPE OF TILE Bucket
		m_pEntities->AddEntity(pFreeze, Entity::ENT_TEMP_FREEZE);

		pFreeze->Release();
	}

}

#pragma region Entity factory methods

/////////////////////////
// CreateHonor
// -Creates an honor collectible at the given coordinates
void GameplayState::CreateHonor(int _x, int _y, int _amount)
{
	Honor * mHonor = new Honor();
	mHonor->SetPosition({ (float)_x, (float)_y });
	mHonor->SetHonorAmount(_amount);
	mHonor->SetEmitter();
	m_pEntities->AddEntity(mHonor, Entity::ENT_HONOR);
	mHonor->Release();
}


/////////////////////////
// CreateActivator
// -Creates an activator at the given coordinates
void GameplayState::CreateActivator(int _x, int _y, bool _isPressure, bool _currState, int _ID)
{
	Activator * pActivator = new Activator(_isPressure);
	pActivator->SetPosition({ (float)_x, (float)_y });
	pActivator->SetOn(_currState);
	pActivator->SetKeyID(_ID);
	m_pEntities->AddEntity(pActivator, Entity::ENT_SWITCH);
	pActivator->Release();
}

/////////////////////////
// CreateLaser
// -Creates a laser at the given coordinates
void GameplayState::CreateLaser(int x, int y, SGD::Vector _direction, int _ID)
{
	Laser* m_pLaser = new Laser;
	m_pLaser->SetPosition({ (float)x, (float)y });
	m_pLaser->SetOrigPosition({ (float)x, (float)y });
	m_pLaser->SetDirection({ _direction });
	m_pLaser->SetFreq(_ID);


	//Activator* m_pLaserSwitch = new Activator(false);
	//m_pLaserSwitch->SetPosition({ (float)_switchX, (float)_switchY });

	//m_pLaserSwitch->SetKeyID(2);

	m_pEntities->AddEntity(m_pLaser, Entity::ENT_LASER);
	//m_pEntities->AddEntity(m_pLaserSwitch, Entity::ENT_SWITCH);
	//m_pLaserSwitch->Release();
	m_pLaser->Release();
}

/////////////////////////
// CreateTurret
// -Creates a turret at the given coordinates
void GameplayState::CreateTurret(int x, int y, int _direction, float _timer)
{
	Turret * mTurret = new Turret();
	mTurret->SetPosition({ (float)x, (float)y });
	mTurret->SetFireTimer(_timer);
	mTurret->SetDirection(_direction);
	m_pEntities->AddEntity(mTurret, Entity::ENT_TURRET);
	mTurret->Release();
}

/////////////////////////
// CreateDoor
// -Creates a door at the given coordinates
void GameplayState::CreateDoor(int _x, int _y, bool _isHorizontal, int _ID)
{
	Door * pDoor = new Door();
	pDoor->SetPosition({ (float)_x, (float)_y });
	if (_isHorizontal)
	{
		pDoor->SetSize({ 128.0f, 32.0f });
	}
	else
	{
		pDoor->SetSize({ 32.0f, 128.0f });
	}
	pDoor->SetKeyID(_ID);
	m_pEntities->AddEntity(pDoor, Entity::ENT_DOOR);
	pDoor->Release();
}

/////////////////////////
// CreateMovingPlatform
// -Creates a moving platform at the given coordinates
void GameplayState::CreateMovingPlatform(int _x, int _y, bool _vertical, float _turnDistance, float _speed)
{
	MovingPlatform * mPlatform = new MovingPlatform();
	mPlatform->SetPosition({ (float)_x, (float)_y });
	mPlatform->SetSize({ 128.0f, 32.0f });
	mPlatform->SetVertical(_vertical);
	mPlatform->SetTurnDistance(100);

	//mPlatform->SetTurnDistance(_turnDistance);
	mPlatform->SetSpeed(_speed);
	m_pEntities->AddEntity(mPlatform, Entity::ENT_MOVING_PLATFORM);
	mPlatform->Release();
}

/////////////////////////
// CreateGeyser
// -Creates a geyser at the given coordinates
void GameplayState::CreateGeyser(int _x, int _y, float _speed, float _maxHeight, int _currState)
{
	Geyser* m_pGeyser = new Geyser;
	m_pGeyser->SetPosition({ (float)_x, (float)_y });
	m_pGeyser->SetOrigPosition({ (float)_x, (float)_y });

	m_pEntities->AddEntity(m_pGeyser, Entity::ENT_GEYSER);

	m_pGeyser->Release();
}


/////////////////////////
// CreateMovingPlatform
// -Creates a lava at the given coordinates
void GameplayState::CreateLava(int x, int y, float _speed, float _maxHeight, int _currState)
{
	Lava* m_pLava = new Lava;
	m_pLava->SetPosition({ (float)x, (float)y });
	m_pLava->SetOrigPosition({ (float)x, (float)y });
	m_pEntities->AddEntity(m_pLava, Entity::ENT_LAVA);

	m_pLava->Release();
}

/////////////////////////
// CreateFallingBlock
// -Creates a falling block at the given coordinates
void GameplayState::CreateFallingBlock(int _x, int _y)
{
	FallingBlock * fBlock = new FallingBlock();
	fBlock->SetPosition({ (float)_x, (float)_y });
	fBlock->SetStartPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(fBlock, Entity::ENT_FALLING_BLOCK);
	fBlock->Release();
}

/////////////////////////
// CreateBlock
// -Creates a block at the given coordinates
void GameplayState::CreateBlock(int _x, int _y)
{
	Block * mBlock = new Block();
	mBlock->SetPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mBlock, Entity::ENT_BLOCK);
	mBlock->Release();
}

/////////////////////////
// CreatePendulum
// -Creates a pendulum at the given coordinates
void GameplayState::CreatePendulum(int _x, int _y)
{
	Pendulum * mPendulum = new Pendulum();
	mPendulum->SetPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mPendulum, Entity::ENT_PENDULUM);
	mPendulum->Release();
}

/////////////////////////
// CreateBuzzsaw
// -Creates a buzz saw at the given coordinates
void GameplayState::CreateBuzzsaw(int _x, int _y, bool _horizontal, unsigned int _returnDistance)
{
	BuzzSaw * mSaw = new BuzzSaw();
	mSaw->SetPosition({ (float)_x, (float)_y });
	mSaw->SetStartPosition({ (float)_x, (float)_y });
	mSaw->SetHorizontal(_horizontal);
	mSaw->SetDistance(_returnDistance);
	m_pEntities->AddEntity(mSaw, Entity::ENT_BUZZSAW);
	mSaw->Release();
}

/////////////////////////
// CreateStalactite
// -Creates a stalactite at the given coordinates
void GameplayState::CreateStalactite(int _x, int _y, float _timer)
{
	Stalactite * mStalactite = new Stalactite();
	mStalactite->SetPosition({ (float)_x, (float)_y });
	mStalactite->SetRespawnTimer(_timer);
	mStalactite->SetStartPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mStalactite, Entity::ENT_STALACTITE);
	mStalactite->Release();
}

/////////////////////////
// CreateArmor
// -Creates an armor at the given coordinates
void GameplayState::CreateArmor(int _x, int _y)
{
	Armor * mArmor = new Armor();
	mArmor->SetPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mArmor, Entity::ENT_ARMOR);
	mArmor->Release();
}

/////////////////////////
// CreateFreezableGround
// -Creates a freezable ground at the given coordinates
void GameplayState::CreateFreezableGround(int _x, int _y)
{
	// TODO
	FreezeableGround * mFreeze = new FreezeableGround();
	mFreeze->SetPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mFreeze, Entity::ENT_FROZEN);
	mFreeze->Release();
}

/////////////////////////
// CreateFreezableLeftRamp
// -Creates a freezable left ramp at the given coordinates
void GameplayState::CreateFreezableLeftRamp(int _x, int _y)
{
	// TODO
}

/////////////////////////
// CreateFreezableRightRamp
// -Creates a freezable right ramp at the given coordinates
void GameplayState::CreateFreezableRightRamp(int _x, int _y)
{
	// TODO
}

/////////////////////////
// CreateHintStatue
// -Creates a hint statue at the given coordinates
void GameplayState::CreateHintStatue(int _x, int _y, std::string _message)
{
	HintStatue * mStatue = new HintStatue();
	mStatue->SetPosition({ (float)_x, (float)_y });
	mStatue->SetMessageString(_message);
	mStatue->SetMessageSize(_message.size()); // TODO
	m_pEntities->AddEntity(mStatue, Entity::ENT_STATUE);
	mStatue->Release();
}

/////////////////////////
// CreateTeleporter
// -Creates a teleporter at the given coordinates
void GameplayState::CreateTeleporter(int _x, int _y, std::string _level)
{
	Teleporter * mTeleporter = new Teleporter();
	mTeleporter->SetPosition({ (float)_x, (float)_y });
	mTeleporter->SetSize({ 64.0f, 64.0f });
	mTeleporter->SetLevel(_level);
	m_pEntities->AddEntity(mTeleporter, Entity::ENT_TELEPORTER);
	mTeleporter->Release();
}

/////////////////////////
// CreateBullBoss
// -Creates a bull boss at the given coordinates
void GameplayState::CreateBullBoss(int _x, int _y)
{
	Bull * mBull = new Bull();
	mBull->SetPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mBull, Entity::ENT_BOSS_BULL);
	mBull->Release();
}

#pragma endregion

void GameplayState::SaveGame()
{

	//Create the doc
	TiXmlDocument doc;

	//Create the declaraction
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	//Create the root element
	TiXmlElement* rootElement = new TiXmlElement("savefile");
	doc.LinkEndChild(rootElement);

	TiXmlElement* element = new TiXmlElement("player");
	rootElement->LinkEndChild(element);
	element->SetAttribute("x", (int)m_pPlayer->GetPosition().x);
	element->SetAttribute("y", (int)m_pPlayer->GetPosition().y);
	doc.SaveFile("Assets/SaveGame.xml");
}

void GameplayState::LoadGame()
{
	//Create the doc
	TiXmlDocument doc;

	doc.LoadFile("Assets/SaveGame.xml");

	TiXmlElement* pRoot = doc.RootElement();

	TiXmlElement* pPlayer = pRoot->FirstChildElement("player");

	int x;
	int y;

	pPlayer->Attribute("x", &x);
	pPlayer->Attribute("y", &y);

	//m_pPlayer->SetPosition(SGD::Point((float)x, (float)y));
}

/////////////////////////////////////////////
// LoadLevelMap
// - Reads in data to fill up the level map
void GameplayState::LoadLevelMap()
{
	//Create the tinyxml document 
	TiXmlDocument doc;

	// Load the file
	if (doc.LoadFile("Assets/Levels/LevelGuide.xml") == false)
	{
		return;
	}

	// Grab a reference to the first element
	TiXmlElement* pLevel = doc.RootElement();
	if (pLevel == nullptr)
	{
		return;
	}

	// Loop through reading each level
	while (pLevel != nullptr)
	{
		// Read in data and push into the map
		std::string key = pLevel->Attribute("key");
		std::string path = pLevel->Attribute("path");
		m_mLevels[key] = path;

		// Move to the next node
		pLevel = pLevel->NextSiblingElement();
	}

}

/////////////////////////////////////////////
// LoadLevel
// - Loads the level at the path for the given key
void GameplayState::LoadLevel(std::string _level)
{
	// Clear the old entities
	m_pEntities->RemoveAll();

	// Add the player back in
	m_pEntities->AddEntity(m_pPlayer, Entity::ENT_PLAYER);

	// First unload the last level
	if (m_pLevel != nullptr)
	{
		m_pLevel->Exit();
		delete m_pLevel;
		m_pLevel = nullptr;
	}

	// Create a new level and load the correct file
	m_pLevel = new Level();
	m_pLevel->LoadLevel(m_mLevels[_level].c_str());

	// Set the players position
	m_pPlayer->SetPosition({ (float)m_pLevel->GetPlayerX(), (float)m_pLevel->GetPlayerY() });
	m_pPlayer->SetStartPosition({ (float)m_pLevel->GetPlayerX(), (float)m_pLevel->GetPlayerY() });
	m_pPlayer->SetVelocity({ 0.0f, 0.0f });

	// TODO anything else to reset the player
}
