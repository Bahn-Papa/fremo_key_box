//##########################################################################
//#
//#		DebuggingClass
//#
//#	Diese Klasse liefert Textausgaben an verschiedene Ausgabegeräte,
//#	z.B.: Serielle Schnittstelle oder OLED Display.
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
//#		-	OLED Display
//#			Das Display ist wie folgt aufgeteilt:
//#			 S                     1 1 1 1 1 1
//#			Z  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
//#			0    K e y   B o x   V x . x x
//#			1  A c t :        <state>
//#			2  K e y :         r e l     O U T
//#						 n o   r e l     I N
//#			3  B t n :   r e l e a s e d
//#						 p r e s s e d
//#			4  S r v :       l o c k   p o s
//#						 u n l o c k   p o s
//#			5    <loconet message type>
//#			6  <loconet message parameter>
//#			7
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include "compile_options.h"

#include "state_machine.h"


#ifdef DEBUGGING_PRINTOUT
//**************************************************************************
//**************************************************************************


#include <avr/pgmspace.h>
#include <Wire.h>
#include <simple_oled_sh1106.h>

#include "debugging.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

//----------------------------------------------------------------------
//	definition of display positions
//
#define STATE_LINE				1
#define STATE_COLUMN			5
#define KEY_LINE				2
#define KEY_PERMISSION_COLUMN	5
#define KEY_POSITION_COLUMN		13
#define BUTTON_LINE				3
#define BUTTON_STATE_COLUMN		5
#define SERVO_LINE				4
#define SERVO_STATE_COLUMN		5
#define MESSAGE_LINE			7


#ifdef COMMAND_CONNECTION_LOCONET

	#define	LOCONET_MSG_LINE		5
	#define LOCONET_MSG_COLUMN		0
	

	//------------------------------------------------------------------
	//	definition of print options
	//
//	#define COUNT_ALL_MESSAGES
//	#define COUNT_MY_MESSAGES

#endif		//	COMMAND_CONNECTION_LOCONET


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

DebuggingClass	g_clDebugging	= DebuggingClass();

const char g_strTitle[] PROGMEM = "  KeyBox V%d.%02d  ";

//----------------------------------------------------------------------
//	Variable für das OLED Display
//
char		g_chDebugString[ 18 ];


#if defined( COUNT_ALL_MESSAGES ) || defined( COUNT_MY_MESSAGES )

uint32_t	g_ulSensorMsgCounter	= 0L;


//==========================================================================
//
//		G L O B A L   F U N C T I O N S
//
//==========================================================================

//******************************************************************
//	PrintMsgCount
//------------------------------------------------------------------
//
void PrintMsgCount( void )
{
	g_clDisplay.ClearLine( MESSAGE_LINE );
	sprintf( g_chDebugString, "Switch: %8lu", g_ulSwitchMsgCounter );
	g_clDisplay.Print( g_chDebugString );
}

#endif


//==========================================================================
//
//		C L A S S   F U N C T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////
//	CLASS: DebuggingClass
//

//******************************************************************
//	Constructor
//------------------------------------------------------------------
//
DebuggingClass::DebuggingClass()
{
	m_counter = 0;
}


//******************************************************************
//	Init
//------------------------------------------------------------------
//
void DebuggingClass::Init( void )
{
	g_clDisplay.Init();
	g_clDisplay.Flip( true );
}


//******************************************************************
//	PrintTitle
//------------------------------------------------------------------
//
void DebuggingClass::PrintTitle( uint8_t versionMain, uint8_t versionMinor )
{
	g_clDisplay.Clear();
	g_clDisplay.SetInverseFont( true );
	sprintf( g_chDebugString, "  KeyBox V%u.%02u  ", versionMain, versionMinor );
	g_clDisplay.Print( g_chDebugString );
	g_clDisplay.SetInverseFont( false );
}


//******************************************************************
//	PrintInfoLine
//------------------------------------------------------------------
//
void DebuggingClass::PrintInfoLine( info_lines_t number )
{
	switch( number )
	{
		case infoLineFields:
			g_clDisplay.Print( "\nAct: State Boot\n" );
			g_clDisplay.Print( "Key: no rel\n" );
			g_clDisplay.Print( "Btn: released\n" );
			g_clDisplay.Print( "Srv: unlock pos" );
			break;

		case infoLineInit:
			g_clDisplay.Print( "\n  Init:\n" );
			break;

		case infoLineLedTest:
			g_clDisplay.Print( "  LED Test\n" );
			break;

		default:
			break;
	}
}


//******************************************************************
//	PrintText
//------------------------------------------------------------------
//
void DebuggingClass::PrintText( char *text )
{
	g_clDisplay.ClearLine( MESSAGE_LINE );
	g_clDisplay.Print( text );
}


//******************************************************************
//	PrintCounter
//------------------------------------------------------------------
//
void DebuggingClass::PrintCounter( void )
{
	m_counter++;
	
	g_clDisplay.ClearLine( MESSAGE_LINE );
	sprintf( g_chDebugString, "Counter: %lu", m_counter );
	g_clDisplay.Print( g_chDebugString );
}


//******************************************************************
//	PrintNotifyType
//------------------------------------------------------------------
//
void DebuggingClass::PrintNotifyType( notify_type_t type )
{
	m_NotifyType = type;

#ifdef COUNT_ALL_MESSAGES

	g_ulSwitchMsgCounter++;

	PrintMsgCount();

#endif
}


