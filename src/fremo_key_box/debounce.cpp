//##########################################################################
//#
//#		DebounceClass
//#
//#	This class provides functions that help debouncing digital inputs
//#	connected to the ports of an Atmel.
//#
//#-------------------------------------------------------------------------
//#
//#	File version: 0.02	vom: 23.01.2022
//#
//#	Implementation:
//#		-	under development
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
//	creates an instance of the class DebounceClass
//
//	Parameter:
//		repeatMask	Specifies in a bit mask for which keys the
//					repeat function will be switched on
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
//	Parameter:
//		keyIn	PIN value of the port that is to be debounced
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
//	Returns the information in a bit field whether a key
//	was pressed.
//	Each key can only be reported ONCE.
//
//	Parameter:
//		key_mask	Specifies in a bit mask which keys should
//					be checked
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
uint8_t DebounceClass::GetKeyRepeat( uint8_t key_mask )
{
	key_mask		&=	m_uiKeyRepeat;
	m_uiKeyRepeat	^=	key_mask;

	return( key_mask );
}


//******************************************************************
//	GetKeyState
//------------------------------------------------------------------
//	Returns the information in a bit field whether a key is
//	currently pressed.
//
//	Parameter:
//		key_mask	Specifies in a bit mask which keys should
//					be checked
//
uint8_t DebounceClass::GetKeyState( uint8_t key_mask )
{
	key_mask &= m_uiKeyState;

	return( key_mask );
}


//******************************************************************
//	GetKeyShort
//------------------------------------------------------------------
//	Returns the information in a bit field whether a key
//	was pressed short-time.
//	Each key can only be reported ONCE.
//
//	Parameter:
//		key_mask	Specifies in a bit mask which keys should
//					be checked
//
uint8_t DebounceClass::GetKeyShort( uint8_t key_mask )
{
	return( GetKeyPress( ~m_uiKeyState & key_mask ) );
}


//******************************************************************
//	GetKeyLong
//------------------------------------------------------------------
//	Returns the information in a bit field whether a key
//	was pressed long-time.
//	Each key can only be reported ONCE.
//
//	Parameter:
//		key_mask	Specifies in a bit mask which keys should
//					be checked
//
uint8_t DebounceClass::GetKeyLong( uint8_t key_mask )
{
	return( GetKeyPress( GetKeyRepeat( key_mask ) ) );
}
