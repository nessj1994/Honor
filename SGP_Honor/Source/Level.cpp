//***********************************************************************
//	File:		Level.cpp
//	Author:		Michael Sciortino
//	Course:		SGD 1408
//	Purpose:	A level will hold all data for tiles and collision for a level
//***********************************************************************

#include "Level.h"
#include "Layer.h"
#include "Camera.h"
#include "IEntity.h"
#include "LevelCollider.h"
#include "Game.h"
#include "GameplayState.h"
#include "Font.h"
#include "BitmapFont.h"

#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

//////////////////////////////
// Constructor
Level::Level()
{
	// Load texture for player
	m_hPlayerSheet = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/Charbeach_beard.png");
}


//////////////////////////////
// Destructor
Level::~Level()
{
	// Unload player texture
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPlayerSheet);

	// Loop through and free each image layer
	for (unsigned int i = 0; i < m_vlLayers.size(); ++i)
	{
		delete m_vlLayers[i];
	}

	// Loop through and free the collision layer
	for (int i = 0; i < m_nWidth; ++i)
	{
		delete[] m_nCollisionLayer[i];
	}
	delete m_nCollisionLayer;

	// Loop through and free the event layer
	for (int i = 0; i < m_nWidth; ++i)
	{
		delete[] m_stEventLayer[i];
	}
	delete m_stEventLayer;

	// Free all other dynamic memory
	delete m_pCamera;
}

//////////////////////////////
// Update
// - Update stuff
void Level::Update(float _dt)
{

}

//////////////////////////////
// Render
// - Draw the collision layer to the screen
void Level::Render()
{
	//// Handle to graphics manager
	//SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	//// TODO - Revamp

	//// Render the collision layer

	//// Loop through the map
	//for (int xx = 0; xx < m_nWidth; ++xx)
	//{
	//	for (int yy = 0; yy < m_nHeight; ++yy)
	//	{
	//		// Determine color based on tile type
	//		int value = m_nCollisionLayer[xx][yy];
	//		SGD::Color color = { 0, 0, 0 };

	//		switch (value)
	//		{
	//			// Walls
	//			case 0:
	//				color = { 150, 150, 150 }; // gray
	//				break;
	//				// Death
	//			case 1:
	//				color = { 255, 150, 150 }; // red
	//				break;
	//				// Left ramp
	//			case 2:
	//				color = { 160, 80, 250 }; // purple
	//				break;
	//				// Right ramp
	//			case 3:
	//				color = { 250, 70, 210 }; // pink
	//				break;
	//				// Ice
	//			case 4:
	//				color = { 0, 220, 220 }; // Cyan
	//				break;
	//				// Ice left ramp
	//			case 5:
	//				color = { 50, 110, 0 }; // dark green
	//				break;
	//				// Ice right ramp
	//			case 6:
	//				color = { 30, 220, 0 }; // light green
	//				break;

	//		}

	//		// Don't draw empty tiles
	//		if (value != -1)
	//		{
	//			// Rectangle for drawing
	//			//SGD::Rectangle rect = { xx * 32.0f, yy * 32.0f, xx * 32.0f + 32, yy * 32.0f + 32 };
	//			//pGraphics->DrawRectangle(rect, color, { 0, 0, 0 }, 2);

	//			Camera::GetInstance()->Draw(SGD::Rectangle(
	//				xx * 32.0f - Camera::GetInstance()->GetCameraPos().x				/** Camera::GetInstance()->GetZoomScale()*/,
	//				yy * 32.0f - Camera::GetInstance()->GetCameraPos().y				/** Camera::GetInstance()->GetZoomScale()*/,
	//				xx * 32.0f - Camera::GetInstance()->GetCameraPos().x + 32.0f		/** Camera::GetInstance()->GetZoomScale()*/,
	//				yy * 32.0f - Camera::GetInstance()->GetCameraPos().y + 32.0f		/** Camera::GetInstance()->GetZoomScale()*/),
	//				color);
	//		}
	//	}
	//}

}

