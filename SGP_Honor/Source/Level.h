//***********************************************************************
//	File:		Level.h
//	Author:		Michael Sciortino
//	Course:		SGD 1408
//	Purpose:	A level will hold all data for tiles and collision for a level
//***********************************************************************

#pragma once

#include <vector>
#include <string>

//////////////////////////////
// Forward declarations
class Layer;
class Camera;
class Honor;
class Armor;
class IEntity;

class Level
{
public:
	//////////////////////////////
	// Ctor/Dtor
	Level();
	~Level();

	//////////////////////////////
	// Main interface
	void Update(float _dt);
	void Render();
	void RenderImageLayer(bool background);
	void Startup();
	void Exit();
	bool LoadLevel(const char * _path);
	void CheckCollision(IEntity * _entity);
	void CheckEvent(IEntity * _entity);

	//////////////////////////////
	// Accessors
	int GetPlayerX() const { return m_nPlayerX; }
	int GetPlayerY() const { return m_nPlayerY; }

private:
	//////////////////////////////
	// Member fields
	int						m_nWidth;
	int						m_nHeight;
	std::vector<Layer*>		m_vlLayers;
	int**					m_nCollisionLayer;
	std::string**			m_stEventLayer;
	bool					m_bFixed;
	bool					m_bUnlocked;
	Camera *				m_pCamera;
	std::vector<Honor*>		m_vHonor;
	std::vector<Armor*>		m_vArmor;
	int						m_nPlayerX;
	int						m_nPlayerY;

};

