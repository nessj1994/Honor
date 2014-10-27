#include "AnimationEngine.h"
#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <string>
#include "IEntity.h"
#include "Entity.h"
#include "Player.h"
#include <Windows.h>
#include "Camera.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

#define relativePath std::string("Assets/graphics/")


//Instantiate the static member for the games instance
AnimationEngine* AnimationEngine::s_pInstance = nullptr;


//////////////////////////////////////////////////////////////////////////
////////////////////////Singleton Accessors//////////////////////////////

//Instantiate ONE instance of the singleton
AnimationEngine* AnimationEngine::GetInstance(void)
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new AnimationEngine;
	}

	return s_pInstance;
}

//Delete ONE instance of the singleton
void AnimationEngine::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////Setup, Play, Cleanup///////////////////////////
bool AnimationEngine::Initialize(void)
{
	return true;
}

void AnimationEngine::Update(float elapsedTime, AnimTimeStamp& ts, IEntity* sender)
{
	if (ts.IsPlaying() == false)
		return;

	ts.SetTimeWaited(ts.GetTimeWaited() + elapsedTime);

	if (m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetEvent() != "none")
	{
		SGD::Event* pATEvent = new SGD::Event(m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetEvent().c_str(), nullptr, this);
		SGD::EventManager::GetInstance()->SendEventNow(pATEvent);
		pATEvent = nullptr;
	}

	if (ts.GetTimeWaited() >= m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetDuration())
	{
		ts.IncreaseCurrFrame();
		ts.SetTimeWaited(0);

		if (ts.GetCurrFrame() == m_mAnimationMap[ts.GetCurrAnimation()].GetFrames().size())
		{
			if (m_mAnimationMap[ts.GetCurrAnimation()].IsLooping() == true)
				ts.ResetCurrFrame();
			else
			{
				ts.DecreaseCurrFrame();
				ts.SetPlaying(false);
			}
		}
	}
}

void AnimationEngine::Render(SGD::Point position, float rotation, AnimTimeStamp& ts, bool flipped, float scale, SGD::Point camerapos)
{
	float scaleX = scale;
	if (flipped == false)
		scaleX = -scaleX;
	SGD::Rectangle frame = m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetSourceRect();
	SGD::Point anchor = m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetAnchor();
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_mAnimationMap[ts.GetCurrAnimation()].GetImage(),
	{ position.x - (anchor.x * scaleX) - camerapos.x, position.y - (anchor.y * scale) - camerapos.y }, frame, rotation, {}, {}, { scaleX, scale });
}

void AnimationEngine::Terminate(void)
{
	std::map<std::string, Animation>::iterator iter;

	for (iter = m_mAnimationMap.begin(); iter != m_mAnimationMap.end(); iter++)
	{
		iter->second.UnloadTexture();
	}

}

SGD::Rectangle AnimationEngine::GetRect(const AnimTimeStamp& ts, bool facingRight, float scale, SGD::Point position)
{
	SGD::Point anchor = m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetAnchor();
	SGD::Rectangle colRect = m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetCollisionRect();
	SGD::Rectangle renderRect = m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetSourceRect();

	SGD::Point plrAnchor = { position.x - anchor.x, position.y - anchor.y };
	SGD::Point plrColOffset = { renderRect.left - colRect.left, renderRect.top - colRect.top };

	SGD::Rectangle rect;
	rect.left = plrAnchor.x - plrColOffset.x;
	rect.top = plrAnchor.y - plrColOffset.y;
	rect.right = rect.left + colRect.ComputeWidth();
	rect.bottom = rect.top + colRect.ComputeHeight();

	if (facingRight == false)
	{
		rect.left -= colRect.ComputeWidth() / 2.0f;
		rect.right -= colRect.ComputeWidth() / 2.0f;
	}
	/*SGD::GraphicsManager::GetInstance()->DrawRectangle({ rect.left - Camera::GetInstance()->GetCameraPos().x,
	rect.top - Camera::GetInstance()->GetCameraPos().y, rect.right - Camera::GetInstance()->GetCameraPos().x,
	rect.bottom - Camera::GetInstance()->GetCameraPos().y }, SGD::Color{ 255, 255, 0, 0 });*/
	return rect;
}