//////////////////////////////
// RenderBackground
// - Render all image layers with a depth lower than 0
void Level::RenderImageLayer(bool background)
{
	// Handle to graphics manager
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Loop through each layer
	for (unsigned int i = 0; i < m_vlLayers.size(); ++i)
	{
		// Reference to the alyer
		Layer * layer = m_vlLayers[i];

		// Only render layers that have a depth lower than 0 if background
		// greater than or equal to 0 if not
		if ((background && layer->GetDepth() < 0) ||
			(!background && layer->GetDepth() >= 0))
		{
			// Variables to help with culling
			int startX = (int)(Camera::GetInstance()->GetCameraPos().x / 32 * layer->GetScrollSpeed()) - 1;
			int startY = (int)(Camera::GetInstance()->GetCameraPos().y / 32 * layer->GetScrollSpeed()) - 1;
			int mapWidth = (int)(((Game::GetInstance()->GetScreenWidth() / 32.0f)	/ Camera::GetInstance()->GetZoomScale()) +100);
			int mapHeight = (int)(((Game::GetInstance()->GetScreenHeight() / 32.0f) / Camera::GetInstance()->GetZoomScale()) + 100);

			// TODO fix culling

			// Loop through the map
			for (int xx = startX; xx < startX + mapWidth + 2; ++xx)
			{
				for (int yy = startY; yy < startY + mapHeight + 2; ++yy)
				{
					// Check if the current tile is within bounds
					if (xx >= 0 && xx < m_nWidth && yy >= 0 && yy < m_nHeight)
					{
						// Type of tile
						int type = layer->GetImageLayer()[xx][yy];

						// Only render tiles that have a value
						if (type != -1)
						{
							// Section on the tileset
							int width = layer->GetTileSetWidth();
							int height = layer->GetTileSetHeight();
							int size = layer->GetTileSetSize();
							SGD::Rectangle section = SGD::Rectangle(
								type % width * size * 1.0f,
								type / width * size * 1.0f,
								type % width * size + 32.0f,
								type / width * size + 32.0f
								);

							// Draw the tile
							Camera::GetInstance()->DrawTextureSection(
								layer->GetTileSet(),
								{	(xx * 32.0f)   - Camera::GetInstance()->GetCameraPos().x * layer->GetScrollSpeed(),
									(yy * 32.0f)  - Camera::GetInstance()->GetCameraPos().y * layer->GetScrollSpeed() },
								section,
								0.0f,
								{ 0.0f, 0.0f },
								{ 255, 255, 255 },
								{ 1.0f, 1.0f });

						//	Camera::GetInstance()->DrawTexture({ xx * 32.0f - Camera::GetInstance()->GetCameraPos().x * layer->GetScrollSpeed(),
						//		yy * 32.0f - Camera::GetInstance()->GetCameraPos().y * layer->GetScrollSpeed() }, 0.0f, layer->GetTileSet(), false);
						}
					}
				}
			}
		}
	}
}

//////////////////////////////
// Startup
// - Called when a level is first opened
void Level::Startup()
{
	// Reset how much honor you collected this level
	m_unHonorBeforeDeath = 0;
}

//////////////////////////////
// Exit
// - Called when a level is over
void Level::Exit()
{
	// Update map of honor in gameplay state
	GameplayState::GetInstance()->SetHonorVector(m_vCollectedHonor);
}

