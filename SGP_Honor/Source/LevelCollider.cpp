#include "LevelCollider.h"


LevelCollider::LevelCollider()
{
}


LevelCollider::~LevelCollider()
{
}


int LevelCollider::GetType() const
{
	switch (m_nCollide)
	{
		// Solid wall
	case 0:
		return ENT_SOLID_WALL;
		// Death
	case 1:
		return ENT_DEATH;
		// Left ramp
	case 2:
		return ENT_LEFT_RAMP;
		// Right ramp
	case 3:
		return ENT_RIGHT_RAMP;
		// Ice
	case 4:
		return ENT_ICE;
		// Ice left ramp
	case 5:
		return ENT_ICE_LEFT_RAMP;
		// Ice right ramp
	case 6:
		return ENT_ICE_RIGHT_RAMP;
	}
	return -1;
}