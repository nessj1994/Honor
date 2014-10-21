#include "Font.h"

#include "Letter.h"
#include <assert.h>
#include "../SGD Wrappers/SGD_GraphicsManager.h"

Font::Font()
{
}


Font::~Font()
{
	m_mCharacters.clear();

}

//Called from BitmapFont::DrawText with the proper font instance
void Font::DrawString(std::string text, int x, int y, float scale, SGD::Color color)
{
	//Validate the image we are using
	assert(m_hImage != SGD::INVALID_HANDLE
		&& "Font::DrawString - image was not loaded");

	//Validate the parameter for our text
	assert(text.c_str() != nullptr
		&& "Font::DrawString - string cannot be null");

	//Is the string visible
	if(text[0] == L'\0' //string is empty
		|| scale == 0.0f //scale is too small
		|| color.alpha == 0) //string is invisible
	{
		return;
	}

	//Store the starting x position for a new line
	float colStart = x;

	//loop through the characters in the string
	for(int i = 0; text[i]; i++)
	{
		//Get the current character (in ASCII)
		char ch = (char)text[i];

		//Check for whitespaces
		if(ch == ' ')
		{
			x += m_mCharacters[text[i - 1]].m_nXAdvance * scale;
			continue;
		}
		else if(ch == '\n')
		{
			y += m_nLineHeight * scale;
			x = colStart;
			continue;
		}

		//Create a source rectangle for the segment of the image to load from
		SGD::Rectangle source;
		source.left = m_mCharacters[ch].m_nX;
		source.top = m_mCharacters[ch].m_nY;
		source.right = source.left + m_mCharacters[ch].m_nWidth;
		source.bottom = source.top + m_mCharacters[ch].m_nHeight;

		//Create proper x and y offsets for lower case letters to print lower
		int xOff = m_mCharacters[ch].m_nXOffset * scale;
		int yOff = m_mCharacters[ch].m_nYOffset * scale; 

		//Draw the character
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, SGD::Point(x + xOff, y + yOff),
			source, 0.0f, {}, color, { scale, scale });

		//Move to the next position
		x += (int)(m_mCharacters[ch].m_nXAdvance * scale);

	}

}



//Called from BitmapFont::DrawText with the proper font instance
void Font::DrawString(const wchar_t* text, int x, int y, float scale, SGD::Color color)
{
	//Validate the image we are using
	assert(m_hImage != SGD::INVALID_HANDLE
		&& "Font::DrawString - image was not loaded");

	//Validate the parameter for our text
	assert(text != nullptr
		&& "Font::DrawString - string cannot be null");

	//Is the string visible
	if(text[0] == L'\0' //string is empty
		|| scale == 0.0f //scale is too small
		|| color.alpha == 0) //string is invisible
	{
		return;
	}

	//Store the starting x position for a new line
	float colStart = x;

	//loop through the characters in the string
	for(int i = 0; text[i]; i++)
	{
		//Get the current character (in ASCII)
		char ch = (char)text[i];

		//Check for whitespaces
		if(ch == ' ')
		{
			x += m_mCharacters[text[i - 1]].m_nXAdvance * scale;
			continue;
		}
		else if(ch == '\n')
		{
			y += m_nLineHeight * scale;
			x = colStart;
			continue;
		}

		//Create a source rectangle for the segment of the image to load from
		SGD::Rectangle source;
		source.left = m_mCharacters[ch].m_nX;
		source.top = m_mCharacters[ch].m_nY;
		source.right = source.left + m_mCharacters[ch].m_nWidth;
		source.bottom = source.top + m_mCharacters[ch].m_nHeight;

		//Create proper x and y offsets for lower case letters to print lower
		int xOff = m_mCharacters[ch].m_nXOffset * scale;
		int yOff = m_mCharacters[ch].m_nYOffset * scale;

		//Draw the character
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, SGD::Point(x + xOff, y + yOff),
			source, 0.0f, {}, color, { scale, scale });

		//Move to the next position
		x += (int)(m_mCharacters[ch].m_nXAdvance * scale);

	}

}