//////////////////////////////
// Load
// - Loads in the level from the XML file
bool Level::LoadLevel(const char * _path)
{

	// Used to keep track of loading progress
	unsigned int progress = 0;
	DrawLoadScreen(progress);

	//Create the tinyxml document 
	TiXmlDocument doc;

	// Load the file
	if (doc.LoadFile(_path) == false)
	{
		return false;
	}

	// Grab reference to the root
	TiXmlElement* pRoot = doc.RootElement();
	if (pRoot == nullptr)
	{
		return false;
	}

	// Update progress
	++progress;
	DrawLoadScreen(progress);

	// Read in width and height of the map
	pRoot->Attribute("width", &m_nWidth);
	pRoot->Attribute("height", &m_nHeight);

	// Read in if fixed
	int fixed;
	pRoot->Attribute("fixed", &fixed);
	m_bFixed = fixed ? true : false;

	// Read in players start coordinates
	pRoot->Attribute("playerX", &m_nPlayerX);
	pRoot->Attribute("playerY", &m_nPlayerY);

	// Load in the image layers by finding out how many there are
	TiXmlElement* pImageLayers = pRoot->FirstChildElement();
	int numLayers = 0;
	pImageLayers->Attribute("numLayers", &numLayers);

	// Update progress
	++progress;
	DrawLoadScreen(progress);

	// Read in each image layer
	TiXmlElement * pImageLayer = pImageLayers->FirstChildElement();
	for (int i = 0; i < numLayers; ++i)
	{
		// Create an empty layer
		Layer * imageLayer = new Layer;

		// Set individual values

		// speed
		double speed;
		pImageLayer->Attribute("speed", &speed);
		imageLayer->SetScrollSpeed((float)(speed));

		// depth
		int depth;
		pImageLayer->Attribute("depth", &depth);
		imageLayer->SetDepth(depth);

		// tileset dimensions
		int tileSetWidth;
		pImageLayer->Attribute("tileSetWidth", &tileSetWidth);
		imageLayer->SetTileSetWidth(tileSetWidth);
		int tileSetHeight;
		pImageLayer->Attribute("tileSetHeight", &tileSetHeight);
		imageLayer->SetTileSetHeight(tileSetHeight);
		int tileSetSize;
		pImageLayer->Attribute("tileSetSize", &tileSetSize);
		imageLayer->SetTileSetSize(tileSetSize);

		// tileset texture
		std::string path = "";
		path += pImageLayer->Attribute("image");
		path = path.erase(0, 1);
		SGD::HTexture texture = SGD::GraphicsManager::GetInstance()->LoadTexture(path.c_str());
		imageLayer->SetTileSet(texture);

		// Read in tiles
		int ** info = new int*[m_nWidth];
		TiXmlElement* pTile = pImageLayer->FirstChildElement();
		for (int xx = 0; xx < m_nWidth; ++xx)
		{
			info[xx] = new int[m_nHeight];
			for (int yy = 0; yy < m_nHeight; ++yy)
			{
				int value = 0;
				pTile->Attribute("type", &value);
				info[xx][yy] = value;
				pTile = pTile->NextSiblingElement();
			}
		}

		// Assign the tile info and push the layer into the vector
		imageLayer->SetImageLayer(info);
		imageLayer->SetWidth(m_nWidth);
		imageLayer->SetHeight(m_nWidth);
		m_vlLayers.push_back(imageLayer);

		// Read in the next layer
		pImageLayer = pImageLayer->NextSiblingElement();
	}

	// Update progress
	++progress;
	DrawLoadScreen(progress);

	// Read in the collision layer
	TiXmlElement * pCollisionLayer = pImageLayers->NextSiblingElement();

	// Read in tiles
	m_nCollisionLayer = new int*[m_nWidth];
	TiXmlElement* pTile = pCollisionLayer->FirstChildElement();
	for (int xx = 0; xx < m_nWidth; ++xx)
	{
		m_nCollisionLayer[xx] = new int[m_nHeight];
		for (int yy = 0; yy < m_nHeight; ++yy)
		{
			int value = 0;
			pTile->Attribute("type", &value);
			m_nCollisionLayer[xx][yy] = value;
			pTile = pTile->NextSiblingElement();
		}
	}

	// Update progress
	++progress;
	DrawLoadScreen(progress);

	// Read in the event layer
	TiXmlElement * pEventLayer = pCollisionLayer->NextSiblingElement();

	// Read in tiles
	m_stEventLayer = new std::string*[m_nWidth];
	pTile = pEventLayer->FirstChildElement();
	for (int xx = 0; xx < m_nWidth; ++xx)
	{
		m_stEventLayer[xx] = new std::string[m_nHeight];
		for (int yy = 0; yy < m_nHeight; ++yy)
		{
			// Read in strings?
			/*char * value = nullptr;
			pTile->Attribute(
			collisionInfo[xx][yy] = value;*/
			std::string value = "";
			value = pTile->Attribute("event");
			m_stEventLayer[xx][yy] = value;
			pTile = pTile->NextSiblingElement();
		}
	}

	// Update progress
	++progress;
	DrawLoadScreen(progress);

	// Read in the entities
	TiXmlElement * pEntities = pEventLayer->NextSiblingElement();
	int numEntities;
	pEntities->Attribute("numEntities", &numEntities);

	// Only proceed if there are any entities
	if (numEntities > 0)
	{
		// Keep track of honor
		unsigned int honorIndex = 0;
		// Loop through each entity
		TiXmlElement * pEntity = pEntities->FirstChildElement();
		for (int i = 0; i < numEntities; ++i)
		{
			// Load in variables
			int type;
			pEntity->Attribute("type", &type);
			int x;
			pEntity->Attribute("x", &x);
			int y;
			pEntity->Attribute("y", &y);
			int numArgs;
			pEntity->Attribute("numArgs", &numArgs);
			
			// TODO when all entities are done, call each ones create method

#pragma region Switch for each type of entity
			switch (type)
			{
				case 0: // Honor
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int amount;
					pArg->Attribute("value", &amount);
					unsigned int vectorSize = GameplayState::GetInstance()->GetHonorVectorSize();
					// If there is a value in collected honor for this
					if (honorIndex < vectorSize)
					{
						// push back the value
						m_vCollectedHonor.push_back(GameplayState::GetInstance()->GetHonorValue(honorIndex));
					}
					else
					{
						// it could not have been collected
						m_vCollectedHonor.push_back(false);
					}
					GameplayState::GetInstance()->CreateHonor(x, y, amount, honorIndex);
					++honorIndex;
					break;
				}
				case 1: // Activator
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int pressurePlate;
					pArg->Attribute("value", &pressurePlate);
					pArg = pArg->NextSiblingElement();
					int active;
					pArg->Attribute("value", &active);
					pArg = pArg->NextSiblingElement();
					int ID;
					pArg->Attribute("value", &ID);
					GameplayState::GetInstance()->CreateActivator(x, y, pressurePlate ? true : false, active ? true : false, ID);
					break;
				}
				case 2: // Laser
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int direction;
					pArg->Attribute("value", &direction);
					pArg = pArg->NextSiblingElement();
					int ID;
					pArg->Attribute("value", &ID);
					SGD::Vector dir;
					switch (direction)
					{
					case 0:
						dir = SGD::Vector(1, 0);
						break;
					case 1:
						dir = SGD::Vector(0, -1);
						break;
					case 2:
						dir = SGD::Vector(-1, 0);
						break;
					case 3:
						dir = SGD::Vector(0, 1);
						break;
					}
					pArg = pArg->NextSiblingElement();
					int on;
					pArg->Attribute("value", &on);
					GameplayState::GetInstance()->CreateLaser(x, y, dir, ID, on ? true : false);
					break;
				}
				case 3: // Turret
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int direction;
					pArg->Attribute("value", &direction);
					pArg = pArg->NextSiblingElement();
					double timer;
					pArg->Attribute("value", &timer);
					GameplayState::GetInstance()->CreateTurret(x, y, direction, (float)timer);
					break;
				}
				case 4: // Door
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int horizontal;
					pArg->Attribute("value", &horizontal);
					pArg = pArg->NextSiblingElement();
					int ID;
					pArg->Attribute("value", &ID);
					pArg = pArg->NextSiblingElement();
					int startOpen;
					pArg->Attribute("value", &startOpen);
					GameplayState::GetInstance()->CreateDoor(x, y, horizontal ? true : false, ID, startOpen ? true : false);
					break;
				}
				case 5: // Moving platforms
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int vertical;
					pArg->Attribute("value", &vertical);
					pArg = pArg->NextSiblingElement();
					double turnDistance;
					pArg->Attribute("value", &turnDistance);
					pArg = pArg->NextSiblingElement();
					double speed;
					pArg->Attribute("value", &speed);
					GameplayState::GetInstance()->CreateMovingPlatform(x, y, vertical ? true : false, (float)turnDistance, (float)speed);
					break;
				}
				case 6: // Geyser
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					double speed;
					pArg->Attribute("value", &speed);
					pArg = pArg->NextSiblingElement();
					double maxHeight;
					pArg->Attribute("value", &maxHeight);
					pArg = pArg->NextSiblingElement();
					int currState;
					pArg->Attribute("value", &currState);
					GameplayState::GetInstance()->CreateGeyser(x, y, (float)speed, (float)maxHeight, currState);
					break;
				}
				case 7: // Lava
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					double speed;
					pArg->Attribute("value", &speed);
					pArg = pArg->NextSiblingElement();
					double maxHeight;
					pArg->Attribute("value", &maxHeight);
					pArg = pArg->NextSiblingElement();
					int currState;
					pArg->Attribute("value", &currState);
					GameplayState::GetInstance()->CreateLava(x, y, (float)speed, (float)maxHeight, currState);
					break;
				}
				case 8: // Falling block
				{
					GameplayState::GetInstance()->CreateFallingBlock(x, y);
					break;
				}
				case 9: // Block
				{
					GameplayState::GetInstance()->CreateBlock(x, y);
					break;
				}
				case 10: // Pendulum
				{
					GameplayState::GetInstance()->CreatePendulum(x, y);
					break;
				}
				case 11: // Buzz saw
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int horizontal;
					pArg->Attribute("value", &horizontal);
					pArg = pArg->NextSiblingElement();
					int returnDistance;
					pArg->Attribute("value", &returnDistance);
					GameplayState::GetInstance()->CreateBuzzsaw(x, y, horizontal ? true : false, returnDistance);
					break;
				}
				case 12: // Stalactite
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					double timer;
					pArg->Attribute("value", &timer);
					GameplayState::GetInstance()->CreateStalactite(x, y, (float)timer);
					break;
				}
				case 13: // Armor
				{
					GameplayState::GetInstance()->CreateArmor(x, y);
					break;
				}
				case 14: // Freezable ground
				{
					GameplayState::GetInstance()->CreateFreezableGround(x, y);
					break;
				}
				case 15: // Freezable left ramp
				{
					GameplayState::GetInstance()->CreateFreezableLeftRamp(x, y);
					break;
				}
				case 16: // Freezable right ramp
				{
					GameplayState::GetInstance()->CreateFreezableRightRamp(x, y);
					break;
				}
				case 17: // Hint statue
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					std::string message = pArg->Attribute("value");
					GameplayState::GetInstance()->CreateHintStatue(x, y, message);
					break;
				}
				case 18: // Teleporter
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int hubTeleporter;
					pArg->Attribute("value", &hubTeleporter);
					pArg = pArg->NextSiblingElement();
					std::string level = pArg->Attribute("value");
					GameplayState::GetInstance()->CreateTeleporter(x, y, level, hubTeleporter ? true : false);
					break;
				}
				case 19: // Enemy
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int enemyType;
					pArg->Attribute("value", &enemyType);
					GameplayState::GetInstance()->CreateEnemy(x, y, enemyType);
					break;
				}
				case 20: // Boss
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int bossType;
					pArg->Attribute("value", &bossType);
					GameplayState::GetInstance()->CreateBoss(x, y, bossType);
					break;
				}
				case 21: // Boss teleporter
				{
					TiXmlElement * pArg = pEntity->FirstChildElement();
					int honorNeeded;
					pArg->Attribute("value", &honorNeeded);
					pArg = pArg->NextSiblingElement();
					std::string level = pArg->Attribute("value");
					GameplayState::GetInstance()->CreateBossTeleporter(x, y, level, honorNeeded);
					break;
				}
			}

			// Go to next node
			if (i != numEntities - 1)
			{
				pEntity = pEntity->NextSiblingElement();
			}