//******************************************************************
//	PrintNotifyMsg
//------------------------------------------------------------------
//
void DebuggingClass::PrintNotifyMsg( uint16_t address, uint8_t dir )
{
	SetLncvMsgPos();

	switch( m_NotifyType )
	{
		case NT_Sensor:
			g_clDisplay.Print( "E:Sensor\n" );
			break;

		case NT_Request:
			g_clDisplay.Print( "E:Switch Reqst\n" );
			break;

		case NT_Report:
			g_clDisplay.Print( "E:Switch Report\n" );
			break;

		case NT_State:
			g_clDisplay.Print( "E:Switch State\n" );
			break;
	}

	sprintf( g_chDebugString, "AR:%5u - ", address );
	g_clDisplay.Print( g_chDebugString );
	g_clDisplay.Print( (dir ? "green" : "red") );

#ifdef COUNT_MY_MESSAGES

	g_ulSwitchMsgCounter++;

	PrintMsgCount();

#endif
}


//******************************************************************
//	PrintLncvDiscoverStart
//
void DebuggingClass::PrintLncvDiscoverStart( bool start, uint16_t artikel, uint16_t address )
{
	SetLncvMsgPos();

	if( start )
	{
		g_clDisplay.Print( "LNCV Prog Start\n" );
	}
	else
	{
		g_clDisplay.Print( "LNCV Discover\n" );
	}
	
	sprintf( g_chDebugString, "AR%5u AD%5u", artikel, address );
	g_clDisplay.Print( g_chDebugString );
}


//******************************************************************
//	PrintLncvStop
//
void DebuggingClass::PrintLncvStop()
{
	SetLncvMsgPos();
	g_clDisplay.Print( "LNCV Prog Stop" );
//	sprintf( g_chDebugString, "AR%5u AD%5u", ArtNr, ModuleAddress );
//	g_clDisplay.Print( g_chDebugString );
}


//******************************************************************
//	PrintLncvReadWrite
//
void DebuggingClass::PrintLncvReadWrite( bool doRead, uint16_t address, uint16_t value )
{
	SetLncvMsgPos();

	if( doRead )
	{
		g_clDisplay.Print( "LNCV Read\n" );
	}
	else
	{
		g_clDisplay.Print( "LNCV Write\n" );
	}

	sprintf( g_chDebugString, "AD%5u VA%5u", address, value );
	g_clDisplay.Print( g_chDebugString );
}


//******************************************************************
//	SetLncvMsgPos
//
void DebuggingClass::SetLncvMsgPos( void )
{
	g_clDisplay.ClearLine( LOCONET_MSG_LINE + 1 );
	g_clDisplay.ClearLine( LOCONET_MSG_LINE );
}


//******************************************************************
//	PrintStorageCheck
//
void DebuggingClass::PrintStorageCheck( uint8_t byte1, uint8_t byte2 )
{
	g_clDisplay.Print( "  Check EEPROM:\n  0:" );
	sprintf( g_chDebugString, "  0:0x%02X 1:0x%02X", byte1, byte2 );
	g_clDisplay.Print( g_chDebugString );
}


//******************************************************************
//	PrintStorageDefault
//
void DebuggingClass::PrintStorageDefault( void )
{
	g_clDisplay.Print( "\nSet default Adr" );
}


//******************************************************************
//	PrintStorageRead
//
void DebuggingClass::PrintStorageRead( void )
{
	g_clDisplay.Print( "\n  Lese LNCVs\n" );
}


//******************************************************************
//	PrintStatus
//
void DebuggingClass::PrintStatus(	box_state_t	state,
									bool		bPermission,
									bool		bKeyIn,
									bool		bButtonPressed,
									bool		bServoLockPos	)
{
	g_clDisplay.SetCursor( STATE_LINE, STATE_COLUMN );
	
	switch( state )
	{
		case STATE_PERMISSION_GRANTED:
			g_clDisplay.Print( "Permission" );
			break;

		case STATE_KEY_OUT_BOOT:
			g_clDisplay.Print( "Out Boot  " );
			break;

		case STATE_KEY_OUT:
			g_clDisplay.Print( "Key Out   " );
			break;

		case STATE_KEY_LOCKED_PRE:
			g_clDisplay.Print( "Locked Pre" );
			break;

		case STATE_KEY_LOCKED:
			g_clDisplay.Print( "Key Locked" );
			break;

		case STATE_KEY_UNLOCKED:
			g_clDisplay.Print( "Key Unlock" );
			break;
	}

	g_clDisplay.SetCursor( KEY_LINE, KEY_PERMISSION_COLUMN );
	g_clDisplay.Print( bPermission ? "  " : "no" );
	g_clDisplay.SetCursor( KEY_LINE, KEY_POSITION_COLUMN );
	g_clDisplay.Print( bKeyIn ? "IN " : "OUT" );
	g_clDisplay.SetCursor( BUTTON_LINE, BUTTON_STATE_COLUMN );
	g_clDisplay.Print( bButtonPressed ? "pressed " : "released" );
	g_clDisplay.SetCursor( SERVO_LINE, SERVO_STATE_COLUMN );
	g_clDisplay.Print( bServoLockPos ? "  " : "un");
}


//**************************************************************************
//**************************************************************************
#endif	//	DEBUGGING_PRINTOUT
