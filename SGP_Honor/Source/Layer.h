//***********************************************************************
//	File:		Layer.h
//	Author:		Michael Sciortino
//	Course:		SGD 1408
//	Purpose:	A layer will hold all information needed for a single image layer
//***********************************************************************

#pragma once

#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_Handle.h"

class Layer
{
public:
	//////////////////////////////
	// Ctor/Dtor
	Layer();
	~Layer();

	//////////////////////////////
	// Accessors/Mutators
	float			GetScrollSpeed() const		{ return m_fScrollSpeed; }
	int				GetWidth() const			{ return m_iWidth; }
	int				GetHeight() const			{ return m_iHeight; }
	int				GetTileSetWidth() const		{ return m_nTileSetWidth; }
	int				GetTileSetHeight() const	{ return m_nTileSetHeight; }
	int				GetTileSetSize() const		{ return m_nTileSetSize; }
	int**			GetImageLayer() const		{ return m_iImageLayer; }
	int			GetDepth() const			{ return m_nDepth; }
	SGD::HTexture	GetTileSet() const			{ return m_tTileSet; }

	void			SetScrollSpeed(float _speed)		{ m_fScrollSpeed = _speed; }
	void			SetWidth(int _width)				{ m_iWidth = _width; }
	void			SetHeight(int _height)				{ m_iHeight = _height; }
	void			SetTileSetWidth(int _width)			{ m_nTileSetWidth = _width; }
	void			SetTileSetHeight(int _height)		{ m_nTileSetHeight = _height; }
	void			SetTileSetSize(int _size)			{ m_nTileSetSize = _size; }
	void			SetImageLayer(int** _layer)			{ m_iImageLayer = _layer; }
	void			SetDepth(int _depth)				{ m_nDepth = _depth; }
	void			SetTileSet(SGD::HTexture _image)	{ m_tTileSet = _image; }

private:
	//////////////////////////////
	// Member fields
	float					m_fScrollSpeed;
	int						m_iWidth;
	int						m_iHeight;
	int**					m_iImageLayer;
	int						m_nDepth;
	int						m_nTileSetWidth;
	int						m_nTileSetHeight;
	int						m_nTileSetSize;
	SGD::HTexture			m_tTileSet;
	
};