#pragma endregion
		}
	}

	// Update progress
	++progress;
	DrawLoadScreen(progress);


	return true;

}

//////////////////////////////
// CheckCollision
// - Handles collision with the passed in entity and collision layer
void Level::CheckCollision(IEntity * _entity)
{
	SGD::Rectangle rect = _entity->GetRect();
	// Determine the position of the enity in the array
	int x = (int)(rect.left / 32);
	int y = (int)(rect.top / 32);

	// Determine how far to check
	int width = (int)((rect.right - 1 - rect.left) / 32);
	int height = (int)((rect.bottom - 1 - rect.top) / 32);

	//int x2 = (int)((rect.right - 1) / 32);
	//int y2 = (int)((rect.bottom - 1) / 32);

	// loop through all adjacent collision pieces
	for (int xx = x; xx <= x + width + 1; ++xx)
	{
		for (int yy = y; yy <= y + height + 1; ++yy)
		{
			// Check if the current piece is in the bounds of the array
			if (xx >= 0 && xx < m_nWidth && yy >= 0 && yy < m_nHeight)
			{
				// Figure out what is at this position
				int id = m_nCollisionLayer[xx][yy];

				if (id != -1) // only check non empty tiles
				{
					LevelCollider * collider = new LevelCollider();
					collider->SetPosition({ xx * 32.0f, yy * 32.0f });
					collider->SetSize({ 32, 32 });
					if (collider->GetRect().IsIntersecting(_entity->GetRect()))
					{
						collider->SetCollide(id);
						_entity->HandleCollision(collider);
					}
					delete collider;
				}
			}
		}
	}
}

