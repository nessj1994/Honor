#include "BitmapFont.h"

#include "Font.h"
#include "Letter.h"

#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

BitmapFont::BitmapFont()
{
}


BitmapFont::~BitmapFont()
{
	for(unsigned int i = 0; i < m_mFonts.size(); i++)
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_mFonts[m_vKeys[i]].m_hImage);
	}

}

//Called to load in fonts to the map
bool BitmapFont::LoadFont(const char* filePath)
{

	//Create the tiny xml doc
	TiXmlDocument doc;

	//If the document does not load return false for failure
	if(doc.LoadFile(filePath) == false)
	{
		return false;
	}

	//find the root element
	TiXmlElement* pRoot = doc.RootElement();

	//If it can't find the root element return false for failure
	if(pRoot == nullptr)
	{
		return false;
	}

	//Create our font 
	Font temp;

	//Used for common data of characters
	TiXmlElement* pCommon = pRoot->FirstChildElement("common");
	
	pCommon->Attribute("lineHeight", &temp.m_nLineHeight);


	//Find the child element that holds the data for the texture
	TiXmlElement* pFont = pRoot->FirstChildElement("pages")->FirstChildElement("page");

	//Get the file for the image
	std::string file = pFont->Attribute("file");

	//Append the image path to the rest of the file path (relative)
	std::string fullPath = "../Assets/Graphics/" + file;

	//Set our fonts texture to the previously obtained elements file attribute
	temp.m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(fullPath.c_str());
	
	TiXmlElement* pChars = pRoot->FirstChildElement("chars");

	//Create a count for how many characters should be loaded;
	int charCount = 0;

	//Set the character count
	pChars->Attribute("count", &charCount);

	pChars = pChars->FirstChildElement("char");
	//Loop through each character setting its data properly
	for(int i = 0; i < charCount; i++)
	{
		//Create a letter to be stored
		Letter tempLetter;

		//create an id variable to convert to a char (this is the letter id in the ascii table)
		int id = 0;

		//Obtain the previously mentioned id
		pChars->Attribute("id", &id);

		//store the obtained id into the letters char variable
		tempLetter.m_cLetter = char(id);

		//Get all data members from the xml (.fnt) file
		pChars->Attribute("x", &tempLetter.m_nX);
		pChars->Attribute("y", &tempLetter.m_nY);
		pChars->Attribute("width", &tempLetter.m_nWidth);
		pChars->Attribute("height", &tempLetter.m_nHeight);
		pChars->Attribute("xoffset", &tempLetter.m_nXOffset);
		pChars->Attribute("yoffset", &tempLetter.m_nYOffset);
		pChars->Attribute("xadvance", &tempLetter.m_nXAdvance);

		//push our created letter into the map from font
		temp.m_mCharacters[tempLetter.m_cLetter] = tempLetter;

		//Move to the next letter
		if(i != charCount - 1)
		pChars = pChars->NextSiblingElement("char");
		
	}
	
	//Store the file name to the vector of keys
	m_vKeys.push_back(file);

	//Add the font to BitmapFont's map
	m_mFonts[file] = temp;
	
	//Return successfully
	return true;
}


//Calls Font::DrawString on the font passed in
void BitmapFont::DrawText(std::string text, int x, int y, std::string font, float scale, SGD::Color color)
{
	//Call fonts draw
	m_mFonts[font].DrawString(text, x, y, scale, color);
}
