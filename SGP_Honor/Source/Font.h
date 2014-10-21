#pragma once

#include <map>
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "Letter.h"

class Font
{
public:
	Font();
	~Font();

	//Function to draw the font to screen
	void DrawString(std::string text, int x, int y, float scale, SGD::Color color);
	void DrawString(const wchar_t* text, int x, int y, float scale, SGD::Color color);

private:
	//Friend class to avoid accessors and mutators and allow access to map.
	friend class BitmapFont;
	friend class Letter;

	//Map to store all of the characters used in this font;
	std::map<char, Letter> m_mCharacters;

	//Image handle
	SGD::HTexture m_hImage = SGD::INVALID_HANDLE;

	//data members 
	int m_nLineHeight;

};

