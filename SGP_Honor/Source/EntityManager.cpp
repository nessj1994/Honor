#include "EntityManager.h"
#include "IEntity.h"
#include "Entity.h"
#include "Level.h"
#include "GameplayState.h"
#include <cassert>



/**************************************************************/
// AddEntity
//	- store the entity into the specified bucket
//	- the Entity Manager holds a reference to the entity
void EntityManager::AddEntity(IEntity* pEntity, unsigned int bucket)
{
	// Validate the parameter
	assert(pEntity != nullptr && "EntityManager::AddEntity - parameter cannot be null");


	// Expand the table?
	if (bucket >= m_tEntities.size())
		m_tEntities.resize(bucket + 1);


	// Append the entity into the specified vector
	m_tEntities[bucket].push_back(pEntity);

	// Hold a reference to keep the entity in memory
	pEntity->AddRef();
}


/**************************************************************/
// RemoveEntity
//	- remove the entity from the specified bucket
//	- release the reference to the entity
void EntityManager::RemoveEntity(IEntity* pEntity, unsigned int bucket)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RemoveEntity - cannot remove while iterating");

	// Validate the parameters
	assert(pEntity != nullptr && "EntityManager::RemoveEntity - cannot remove NULL");
	assert(bucket < m_tEntities.size() && "EntityManager::RemoveEntity - invalid bucket");


	// Try to find the entity
	EntityVector& vec = m_tEntities[bucket];
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == pEntity)
		{
			// Remove the entity
			vec.erase(vec.begin() + i);
			pEntity->Release();
			break;
		}
	}
}


/**************************************************************/
// RemoveEntity
//	- remove & release the entity from any bucket
void EntityManager::RemoveEntity(IEntity* pEntity)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RemoveEntity - cannot remove while iterating");

	// Validate the parameters
	assert(pEntity != nullptr && "EntityManager::RemoveEntity - pointer cannot be null");


	// Try to find the entity in any buckect
	for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
	{
		EntityVector& vec = m_tEntities[bucket];
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i] == pEntity)
			{
				// Remove the entity
				vec.erase(vec.begin() + i);
				pEntity->Release();
				return;
			}
		}
	}
}


/**************************************************************/
// RemoveAll
//	- remove all entities from a specific bucket
void EntityManager::RemoveAll(unsigned int unBucket)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RemoveAll - cannot remove while iterating");

	// Validate the parameter
	assert(unBucket < m_tEntities.size() && "EntityManager::RemoveAll - invalid bucket");


	// Lock the iterator
	m_bIterating = true;
	{
		// Release the reference to EVERY entity
		EntityVector& vec = m_tEntities[unBucket];
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			vec[i]->Release();
			vec[i] = nullptr;
		}

		vec.clear();
	}
	// Unlock the iterator
	m_bIterating = false;
}



/**************************************************************/
// RemoveAll
//	- release each entity in the table
void EntityManager::RemoveAll(void)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RemoveAll - cannot remove while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Release every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
			{
				vec[i]->Release();
				vec[i] = nullptr;
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;


	// Collapse the table
	m_tEntities.clear();
}


/**************************************************************/
// UpdateAll
//	- update each entity in the table
void EntityManager::UpdateAll(float elapsedTime)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::UpdateAll - cannot update while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Update every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
				vec[i]->Update(elapsedTime);
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}


/**************************************************************/
// RenderAll
//	- render each entity in the table
void EntityManager::RenderAll(void)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RenderAll - cannot render while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Render every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
				vec[i]->Render();
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}


