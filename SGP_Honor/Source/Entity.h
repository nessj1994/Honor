#pragma once

#include "IEntity.h"
#include "../SGD Wrappers/SGD_Handle.h" //HTexture type

class Entity :
	public IEntity
{
public:
	Entity(void) = default;

	//enum EntityType { ENT_BASE, ENT_UNIT, ENT_PLAYER, ENT_PROJ, ENT_ENEMY, ENT_BOSS, ENT_BLOCK, ENT_TEMP_FREEZE, ENT_PERM_FREEZE, ENT_FROZEN, ENT_NOT_FROZEN, ENT_FALLING_BLOCK };
	//enum EntityType { ENT_BASE, ENT_UNIT, ENT_PLAYER, ENT_PROJ, ENT_ENEMY, ENT_BOSS, ENT_BLOCK, ENT_TEMP_FREEZE, ENT_PERM_FREEZE, ENT_FROZEN, ENT_NOT_FROZEN, ENT_FALLING_BLOCK, ENT_SPRAY, ENT_LEVEL_COLLIDER };
	enum EntityType {
		ENT_BASE, ENT_UNIT, ENT_PROJ, ENT_ENEMY, ENT_BOSS,
		ENT_BLOCK, ENT_TEMP_FREEZE, ENT_PERM_FREEZE, ENT_FROZEN, ENT_NOT_FROZEN, ENT_HAWK,
		ENT_FALLING_BLOCK, ENT_SPRAY, ENT_DOOR, ENT_BOSS_DOOR, ENT_SWITCH,
		ENT_PRESSURE_PLATE, ENT_MOVING_PLATFORM, ENT_STALACTITE, ENT_SOLID_WALL, ENT_DEATH,
		ENT_LEFT_RAMP, ENT_RIGHT_RAMP, ENT_ICE, ENT_ICE_LEFT_RAMP, ENT_ICE_RIGHT_RAMP, ENT_TURRET, ENT_BUZZSAW,
		ENT_ARMOR, ENT_HONOR, ENT_STATUE, ENT_PENDULUM, ENT_GEYSER, ENT_LASER, ENT_LAVA, ENT_TELEPORTER, ENT_JELLYFISH,
		ENT_BOSS_BULL, ENT_BOSS_CAVEMAN, ENT_BOSS_YETI, ENT_BOSS_CRAB, ENT_BOSS_WIZARD, ENT_SWORD , ENT_SQUID, ENT_POUNCER,
		ENT_BULL_ENEMY, ENT_SKELETON, ENT_MUTANT_MAN, ENT_MUTANT_BIRD, ENT_ICE_GOLEM, ENT_ICE_BAT, ENT_ICE_TURTLE, ENT_PLAYER,
		ENT_VOMIT, ENT_POOP,
	};

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	///////////////////////////////////////////////
	//////////////Reference Counting//////////////
	virtual void AddRef(void) final; //Cannot be overridden
	virtual void Release(void) final; //Cannot be overridden


	//////////////////////////////////////////////
	///////////////////Accessors/////////////////
	SGD::HTexture		GetImage(void) const { return m_hImage; }
	SGD::Point			GetPosition(void) const { return m_ptPosition; }
	SGD::Point			GetStartPosition(void) const { return m_ptStartPosition; }
	SGD::Vector			GetVelocity(void) const { return m_vtVelocity; }
	SGD::Size			GetSize(void) const { return m_szSize; }
	SGD::Size			GetScale(void)	const			{ return m_szScale; }
	float				GetSpeed(void)	const			{ return m_fSpeed; }
	SGD::Vector			GetDirection(void) const		{ return m_vtDirection; }
	float				GetFriction(void) const { return m_fFriction; }



	////////////////////////////////////////////
	//////////////////Mutators/////////////////
	void				SetImage(SGD::HTexture img)		{ m_hImage = img; }
	void				SetPosition(SGD::Point pos)		{ m_ptPosition = pos; }
	void				SetStartPosition(SGD::Point pos){ m_ptStartPosition = pos; }
	void				SetVelocity(SGD::Vector vel)	{ m_vtVelocity = vel; }
	void				SetSize(SGD::Size size)			{ m_szSize = size; }
	void				SetScale(SGD::Size scale)		{ m_szScale = scale; }
	void				SetSpeed(float speed)			{ m_fSpeed = speed; }
	void				SetDirection(SGD::Vector _dir)	{ m_vtDirection = _dir; }
	void				SetFriction(float _friction) { m_fFriction = _friction; }

protected:
	virtual ~Entity(void);

	//////////////////////////////////////////////////
	/////////////////////Members/////////////////////

	//SGD::Point			m_ptPosition	= SGD::Point{ 0, 0 }; //2D Position
	//SGD::Vector			m_vtVelocity	= SGD::Vector{ 0, 0 }; //2D Velocity
	//SGD::Size			m_szSize		= SGD::Size{ 0, 0 }; //2D size
	//SGD::Size			m_szScale		= SGD::Size{ 1, 1 };//2D Scale
	//float				m_fSpeed		= 750.0f;// 250.0f;
	//SGD::Vector			m_vtDirection	= { 0, 0 };     //1,0 right 0,1 down


	SGD::HTexture		m_hImage				= SGD::INVALID_HANDLE; //image handle
	SGD::Point			m_ptPosition			= SGD::Point{ 0, 0 }; //2D Position
	SGD::Point			m_ptStartPosition		= SGD::Point{ 100, 100 }; //2D position
	SGD::Vector			m_vtVelocity			= SGD::Vector{ 0, 0 }; //2D Velocity
	SGD::Vector			m_vtDirection			= { 0, 0 };     //1,0 right 0,1 down
	SGD::Size			m_szSize				= SGD::Size{ 0, 0 }; //2D size
	SGD::Size			m_szScale				= SGD::Size{ 1, 1 };//2D Scale
	float				m_fSpeed				= 2550.0f;// 250.0f;
	float				m_fFriction = 10.0f;


private:

	///////////////////////////////////////////////
	///////////////Reference Counting/////////////
	unsigned int m_unRefCount = 1; //Calling 'new' gives the prime reference
};

