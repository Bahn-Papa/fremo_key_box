
#pragma once

//##########################################################################
//#
//#		DebounceClass
//#
//#	This class provides functions that help debouncing digital inputs
//#	connected to the ports of an Atmel.
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	 0.01	vom: 21.01.2022
//#
//#	Implementation:
//#		-	first version
//#			based on the algorithm of Peter Dannegger
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include <stdint.h>


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================


///////////////////////////////////////////////////////////////////////
//	CLASS:	DebounceClass
//
//	This class provides functions that help debouncing digital inputs
//	connected to the ports of an Atmel.
//
class DebounceClass
{
	public:
		DebounceClass( uint8_t repeatMask );

		//--------------------------------------------------------------
		//	This is where the actual debouncing takes place.
		//	This function must therefore be called as quickly
		//	and as often as possible.
		//
		//	Parameter:
		//		keyIn	PIN value of the port that is to be debounced
		//
		void Work( uint8_t keyIn );


		//--------------------------------------------------------------
		//	Returns the information in a bit field whether a key
		//	was pressed.
		//	Each key can only be reported ONCE.
		//
		//	Parameter:
		//		key_mask	Specifies in a bit mask which keys should
		//					be checked
		//
		uint8_t GetKeyPress( uint8_t key_mask );


		//--------------------------------------------------------------
		//	Returns the information in a bit field whether a key
		//	was pressed long enough to activate the REPEAT function.
		//	After a start-up delay, the respective key is reported
		//	again and again in successive function calls as long as
		//	it remains pressed.
		//
		//	Parameter:
		//		key_mask	Specifies in a bit mask which keys should
		//					be checked
		//
		uint8_t GetKeyRepeat( uint8_t key_mask );


		//--------------------------------------------------------------
		//	Returns the information in a bit field whether a key
		//	is currently pressed.
		//
		//	Parameter:
		//		key_mask	Specifies in a bit mask which keys should
		//					be checked
		//
		uint8_t GetKeyState( uint8_t key_mask );


		//--------------------------------------------------------------
		//	Returns the information in a bit field whether a key
		//	was pressed short-time.
		//	Each key can only be reported ONCE.
		//
		//	Parameter:
		//		key_mask	Specifies in a bit mask which keys should
		//					be checked
		//
		uint8_t GetKeyShort( uint8_t key_mask );


		//--------------------------------------------------------------
		//	Returns the information in a bit field whether a key
		//	was pressed long-time.
		//	Each key can only be reported ONCE.
		//
		//	Parameter:
		//		key_mask	Specifies in a bit mask which keys should
		//					be checked
		//
		uint8_t GetKeyLong( uint8_t key_mask );

	private:
		//--------------------------------------------------------------
		//		m_uiRepeatMask
		//
		//	contains the mask for which keys the repeat function
		//	is activated.
		//
		uint8_t	m_uiRepeatMask;

		//--------------------------------------------------------------
		//		m_uiKeyState
		//
		//	bit field containing the debounced current key state
		//	(bit set = key pressed)
		//
		uint8_t m_uiKeyState;

		//--------------------------------------------------------------
		//		m_uiKeyPress
		//
		//	bit field containing all newly pressed keys since the last
		//	call of function GetKeyPress, GetKeyShort or GetKeyLong.
		//	(bit set = key pressed)
		//
		uint8_t m_uiKeyPress;

		//--------------------------------------------------------------
		//		m_uiKeyRepeat
		//	enthält die Info, welche Taste lange gedrückt wurde.
		//
		uint8_t m_uiKeyRepeat;

		//--------------------------------------------------------------
		//		m_uiEntprellung1
		//		m_uiEntprellung2
		//		m_uiRepeatCounter
		//	Hilfsvariable um die Tastenentprellung durchführen
		//	zu können.
		//
		uint8_t m_uiEntprellung1;
		uint8_t m_uiEntprellung2;
		uint8_t m_uiRepeatCounter;
};
