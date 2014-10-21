//***********************************************************************
//	File:		Layer.cpp
//	Author:		Michael Sciortino
//	Course:		SGD 1408
//	Purpose:	A layer will hold all information needed for a single image layer
//***********************************************************************

#include "Layer.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


//////////////////////////////
// Constructor
Layer::Layer()
{
}

//////////////////////////////
// Destructor
Layer::~Layer()
{
	// Loop through and free the array
	for (int i = 0; i < m_iWidth; ++i)
	{
		delete[] m_iImageLayer[i];
	}
	delete[] m_iImageLayer;

	// release the texture
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_tTileSet);

}
