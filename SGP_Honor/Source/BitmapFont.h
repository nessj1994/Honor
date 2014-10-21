#pragma once

#include <map>
#include "../SGD Wrappers/SGD_Color.h"
#include "Font.h"
#include <vector>

class BitmapFont
{
public:
	BitmapFont();
	~BitmapFont();

	//Load in new fonts
	bool LoadFont(const char* filePath);

	//Draw the given string with the given font
	void DrawText(std::string text, int x, int y, std::string font, float scale, SGD::Color color);

	/////////////////////////////////////////////////////
	////////////////// Font Accessor ///////////////////
	Font GetFont(std::string font) { return m_mFonts[font]; }

private:
	//friends with font class to access its private members
	friend class Font;
	friend class Letter;
	//Map for easy storage of font instances
	std::map<std::string, Font> m_mFonts;

	//Vector to store keys of the map
	std::vector<std::string> m_vKeys;

};

