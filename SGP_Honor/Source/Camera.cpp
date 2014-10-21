#include "Camera.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "Player.h"
#include "Game.h"



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

void Camera::Update(float _elapsedTime)
{
	m_ptCameraPosition.x = (m_pPlayer->GetPosition().x - Game::GetInstance()->GetScreenWidth() / 3) /** m_fScale*/  ; // Divide by Scale
	m_ptCameraPosition.y = (m_pPlayer->GetPosition().y - Game::GetInstance()->GetScreenHeight() / 2) /** m_fScale*/   ;// Divide by Scale

}

