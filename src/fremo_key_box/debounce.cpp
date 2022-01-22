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

#include "debounce.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define	REPEAT_START	100
#define REPEAT_NEXT		 20


//==========================================================================
//
//		C L A S S   F U N C T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////
//	CLASS: DebounceClass
//

//******************************************************************
//	Constructor
//------------------------------------------------------------------
//
DebounceClass::DebounceClass( uint8_t repeatMask )
{
	m_uiRepeatMask		= repeatMask;

	m_uiKeyState		= 0;
	m_uiKeyPress		= 0;
	m_uiKeyRepeat		= 0;

	m_uiDebounce1		= 0xFF;
	m_uiDebounce2		= 0xFF;
	m_uiRepeatCounter	= 0;
}

//******************************************************************
//	Work
//------------------------------------------------------------------
//	This is where the actual debouncing takes place.
//	This function must therefore be called as quickly and
//	as often as possible.
//
//	Parameter:	PIN value of the port that is to be debounced
//
void DebounceClass::Work( uint8_t keyIn )
{
	uint8_t	help	 =	m_uiKeyState ^ ~keyIn;

	m_uiDebounce1	 = ~(m_uiDebounce1 & help);
	m_uiDebounce2	 =   m_uiDebounce1 ^ (m_uiDebounce2 & help);
	help			&=  (m_uiDebounce1 &  m_uiDebounce2);

	m_uiKeyState	^=  help;
	m_uiKeyPress	|= (m_uiKeyState & help);

	if( 0 == (m_uiKeyState & m_uiRepeatMask) )
	{
		m_uiRepeatCounter = REPEAT_START;
	}

	if( --m_uiRepeatCounter == 0 )
	{
		m_uiRepeatCounter	 = REPEAT_NEXT;
		m_uiKeyRepeat		|= (m_uiKeyState & m_uiRepeatMask);
	}
}


//******************************************************************
//	GetKeyPress
//------------------------------------------------------------------
//	Gibt die Information zurück, ob eine Taste neu gedrückt wurde.
//	Dabei wird jede Taste wird nur EINMAL gemeldet.
//
uint8_t DebounceClass::GetKeyPress( uint8_t key_mask )
{
	key_mask		&=	m_uiKeyPress;
	m_uiKeyPress	^=	key_mask;

	return( key_mask );
}


//******************************************************************
//	GetKeyRepeat
//------------------------------------------------------------------
//	Gibt die Information zurück, ob eine Taste lang genug
//	gedrückt wurde, so dass die REPEAT-Funktion aktiviert wurde.
//	Nach einer Anlaufverzögerung wird die jeweilige Taste in
//	aufeinander folgenden Funktionsaufrufen immer wieder
//	gemeldet, solange sie gedrückt bleibt.
//
uint8_t DebounceClass::GetKeyRepeat( uint8_t key_mask )
{
	key_mask		&=	m_uiKeyRepeat;
	m_uiKeyRepeat	^=	key_mask;

	return( key_mask );
}


//******************************************************************
//	GetKeyState
//------------------------------------------------------------------
//	Gibt die Information zurück, ob eine Taste im Augenblick
//	gedrückt ist.
//
uint8_t DebounceClass::GetKeyState( uint8_t key_mask )
{
	key_mask &= m_uiKeyState;

	return( key_mask );
}


//******************************************************************
//	GetKeyShort
//------------------------------------------------------------------
//	Gibt die Information zurück, ob eine Taste kurz gedrückt wurde.
//
uint8_t DebounceClass::GetKeyShort( uint8_t key_mask )
{
	return( GetKeyPress( ~m_uiKeyState & key_mask ) );
}


//******************************************************************
//	GetKeyLong
//------------------------------------------------------------------
//	Gibt die Information zurück, ob eine Taste lang gedrückt wurde.
//
uint8_t DebounceClass::GetKeyLong( uint8_t key_mask )
{
	return( GetKeyPress( GetKeyRepeat( key_mask ) ) );
}
