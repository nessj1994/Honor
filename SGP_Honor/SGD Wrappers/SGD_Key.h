/***********************************************************************\
|																		|
|	File:			SGD_Key.h  											|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		Keyboard, mouse, and gamepad key identifiers		|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_KEY_H
#define SGD_KEY_H


namespace SGD
{
	//*****************************************************************//
	// Key
	//	- enumeration of key-code constants for keyboard & mouse
	//	- enumerators REQUIRE the enum typename scope: Key::None
	enum class Key
	{ 
		None				= 0,	// NO KEY

		MouseLeft			= 1,	LButton				= 1,	// Left Mouse Button
		MouseRight			= 2,	RButton				= 2,	// Right Mouse Button
		Break				= 3,	Cancel				= 3,
		MouseMiddle			= 4,	MButton				= 4,	// Middle Mouse Button
		MouseX1				= 5,	XButton1			= 5,	// X1 Mouse Button (back?)
		MouseX2				= 6,	XButton2			= 6,	// X2 Mouse Button (forward?)
		Backspace			= 8,	Back				= 8,
		Tab					= 9,
		Clear				= 12,
		Enter				= 13,	Return				= 13,
		Shift				= 16,								// Either Shift Key
		Control				= 17,								// Either Ctrl Key
		Alt					= 18,	Menu				= 18,	// Either Alt / Menu Key
		Pause				= 19,
		CapsLock			= 20,	Capital				= 20,
		Escape				= 27,
		Spacebar			= 32,	Space				= 32,
		PageUp				= 33,	Prior				= 33,
		PageDown			= 34,	Next				= 34,
		End					= 35,
		Home				= 36,
		LeftArrow			= 37,	Left				= 37,
		UpArrow				= 38,	Up					= 38,
		RightArrow			= 39,	Right				= 39,
		DownArrow			= 40,	Down				= 40,
		Select				= 41,
		Print				= 42,
		Execute				= 43,
		PrintScreen			= 44,	Snapshot			= 44,
		Insert				= 45,
		Delete				= 46,
		Help				= 47,
		N0					= 48,	Zero				= 48,	// Number 0
		N1					= 49,	One					= 49,	// Number 1
		N2					= 50,	Two					= 50,	// Number 2
		N3					= 51,	Three				= 51,	// Number 3
		N4					= 52,	Four				= 52,	// Number 4
		N5					= 53,	Five				= 53,	// Number 5
		N6					= 54,	Six					= 54,	// Number 6
		N7					= 55,	Seven				= 55,	// Number 7
		N8					= 56,	Eight				= 56,	// Number 8
		N9					= 57,	Nine				= 57,	// Number 9
		A					= 65,
		B					= 66,
		C					= 67,
		D					= 68,
		E					= 69,
		F					= 70,
		G					= 71,
		H					= 72,
		I					= 73,
		J					= 74,
		K					= 75,
		L					= 76,
		M					= 77,
		N					= 78,
		O					= 79,
		P					= 80,
		Q					= 81,
		R					= 82,
		S					= 83,
		T					= 84,
		U					= 85,
		V					= 86,
		W					= 87,
		X					= 88,
		Y					= 89,
		Z					= 90,
		LeftWindows			= 91,
		RightWindows		= 92,
		Apps				= 93,	// Applications
		Sleep				= 95,
		NumPad0				= 96,
		NumPad1				= 97,
		NumPad2				= 98,
		NumPad3				= 99,
		NumPad4				= 100,
		NumPad5				= 101,
		NumPad6				= 102,
		NumPad7				= 103,
		NumPad8				= 104,
		NumPad9				= 105,
		Multiply			= 106,
		Add					= 107,
		Separator			= 108,
		Subtract			= 109,
		Decimal				= 110,
		Divide				= 111,
		F1					= 112,
		F2					= 113,
		F3					= 114,
		F4					= 115,
		F5					= 116,
		F6					= 117,
		F7					= 118,
		F8					= 119,
		F9					= 120,
		F10					= 121,
		F11					= 122,
		F12					= 123,
		F13					= 124,
		F14					= 125,
		F15					= 126,
		F16					= 127,
		F17					= 128,
		F18					= 129,
		F19					= 130,
		F20					= 131,
		F21					= 132,
		F22					= 133,
		F23					= 134,
		F24					= 135,
		NumLock				= 144,
		ScrollLock			= 145,	Scroll				= 145,
		LeftShift			= 160,
		RightShift			= 161,
		LeftControl			= 162,
		RightControl		= 163,
		LeftAlt				= 164,	LeftMenu			= 164,
		RightAlt			= 165,	RightMenu			= 165,
		BrowserBack			= 166,
		BrowserForward		= 167,
		BrowserRefresh		= 168,
		BrowserStop			= 169,
		BrowserSearch		= 170,
		BrowserFavorites	= 171,
		BrowserHome			= 172,
		VolumeMute			= 173,
		VolumeDown			= 174,
		VolumeUp			= 175,
		MediaNextTrack		= 176,
		MediaPreviousTrack	= 177,
		MediaStop			= 178,
		MediaPlayPause		= 179,
		LaunchMail			= 180,
		SelectMedia			= 181,	LaunchMediaSelect	= 181,
		LaunchApplication1	= 182,
		LaunchApplication2	= 183,
		Semicolon			= 186,	OemSemicolon		= 186,
		Plus				= 187,	OemPlus				= 187,
		Comma				= 188,	OemComma			= 188,
		Minus				= 189,	OemMinus			= 189,
		Period				= 190,	OemPeriod			= 190,
		Question			= 191,	OemQuestion			= 191,
		Tilde				= 192,	OemTilde			= 192,
		OpenBracket			= 219,	OemOpenBracket		= 219,
		Pipe				= 220,	OemPipe				= 220,
		CloseBracket		= 221,	OemCloseBracket		= 221,
		Quote				= 222,	OemQuote			= 222,
		Oem8				= 223,	// Miscellaneous
		Backslash			= 226,	OemBackslash		= 226,
		Attn				= 246,
		CrSel				= 247,
		ExSel				= 248,
		EraseEof			= 249,
		Play				= 250,
		Zoom				= 251,
		Pa1					= 253,
		OemClear			= 254

	};	// enum class Key
	
	
	//*****************************************************************//
	// DPad
	//	- enumeration of direction constants for gamepad D-Pad (pov / hat)
	//	- enumerators REQUIRE the enum typename scope: DPad::Neutral
	enum class DPad
	{
		Neutral				= 0,
		Up					= 1,
		Down				= 2,
		Left				= 4,
		Right				= 8,
		UpLeft				= 5,
		UpRight				= 9,
		DownLeft			= 6,
		DownRight			= 10,

	};	// enum class DPad

}	// namespace SGD



#endif //SGD_KEY_H
