#include "Animation.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

Animation::Animation()
{
}


Animation::~Animation()
{
	
}

void Animation::UnloadTexture()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Animation::Update(float elapsedTime, AnimTimeStamp& ts)
{
	
}

void Animation::Render(SGD::Point position, float rotation)
{

}


void Animation::Reset()
{

}

void Animation::AddFrame(SGD::Rectangle source, SGD::Rectangle collision, SGD::Rectangle attack,
SGD::Point ptEv, SGD::Point anchor, float dur, std::string stEv)
{
	Frame newFrame;
	newFrame.SetAnchor(anchor);
	newFrame.SetAttackRect(attack);
	newFrame.SetCollisionRect(collision);
	newFrame.SetEvent(stEv);
	newFrame.SetEventPt(ptEv);
	newFrame.SetSourceRect(source);
	newFrame.SetDuration(dur);

	m_vFrames.push_back(newFrame);
}