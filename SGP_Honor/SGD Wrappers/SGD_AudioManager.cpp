/***********************************************************************\
|																		|
|	File:			SGD_AudioManager.cpp								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-03-10											|
|																		|
|	Purpose:		To load and play audio files						|
|					.wav - sound effect									|
|					.xwm - music										|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
|	Derived From:	"XAudio2 Programming Guide"	- MSDN					|
|	 http://msdn.microsoft.com/en-us/library/hh405049%28v=vs.85%29.aspx	|
|																		|
\***********************************************************************/

#include "SGD_AudioManager.h"


// Uses assert for debug breaks
#include <cassert>

// Uses OutputDebugString for debug text
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstring>

// Uses std::multimap for storing voices
#include <map>

// Uses DirectInput to solve random memory-leak detection bug?!?
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

// Uses XAudio2 for audio
#include <XAudio2.h>
#pragma comment (lib, "dxguid.lib")

// Uses HandleManager for storing data
#include "SGD_HandleManager.h"


namespace SGD
{
	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//
		// AudioInfo
		//	- stores info for the audio file: name, buffer, reference count
		struct AudioInfo
		{
			wchar_t*				wszFilename;		// file name
			unsigned int			unRefCount;			// reference count
			WAVEFORMATEXTENSIBLE	format;				// wave format (sample rate, etc)
			XAUDIO2_BUFFER			buffer;				// buffer
			XAUDIO2_BUFFER_WMA		bufferwma;			// additional buffer packets for xwm
			float					fVolume;			// audio volume
		};
		//*************************************************************//


		
		//*************************************************************//
		// VoiceInfo
		//	- stores info for the source voice instance: audio handle, buffer, state
		struct VoiceInfo
		{
			HAudio					audio;				// audio handle
			IXAudio2SourceVoice*	voice;				// source voice
			bool					loop;				// should repeat
			bool					paused;				// currently paused
		};
		//*************************************************************//



		//*************************************************************//
		// AudioManager
		//	- concrete class for loading and playing audio files
		//	- only supports .wav and .xwm files
		//	- .wav files are categorized as 'Sound Effects'
		//	- .xwm files are categorized as 'Music'
		//	- uses IHandleManager to store audio data
		class AudioManager : public SGD::AudioManager
		{
		public:
			// SINGLETON
			static	AudioManager*	GetInstance		( void );
			static	void			DeleteInstance	( void );


			virtual	bool		Initialize			( void )	override;
			virtual	bool		Update				( void )	override;
			virtual	bool		Terminate			( void )	override;
			

			//enum class AudioCategory 
			//{ 
			//	Music,				// *.xwm files
			//	SoundEffects,		// *.wav files
			//};
			virtual int			GetMasterVolume		( AudioGroup group )				override;
			virtual bool		SetMasterVolume		( AudioGroup group, int value )		override;


			virtual	HAudio		LoadAudio			( const wchar_t* filename )			override;
			virtual	HAudio		LoadAudio			( const char* filename )			override;
			virtual	HVoice		PlayAudio			( HAudio handle, bool looping )		override;
			virtual bool		IsAudioPlaying		( HAudio handle )					override;
			virtual	bool		StopAudio			( HAudio handle )					override;
			virtual	bool		UnloadAudio			( HAudio& handle )					override;
			
			virtual bool		IsVoiceValid		( HVoice handle )					override;
			virtual bool		IsVoicePlaying		( HVoice handle )					override;
			virtual bool		PauseVoice			( HVoice handle, bool pause )		override;
			virtual bool		StopVoice			( HVoice& handle )					override;		

			virtual int			GetVoiceVolume		( HVoice handle )					override;
			virtual bool		SetVoiceVolume		( HVoice handle, int value )		override;
			virtual int			GetAudioVolume		( HAudio handle )					override;
			virtual bool		SetAudioVolume		( HAudio handle, int value )		override;


		private:
			// SINGLETON
			static	AudioManager*		s_Instance;		// the ONE instance

			AudioManager				( void )					= default;	// Default constructor
			virtual	~AudioManager		( void )					= default;	// Destructor

			AudioManager				( const AudioManager& )		= delete;	// Copy constructor
			AudioManager&	operator=	( const AudioManager& )		= delete;	// Assignment operator


			// Wrapper Status
			enum EAudioManagerStatus
			{	
				E_UNINITIALIZED,
				E_INITIALIZED,
				E_DESTROYED
			};

