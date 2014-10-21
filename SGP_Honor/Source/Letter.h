#pragma once
class Letter
{
public:
	Letter();
	~Letter();

private:

	friend class Font;
	friend class BitmapFont;

	char m_cLetter;
	int m_nX;
	int m_nY;
	int m_nWidth;
	int m_nHeight;
	int m_nXOffset;
	int m_nYOffset;
	int m_nXAdvance;
};


