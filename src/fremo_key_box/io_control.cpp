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
//#	File version: 0.05	vom: 28.01.2022
//#
//#	Implementation:
//#		-	add function IsPermissionGranted()
//#			this function is used if no LN is available (debug)
//#
//#-------------------------------------------------------------------------
//#
//#	File version: 0.04	vom: 28.01.2022
//#
//#	Implementation:
//#		-	correct the settings for timer 3 to control the servo
//#		-	correct switching of led
//#
//#-------------------------------------------------------------------------
//#
//#	File version: 0.03	vom: 27.01.2022
//#
//#	Implementation:
//#		-	add I/O handling
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
//	Servo
//
#define SERVO_LOCK_POS		3999	//	2 ms pulse
#define SERVO_UNLOCK_POS	1999	//	1 ms pulse


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
//		PF0		Button Release
//		PF1		Switch Lock Position (true if key is in)
//		PF2		N/A
//		PF3		N/A
//		PF4		Permission granted (if no LN available)
//		PF5		not used
//		PF6		Permission LED
//		PF7		not used
//
#define BUTTON_RELEASE				PF0
#define	SWITCH_LOCK_POS				PF1
#define DEBUG_PERMISSION_GRANTED	PF4
#define	LED_PERMISSION				PF6

#define PORT_F_FREE_BITS	(_BV( PF5 ) | _BV( PF7 ))
#define PORT_F_OUTPUTS		 _BV( LED_PERMISSION )
#define PORT_F_INPUTS		(_BV( PF0 ) | _BV( PF1 ) | _BV( PF4 ))


//==========================================================================
//
//		M A C R O S
//
//==========================================================================

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


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

DebounceClass	g_clPortF( 0x00 );


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
	cli();

	//----	set unused pins to input with pullup  ------------------
	//
	DDRF	&= ~PORT_F_FREE_BITS;
	PORTF	|=  PORT_F_FREE_BITS;

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
	DDRF	&= ~PORT_F_INPUTS;		//	configure as Input
	PORTF	|=  PORT_F_INPUTS;		//	Pull-Up on

	DDRF	|=  PORT_F_OUTPUTS;		//	configure as Output
	PORTF	&= ~PORT_F_OUTPUTS;		//	switch off

	//--------------------------------------------------------------
	//	set timer 3 for servo control
	//
	//	Fast PWM mode, Top defined by ICR3
	//	Compare Match on OCR3A
	//
	//	F CPU		= 16 MHz
	//	Precaler	= 8
	//
	//	16 MHz / 8 = 2 MHz entspricht 0,5 µsec
	//	0,5 µsec x 40000 = 20 ms
	//	0,5 µsec x  2000 =  1 ms
	//
	TCCR3B &= ~(_BV( CS32 ) | _BV( CS31 ) | _BV( CS30 ));	//	Timer 3 stoppen

	TCCR3A |=  (_BV( COM3A1 ) | _BV( WGM31 ));
	TCCR3A &= ~(_BV( COM3A0 ) | _BV( WGM30 ));

	TCCR3B |=  (_BV( WGM33 )  | _BV( WGM32 ));

	ICR3	= 39999;	//	20 ms cycle time

	//--------------------------------------------------------------
	//	set servo to unlock position
	//	may be the key is not inside of the key box
	//
	OCR3A   = SERVO_UNLOCK_POS;		//	Servo to unlock position
	sbi( DDRC, PC6 );				//	set pin to output

	sei();

	sbi( TCCR3B, CS31 );			//	Timer starten (set prescaler to 8)

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
	g_clPortF.Work( PINF );


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
	sbi( PORTF, LED_PERMISSION );
}


//******************************************************************
//	LedOff
//
void IO_ControlClass::LedOff( void )
{
	cbi( PORTF, LED_PERMISSION );
}


//******************************************************************
//	SetServoToLockPosition
//------------------------------------------------------------------
//
void IO_ControlClass::SetServoToLockPosition( void )
{
	OCR3A = SERVO_LOCK_POS;

	g_bServoInLockPos = true;
}


//******************************************************************
//	SetServoToUnlockPosition
//------------------------------------------------------------------
//
void IO_ControlClass::SetServoToUnlockPosition( void )
{
	OCR3A = SERVO_UNLOCK_POS;

	g_bServoInLockPos = false;
}


//******************************************************************
//	IsLedOn
//------------------------------------------------------------------
//
bool IO_ControlClass::IsLedOn( void )
{
	return( bit_is_set( PINF, LED_PERMISSION ) );
//	return( false );
}


//******************************************************************
//	IsKeyIn
//------------------------------------------------------------------
//
bool IO_ControlClass::IsKeyIn( void )
{
	return( 0 != g_clPortF.GetKeyState( _BV( SWITCH_LOCK_POS ) ) );
//	return( false );
}


//******************************************************************
//	IsButtonPressed
//------------------------------------------------------------------
//
bool IO_ControlClass::IsButtonPressed( void )
{
	return( 0 != g_clPortF.GetKeyState( _BV( BUTTON_RELEASE ) ) );
//	return( false );
}


//******************************************************************
//	IsServoInLockPosition
//------------------------------------------------------------------
//
bool IO_ControlClass::IsServoInLockPosition( void )
{
	return( g_bServoInLockPos );
}


//******************************************************************
//	IsPermissionGranted
//------------------------------------------------------------------
//
bool IO_ControlClass::IsPermissionGranted( void )
{
	return( 0 != g_clPortF.GetKeyState( _BV( DEBUG_PERMISSION_GRANTED ) ) );
//	return( false );
}
