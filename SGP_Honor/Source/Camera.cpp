#include "Camera.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "Player.h"
#include "Game.h"
#include "AnimationEngine.h"
#include <string>


Camera* Camera::GetInstance(void)
{
	static Camera s_instance;
	return &s_instance;

}

void Camera::Draw(SGD::Rectangle _rect, SGD::Color _color)
{
	
	//_rect.SetTopLeft({ _rect.GetTopLeft().x / m_fScale, _rect.GetTopLeft().y / m_fScale });
	//_rect.SetBottomRight({ _rect.GetBottomRight().x / m_fScale, _rect.GetBottomRight().y / m_fScale });

		/////CHANGE FROM RECT TO ENTITY to control the ZOOM scale



	//SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle(_rect.left / m_szZoomScale.width, _rect.top / m_szZoomScale.height, _rect.right / m_szZoomScale.width, _rect.bottom / m_szZoomScale.height),
	//	_color);

	SGD::GraphicsManager::GetInstance()->DrawRectangle(_rect, _color);



}

void Camera::DrawAnimation(SGD::Point position, float rotation, AnimTimeStamp& ts, bool flipped)
{
	AnimationEngine::GetInstance()->Render({ position.x , position.y }, rotation, ts, flipped, m_szZoomScale.width, m_ptCameraPosition);
}

void Camera::DrawString(std::string str, SGD::Point position)
{
	SGD::GraphicsManager::GetInstance()->DrawString(str.c_str(), { position.x - m_ptCameraPosition.x, position.y - m_ptCameraPosition.y },
		SGD::Color(255, 0, 0, 0));
}

void Camera::DrawTexture(SGD::Point position, float rotation, SGD::HTexture m_hImage, bool flipped)
{
	float scaleX = 1;
	if (flipped == true)
		scaleX = -scaleX;

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage,
	{ position.x - m_ptCameraPosition.x, position.y - m_ptCameraPosition.y }, rotation, {}, {}, { m_szZoomScale.width, m_szZoomScale.height });
}

void Camera::DrawTextureSection(SGD::HTexture handle, SGD::Point position, SGD::Rectangle section, float rotation, SGD::Vector rotationOffset, SGD::Color color, SGD::Size scale)
{
	//SGD::Rectangle tempRect;

	//tempRect.left = section.left / m_szZoomScale.width;
	//tempRect.top = section.top / m_szZoomScale.height;
	//tempRect.right = tempRect.left + (section.right - section.left);
	//tempRect.bottom = tempRect.top + (section.bottom - section.top);



	SGD::GraphicsManager::GetInstance()->DrawTextureSection(handle,
	{	(position.x /** m_szZoomScale.width*/)	/*- Camera::GetInstance()->GetCameraPos().x*/, 
		(position.y /** m_szZoomScale.height*/) /*- Camera::GetInstance()->GetCameraPos().y*/ },
	section, rotation, rotationOffset, color, { m_szZoomScale.width, m_szZoomScale.height });
}


void Camera::Update(float _elapsedTime)
{
	m_ptCameraPosition.x = (m_pPlayer->GetPosition().x - Game::GetInstance()->GetScreenWidth()  / 3)  /** m_fScale*/; // Divide by Scale
	m_ptCameraPosition.y = (m_pPlayer->GetPosition().y - Game::GetInstance()->GetScreenHeight() / 2)  /** m_fScale*/;// Divide by Scale

}