			EAudioManagerStatus			m_eStatus			= E_UNINITIALIZED;	// wrapper initialization status

			IXAudio2*					m_pXAudio			= nullptr;			// XAudio2 api
			IXAudio2MasteringVoice*		m_pMasterVoice		= nullptr;			// master voice
			IXAudio2SubmixVoice*		m_pSfxVoice			= nullptr;			// sound effects submix voice
			IXAudio2SubmixVoice*		m_pMusVoice			= nullptr;			// music submix voice
			DWORD						m_dwChannelMask		= 0;				// speaker configuration
			UINT32						m_unChannels		= 0;				// speaker configuration count


			typedef std::multimap< HAudio, HVoice >	VoiceMap;
			VoiceMap					m_mVoices;								// source voice map

			HandleManager< AudioInfo >	m_HandleManager;						// data storage
			HandleManager< VoiceInfo >	m_VoiceManager;							// voice storage


			// AUDIO LOADING HELPER METHODS
			static	HRESULT		FindChunk		( HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition );
			static	HRESULT		ReadChunkData	( HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset );
			static	HRESULT		LoadAudio		( const wchar_t* filename, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer, XAUDIO2_BUFFER_WMA& bufferWMA );


			// AUDIO REFERENCE HELPER METHOD
			struct SearchInfo
			{
				const wchar_t*	filename;	// input
				AudioInfo*		audio;		// output
				HAudio			handle;		// output
			};
			static	bool	FindAudioByName( Handle handle, AudioInfo& data, SearchInfo* extra );
		};
		//*************************************************************//

	}	// namespace SGD_IMPLEMENTATION



	//*****************************************************************//
	// Interface singleton accessor
	/*static*/ AudioManager* AudioManager::GetInstance( void )
	{
		// Return the implementation singleton (upcasted to interface)
		return (SGD::AudioManager*)SGD_IMPLEMENTATION::AudioManager::GetInstance();
	}

	// Interface singleton destructor
	/*static*/ void AudioManager::DeleteInstance( void )
	{
		// Deallocate the implementation singleton
		return SGD_IMPLEMENTATION::AudioManager::DeleteInstance();
	}
	//*****************************************************************//



	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//	
		// SINGLETON

		// Instantiate static pointer to null (no instance yet)
		/*static*/ AudioManager* AudioManager::s_Instance = nullptr;

		// Singleton accessor
		/*static*/ AudioManager* AudioManager::GetInstance( void )
		{
			// Allocate singleton on first use
			if( AudioManager::s_Instance == nullptr )
				AudioManager::s_Instance = new AudioManager;

			// Return the singleton
			return AudioManager::s_Instance;
		}

		// Singleton destructor
		/*static*/ void AudioManager::DeleteInstance( void )
		{
			// Deallocate singleton
			delete AudioManager::s_Instance;
			AudioManager::s_Instance = nullptr;
		}
		//*************************************************************//



		//*************************************************************//
		// INITIALIZE
		bool AudioManager::Initialize( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_UNINITIALIZED && "AudioManager::Initialize - wrapper has already been initialized" );
			if( m_eStatus != E_UNINITIALIZED )
				return false;


			HRESULT hResult = S_OK;

			// When using a COM object, the thread should be CoInitialized
			CoInitializeEx( nullptr, COINIT_MULTITHREADED );


			// HACK: Somehow creating DirectInput will allow memory leak detection?!?
			IDirectInput8W* pDI_Hack;
			DirectInput8Create( (HINSTANCE)GetModuleHandle( nullptr ), DIRECTINPUT_VERSION, IID_IDirectInput8W, (void**)&pDI_Hack, NULL);
			pDI_Hack->Release();


			// Attempt to create the XAudio2 interface
			hResult = XAudio2Create( &m_pXAudio, 0 );

			if( FAILED( hResult ) )
			{
				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::Initialize - failed to initialize XAudio2 (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Attempt to create the mastering voice
			hResult = m_pXAudio->CreateMasteringVoice( &m_pMasterVoice );
			if( FAILED( hResult ) )
			{
				m_pXAudio->Release();
				m_pXAudio = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::Initialize - failed to initialize XAudio2 master voice (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Get the speaker configuration
			XAUDIO2_DEVICE_DETAILS details;
			m_pXAudio->GetDeviceDetails( 0U, &details );
			m_dwChannelMask = details.OutputFormat.dwChannelMask;
			m_unChannels = details.OutputFormat.Format.nChannels;


			// Attempt to create the sfx submix voice
			hResult = m_pXAudio->CreateSubmixVoice( &m_pSfxVoice, m_unChannels, 44100, XAUDIO2_VOICE_USEFILTER );
			if( FAILED( hResult ) )
			{
				m_pMasterVoice->DestroyVoice();
				m_pMasterVoice = nullptr;

				m_pXAudio->Release();
				m_pXAudio = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::Initialize - failed to initialize XAudio2 sfx submix voice (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Attempt to create the music submix voice
			hResult = m_pXAudio->CreateSubmixVoice( &m_pMusVoice, m_unChannels, 44100, XAUDIO2_VOICE_USEFILTER );
			if( FAILED( hResult ) )
			{
				m_pSfxVoice->DestroyVoice();
				m_pSfxVoice = nullptr;

				m_pMasterVoice->DestroyVoice();
				m_pMasterVoice = nullptr;

				m_pXAudio->Release();
				m_pXAudio = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::Initialize - failed to initialize XAudio2 music submix voice (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Success!
			m_eStatus = E_INITIALIZED;

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// UPDATE
		bool AudioManager::Update( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::Update - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Update the current voices
			VoiceMap::iterator iter = m_mVoices.begin();
			while( iter != m_mVoices.end() )
			{
				// Get the voice info from the handle manager
				VoiceInfo* info = m_VoiceManager.GetData( iter->second );
				assert( info != nullptr && "AudioManager::Update - voice handle has expired" );
				if( info == nullptr )
				{
					iter = m_mVoices.erase( iter );
					continue;
				}


				// Has the voice ended?
				XAUDIO2_VOICE_STATE state;
				info->voice->GetState( &state );
				if( state.BuffersQueued == 0 )
				{
					// Should it loop?
					if( info->loop == true )
					{
						// Get the data from the Handle Manager
						AudioInfo* data = m_HandleManager.GetData( iter->first );
						assert( data != nullptr && "AudioManager::Update - voice refers to removed audio" );
						if( data == nullptr )
						{
							// Remove the voice (could recycle it ...)
							info->voice->DestroyVoice();
							iter = m_mVoices.erase( iter );
							continue;
						}

						// Play wav or wma?
						if( data->bufferwma.PacketCount == 0 )
							info->voice->SubmitSourceBuffer( &data->buffer );
						else
							info->voice->SubmitSourceBuffer( &data->buffer, &data->bufferwma );
					}
					else
					{
						// Remove the voice (could recycle it ...)
						info->voice->DestroyVoice();
						iter = m_mVoices.erase( iter );
						continue;
					}
				}

				++iter;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// TERMINATE
		bool AudioManager::Terminate( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::Terminate - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Release all audio voices
			for( VoiceMap::iterator iter = m_mVoices.begin(); iter != m_mVoices.end(); ++iter )
			{
				// Get the voice info from the handle manager
				VoiceInfo* info = m_VoiceManager.GetData( iter->second );
				if( info == nullptr )
					continue;


				//info->voice->Stop();
				//info->voice->FlushSourceBuffers();
				info->voice->DestroyVoice();
			}
			m_mVoices.clear();


			// Clear handles
			m_VoiceManager.Clear();
			m_HandleManager.Clear();

			
			// Release submix & master voices
			m_pMusVoice->DestroyVoice();
			m_pMusVoice = nullptr;

			m_pSfxVoice->DestroyVoice();
			m_pSfxVoice = nullptr;

			m_pMasterVoice->DestroyVoice();
			m_pMasterVoice = nullptr;

			// Release XAudio2
			m_pXAudio->Release();
			m_pXAudio = nullptr;

			CoUninitialize();

			m_eStatus = E_DESTROYED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// GET MASTER VOLUME
		int AudioManager::GetMasterVolume( AudioGroup group )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::GetMasterVolume - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return 0;
			
			// Sanity-check the parameter
			assert( (group == AudioGroup::Music || group == AudioGroup::SoundEffects) && "AudioManager::GetMasterVolume - invalid group" );


			// Get the master volume from the submix voice
			float fVolume = 0.0f;
			if( group == AudioGroup::Music )
				m_pMusVoice->GetVolume( &fVolume );
			else //if( type == AudioGroup::SoundEffects )
				m_pSfxVoice->GetVolume( &fVolume );


			// Scale 0 -> +100 (account for floating point error)
			return (int)(fVolume * 100.0f + 0.5f);
		}
		//*************************************************************//



		//*************************************************************//
		// SET MASTER VOLUME
		bool AudioManager::SetMasterVolume( AudioGroup group, int value )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::SetMasterVolume - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Sanity-check the parameter
			assert( (group == AudioGroup::Music || group == AudioGroup::SoundEffects) && "AudioManager::SetMasterVolume - invalid group" );

			
			//assert( value >= 0 && value <= 100 && "AudioManager::SetMasterVolume - volume must be between 0 -> 100" );

			// Cap the range 0->100
			if( value < 0 )
				value = 0;
			else if( value > 100 )
				value = 100;
			
			// Set the submix voice volume
			HRESULT result = 0;
			if( group == AudioGroup::Music )
				result = m_pMusVoice->SetVolume( (float)( value / 100.0f ) );
			else //if( group == AudioGroup::SoundEffects )
				result = m_pSfxVoice->SetVolume( (float)( value / 100.0f ) );

			return SUCCEEDED( result );
		}
		//*************************************************************//



		//*************************************************************//
		// LOAD AUDIO
		HAudio AudioManager::LoadAudio( const wchar_t* filename )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::LoadAudio - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return SGD::INVALID_HANDLE;

			assert( filename != nullptr && filename[0] != L'\0' && "AudioManager::LoadAudio - invalid filename" );
			if( filename == nullptr || filename[0] == L'\0' )
				return SGD::INVALID_HANDLE;


			// Attempt to find the audio in the Handle Manager
			SearchInfo search = { filename, nullptr, SGD::INVALID_HANDLE };
			m_HandleManager.ForEach( &AudioManager::FindAudioByName, &search );

			// If it was found, increase the reference & return the existing handle
			if( search.audio != NULL )
			{
				search.audio->unRefCount++;
				return search.handle;
			}


			// Could not find audio in the Handle Manager
			AudioInfo data = { };
			ZeroMemory( &data.format, sizeof( data.format ) );
			ZeroMemory( &data.buffer, sizeof( data.buffer ) );
			ZeroMemory( &data.bufferwma, sizeof( data.bufferwma ) );


			// Attempt to load from file
			HRESULT hResult = LoadAudio( filename, data.format, data.buffer, data.bufferwma );
			if( FAILED( hResult ) )
			{
				// MESSAGE
				wchar_t wszBuffer[ 256 ];
				_snwprintf_s( wszBuffer, 256, _TRUNCATE, L"!!! AudioManager::LoadAudio - failed to load audio file \"%ws\" (0x%X) !!!", filename, hResult );
				OutputDebugStringW( wszBuffer );
				OutputDebugStringA( "\n" );
				MessageBoxW( GetActiveWindow(), wszBuffer, L"AudioManager::LoadAudio", MB_OK );

				return SGD::INVALID_HANDLE;
			}


			// Audio loaded successfully
			data.wszFilename	= _wcsdup( filename );
			data.unRefCount	= 1;
			data.fVolume	= 1.0f;


			// Store audio into the Handle Manager
			return m_HandleManager.StoreData( data );
		}
		//*************************************************************//



		//*************************************************************//
		// LOAD AUDIO
		HAudio AudioManager::LoadAudio( const char* filename )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::LoadAudio - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return SGD::INVALID_HANDLE;

			assert( filename != nullptr && filename[0] != '\0' && "AudioManager::LoadAudio - invalid filename" );
			if( filename == nullptr || filename[0] == '\0' )
				return SGD::INVALID_HANDLE;


			// Convert the filename to UTF16
			wchar_t widename[ MAX_PATH * 4 ];
			int ret = MultiByteToWideChar( CP_UTF8, 0, filename, -1, widename, MAX_PATH * 4 );

			if( ret == 0 )
			{
				// MESSAGE
				char szBuffer[ 256 ];
				_snprintf_s( szBuffer, 256, _TRUNCATE, "!!! AudioManager::LoadAudio - invalid filename \"%hs\" (0x%X) !!!", filename, GetLastError() );
				OutputDebugStringA( szBuffer );
				OutputDebugStringA( "\n" );

				return SGD::INVALID_HANDLE;
			}


			// Use the UTF16 load
			return LoadAudio( widename );
		}
		//*************************************************************//



		//*************************************************************//
		// PLAY AUDIO
		HVoice AudioManager::PlayAudio( HAudio handle, bool looping )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::PlayAudio - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return SGD::INVALID_HANDLE;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::PlayAudio - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return SGD::INVALID_HANDLE;


			// Could create a VoicePool to recycle voices
			// Voices can be reused for other audio files as long
			// as they have the same samplerate and channels
			// (the frequency rate may need to be modified)


			// Get the audio info from the handle manager
			AudioInfo* data = m_HandleManager.GetData( handle );
			assert( data != nullptr && "AudioManager::PlayAudio - handle has expired" );
			if( data == nullptr )
				return SGD::INVALID_HANDLE;


			HRESULT hResult = S_OK;

			// Create parameter (send descriptor) for submix voice
			XAUDIO2_SEND_DESCRIPTOR desc = { 0 };
			if( data->bufferwma.PacketCount == 0 )
				desc.pOutputVoice = m_pSfxVoice;
			else
				desc.pOutputVoice = m_pMusVoice;

			XAUDIO2_VOICE_SENDS sendlist = { 1, &desc };

			// Create a voice with the proper wave format
			IXAudio2SourceVoice* pVoice = nullptr;
			hResult = m_pXAudio->CreateSourceVoice( &pVoice, (WAVEFORMATEX*)&data->format, 0U, 2.0f, nullptr, &sendlist );
			if( FAILED( hResult ) ) 
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::PlayAudio - failed to create voice (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return SGD::INVALID_HANDLE;
			}

			// Use the XAUDIO2_BUFFER for the voice's source
			if( data->bufferwma.PacketCount == 0 )
				hResult = pVoice->SubmitSourceBuffer( &data->buffer );
			else
				hResult = pVoice->SubmitSourceBuffer( &data->buffer, &data->bufferwma );

			if( FAILED( hResult ) )
			{
				pVoice->DestroyVoice();
				pVoice = nullptr;

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::PlayAudio - failed to submit source buffer (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return SGD::INVALID_HANDLE;
			}


			// Set the volume
			pVoice->SetVolume( data->fVolume );


			// Start the voice's thread
			hResult = pVoice->Start( 0 );
			if( FAILED( hResult ) )
			{
				pVoice->DestroyVoice();
				pVoice = nullptr;

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::PlayAudio - failed to start voice (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return SGD::INVALID_HANDLE;
			}


			// Store the voice
			VoiceInfo info = { handle, pVoice, looping, false };
			HVoice hv = m_VoiceManager.StoreData( info );
			if( hv != SGD::INVALID_HANDLE )
				m_mVoices.insert( VoiceMap::value_type( handle, hv ) );

			return hv;
		}
		//*************************************************************//



		//*************************************************************//
		// IS AUDIO PLAYING
		bool AudioManager::IsAudioPlaying( HAudio handle )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::IsAudioPlaying - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::IsAudioPlaying - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Find all voices with this handle
			VoiceMap::_Paircc range = m_mVoices.equal_range( handle );
			for( VoiceMap::const_iterator iter = range.first; iter != range.second; ++iter )
			{
				// Check if there are any active voices for this handle
				if( IsVoicePlaying( iter->second ) == true )
					return true;
			}

			return false;
		}
		//*************************************************************//



		//*************************************************************//
		// STOP AUDIO
		bool AudioManager::StopAudio( HAudio handle )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::StopAudio - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::StopAudio - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Find all voices with this handle
			VoiceMap::_Paircc range = m_mVoices.equal_range( handle );
			for( VoiceMap::const_iterator iter = range.first; iter != range.second; ++iter )
			{
				// Get the voice info from the Handle Manager
				VoiceInfo* info = m_VoiceManager.GetData( iter->second );
				if( info == nullptr )
					continue;

				// Destroy (or recycle) the voice
				info->voice->DestroyVoice();
				info->voice = nullptr;

				// Remove the voice from the HandleManager
				m_VoiceManager.RemoveData( iter->second, nullptr );
			}

			// Remove the voices from the active map
			m_mVoices.erase( handle );

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// UNLOAD AUDIO
		bool AudioManager::UnloadAudio( HAudio& handle )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::UnloadAudio - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Quietly ignore bad handles
			if( m_HandleManager.IsHandleValid( handle ) == false )
			{
				handle = SGD::INVALID_HANDLE;
				return false;
			}


			// Get the audio info from the handle manager
			AudioInfo* data = m_HandleManager.GetData( handle );
			if( data == nullptr )
				return false;


			// Release a reference
			data->unRefCount--;

			// Is this the last reference?
			if( data->unRefCount == 0 )
			{
				// Stop the audio
				StopAudio( handle );


				// Deallocate the audio buffers
				delete[] data->buffer.pAudioData;
				delete[] data->bufferwma.pDecodedPacketCumulativeBytes;

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
		// IS VOICE VALID
		bool AudioManager::IsVoiceValid( HVoice handle )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::IsVoicePlaying - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Validate the handle
			return m_VoiceManager.IsHandleValid( handle );
		}
		//*************************************************************//


		
		//*************************************************************//
		// IS VOICE PLAYING
		bool AudioManager::IsVoicePlaying( HVoice handle )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::IsVoicePlaying - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::IsVoicePlaying - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Get the voice info from the handle manager
			VoiceInfo* data = m_VoiceManager.GetData( handle );
			assert( data != nullptr && "AudioManager::IsVoicePlaying - handle has expired" );
			if( data == nullptr )
				return false;

		
			// Is the voice playing?
			return !data->paused;
		}
		//*************************************************************//


		
		//*************************************************************//
		// PAUSE VOICE
		bool AudioManager::PauseVoice( HVoice handle, bool pause )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::PauseVoice - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::PauseVoice - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Get the voice info from the handle manager
			VoiceInfo* data = m_VoiceManager.GetData( handle );
			assert( data != nullptr && "AudioManager::PauseVoice - handle has expired" );
			if( data == nullptr )
				return false;


			// Stop the voice?
			if( pause == true )
			{
				HRESULT hResult = data->voice->Stop( 0 );
				if( FAILED( hResult ) )
				{
					// MESSAGE
					char szBuffer[ 128 ];
					_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::PauseVoice - failed to stop voice (0x%X) !!!\n", hResult );
					OutputDebugStringA( szBuffer );

					return false;
				}
			}
			else 
			{
				HRESULT hResult = data->voice->Start( 0 );
				if( FAILED( hResult ) )
				{
					// MESSAGE
					char szBuffer[ 128 ];
					_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::PauseVoice - failed to start voice (0x%X) !!!\n", hResult );
					OutputDebugStringA( szBuffer );

					return false;
				}
			}
				
			data->paused = pause;
			return true;
		}
		//*************************************************************//


		
		//*************************************************************//
		// STOP VOICE
		bool AudioManager::StopVoice( HVoice& handle )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::StopVoice - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::StopVoice - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Get the voice info from the handle manager
			VoiceInfo* data = m_VoiceManager.GetData( handle );
			assert( data != nullptr && "AudioManager::StopVoice - handle has expired" );
			if( data == nullptr )
				return false;


			// Stop the voice
			HRESULT hResult = data->voice->Stop( 0 );
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! AudioManager::StopVoice - failed to stop voice (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Destroy the voice
			data->voice->DestroyVoice();
			data->voice = nullptr;

			
			// Find all voices with the audio handle
			VoiceMap::_Paircc range = m_mVoices.equal_range( data->audio );
			for( VoiceMap::const_iterator iter = range.first; iter != range.second; ++iter )
			{
				// Remove this voice handle
				if( iter->second == handle )
				{
					m_mVoices.erase( iter );
					break;
				}
			}


			// Remove the voice info from the handle manager
			m_VoiceManager.RemoveData( handle, nullptr );
			

			// Invalidate the handle
			handle = SGD::INVALID_HANDLE;
			return true;
		}
		//*************************************************************//


		
		//*************************************************************//
		// GET VOICE VOLUME
		int AudioManager::GetVoiceVolume( HVoice handle )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::GetVoiceVolume - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return 0;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::GetVoiceVolume - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return 0;


			// Get the voice info from the handle manager
			VoiceInfo* data = m_VoiceManager.GetData( handle );
			assert( data != nullptr && "AudioManager::GetVoiceVolume - handle has expired" );
			if( data == nullptr )
				return 0;
			

			// Scale 0 -> +100 (account for floating point error)
			float fVolume = 0.0f;
			data->voice->GetVolume( &fVolume );
			return (int)(fVolume * 100.0f + 0.5f);
		}		
		//*************************************************************//


		
		//*************************************************************//
		// SET VOICE VOLUME
		bool AudioManager::SetVoiceVolume( HVoice handle, int value )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::SetVoiceVolume - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::SetVoiceVolume - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Get the voice info from the handle manager
			VoiceInfo* data = m_VoiceManager.GetData( handle );
			assert( data != nullptr && "AudioManager::SetVoiceVolume - handle has expired" );
			if( data == nullptr )
				return false;

			


			// Cap the range 0->100
			if( value < 0 )
				value = 0;
			else if( value > 100 )
				value = 100;

			float fVolume = value / 100.0f;	// scaled to 0 -> +1
			
			HRESULT hResult = data->voice->SetVolume( fVolume );
			if( FAILED( hResult ) )
				return false;

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// GET AUDIO VOLUME
		int AudioManager::GetAudioVolume( HAudio handle )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::GetAudioVolume - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return 0;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::GetAudioVolume - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return 0;


			// Get the audio info from the handle manager
			AudioInfo* data = m_HandleManager.GetData( handle );
			assert( data != nullptr && "AudioManager::GetAudioVolume - handle has expired" );
			if( data == nullptr )
				return 0;
			

			// Scale 0 -> +100 (account for floating point error)
			return (int)(data->fVolume * 100.0f + 0.5f);
		}
		//*************************************************************//



		//*************************************************************//
		// SET AUDIO ATTRIBUTE
		bool AudioManager::SetAudioVolume( HAudio handle, int value )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "AudioManager::SetAudioVolume - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			assert( handle != SGD::INVALID_HANDLE && "AudioManager::SetAudioVolume - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Get the audio info from the handle manager
			AudioInfo* data = m_HandleManager.GetData( handle );
			assert( data != nullptr && "AudioManager::SetAudioVolume - handle has expired" );
			if( data == nullptr )
				return false;


			// Cap the range 0->100
			if( value < 0 )
				value = 0;
			else if( value > 100 )
				value = 100;

			data->fVolume = value / 100.0f;	// scaled to 0 -> +1


			// Set active voices' volume
			bool success = true;
			std::pair< VoiceMap::const_iterator, VoiceMap::const_iterator > rangePair = m_mVoices.equal_range( handle );

			for( VoiceMap::const_iterator iter = rangePair.first; iter != rangePair.second; ++iter )
			{
				// Get the voice info from the handle manager
				VoiceInfo* info = m_VoiceManager.GetData( iter->second );
				assert( info != nullptr && "AudioManager::SetAudioVolume - voice handle has expired" );
				if( info == nullptr )
					continue;

				HRESULT hResult = info->voice->SetVolume( data->fVolume );
				if( FAILED( hResult ) )
					success = false;
			}

			return success;
		}
		//*************************************************************//



		//*************************************************************//
		// XAudio2 file input
		//	- MSDN http://msdn.microsoft.com/en-us/library/windows/desktop/ee415781%28v=vs.85%29.aspx
#ifdef _XBOX				// Big-Endian
		enum ECharacterCode
		{
			E_CC_RIFF	= 'RIFF',
			E_CC_DATA	= 'data',
			E_CC_FMT	= 'fmt ',
			E_CC_WAVE	= 'WAVE',
			E_CC_XWMA	= 'XWMA',
			E_CC_DPDS	= 'dpds'
		};
#else						// Little-Endian
		enum ECharacterCode
		{
			E_CC_RIFF	= 'FFIR',
			E_CC_DATA	= 'atad',
			E_CC_FMT	= ' tmf',
			E_CC_WAVE	= 'EVAW',
			E_CC_XWMA	= 'AMWX',
			E_CC_DPDS	= 'sdpd'
		};
#endif

		/*static*/ HRESULT AudioManager::FindChunk( HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition )
		{
			HRESULT hResult = S_OK;
			if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
				return HRESULT_FROM_WIN32( GetLastError() );

			DWORD dwChunkType;
			DWORD dwChunkDataSize;
			DWORD dwRIFFDataSize = 0;
			DWORD dwFileType;
			DWORD bytesRead = 0;
			DWORD dwOffset = 0;

			while( hResult == S_OK )
			{
				DWORD dwRead;
				if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
					hResult = HRESULT_FROM_WIN32( GetLastError() );

				if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
					hResult = HRESULT_FROM_WIN32( GetLastError() );

				switch( dwChunkType )
				{
				case E_CC_RIFF:
					dwRIFFDataSize = dwChunkDataSize;
					dwChunkDataSize = 4;
					if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
						hResult = HRESULT_FROM_WIN32( GetLastError() );
					break;

				default:
					if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, NULL, FILE_CURRENT ) )
						return HRESULT_FROM_WIN32( GetLastError() );            
				}

				dwOffset += sizeof(DWORD) * 2;

				if( dwChunkType == fourcc )
				{
					dwChunkSize = dwChunkDataSize;
					dwChunkDataPosition = dwOffset;
					return S_OK;
				}

				dwOffset += dwChunkDataSize;

				if( bytesRead >= dwRIFFDataSize ) 
					return S_FALSE;
			}

			return S_OK;
		}

		/*static*/ HRESULT AudioManager::ReadChunkData( HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset )
		{
			HRESULT hResult = S_OK;

			if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferoffset, NULL, FILE_BEGIN ) )
				return HRESULT_FROM_WIN32( GetLastError() );

			DWORD dwRead;
			if( 0 == ReadFile( hFile, buffer, buffersize, &dwRead, NULL ) )
				hResult = HRESULT_FROM_WIN32( GetLastError() );

			return hResult;
		}

		/*static*/ HRESULT AudioManager::LoadAudio( const wchar_t* filename, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer, XAUDIO2_BUFFER_WMA& bufferWMA )
		{
			// Open the file
			HANDLE hFile = CreateFileW( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

			if( hFile == INVALID_HANDLE_VALUE )
				return HRESULT_FROM_WIN32( GetLastError() );

			if( SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) == INVALID_SET_FILE_POINTER )
				return HRESULT_FROM_WIN32( GetLastError() );


			// Check the file type, should be 'WAVE' or 'XWMA'
			DWORD dwChunkSize;
			DWORD dwChunkPosition;
			FindChunk( hFile, E_CC_RIFF, dwChunkSize, dwChunkPosition );

			DWORD filetype;
			ReadChunkData( hFile, &filetype, sizeof(DWORD), dwChunkPosition );
			if( filetype == E_CC_WAVE || filetype == E_CC_XWMA )
			{
				// Fill out the WAVEFORMATEXTENSIBLE structure with the contents of the FMT chunk. 		
				FindChunk( hFile, E_CC_FMT, dwChunkSize, dwChunkPosition );
				ReadChunkData( hFile, &wfx, dwChunkSize, dwChunkPosition );


				// Read the contents of the DATA chunk into the audio data buffer
				FindChunk( hFile, E_CC_DATA, dwChunkSize, dwChunkPosition );
				BYTE* pDataBuffer = new BYTE[ dwChunkSize ];
				ReadChunkData( hFile, pDataBuffer, dwChunkSize, dwChunkPosition );


				// Fill the XAUDIO2_BUFFER
				buffer.AudioBytes = dwChunkSize;		// size of the audio buffer in bytes
				buffer.pAudioData = pDataBuffer;		// buffer containing audio data
				buffer.Flags = XAUDIO2_END_OF_STREAM;	// tell the source voice not to expect any data after this buffer


				// Fill the wma buffer if necessary
				if( filetype == E_CC_XWMA )
				{
					// Read the contents of the DPDS chunk into the wma data buffer
					FindChunk( hFile, E_CC_DPDS, dwChunkSize, dwChunkPosition );
					UINT32  nPackets = (dwChunkSize + (sizeof(UINT32)-1)) / sizeof(UINT32);	// round size to number of DWORDS
					UINT32* pWmaDataBuffer = new UINT32[ nPackets ];
					ReadChunkData( hFile, pWmaDataBuffer, dwChunkSize, dwChunkPosition );


					// Fill the XAUDIO2_BUFFER_WMA
					bufferWMA.PacketCount = nPackets;							// size of the audio buffer in DWORDS
					bufferWMA.pDecodedPacketCumulativeBytes = pWmaDataBuffer;	// buffer containing wma data
				}

				return S_OK;
			}
			else
			{
				return E_UNEXPECTED;
			}
		}
		//*************************************************************//



		//*************************************************************//
		// FIND AUDIO BY NAME
		/*static*/ bool AudioManager::FindAudioByName( Handle handle, AudioInfo& data, SearchInfo* extra )
		{		
			// Compare the names
			if( wcscmp( data.wszFilename, extra->filename ) == 0 )
			{
				// Audio does exist!
				extra->audio	= &data;
				extra->handle	= handle;
				return false;
			}

			// Did not find yet
			return true;
		}
		//*************************************************************//
		

	}	// namespace SGD_IMPLEMENTATION

}	// namespace SGD
