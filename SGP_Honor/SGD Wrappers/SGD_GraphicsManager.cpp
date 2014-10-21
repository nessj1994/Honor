/***********************************************************************\
|																		|
|	File:			SGD_GraphicsManager.cpp								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-09											|
|																		|
|	Purpose:		To render simple geometry and						|
|					load and render image files							|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#include "SGD_GraphicsManager.h"


// Uses assert for debug breaks
#include <cassert>

// Uses OutputDebugString for debug text
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstring>
#include <cstdio>

// Uses Direct3D9 for rendering
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")

// Uses HandleManager for storing data
#include "SGD_HandleManager.h"


namespace SGD
{
	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//
		// TextureInfo
		//	- stores info for the texture file: name, buffer, reference count
		struct TextureInfo
		{
			wchar_t*				wszFilename;		// file name
			unsigned int			unRefCount;			// reference count
			IDirect3DTexture9*		texture;			// texture
			float					fWidth;				// width
			float					fHeight;			// height
		};
		//*************************************************************//



		//*************************************************************//
		// GraphicsManager
		//	- concrete class for rendering simple geometry and image files
		//	- supports .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga files
		//	- texture dimensions will be rounded up to the nearest power of 2 (e.g. 2,4,8,16,32,64, etc.)
		//	- uses IHandleManager to store texture data
		class GraphicsManager : public SGD::GraphicsManager
		{
		public:
			// SINGLETON
			static	GraphicsManager*	GetInstance		( void );
			static	void				DeleteInstance	( void );


			virtual	bool		Initialize				( bool bVsync = true )	override;
			virtual	bool		Update					( void )				override;
			virtual	bool		Terminate				( void )				override;


			virtual bool		SetClearColor			( Color color )					override;
			virtual bool		Resize					( Size size, bool windowed )	override;


			virtual bool		DrawString				( const wchar_t* text, Point position,  Color color )						override;
			virtual bool		DrawString				( const char* text, Point position,  Color color )							override;
			virtual bool		DrawLine				( Point position1, Point position2, Color color, unsigned int width )		override;
			virtual bool		DrawRectangle			( Rectangle rect, Color fillColor, Color lineColor, unsigned int width )	override;


			virtual	HTexture	LoadTexture				( const wchar_t* filename, Color colorKey )		override;
			virtual	HTexture	LoadTexture				( const char* filename, Color colorKey )		override;
			virtual	bool		DrawTexture				( HTexture handle, Point position, float rotation, Vector rotationOffset, Color color, Size scale )						override;
			virtual	bool		DrawTextureSection		( HTexture handle, Point position, Rectangle section, float rotation, Vector rotationOffset, Color color, Size scale )	override;
			virtual	bool		UnloadTexture			( HTexture& handle )							override;
			virtual SGD::Size	GetTextureSize			(SGD::HTexture _Texture) override;

		private:
			// SINGLETON
			static	GraphicsManager*		s_Instance;		// the ONE instance

			GraphicsManager					( void )					= default;		// Default constructor
			virtual	~GraphicsManager		( void )					= default;		// Destructor

			GraphicsManager					( const GraphicsManager& )	= delete;		// Copy constructor
			GraphicsManager&	operator=	( const GraphicsManager& )	= delete;		// Assignment operator


			// Wrapper Status
			enum EGraphicsManagerStatus
			{	
				E_UNINITIALIZED,
				E_INITIALIZED,
				E_DESTROYED
			};

			EGraphicsManagerStatus		m_eStatus			= E_UNINITIALIZED;			// wrapper initialization status

			HandleManager< TextureInfo > m_HandleManager;								// data storage

			IDirect3D9*					m_pDirect3D			= nullptr;					// Direct3D api
			IDirect3DDevice9*			m_pDevice			= nullptr;					// device
			ID3DXSprite*				m_pSprite			= nullptr;					// sprite
			ID3DXFont*					m_pFont				= nullptr;					// font
			IDirect3DTexture9*			m_pTexture			= nullptr;					// rectangle texture
			D3DPRESENT_PARAMETERS		m_PresentParams		= D3DPRESENT_PARAMETERS{};	// device presentation parameters
			
			HWND						m_hWnd				= NULL;						// window handle
			LONG						m_lWindowStyle		= 0;						// starting window style flags
			Color						m_ClearColor		= Color{0, 0, 0};			// background clear color
			Size						m_WindowSize		= Size{};					// window size
			Size						m_DesiredSize		= Size{};					// desired resolution (for fullscreen adjustment)
			bool						m_bCursorClipped	= false;					// is the cursor clipped to a region

			wchar_t*					m_pwszBuffer		= nullptr;					// output buffer storage (preallocated to hasten ASCII -> UTF16 conversion)
			int							m_nBufferSize		= 0;						// size (in wchar_t) of output buffer


			// CLEAR SCREEN HELPER METHOD
			bool			ClearScreen( void );


			// TEXTURE REFERENCE HELPER METHOD
			struct SearchInfo
			{
				const wchar_t*	filename;	// input
				TextureInfo*	texture;	// output
				HTexture		handle;		// output
			};
			static	bool	FindTextureByName( Handle handle, TextureInfo& data, SearchInfo* extra );
		};
		//*************************************************************//

	}	// namespace SGD_IMPLEMENTATION



	//*****************************************************************//
	// Interface singleton accessor
	/*static*/ GraphicsManager* GraphicsManager::GetInstance( void )
	{
		return (SGD::GraphicsManager*)SGD_IMPLEMENTATION::GraphicsManager::GetInstance();
	}

	// Interface singleton destructor
	/*static*/ void GraphicsManager::DeleteInstance( void )
	{
		return SGD_IMPLEMENTATION::GraphicsManager::DeleteInstance();
	}
	//*****************************************************************//



	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//	
		// SINGLETON

		// Instantiate static pointer to null (no instance yet)
		/*static*/ GraphicsManager* GraphicsManager::s_Instance = nullptr;

		// Singleton accessor
		/*static*/ GraphicsManager* GraphicsManager::GetInstance( void )
		{
			// Allocate singleton on first use
			if( GraphicsManager::s_Instance == nullptr )
				GraphicsManager::s_Instance = new GraphicsManager;

			// Return the singleton
			return GraphicsManager::s_Instance;
		}

		// Singleton destructor
		/*static*/ void GraphicsManager::DeleteInstance( void )
		{
			// Deallocate singleton
			delete GraphicsManager::s_Instance;
			GraphicsManager::s_Instance = nullptr;
		}
		//*************************************************************//



		//*************************************************************//
		// INITIALIZE
		bool GraphicsManager::Initialize( bool bVsync )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_UNINITIALIZED && "GraphicsManager::Initialize - wrapper has already been initialized" );
			if( m_eStatus != E_UNINITIALIZED )
				return false;

			// Validate the thread's window handle
			HWND hWnd = GetActiveWindow();
			assert( hWnd != nullptr && "GraphicsManager::Initialize - invalid window handle" );
			if( hWnd == nullptr )
			{
				// MESSAGE
				char szBuffer[ 64 ];
				_snprintf_s( szBuffer, 64, _TRUNCATE, "!!! GraphicsManager::Initialize - invalid window handle !!!\n" );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// When using a COM object, the thread should be CoInitialized
			CoInitializeEx( nullptr, COINIT_MULTITHREADED );


			// Attempt to create the Direct3D interface
			HRESULT hResult = S_OK;
			m_pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
			if( m_pDirect3D == nullptr )
			{
				CoUninitialize();

				// MESSAGE
				char szBuffer[ 64 ];
				_snprintf_s( szBuffer, 64, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D !!!\n" );
				OutputDebugStringA( szBuffer );

				return false;
			}

			
			// Get the window dimensions
			RECT rWindow;
			GetClientRect( hWnd, &rWindow );
			m_WindowSize = { (float)rWindow.right, (float)rWindow.bottom };
			m_DesiredSize = m_WindowSize;


			// Setup device presentation parameters
			m_PresentParams.BackBufferWidth				= rWindow.right;
			m_PresentParams.BackBufferHeight			= rWindow.bottom;
			m_PresentParams.BackBufferFormat			= D3DFMT_UNKNOWN;
			m_PresentParams.BackBufferCount				= 1;
			m_PresentParams.MultiSampleType				= D3DMULTISAMPLE_NONE;
			m_PresentParams.MultiSampleQuality			= 0;
			m_PresentParams.SwapEffect					= D3DSWAPEFFECT_COPY;
			m_PresentParams.hDeviceWindow				= hWnd;
			m_PresentParams.Windowed					= TRUE;
			m_PresentParams.EnableAutoDepthStencil		= FALSE;
			m_PresentParams.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
			m_PresentParams.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
			m_PresentParams.PresentationInterval		= ( (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE );


			// Attempt to create the device
			hResult = m_pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_PresentParams, &m_pDevice );
			if( FAILED( hResult ) )
			{
				m_pDirect3D->Release();
				m_pDirect3D = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D device (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Attempt to create the sprite library
			hResult = D3DXCreateSprite( m_pDevice, &m_pSprite );
			if( FAILED( hResult ) )
			{
				m_pDevice->Release();
				m_pDevice = nullptr;

				m_pDirect3D->Release();
				m_pDirect3D = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D sprite (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Attempt to create the font
			hResult = D3DXCreateFontW( m_pDevice, 23, 0, FW_SEMIBOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"ARIAL", &m_pFont );
			if( FAILED( hResult ) )
			{
				m_pSprite->Release();
				m_pSprite = nullptr;

				m_pDevice->Release();
				m_pDevice = nullptr;

				m_pDirect3D->Release();
				m_pDirect3D = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D font (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Attempt to create the rect texture
			hResult = D3DXCreateTexture( m_pDevice, 1, 1, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &m_pTexture );
			if( FAILED( hResult ) )
			{
				m_pFont->Release();
				m_pFont = nullptr;

				m_pSprite->Release();
				m_pSprite = nullptr;

				m_pDevice->Release();
				m_pDevice = nullptr;

				m_pDirect3D->Release();
				m_pDirect3D = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D rect (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}
			

			// Fill the rect texture
			D3DLOCKED_RECT area;
			m_pTexture->LockRect( 0, &area, 0, D3DLOCK_DISCARD );
			((DWORD*)area.pBits)[ 0 ] = 0xFFFFFFFF;
			m_pTexture->UnlockRect( 0 );


			// Preload font characters for speed.
			m_pFont->PreloadCharacters( '0', '9' );
			m_pFont->PreloadCharacters( 'A', 'Z' );
			m_pFont->PreloadCharacters( 'a', 'z' );


			// Store the window and its style flags
			m_hWnd			= hWnd;
			m_lWindowStyle	= GetWindowLong( m_hWnd, GWL_STYLE );


			// Begin the new scene (graphics are always in an active state)
			m_pDevice->Clear( 0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB( m_ClearColor.red, m_ClearColor.green, m_ClearColor.blue ), 1.0f, 0 );

			hResult = m_pDevice->BeginScene();
			if( SUCCEEDED( hResult ) )
				hResult = m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );


			// Preallocate output buffer
			m_nBufferSize		= 16;
			m_pwszBuffer		= new wchar_t[ m_nBufferSize ];


			// Success!
			m_eStatus = E_INITIALIZED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// UPDATE
		bool GraphicsManager::Update( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::Update - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			
			// Centered output onto fullscreen display?
			float offsetX = (m_WindowSize.width - m_DesiredSize.width) / 2;
			float offsetY = (m_WindowSize.height - m_DesiredSize.height) / 2;
			if( offsetX > 0 || offsetY > 0 )
			{	
				// Store original transform
				HRESULT result;
				D3DXMATRIX original, transform;
				m_pSprite->GetTransform( &original );
				D3DXMatrixIdentity( &transform );
				
				Rectangle sides[ 4 ] = 
				{
					{ 0, 0, offsetX, m_WindowSize.height },	// left
					{ m_WindowSize.width - offsetX, 0, m_WindowSize.width, m_WindowSize.height }, // right
					
					{ 0, 0, m_WindowSize.width, offsetY },	// top
					{ 0, m_WindowSize.height - offsetY, m_WindowSize.width, m_WindowSize.height }, // bottom
				};

				for( int i = 0; i < 4; i++ )
				{
					transform.m[0][0] = (FLOAT)(sides[i].right  - sides[i].left);
					transform.m[1][1] = (FLOAT)(sides[i].bottom - sides[i].top);

					transform.m[3][0] = (FLOAT)(sides[i].left);
					transform.m[3][1] = (FLOAT)(sides[i].top);

					// Apply the transform
					m_pSprite->SetTransform(&transform);

					// Draw the rectangle
					result = m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, D3DCOLOR_XRGB( 0, 0, 0 ) );
				}


				// Restore the original transform
				m_pSprite->SetTransform(&original);
				
				

				// Clip cursor only if the window is active
				if( m_hWnd == GetActiveWindow() )
				{
					RECT clip = { (LONG)offsetX, (LONG)offsetY, (LONG)(m_WindowSize.width-offsetX), (LONG)(m_WindowSize.height-offsetY) };
					ClipCursor( &clip );
					m_bCursorClipped = true;
				}
				else if( m_bCursorClipped == true )
				{
					// Unclip the cursor
					ClipCursor( nullptr );
					m_bCursorClipped = false;
				}
			}
			else if( m_bCursorClipped == true )
			{
				// Unclip the cursor
				ClipCursor( nullptr );
				m_bCursorClipped = false;
			}


			// End sprite rendering
			HRESULT hResult = m_pSprite->End();
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to end Direct3D sprite scene (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				//return false;
			}

			// End the scene
			hResult = m_pDevice->EndScene();
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to end Direct3D scene (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				//return false;
			}


			// Check for lost device (could happen from an ALT+TAB or ALT+ENTER)
			hResult = m_pDevice->TestCooperativeLevel();
			if( hResult == D3DERR_DEVICENOTRESET )
			{
				m_pFont->OnLostDevice();
				m_pSprite->OnLostDevice();
				m_pDevice->Reset( &m_PresentParams );
				m_pSprite->OnResetDevice();
				m_pFont->OnResetDevice();
			}
			else if( hResult == D3D_OK )
			{
				// Present the current frame to the screen
				hResult = m_pDevice->Present( nullptr, nullptr, nullptr, nullptr );

				// Could fail if the fullscreen application loses focus,
				// but that can be ignored
				if( FAILED( hResult ) )
				{
					// MESSAGE
					char szBuffer[ 128 ];
					_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to present Direct3D scene (0x%X) !!!\n", hResult );
					OutputDebugStringA( szBuffer );

					//return false;
				}
			}


			// Clear the new frame
			m_pDevice->Clear( 0, nullptr, D3DCLEAR_TARGET, (D3DCOLOR)m_ClearColor, 1.0f, 0 );


			// Begin the new scene
			hResult = m_pDevice->BeginScene();
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to begin Direct3D scene (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}

			// Begin sprite rendering
			hResult = m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to begin Direct3D sprite scene (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}

			
			//Pixelated?
			hResult = m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
			hResult = m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );


			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// TERMINATE
		bool GraphicsManager::Terminate( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::Terminate - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			
			// Unclip cursor
			if( m_bCursorClipped == true )
			{
				ClipCursor( nullptr );
				m_bCursorClipped = false;
			}


			// Deallocate output buffer
			delete[] m_pwszBuffer;
			m_pwszBuffer = nullptr;
			m_nBufferSize = 0;


			// Clear handles
			m_HandleManager.Clear();


			// Release resources
			m_pTexture->Release();
			m_pTexture = nullptr;

			m_pFont->Release();
			m_pFont = nullptr;

			m_pSprite->Release();
			m_pSprite = nullptr;

			m_pDevice->Release();
			m_pDevice = nullptr;

			// Release Direct3D
			m_pDirect3D->Release();
			m_pDirect3D = nullptr;

			CoUninitialize();

			m_eStatus = E_DESTROYED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// SET CLEAR COLOR
		bool GraphicsManager::SetClearColor( Color color )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::SetClearColor - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Store the parameter (and force opacity)
			m_ClearColor = color;
			m_ClearColor.alpha = 255;

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// RESIZE
		bool GraphicsManager::Resize( Size size, bool windowed )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::Resize - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Round the size
			int width	= (int)(size.width  + 0.5f);
			int height	= (int)(size.height + 0.5f);


			// Supported format
			D3DFORMAT format = D3DFMT_UNKNOWN;

			// Verify the resolution for full screen
			if( windowed == false )
			{
				D3DFORMAT larger = D3DFMT_UNKNOWN;
				int nLargerWidth  = INT_MAX;
				int nLargerHeight = INT_MAX;


				// Search the device types for a supported color format
				D3DFORMAT formats[ ] = { D3DFMT_X8R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5 };
				for( int i = 0; i < _countof(formats) && format == D3DFMT_UNKNOWN; i++ )
				{	
					HRESULT hResult = m_pDirect3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, formats[ i ], formats[ i ], FALSE );
					if( FAILED( hResult ) )
						continue;

					// Search the adapter modes for a matching resolution
					int numFormats = m_pDirect3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, formats[ i ] );
					for( int j = 0; j < numFormats; j++ )
					{
						D3DDISPLAYMODE mode = { };
						m_pDirect3D->EnumAdapterModes( D3DADAPTER_DEFAULT, formats[ i ], j, &mode );

						// Exact resolution?
						if( (int)mode.Width == width && (int)mode.Height == height )
						{
							format = formats[ i ];
							break;
						}
						// Larger?
						else if( (int)mode.Width >= width && (int)mode.Height >= height )
						{
							// Smaller than the existing larger-size?
							if( (int)mode.Width <= nLargerWidth && (int)mode.Height <= nLargerHeight )
							{
								// But not the exact same size as the existing larger-size?
								if( (int)mode.Width != nLargerWidth || (int)mode.Height != nLargerHeight )
								{
									larger = formats[ i ];
									nLargerWidth  = (int)mode.Width;
									nLargerHeight = (int)mode.Height;
								}
							}
						}
					}
				}

				// Use a larger resolution?
				if( format == D3DFMT_UNKNOWN )
				{
					if( larger != D3DFMT_UNKNOWN )	// larger resolution exists
					{
						// MESSAGE
						char szBuffer[ 128 ];
						_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Resize - Fullscreen resolution %ix%i is enlarged to %ix%i !!!\n", width, height, nLargerWidth, nLargerHeight );
						OutputDebugStringA( szBuffer );
						

						format = larger;
						width  = nLargerWidth;
						height = nLargerHeight;
					}
					else							// larger resolution DOES NOT exist
					{
						// MESSAGE
						char szBuffer[ 128 ];
						_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Resize - Failed to go fullscreen with resolution %ix%i !!!\n", width, height );
						OutputDebugStringA( szBuffer );

						return false;
					}
				}

			}


			// Set the new device presentation parameters
			m_PresentParams.BackBufferFormat	= format;
			m_PresentParams.Windowed			= windowed;
			m_PresentParams.BackBufferWidth		= width;
			m_PresentParams.BackBufferHeight	= height;

			// Reset the device
			m_pFont->OnLostDevice();
			m_pSprite->OnLostDevice();
			m_pDevice->Reset( &m_PresentParams );
			m_pSprite->OnResetDevice();
			m_pFont->OnResetDevice();


			// Reset the window
			if( windowed == true )
			{
				// Set window style flags
				SetWindowLong( m_hWnd, GWL_STYLE, m_lWindowStyle );

				// Setup the desired client area size
				RECT rWindow;
				rWindow.left	= 0;
				rWindow.top		= 0;
				rWindow.right	= width;
				rWindow.bottom	= height;

				// Get the window dimensions that have the desired client area within
				AdjustWindowRectEx( &rWindow, (DWORD)m_lWindowStyle, FALSE, WS_EX_APPWINDOW );

				// Calculate the width/height of that window's dimensions
				int windowWidth		= rWindow.right - rWindow.left;
				int windowHeight	= rWindow.bottom - rWindow.top;

				// Center the window on the screen
				SetWindowPos( m_hWnd, HWND_NOTOPMOST, (GetSystemMetrics(SM_CXSCREEN) - windowWidth)/2, (GetSystemMetrics(SM_CYSCREEN) - windowHeight)/2, windowWidth, windowHeight, SWP_SHOWWINDOW );
			}
			else
			{
				// Set window style flags
				SetWindowLong( m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP );

				// Refresh the window
				SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
			}


			// Store the new dimensions
			m_DesiredSize = size;
			m_WindowSize = { (float)width, (float)height };

			// Center output onto fullscreen display
			float offsetX = (m_WindowSize.width - m_DesiredSize.width) / 2;
			float offsetY = (m_WindowSize.height - m_DesiredSize.height) / 2;
			if( offsetX > 0 || offsetY > 0 )
			{
				D3DXMATRIX transform;
				D3DXMatrixTranslation( &transform, offsetX, offsetY, 0.0f );
				
				m_pSprite->SetTransform( &transform );
			}
			else 
			{
				D3DXMATRIX identity;
				D3DXMatrixIdentity( &identity );
				
				m_pSprite->SetTransform( &identity );
			}

			// Begin the new scene
			HRESULT hResult = m_pDevice->BeginScene();

			if( SUCCEEDED( hResult ) )
				hResult = m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );


			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW STRING
		bool GraphicsManager::DrawString( const wchar_t* text, Point position, Color color )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::DrawString - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Sanity-check the text parameter
			assert( text != nullptr && "GraphicsManager::DrawString - text cannot be null" );
			if( text == nullptr || text[0] == L'\0' )
				return false;


			RECT region = { (LONG)position.x, (LONG)position.y };
			
			int result = m_pFont->DrawTextW( m_pSprite, text, -1, &region, DT_NOCLIP, (D3DCOLOR)color );
			if( result == 0 )
			{
				// MESSAGE
				wchar_t wszBuffer[ 256 ];
				_snwprintf_s( wszBuffer, 256, _TRUNCATE, L"!!! GraphicsManager::DrawString - failed to draw Direct3D text (\"%ws\") !!!", text );
				OutputDebugStringW( wszBuffer );
				OutputDebugStringA( "\n" );

				return false;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW STRING
		bool GraphicsManager::DrawString( const char* text, Point position, Color color )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::DrawString - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Sanity-check the text parameter
			assert( text != nullptr && "GraphicsManager::DrawString - text cannot be null" );
			if( text == nullptr || text[0] == '\0' )
				return false;


			// Reallocate output buffer?
			int size = MultiByteToWideChar( CP_UTF8, 0, text, -1, nullptr, 0 );
			if( size > m_nBufferSize )
			{
				delete[] m_pwszBuffer;
				m_nBufferSize = size;
				m_pwszBuffer = new wchar_t[ m_nBufferSize ];
			}

			// Convert to UTF16
			int ret = MultiByteToWideChar( CP_UTF8, 0, text, -1, m_pwszBuffer, m_nBufferSize );			
			if( ret == 0 )
			{
				// MESSAGE
				char szBuffer[ 256 ];
				_snprintf_s( szBuffer, 256, _TRUNCATE, "!!! GraphicsManager::DrawString - invalid string (\"%hs\") !!!", text );
				OutputDebugStringA( szBuffer );
				OutputDebugStringA( "\n" );

				return false;
			}

			
			// Use the UTF16 draw
			return DrawString( m_pwszBuffer, position, color );
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW LINE
		bool GraphicsManager::DrawLine( Point position1, Point position2, Color color, unsigned int lineWidth )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::DrawLine - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Calculate the difference in positions
			float dX = position2.x - position1.x;
			float dY = position2.y - position1.y;

			// Is the line too small?
			if( dX == 0 && dY == 0 || lineWidth <= 0 )
				return false;

			
			// Store original transform
			D3DXMATRIX original, world;
			m_pSprite->GetTransform( &original );

			// Create transform matrix
			D3DXMATRIX transform;
			D3DXMatrixIdentity( &transform );

			if( dX == 0.0f )
			{
				// Vertical line
				transform.m[ 0 ][ 0 ] = (FLOAT)(lineWidth);
				transform.m[ 3 ][ 0 ] = (FLOAT)(position1.x - (lineWidth*0.5f) + 0.5f);
				if( dY > 0.0f )
				{
					transform.m[ 1 ][ 1 ] = (FLOAT)(dY + 1);
					transform.m[ 3 ][ 1 ] = (FLOAT)(position1.y);
				}
				else 
				{
					transform.m[ 1 ][ 1 ] = (FLOAT)(-dY + 1);
					transform.m[ 3 ][ 1 ] = (FLOAT)(position2.y);
				}
			}
			else if( dY == 0.0f )
			{
				// Horizontal line
				transform.m[ 1 ][ 1 ] = (FLOAT)(lineWidth);
				transform.m[ 3 ][ 1 ] = (FLOAT)(position1.y - (lineWidth*0.5f) + 0.5f);
				if( dX > 0.0f )
				{
					transform.m[ 0 ][ 0 ] = (FLOAT)(dX + 1);
					transform.m[ 3 ][ 0 ] = (FLOAT)(position1.x);
				}
				else 
				{
					transform.m[ 0 ][ 0 ] = (FLOAT)(-dX + 1);
					transform.m[ 3 ][ 0 ] = (FLOAT)(position2.x);
				}
			}
			else 
			{
				// Adjust for counter-clockwise rotation
				if( dY < 0 )
				{
					position1 = position2;		// partial swap, as pt2 is not used again
					dX = -dX;
					dY = -dY;
				}


				// Calculate the length and direction
				float length = sqrtf( dX * dX + dY * dY );
				float angle  = acos( dX / length );			// abbreviated DotProduct against <1, 0>
				
				// Indefinite angle is treated as 0 ...
				if( _isnan( angle ) != 0 )
					angle = 0.0f;


				float cos = cosf( angle );
				float sin = sinf( angle );


				// The positions are inclusive: (0, 0) -> (1, 0) should be 2 pixels
				length = length + 1.0f;
				float halfWidth = (float)(lineWidth * 0.5f);



				// Rotation & scale values
				transform.m[ 0 ][ 0 ] = (FLOAT)(length    *  cos);
				transform.m[ 0 ][ 1 ] = (FLOAT)(length    *  sin);
				transform.m[ 1 ][ 0 ] = (FLOAT)(lineWidth * -sin);
				transform.m[ 1 ][ 1 ] = (FLOAT)(lineWidth *  cos);

				// Translate and rotate around (0, halfWidth)
				if( dX < 0.0f || dY < 0.0f )
				{
					transform.m[ 3 ][ 0 ] = (FLOAT)( position1.x + (halfWidth *  sin) + 1.0f );	// offset ...
					transform.m[ 3 ][ 1 ] = (FLOAT)( position1.y + (halfWidth * -cos) );
				}
				else 
				{
					transform.m[ 3 ][ 0 ] = (FLOAT)( position1.x + (halfWidth *  sin) );
					transform.m[ 3 ][ 1 ] = (FLOAT)( position1.y + (halfWidth * -cos) );
				}
			}

			

			// Apply the transform
			D3DXMatrixMultiply( &world, &transform, &original );
			m_pSprite->SetTransform( &world );


			// Draw the rectangle
			HRESULT result = m_pSprite->Draw( m_pTexture, nullptr, nullptr, nullptr, (D3DCOLOR)color );


			// Restore the transform
			m_pSprite->SetTransform( &original );

			if( FAILED( result ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::DrawLine - failed to draw line (0x%X) !!!\n", result );
				OutputDebugStringA( szBuffer );

				return false;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW RECTANGLE
		bool GraphicsManager::DrawRectangle( Rectangle rect, Color fillColor, Color lineColor, unsigned int lineWidth )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::DrawRectangle - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Is the rectangle inverted?
			if( rect.right <= rect.left || rect.bottom <= rect.top )
				return false;


			HRESULT result = 0;
			
			// Store original transform
			D3DXMATRIX original, world;
			m_pSprite->GetTransform( &original );

			// Create transform matrix
			D3DXMATRIX transform;
			D3DXMatrixIdentity( &transform );


			// Clamp the rectangle to pixel coordinates
			int width	= (int)(rect.right  - rect.left + 0.5f);
			int height	= (int)(rect.bottom - rect.top  + 0.5f);
			int left	= (int)(rect.left + (rect.left > 0 ? 0.5f : -0.5f) );
			int top		= (int)(rect.top  + (rect.top  > 0 ? 0.5f : -0.5f) );
			int right	= left + width;
			int bottom	= top  + height;
			

			// Should the frame be drawn?
			if( lineColor.alpha > 0 && lineWidth > 0 )
			{
				// Calculate the size
				int smallLineHalf = (int)(lineWidth / 2);
				int largeLineHalf = (int)(lineWidth - smallLineHalf);


				// Calculate the D3D color
				DWORD dwColor = (D3DCOLOR)lineColor;


				// Set transform for top line
				transform.m[ 0 ][ 0 ] = (FLOAT)(width + (largeLineHalf - smallLineHalf));
				transform.m[ 1 ][ 1 ] = (FLOAT)lineWidth;

				transform.m[ 3 ][ 0 ] = (FLOAT)(left - largeLineHalf);
				transform.m[ 3 ][ 1 ] = (FLOAT)(top  - largeLineHalf);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the top line
				m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, dwColor );


				// Set transform for bottom line
				transform.m[ 3 ][ 0 ] = (FLOAT)(left   + smallLineHalf);
				transform.m[ 3 ][ 1 ] = (FLOAT)(bottom - smallLineHalf);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the bottom line
				m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, dwColor );
				

				
				// Set transform for left line
				transform.m[ 0 ][ 0 ] = (FLOAT)lineWidth;
				transform.m[ 1 ][ 1 ] = (FLOAT)(height + (largeLineHalf - smallLineHalf));

				transform.m[ 3 ][ 0 ] = (FLOAT)(left - largeLineHalf);
				transform.m[ 3 ][ 1 ] = (FLOAT)(top  + smallLineHalf);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the left line
				m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, dwColor );


				// Set transform for right line
				transform.m[ 3 ][ 0 ] = (FLOAT)(right - smallLineHalf);
				transform.m[ 3 ][ 1 ] = (FLOAT)(top   - largeLineHalf);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the right line
				result = m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, dwColor );


				
				// Should the rect within be filled?
				if( fillColor.alpha > 0 )
				{
					// Set the transform for the fill rect (shrunk within the frame)
					transform.m[0][0] = (FLOAT)(width  - (smallLineHalf * 2));
					transform.m[1][1] = (FLOAT)(height - (smallLineHalf * 2));

					transform.m[3][0] = (FLOAT)(left + smallLineHalf);
					transform.m[3][1] = (FLOAT)(top  + smallLineHalf);

					// Apply the transform
					D3DXMatrixMultiply( &world, &transform, &original );
					m_pSprite->SetTransform( &world );

					// Draw the rectangle
					result = m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, (D3DCOLOR)fillColor );
				}
			}
			else if( fillColor.alpha > 0 ) // Just the rect?
			{
				// Set the transform for the full fill rect
				transform.m[0][0] = (FLOAT)(right  - rect.left);
				transform.m[1][1] = (FLOAT)(bottom - rect.top);

				transform.m[3][0] = (FLOAT)(left);
				transform.m[3][1] = (FLOAT)(top);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the rectangle
				result = m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, (D3DCOLOR)fillColor );
			}


			// Restore the transform
			m_pSprite->SetTransform( &original );

			
			if( FAILED( result ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::DrawRectangle - failed to draw rectangle (0x%X) !!!\n", result );
				OutputDebugStringA( szBuffer );

				return false;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// LOAD TEXTURE
		HTexture GraphicsManager::LoadTexture( const wchar_t* filename, Color colorKey )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::LoadTexture - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return SGD::INVALID_HANDLE;

			assert( filename != nullptr && filename[0] != L'\0' && "GraphicsManager::LoadTexture - invalid filename" );
			if( filename == nullptr || filename[0] == L'\0' )
				return SGD::INVALID_HANDLE;


			// Attempt to find the texture in the Handle Manager
			SearchInfo search = { filename, nullptr, SGD::INVALID_HANDLE };
			m_HandleManager.ForEach( &GraphicsManager::FindTextureByName, &search );

			// If it was found, increase the reference & return the existing handle
			if( search.texture != NULL )
			{
				search.texture->unRefCount++;
				return search.handle;
			}


			// Could not find texture in the Handle Manager
			TextureInfo data = { };
			D3DXIMAGE_INFO info = { };
			D3DSURFACE_DESC surface = { };


			// Attempt to load from file
			HRESULT hResult = D3DXCreateTextureFromFileExW( m_pDevice, filename, 0, 0, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, (D3DCOLOR)colorKey, &info, nullptr, &data.texture );
			if( FAILED( hResult ) )
			{
				// MESSAGE
				wchar_t wszBuffer[ 256 ];
				_snwprintf_s( wszBuffer, 256, _TRUNCATE, L"!!! GraphicsManager::LoadTexture - failed to load texture file \"%ws\" (0x%X) !!!", filename, hResult );
				OutputDebugStringW( wszBuffer );
				OutputDebugStringA( "\n" );
				MessageBoxW( m_hWnd, wszBuffer, L"GraphicsManager::LoadTexture", MB_OK );

				return SGD::INVALID_HANDLE;
			}


			// Texture loaded successfully
			data.wszFilename = _wcsdup( filename );
			data.unRefCount = 1;


			// Compare surface description to the original image
			data.texture->GetLevelDesc( 0, &surface );

			if( surface.Width != info.Width || surface.Height != info.Height )
			{
				// MESSAGE
				wchar_t wszBuffer[ 256 ];
				_snwprintf_s( wszBuffer, 256, _TRUNCATE, L"!!! GraphicsManager::LoadTexture - Texture file \"%ws\" is stretched from %ux%u to %ux%u !!!\n", filename, info.Width, info.Height, surface.Width, surface.Height );
				OutputDebugStringW( wszBuffer );
			}


			// Store the buffer size
			data.fWidth  = (float)surface.Width;
			data.fHeight = (float)surface.Height;


			// Store texture into the Handle Manager
			return m_HandleManager.StoreData( data );
		}
		//*************************************************************//



		//*************************************************************//
		// LOAD TEXTURE
		HTexture GraphicsManager::LoadTexture( const char* filename, Color colorKey )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::LoadTexture - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return SGD::INVALID_HANDLE;

			assert( filename != nullptr && filename[0] != '\0' && "GraphicsManager::LoadTexture - invalid filename" );
			if( filename == nullptr || filename[0] == '\0' )
				return SGD::INVALID_HANDLE;


			// Convert the filename to UTF16
			wchar_t widename[ MAX_PATH * 4 ];
			int ret = MultiByteToWideChar( CP_UTF8, 0, filename, -1, widename, MAX_PATH * 4 );

			if( ret == 0 )
			{
				// MESSAGE
				char szBuffer[ 256 ];
				_snprintf_s( szBuffer, 256, _TRUNCATE, "!!! GraphicsManager::LoadTexture - invalid filename \"%hs\" (0x%X) !!!", filename, GetLastError() );
				OutputDebugStringA( szBuffer );
				OutputDebugStringA( "\n" );

				return SGD::INVALID_HANDLE;
			}


			// Use the UTF16 load
			return LoadTexture( widename, colorKey );
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW TEXTURE
		bool GraphicsManager::DrawTexture( HTexture handle, Point position, float rotation, Vector rotationOffset, Color color, Size scale )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::DrawTexture - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			assert( handle != SGD::INVALID_HANDLE && "GraphicsManager::DrawTexture - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Get the texture info from the handle manager
			TextureInfo* data = m_HandleManager.GetData( handle );
			assert( data != nullptr && "GraphicsManager::DrawTexture - handle has expired" );
			if( data == nullptr )
				return false;

			
			// Store original transform
			D3DXMATRIX original, world;
			m_pSprite->GetTransform( &original );

			// Calculate transform matrix		
			D3DXMATRIX scaled;
			D3DXMATRIX rotated;
			D3DXMATRIX translated;

			D3DXMATRIX transform;
			D3DXMatrixIdentity( &transform );

			// Scale
			D3DXMatrixScaling( &scaled, scale.width, scale.height, 1.0f );
			transform *= scaled;

			// Rotate around the local point
			D3DXMatrixTranslation( &translated, -rotationOffset.x * scale.width, -rotationOffset.y * scale.height, 0.0f );
			transform *= translated;
			D3DXMatrixRotationZ( &rotated, rotation );
			transform *= rotated;
			D3DXMatrixTranslation( &translated, rotationOffset.x * scale.width, rotationOffset.y * scale.height, 0.0f );
			transform *= translated;

			// Translate
			D3DXMatrixTranslation( &translated, position.x, position.y, 0.0f );
			transform *= translated;


			// Apply the transform
			D3DXMatrixMultiply( &world, &transform, &original );
			m_pSprite->SetTransform( &world );


			// Draw the texture
			HRESULT result = m_pSprite->Draw( data->texture, nullptr, nullptr, nullptr, (D3DCOLOR)color );


			// Restore the transform
			m_pSprite->SetTransform( &original );


			if( FAILED( result ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::DrawTexture - failed to draw texture (0x%X) !!!\n", result );
				OutputDebugStringA( szBuffer );

				return false;
			}

			return true;
		}
		//*************************************************************//


		SGD::Size GraphicsManager::GetTextureSize(SGD::HTexture _Texture)
		{
			SGD::Size _size;
			// Sanity-check the wrapper's status
			assert(m_eStatus == E_INITIALIZED && "GraphicsManager::UnloadTexture - wrapper has not been initialized");
			if (m_eStatus != E_INITIALIZED)
				return _size;

			// Quietly ignore bad handles
			if (_Texture == INVALID_HANDLE)
				return _size;


			// Get the texture info from the handle manager
			TextureInfo* data = m_HandleManager.GetData(_Texture);
			if (data == nullptr)
				return _size;
			_size.height = data->fHeight;
			_size.width = data->fWidth;
			return _size;
		}

		//*************************************************************//
		// DRAW TEXTURE SECTION
		bool GraphicsManager::DrawTextureSection( HTexture handle, Point position, Rectangle section, float rotation, Vector rotationOffset, Color color, Size scale )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::DrawTextureSection - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			assert( handle != SGD::INVALID_HANDLE && "GraphicsManager::DrawTexture - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Get the texture info from the handle manager
			TextureInfo* data = m_HandleManager.GetData( handle );
			assert( data != nullptr && "GraphicsManager::DrawTextureSection - handle has expired" );
			if( data == nullptr )
				return false;

		
			// Store original transform
			D3DXMATRIX original, world;
			m_pSprite->GetTransform( &original );

			// Calculate transform matrix
			D3DXMATRIX scaled;
			D3DXMATRIX rotated;
			D3DXMATRIX translated;

			D3DXMATRIX transform;
			D3DXMatrixIdentity( &transform );

			// Scale
			D3DXMatrixScaling( &scaled, scale.width, scale.height, 1.0f );
			transform *= scaled;

			// Rotate around the local point
			D3DXMatrixTranslation( &translated, -rotationOffset.x * scale.width, -rotationOffset.y * scale.height, 0.0f );
			transform *= translated;
			D3DXMatrixRotationZ( &rotated, rotation );
			transform *= rotated;
			D3DXMatrixTranslation( &translated, rotationOffset.x * scale.width, rotationOffset.y * scale.height, 0.0f );
			transform *= translated;

			// Translate
			D3DXMatrixTranslation( &translated, position.x, position.y, 0.0f );
			transform *= translated;


			// Apply the transform
			D3DXMatrixMultiply( &world, &transform, &original );
			m_pSprite->SetTransform( &world );


			// Draw the texture
			RECT source = { (LONG)section.left, (LONG)section.top, (LONG)section.right, (LONG)section.bottom };
			HRESULT result = m_pSprite->Draw( data->texture, &source, nullptr, nullptr, (D3DCOLOR)color );


			// Restore the transform
			m_pSprite->SetTransform( &original );

			
			if( FAILED( result ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::DrawTexture - failed to draw texture (0x%X) !!!\n", result );
				OutputDebugStringA( szBuffer );

				return false;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// UNLOAD TEXTURE
		bool GraphicsManager::UnloadTexture( HTexture& handle )	
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "GraphicsManager::UnloadTexture - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Quietly ignore bad handles
			if( handle == INVALID_HANDLE )
				return false;


			// Get the texture info from the handle manager
			TextureInfo* data = m_HandleManager.GetData( handle );
			if( data == nullptr )
				return false;

			// Release a reference
			data->unRefCount--;

			// Is this the last reference?
			if( data->unRefCount == 0 )
			{
				// Release the texture
				data->texture->Release();

				// Deallocate the name
				delete[] data->wszFilename;

				// Remove the audio info from the handle manager
				m_HandleManager.RemoveData( handle, nullptr );
			}


			// Invalidate the handle
			handle = INVALID_HANDLE;
			return true;
		}


		//*************************************************************//



		//*************************************************************//
		// FIND TEXTURE BY NAME
		/*static*/ bool GraphicsManager::FindTextureByName( Handle handle, TextureInfo& data, SearchInfo* extra )
		{		
			// Compare the names
			if( wcscmp( data.wszFilename, extra->filename ) == 0 )
			{
				// Texture does exist!
				extra->texture	= &data;
				extra->handle	= handle;
				return false;
			}

			// Did not find yet
			return true;
		}
		//*************************************************************//

		
	}	// namespace SGD_IMPLEMENTATION

}	// namespace SGD
