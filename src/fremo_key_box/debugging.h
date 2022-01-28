
#pragma once

//##########################################################################
//#
//#		DebuggingClass
//#
//#	This class will deliver text output to different output devices.
//#	e.g.: OLED display
//#
//#-------------------------------------------------------------------------
//#
//#	File version: 0.03	vom: 28.01.2022
//#
//#	Implementation:
//#		-	add state machine info to function PrintStatus()
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
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include "state_machine.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

typedef enum info_lines
{
	infoLineFields = 1,
	infoLineInit,
	infoLineLedTest
	
}	info_lines_t;


typedef enum notify_type
{
	NT_Sensor = 0,
	NT_Request,
	NT_Report,
	NT_State
	
}	notify_type_t;


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================


///////////////////////////////////////////////////////////////////////
//	CLASS:	DebuggingClass
//
class DebuggingClass
{
	public:
		DebuggingClass();

		void Init( void );

		void PrintTitle( uint8_t versionMain, uint8_t versionMinor );
		void PrintInfoLine( info_lines_t number );

		void PrintNotifyType( notify_type_t type );
		void PrintNotifyMsg( uint16_t address, uint8_t dir );

		void PrintLncvDiscoverStart( bool start, uint16_t artikel, uint16_t address );
		void PrintLncvStop();
		void PrintLncvReadWrite( bool doRead, uint16_t address, uint16_t value );

		void PrintStorageCheck( uint8_t byte1, uint8_t byte2 );
		void PrintStorageDefault( void );
		void PrintStorageRead( void );
		
		void PrintStatus(	box_state_t	state,
							bool		bPermission,
							bool		bKeyIn,
							bool		bButtonPressed,
							bool		bServoLockPos	);

		void PrintText( char *text );
		void PrintCounter( void );

	private:
		notify_type_t	m_NotifyType;
		uint32_t		m_counter;

		void SetLncvMsgPos( void );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern DebuggingClass	g_clDebugging;
