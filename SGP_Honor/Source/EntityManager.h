#pragma once


class IEntity;
#include <vector>

class EntityManager
{
public:
	EntityManager() = default;
	~EntityManager() = default;



	/**********************************************************/
	//Entity Storage
	void AddEntity		(IEntity* pEntity, unsigned int _bucket);
	void RemoveEntity	(IEntity* pEntity, unsigned int _bucket);
	void RemoveEntity	(IEntity* pEntity);
	void RemoveAll		(unsigned int _bucket);
	void RemoveAll		(void);

	/**********************************************************/
	//Entity Upkeep
	void UpdateAll		(float elapsedTime);
	void RenderAll		(void);

	void CheckCollisions(unsigned int _bucket1, unsigned int _bucket2);
	void CheckWorldCollision(unsigned int _bucket);

private:

	/**********************************************************/
	// Not a singleton, for safety 
	EntityManager(const EntityManager&)				= default;
	EntityManager& operator= (const EntityManager&)	= delete;

	/**********************************************************/
	// Containers
	typedef std::vector< IEntity* >		EntityVector;
	typedef std::vector< EntityVector >	EntityTable;


	/**********************************************************/
	// members:
	EntityTable	m_tEntities;			// vector-of-vector-of-IEntity* (2D table)
	bool		m_bIterating = false;	// read/write lock

};