//////////////////////////////
// CheckEvent
// - Handles the event with the passed in entity and collision layer
void Level::CheckEvent(IEntity * _entity)
{
	SGD::Rectangle rect = _entity->GetRect();
	// Determine the position of the enity in the array
	int x = (int)(rect.left / 32);
	int y = (int)(rect.top / 32);

	// Determine how far to check
	int width = (int)((rect.right - 1 - rect.left) / 32);
	int height = (int)((rect.bottom - 1 - rect.top) / 32);

	//int x2 = (int)((rect.right - 1) / 32);
	//int y2 = (int)((rect.bottom - 1) / 32);

	// loop through all adjacent collision pieces
	for (int xx = x; xx <= x + width + 1; ++xx)
	{
		for (int yy = y; yy <= y + height + 1; ++yy)
		{
			// Check if the current piece is in the bounds of the array
			if (xx >= 0 && xx < m_nWidth && yy >= 0 && yy < m_nHeight)
			{
				// Figure out what is at this position
				std::string levelEvent = m_stEventLayer[xx][yy];

				SGD::Rectangle rEvent = { xx*32.0f, yy*32.0f, xx*32.0f + 32.0f, yy*32.0f + 32.0f };
				if (levelEvent != "" && rEvent.IsIntersecting(_entity->GetRect())) // only throw non blank events
				{
					// Throw event
 					SGD::Event* pATEvent = new SGD::Event(levelEvent.c_str(), nullptr, _entity);
					SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
					pATEvent = nullptr;
				}
			}
		}
	}
}

