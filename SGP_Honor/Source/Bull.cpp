#include "Bull.h"


///////////////////////////////////////////////////
// Constructor
// -Setup the size
Bull::Bull()
{
	SetSize({ 128.0f, 128.0f });
}

///////////////////////////////////////////////////
// Destructor
// -Setup the size
Bull::~Bull()
{
}

///////////////////////////////////////////////////
// Update
// -Main update loop for the bull
void Bull::Update(float elapsedTime)
{
	Boss::Update(elapsedTime);
}

///////////////////////////////////////////////////
// Render
// -Draw the bull and his animations
void Bull::Render(void)
{
	Boss::Render();
}