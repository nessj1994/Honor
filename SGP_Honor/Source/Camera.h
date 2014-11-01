#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"	// Rectangle type
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "AnimTimeStamp.h"

class Level;
class Player;

class Camera
{


public:
	
	static Camera* GetInstance(void);
	
	//Camera Accessor
	SGD::Point GetCameraPos(void) const{ return SGD::Point(m_ptCameraPosition.x, m_ptCameraPosition.y); }

	unsigned int GetScreenWidth(void) const { return m_nScreenWidth; }
	unsigned int GetScreenHeight(void) const { return m_nScreenHeight; }

	unsigned int GetHorizontalBuffer(void) const { return m_nHorizontalBuffer; }
	unsigned int GetVerticalBuffer(void) const { return m_nVerticalBuffer; }
	float GetZoomScale() const { return m_fScale; }



	//Camera Mutator
	void SetCameraPos(float x, float y) { m_ptCameraPosition.x = x, m_ptCameraPosition.y = y; }
	
	void SetScreenWidth(unsigned int _width) { m_nScreenWidth = _width; }
	void SetScreenHeight(unsigned int _height) { m_nScreenHeight = _height; }

	void SetHorizontalBuffer(unsigned int _buffer) { m_nHorizontalBuffer = _buffer; }
	void SetVerticalBuffer(unsigned int _buffer) { m_nVerticalBuffer = _buffer; }
	void SetZoomScale(float _zoom) { m_fScale = _zoom; }

	void SetPlayer(Player* _player) { m_pPlayer = _player; }

	//Draw
	void Draw(SGD::Rectangle _rect, SGD::Color _color);
	void DrawAnimation(SGD::Point position, float rotation, AnimTimeStamp& ts, bool flipped, float scale);
	void DrawTexture(SGD::Point position, float rotation, SGD::HTexture m_hImage, bool flipped, float scale, SGD::Color color, SGD::Vector rotationOffset);
	void DrawTextureSection(SGD::HTexture handle, SGD::Point position, SGD::Rectangle section, float rotation, SGD::Vector rotationOffset, SGD::Color color, SGD::Size scale);
	void DrawString(std::string str, SGD::Point position);

	void Update(float _elapsedTime);

private:

	Camera(void) = default;
	~Camera(void) = default;

	Camera(const Camera&)				= delete;
	Camera& operator= (const Camera&)	= delete;

	

	unsigned int m_nScreenWidth = 0;
	unsigned int m_nScreenHeight = 0;
	unsigned int m_nHorizontalBuffer = 0;
	unsigned int m_nVerticalBuffer = 0;

	Level* m_pCurrentLevel = nullptr;
	Player* m_pPlayer = nullptr;

	SGD::Point m_ptCameraPosition = { 0, 0 };

	//SGD::Size m_szZoomScale = { 0.5f, 0.5f };
	float m_fScale = 1;


	//Pan Values
	float m_fCurrentPanX = 5.5f;
	float m_fCurrentPanY = 0.5f;

	float m_fDesiredPanX = 0;
	float m_fDesiredPanY = 0;

	
};