//////////////////////////////
// UpdateHonorVector
// -Updates if the honor at the given index has been collected
void Level::UpdateHonorVector(int _index, bool _value)
{
	m_vCollectedHonor[_index] = _value;
}

//////////////////////////////
// RenderMiniMap
// -Draws the collisionlayer onto the mini map
void Level::RenderMiniMap()
{
	// Reference to the graphics manager
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	// References to width and height of the mini map
	float mapWidth = GameplayState::GetInstance()->GetMiniMapWidth();
	float mapHeight = GameplayState::GetInstance()->GetMiniMapHeight();
	float mapOffset = GameplayState::GetInstance()->GetBorderSize();

	// Figure out some values needed
	float tileWidth = mapWidth / m_nWidth;
	float tileHeight = mapHeight / m_nHeight;

	// Loop through collision layer
	for (int xx = 0; xx < m_nWidth; ++xx)
	{
		for (int yy = 0; yy < m_nHeight; ++yy)
		{
			// Determine color based on tile type
			int value = m_nCollisionLayer[xx][yy];
			SGD::Color color = { 100, 100, 100, 100 };

			switch (value)
			{
				// Walls
				case 0:
					color = { 100, 100, 100, 100 }; // gray
					break;
					// Death
				case 1:
					color = { 100, 200, 0, 0 }; // red
					break;
					// Left/Right ramp
				case 2:
				case 3:
					color = { 100, 30, 220, 0 }; // light green
					break;
					// Ice
				case 4:
					color = { 100, 0, 220, 220 }; // Cyan
					break;
					// Ice left/right ramp
				case 5:
				case 6:
					color = { 100, 30, 220, 0 }; // light green
					break;

			}

			// Don't draw empty tiles
			if (value != -1)
			{
				float xScale = ((float)xx / (float)m_nWidth) * mapWidth;
				float yScale = ((float)yy / (float)m_nHeight) * mapHeight;
				// Rectangle for drawing
				SGD::Rectangle rect = { xx * tileWidth + mapOffset,
										yy * tileHeight + mapOffset,
										xx * tileWidth + tileWidth + mapOffset,
										yy * tileHeight + tileHeight + mapOffset };
				pGraphics->DrawRectangle(rect, color, {100, 0, 0, 0 }, 2);
			}

		}
	}
}

