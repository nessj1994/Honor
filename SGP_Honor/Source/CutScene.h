/*
|	File: CutScene.h
|	Author: Michael Sciortino
|	Purpose: Used to manage a slideshow to use as a cutscene
|
*/

#pragma once

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <string>
#include <vector>

class CutScene
{
public:
	///////////////////////////////////
	// Ctor/Dtor
	CutScene();
	~CutScene();

	///////////////////////////////////
	// Main interface
	void Update(float _elapsedTime);
	void Render();
	void LoadCutScene(std::string _path);
	bool GetFinished() const { return m_unCurrSlide >= m_vtImages.size(); }

private:
	///////////////////////////////////
	// Member fields
	unsigned int m_unCurrSlide = 0;
	std::vector<SGD::HTexture> m_vtImages;

};