/**************************************************************/
// CheckCollisions
//	- check collision between the entities within the two buckets
void EntityManager::CheckCollisions(unsigned int bucket1, unsigned int bucket2)
{
	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::CheckCollisions - cannot collide while iterating");

	// Quietly validate the parameters
	if (bucket1 >= m_tEntities.size()
		|| bucket2 >= m_tEntities.size()
		|| m_tEntities[bucket1].size() == 0
		|| m_tEntities[bucket2].size() == 0)
		return;


	// Lock the iterator
	m_bIterating = true;
	{
		// Are they different buckets?
		if (bucket1 != bucket2)
		{
			// Which bucket is smaller?
			//	should be the outer loop for less checks (n0)*(n1+1) + 1
			EntityVector* pVec1 = &m_tEntities[bucket1];
			EntityVector* pVec2 = &m_tEntities[bucket2];

			if (pVec2->size() < pVec1->size())
			{
				EntityVector* pTemp = pVec1;
				pVec1 = pVec2;
				pVec2 = pTemp;
			}

			EntityVector& vec1 = *pVec1;
			EntityVector& vec2 = *pVec2;


			// Iterate through the smaller bucket
			for (unsigned int i = 0; i < vec1.size(); i++)
			{
				// Iterate through the larger bucket
				for (unsigned int j = 0; j < vec2.size(); j++)
				{
					// Ignore self-collision
					if (vec1[i] == vec2[j])
						continue;

					// Local variables help with debugging
					SGD::Rectangle rEntity1 = vec1[i]->GetRect();
					SGD::Rectangle rEntity2 = vec2[j]->GetRect();

					// Check for collision between the entities
					if (rEntity1.IsIntersecting(rEntity2) == true)
					{
						// Both objects handle collision
						vec1[i]->HandleCollision(vec2[j]);
						vec2[j]->HandleCollision(vec1[i]);
					}
				}
			}
		}
		else // bucket1 == bucket2
		{
			EntityVector& vec = m_tEntities[bucket1];

			// Optimized loop to ensure objects do not collide with
			// each other twice
			for (unsigned int i = 0; i < vec.size() - 1; i++)
			{
				for (unsigned int j = i + 1; j < vec.size(); j++)
				{
					// Ignore self-collision
					if (vec[i] == vec[j])
						continue;

					// Local variables help with debugging
					SGD::Rectangle rEntity1 = vec[i]->GetRect();
					SGD::Rectangle rEntity2 = vec[j]->GetRect();

					// Check for collision between the entities
					if (rEntity1.IsIntersecting(rEntity2) == true)
					{
						// Both objects handle collision
						vec[i]->HandleCollision(vec[j]);
						vec[j]->HandleCollision(vec[i]);
					}
				}
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

/**************************************************************/
// CheckCollisions
//	- check collision between the given bucket and the world
void EntityManager::CheckWorldCollision(unsigned int _bucket)
{
	// Make sure there is something in the bucket to check
	if (_bucket >= m_tEntities.size()
		|| m_tEntities[_bucket].size() == 0)
		return;

	// Reference to the level
	Level * level = GameplayState::GetInstance()->GetCurrentLevel();

	// The vector of the given bucket
	EntityVector vec = m_tEntities[_bucket];

	// Loop through the bucket
	for (unsigned int i = 0; i < vec.size(); ++i)
	{
		// Call the check collision function and pass in each entity
		level->CheckCollision(vec[i]);
	}
}

/**************************************************************/
// CheckEvents
//	- check collision between the given bucket and the world
void EntityManager::CheckWorldEvent(unsigned int _bucket)
{
	// Reference to the level
	Level * level = GameplayState::GetInstance()->GetCurrentLevel();

	// The vector of the given bucket
	EntityVector vec = m_tEntities[_bucket];

	// Loop through the bucket
	for (unsigned int i = 0; i < vec.size(); ++i)
	{
		// Call the check collision function and pass in each entity
		level->CheckEvent(vec[i]);
	}
}

/**************************************************************/
// RenderMiniMap
//	- Renders each entity as a dot on the minimap
void EntityManager::RenderMiniMap(void)
{
	// Reference to the graphics manager
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	// References to width and height of the mini map
	float mapWidth = GameplayState::GetInstance()->GetMiniMapWidth();
	float mapHeight = GameplayState::GetInstance()->GetMiniMapHeight();
	float mapOffset = GameplayState::GetInstance()->GetBorderSize();

	// References to width and height of the level
	int levelWidth = GameplayState::GetInstance()->GetCurrentLevel()->GetWidth();
	int levelHeight = GameplayState::GetInstance()->GetCurrentLevel()->GetHeight();
	
	// Figure out some values needed
	float tileWidth = mapWidth / levelWidth;
	float tileHeight = mapHeight / levelHeight;

	// Validate the iteration state
	assert(m_bIterating == false && "EntityManager::RenderAll - cannot render while iterating");

	// Lock the iterator
	m_bIterating = true;
	{
		// Render every entity
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++)
		{
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
			{
				SGD::Color color = { 100, 0, 0, 200 };
				// Change colors for different things
				switch (vec[i]->GetType())
				{
					case Entity::ENT_HONOR:
					{
						color = { 100, 200, 100, 50 };
						break;
					}
					case Entity::ENT_PLAYER:
					{
						color = { 100, 200, 200, 60 };
						break;
					}
					case Entity::ENT_TELEPORTER:
					{
						color = { 100, 0, 200, 0 };
						break;
					}
					case Entity::ENT_BULL_ENEMY:
					case Entity::ENT_SKELETON:
					case Entity::ENT_MUTANT_MAN:
					case Entity::ENT_MUTANT_BIRD:
					case Entity::ENT_ICE_BAT:
					case Entity::ENT_ICE_GOLEM:
					case Entity::ENT_ICE_TURTLE:
					case Entity::ENT_SQUID:
					case Entity::ENT_POUNCER:
					case Entity::ENT_JELLYFISH:
					case Entity::ENT_BOSS_BULL:
					case Entity::ENT_BOSS_CAVEMAN:
					case Entity::ENT_BOSS_YETI:
					case Entity::ENT_BOSS_CRAB:
					case Entity::ENT_BOSS_WIZARD:
					case Entity::ENT_LASER:
					case Entity::ENT_TURRET:
					case Entity::ENT_LAVA:
					case Entity::ENT_FALLING_BLOCK:
					case Entity::ENT_PENDULUM:
					case Entity::ENT_BUZZSAW:
					{
						color = { 100, 200, 0, 0 };
					}

				}

				// Render as a dot at the correct position
				float xPos = vec[i]->GetRect().left / 32.0f;
				float yPos = vec[i]->GetRect().top / 32.0f;
				float eWidth = (vec[i]->GetRect().right - vec[i]->GetRect().left) / 32.0f * tileWidth;
				float eHeight = (vec[i]->GetRect().bottom - vec[i]->GetRect().top) / 32.0f * tileHeight;

				// Rectangle for drawing
				SGD::Rectangle rect = { xPos * tileWidth + mapOffset,
					yPos * tileHeight + mapOffset,
					xPos * tileWidth + eWidth + mapOffset,
					yPos * tileHeight + eHeight + mapOffset };

				pGraphics->DrawRectangle(rect, color, { 100, 255, 255, 255 }, 2);
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;

}