void Level::DrawLoadScreen(unsigned int _progress)
{

	// Reference to graphics manager
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	// Clear screen
	SGD::Rectangle rect = SGD::Rectangle(0, 0, Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight());
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }, 0);


	// First frame
	if (_progress >= 0)
	{
		pGraphics->DrawTextureSection(m_hPlayerSheet,
		{ 150, 300 }, { 193, 192, 250, 287 },
		0.0f, { 0.0f, 0.0f },
		{ 255, 255, 255, 255 }, { -2.0f, 2.0f });
	}

	// Second frame
	if (_progress >= 1)
	{
		pGraphics->DrawTextureSection(m_hPlayerSheet,
		{ 250, 300 }, { 256, 192, 319, 287 },
		0.0f, { 0.0f, 0.0f },
		{ 255, 255, 255, 255 }, { -2.0f, 2.0f });
	}

	// Third frame
	if (_progress >= 2)
	{
		pGraphics->DrawTextureSection(m_hPlayerSheet,
		{ 350, 316 }, { 317, 200, 383, 287 },
		0.0f, { 0.0f, 0.0f },
		{ 255, 255, 255, 255 }, { -2.0f, 2.0f });
	}

	// Fourth frame
	if (_progress >= 3)
	{
		pGraphics->DrawTextureSection(m_hPlayerSheet,
		{ 450, 300 }, { 379, 192, 440, 287 },
		0.0f, { 0.0f, 0.0f },
		{ 255, 255, 255, 255 }, { -2.0f, 2.0f });
	}

	// Fifth frame
	if (_progress >= 4)
	{
		pGraphics->DrawTextureSection(m_hPlayerSheet,
		{ 550, 300 }, { 182, 83, 237, 163 },
		0.0f, { 0.0f, 0.0f },
		{ 255, 255, 255, 255 }, { -2.0f, 2.0f });
	}

	// Sixth frame
	if (_progress >= 5)
	{
		pGraphics->DrawTextureSection(m_hPlayerSheet,
		{ 650, 285 }, { 243, 83, 288, 171 },
		0.0f, { 0.0f, 0.0f },
		{ 255, 255, 255, 255 }, { -2.0f, 2.0f });
	}


	// Seventh frame
	if (_progress >= 6)
	{
		pGraphics->DrawTextureSection(m_hPlayerSheet,
		{ 770, 320 }, { 290, 83, 344, 194 },
		0.0f, { 0.0f, 0.0f },
		{ 255, 255, 255, 255 }, { -2.0f, 2.0f });
	}

	// Draw loading
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");
	font.DrawString("LOADING", 250, 200, 2.0f, { 255, 0, 0 });

	// Push final changes
	pGraphics->Update();

}