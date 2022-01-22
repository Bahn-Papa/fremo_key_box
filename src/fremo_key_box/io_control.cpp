//##########################################################################
//#
//#		IO_ControlClass
//#
//#-------------------------------------------------------------------------
//#
//#	This class operates the ports of the Atmel chip.
//#	There are functions to
//#		-	check digital inputs
//#		-	set digital outputs
//#		-	position a servo
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
//#	File version: 0.01	vom: 21.01.2022
//#
//#	Implementation:
//#		-	first version
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include "compile_options.h"

#include <Arduino.h>

#include "io_control.h"
#include "debounce.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define	INIT_READ_INPUT_COUNT	6


//---------------------------------------------------------------------
//	Port B


//---------------------------------------------------------------------
//	Port C


//---------------------------------------------------------------------
//	Port D


//---------------------------------------------------------------------
//	Port E


//---------------------------------------------------------------------
//	Port F


//==========================================================================
//
//		K O N S T A N T E
//
//==========================================================================

const uint32_t	cg_ulInterval_20_ms		= 20;
const uint32_t	cg_ulInterval_100_ms	= 100;		//	 0,1 sec
const uint32_t	cg_ulInterval_500_ms	= 500;		//	 0,5 sec
const uint32_t	cg_ulInterval_2_s		= 2000;		//	 2,0 sec


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

IO_ControlClass	g_clControl	= IO_ControlClass();

uint32_t		g_ulMillisFlash;
uint32_t		g_ulMillisReadInputs;
bool			g_bServoInLockPos;


//==========================================================================
//
//		C L A S S   F U N C T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////
//	CLASS: IO_ControlClass
//

//******************************************************************
//	Constructor
//------------------------------------------------------------------
IO_ControlClass::IO_ControlClass()
{
	g_ulMillisFlash			= 0L;
	g_ulMillisReadInputs	= 0L;
	g_bServoInLockPos		= false;
}


//******************************************************************
//	Init
//------------------------------------------------------------------
//	here for all Ports the relevant I/O pins will be configured.
//	Not used I/O's will be set to Input with PullUp.
//	all connected devices (like LED's) will set to 'off'.
//
void IO_ControlClass::Init( void )
{
	//----	set unused pins to input with pullup  ------------------
	//

	//----	Port B  ------------------------------------------------
	//

	//----	Port C  ------------------------------------------------
	//

	//----	Port D  ------------------------------------------------
	//

	//----	Port E  ------------------------------------------------
	//

	//----	Port F  ------------------------------------------------
	//

	//--------------------------------------------------------------
	//	set servo to unlock position
	//	may be the key is not inside of the key box
	//
	SetServoToUnlockPosition();

	//----	start read timer  --------------------------------------
	g_ulMillisReadInputs = millis() + cg_ulInterval_20_ms;

	//----	Read actual Inputs  ------------------------------------
	//
	for( uint8_t idx = 0 ; idx < INIT_READ_INPUT_COUNT ; idx++ )
	{
		delay( 20 );

		ReadInputs();
	}

	delay( 20 );
}


//******************************************************************
//	ReadInputs
//------------------------------------------------------------------
//	This function should be called at regular intervals,
//	for example: every 20 ms
//
void IO_ControlClass::ReadInputs( void )
{
	//----------------------------------------------------------
	//	get the inputs from the ports
	//


	//----------------------------------------------------------
	//	let the LED flash
	//
	if( m_bLedFast || m_bLedSlow )
	{
		if( millis() > g_ulMillisFlash )
		{
			if( m_bLedFast )
			{
				g_ulMillisFlash = cg_ulInterval_100_ms;
			}
			else
			{
				g_ulMillisFlash = cg_ulInterval_500_ms;
			}

			g_ulMillisFlash += millis();

			if( IsLedOn() )
			{
				//----	switch LED off  ----
				g_clControl.LedOff();
			}
			else
			{
				//----	switch LED on  ----
				g_clControl.LedOn();
			}
		}
	}
}


//******************************************************************
//	LedOn
//
void IO_ControlClass::LedOn( void )
{
}


//******************************************************************
//	LedOff
//
void IO_ControlClass::LedOff( void )
{
}


//******************************************************************
//	SetServoToLockPosition
//------------------------------------------------------------------
//
void IO_ControlClass::SetServoToLockPosition( void )
{
	g_bServoInLockPos = true;
}


//******************************************************************
//	SetServoToUnlockPosition
//------------------------------------------------------------------
//
void IO_ControlClass::SetServoToUnlockPosition( void )
{
	g_bServoInLockPos = false;
}


//******************************************************************
//	IsLedOn
//------------------------------------------------------------------
//
bool IO_ControlClass::IsLedOn( void )
{
//	return( 0 == (PINC & leds) );
	return( false );
}


//******************************************************************
//	IsKeyIn
//------------------------------------------------------------------
//
bool IO_ControlClass::IsKeyIn( void )
{
	return( false );
}


//******************************************************************
//	IsButtonPressed
//------------------------------------------------------------------
//
bool IO_ControlClass::IsButtonPressed( void )
{
	return( false );
}


//******************************************************************
//	IsServoInLockPosition
//------------------------------------------------------------------
//
bool IO_ControlClass::IsServoInLockPosition( void )
{
	return( g_bServoInLockPos );
}
