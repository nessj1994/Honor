/***********************************************************************\
|																		|
|	File:			SGD_AudioManager.h  								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To load and play audio files						|
|					.wav - sound effect									|
|					.xwm - music										|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_AUDIOMANAGER_H
#define SGD_AUDIOMANAGER_H


#include "SGD_Handle.h"			// Accesses data using HAudio handles


namespace SGD
{
	//*****************************************************************//
	// AudioGroup
	//	- enumeration of audio category constants for master volumes
	//	- enumerators REQUIRE the enum typename scope: AudioCategory::MasterMusic
	enum class AudioGroup 
	{ 
		Music,				// *.xwm files
		SoundEffects,		// *.wav files
	};


	//*****************************************************************//
	// AudioManager
	//	- SINGLETON class for playing audio
	//	- supports .wav and .xwm files
	class AudioManager
	{
	public:
		static	AudioManager*	GetInstance		( void );
		static	void			DeleteInstance	( void );


		virtual	bool		Initialize			( void )	= 0;
		virtual	bool		Update				( void )	= 0;
		virtual	bool		Terminate			( void )	= 0;
			
		
		virtual int			GetMasterVolume		( AudioGroup group )						= 0;
		virtual bool		SetMasterVolume		( AudioGroup group, int value = 100 )		= 0;


		virtual	HAudio		LoadAudio			( const wchar_t* filename )					= 0;
		virtual	HAudio		LoadAudio			( const char* filename )					= 0;
		virtual	HVoice		PlayAudio			( HAudio handle, bool looping = false )		= 0;
		virtual bool		IsAudioPlaying		( HAudio handle )							= 0;
		virtual	bool		StopAudio			( HAudio handle )							= 0;
		virtual	bool		UnloadAudio			( HAudio& handle )							= 0;

		virtual bool		IsVoiceValid		( HVoice handle )							= 0;
		virtual bool		IsVoicePlaying		( HVoice handle )							= 0;
		virtual bool		PauseVoice			( HVoice handle, bool pause = true )		= 0;
		virtual bool		StopVoice			( HVoice& handle )							= 0;
		
		virtual int			GetVoiceVolume		( HVoice handle )							= 0;
		virtual bool		SetVoiceVolume		( HVoice handle, int value = 100 )			= 0;
		virtual int			GetAudioVolume		( HAudio handle )							= 0;
		virtual bool		SetAudioVolume		( HAudio handle, int value = 100 )			= 0;
		

	protected:
		AudioManager				( void )					= default;
		virtual	~AudioManager		( void )					= default;

		AudioManager				( const AudioManager& )		= delete;
		AudioManager&	operator=	( const AudioManager& )		= delete;
	};

}	// namespace SGD

#endif	//SGD_AUDIOMANAGER_H