SGD::Rectangle AnimationEngine::GetAttackRect(const AnimTimeStamp& ts, bool facingRight, float scale, SGD::Point position)
{
	SGD::Point anchor = m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetAnchor();
	SGD::Rectangle atkRect = m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetAttackRect();
	SGD::Rectangle renderRect = m_mAnimationMap[ts.GetCurrAnimation()].GetFrames()[ts.GetCurrFrame()].GetSourceRect();
	SGD::Point plrAnchor = { position.x - anchor.x, position.y - anchor.y };
	SGD::Point plrAtkOffset = { renderRect.left - atkRect.left, renderRect.top - atkRect.top };

	SGD::Rectangle rect;
	rect.left = plrAnchor.x - plrAtkOffset.x;
	rect.top = plrAnchor.y - plrAtkOffset.y;
	rect.right = rect.left + atkRect.ComputeWidth();
	rect.bottom = rect.top + atkRect.ComputeHeight();

	if (facingRight == false)
	{
		rect.left -= atkRect.ComputeWidth() / 2.0f;
		rect.right -= atkRect.ComputeWidth() / 2.0f;
	}
	/*SGD::GraphicsManager::GetInstance()->DrawRectangle({ rect.left - Camera::GetInstance()->GetCameraPos().x,
	rect.top - Camera::GetInstance()->GetCameraPos().y, rect.right - Camera::GetInstance()->GetCameraPos().x,
	rect.bottom - Camera::GetInstance()->GetCameraPos().y }, SGD::Color{ 255, 0, 0, 255 });*/
	return rect;
}


/////////////////////////////////////////////////////////////////////////
///////////////////////////////Interface////////////////////////////////
void AnimationEngine::LoadAnimation(std::string filePath)
{
	TiXmlDocument doc;

	if (doc.LoadFile(filePath.c_str()) == false)
		return;

	TiXmlElement* pRoot = doc.RootElement();
	if (pRoot == nullptr)
		return;

	TiXmlElement* pAnim = pRoot->FirstChildElement("Animation");
	TiXmlElement* pFrame = pAnim->FirstChildElement("Frame");

	while (pAnim != nullptr)
	{
		Animation newAnim;
		SGD::HTexture img = SGD::GraphicsManager::GetInstance()->LoadTexture(((relativePath + pAnim->Attribute("spritesheet")).c_str()));
		newAnim.SetImage(img);
		int looping;
		pAnim->Attribute("looping", &looping);
		if (looping == 0)
			newAnim.SetLooping(false);
		else
			newAnim.SetLooping(true);

		while (pFrame != nullptr)
		{
			double left, top, right, bottom, x, y, dur;
			SGD::Rectangle source;
			pFrame->Attribute("RenderRectLeft", &left);
			pFrame->Attribute("RenderRectTop", &top);
			pFrame->Attribute("RenderRectRight", &right);
			pFrame->Attribute("RenderRectBottom", &bottom);
			source.left = (float)left;
			source.top = (float)top;
			source.right = (float)right;
			source.bottom = (float)bottom;

			SGD::Rectangle collision;
			pFrame->Attribute("CollisionRectLeft", &left);
			pFrame->Attribute("CollisionRectTop", &top);
			pFrame->Attribute("CollisionRectRight", &right);
			pFrame->Attribute("CollisionRectBottom", &bottom);
			collision.left = (float)left;
			collision.top = (float)top;
			collision.right = (float)right;
			collision.bottom = (float)bottom;

			SGD::Rectangle active;
			pFrame->Attribute("ActiveRectLeft", &left);
			pFrame->Attribute("ActiveRectTop", &top);
			pFrame->Attribute("ActiveRectRight", &right);
			pFrame->Attribute("ActiveRectBottom", &bottom);
			active.left = (float)left;
			active.top = (float)top;
			active.right = (float)right;
			active.bottom = (float)bottom;

			SGD::Point evPt;
			pFrame->Attribute("EventPointX", &x);
			pFrame->Attribute("EventPointY", &y);
			evPt.x = (float)x;
			evPt.y = (float)y;

			SGD::Point anchor;
			pFrame->Attribute("anchorpointX", &x);
			pFrame->Attribute("anchorpointY", &y);
			anchor.x = (float)x;
			anchor.y = (float)y;

			pFrame->Attribute("duration", &dur);

			newAnim.AddFrame(source, collision, active, evPt, anchor, (float)dur, pFrame->Attribute("event"));

			pFrame = pFrame->NextSiblingElement("Frame");
		}

		m_mAnimationMap[pAnim->Attribute("name")] = newAnim;
		pAnim = pAnim->NextSiblingElement("Animation");
		if (pAnim != nullptr)
			pFrame = pAnim->FirstChildElement("Frame");
	}
}