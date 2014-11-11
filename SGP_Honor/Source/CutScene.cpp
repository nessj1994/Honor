/*
|	File: CutScene.cpp
|	Author: Michael Sciortino
|	Purpose: Used to manage a slideshow to use as a cutscene
|
*/

#include "CutScene.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../TinyXML/tinyxml.h"


///////////////////////////////////
// Ctor/Dtor
CutScene::CutScene()
{

}

CutScene::~CutScene()
{
	// Unload each texture
	for (unsigned int i = 0; i < m_vtImages.size(); ++i)
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_vtImages[i]);
	}
}


///////////////////////////////////
// Update
// -Main update loop
void CutScene::Update(float _elapsedTime)
{
	SGD::InputManager * pInput = SGD::InputManager::GetInstance();

	// Go to the next slide when a key is pressed
	if (pInput->IsAnyKeyPressed())
	{
		++m_unCurrSlide;
	}
}

///////////////////////////////////
// Render
// -Main render loop
void CutScene::Render()
{
	// Don't render when finished
	if (!GetFinished())
	{
		// Render the current image
		SGD::HTexture currentImage = m_vtImages[m_unCurrSlide];
		SGD::GraphicsManager::GetInstance()->DrawTexture(currentImage,
		{ 0, 0 }, 0.0f, { 0.0f, 0.0f }, { 255, 255, 255 }, { 1.0f, 1.0f });
	}
}

///////////////////////////////////
// LoadCutScene
// -Loads an XML file with information for this cutscene
void CutScene::LoadCutScene(std::string _path)
{
	//Create the tinyxml document 
	TiXmlDocument doc;

	// Load the file
	if (doc.LoadFile(_path.c_str()) == false)
	{
		return;
	}

	// Grab a reference to the first element
	TiXmlElement* pSlide = doc.RootElement();
	if (pSlide == nullptr)
	{
		return;
	}

	// Loop through and load each slide
	while(pSlide != nullptr)
	{
		// Read in the path and load it into the vector
		std::string imagePath = pSlide->Attribute("path");
		SGD::HTexture temp = SGD::GraphicsManager::GetInstance()->LoadTexture(imagePath.c_str());
		m_vtImages.push_back(temp);

		// Go to the next slide
		pSlide = pSlide->NextSiblingElement();
	}

}