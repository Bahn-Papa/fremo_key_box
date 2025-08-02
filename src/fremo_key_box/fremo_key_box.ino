//##########################################################################
//#
//#	fremo_key_box		copyright: Michael Pfeil
//#
//#	This program controls the hardware fremo_key_box.
//#
//#-------------------------------------------------------------------------
//#
//# board:		Leonardo
//#	processor:	ATmega32U4, 16 MHz
//#
//#-------------------------------------------------------------------------
//#
//#	Needed resources:
//#		LocoNet Library		V 1.1.13
//#		SimpleOled Library	V 1.2.1
//#
//##########################################################################


#define VERSION_MAIN	1
#define	VERSION_MINOR	6

#define VERSION_NUMBER		((VERSION_MAIN * 100) + VERSION_MINOR)


//##########################################################################
//#
//#		Version History:
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	1.6		from: 02.08.2025
//#
//#	Implementation:
//#		lncv_storage
//#		my_loconet
//#			add a second LNCV address for the key state
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	1.5		from: 02.08.2025
//#
//#	Implementation:
//#		my_loconet
//#		-	change address handling as in other SW like ln2block
//#
//#		state_machine
//#		-	if key is out and button is released then switch off LED
//#
//#		io_control
//#		-	read servo position at init
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	1.4		from: 18.11.2023
//#
//#	Implementation:
//#		-	switch to LocoNet library version 1.1.13
//#		-	switch to SimpleOled library version 1.2.1
//#
//#-------------------------------------------------------------------------
//#
//#	Version:	1.3		vom: 13.11.2022
//#
//#	Implementation:
//#		-	streamlining lncv_storage
//#
//#-------------------------------------------------------------------------
//#
//#	Version: 1.2	vom: 13.11.2022
//#
//#	Implementation:
//#		-	red led integrated (LED_ALERT)
//#
//#-------------------------------------------------------------------------
//#
//#	Version: 1.1	vom: 08.02.2022
//#
//#	Implementation:
//#		-	add configurable values for servo lock and unlock position
//#			use these variables to position the servo
//#
//#-------------------------------------------------------------------------
//#
//#	Version: 1.0	vom: 28.01.2022
//#
//#	Implementation:
//#		-	finished the sequence of the state machine
//#			this leads in the first deliverable version of the program
//#
//#-------------------------------------------------------------------------
//#
//#	Version: 0.04	vom: 28.01.2022
//#
//#	Implementation:
//#		-	correct the settings for timer 3 to control the servo
//#		-	correct switching of led
//#
//#-------------------------------------------------------------------------
//#
//#	Version: 0.03	vom: 27.01.2022
//#
//#	Implementation:
//#		-	add I/O handling
//#
//#-------------------------------------------------------------------------
//#
//#	Version: 0.02	vom: 23.01.2022
//#
//#	Implementation:
//#		-	under development
//#
//#-------------------------------------------------------------------------
//#
//#	Version: 0.01	vom: 21.01.2022
//#
//#	Implementation:
//#		-	first working version
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include "compile_options.h"

#ifdef DEBUGGING_PRINTOUT
#include "debugging.h"
#endif

#include "io_control.h"
#include "lncv_storage.h"
#include "my_loconet.h"
#include "state_machine.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

bool		g_bIsProgMode;


#ifdef DEBUGGING_PRINTOUT
uint32_t	g_ulPrintStatusTimer;
#endif


//==========================================================================
//
//		G L O B A L   F U N C T I O N S
//
//==========================================================================

//**********************************************************************
//	resetFunc
//----------------------------------------------------------------------
//	This function will reset the hole processor because by calling the
//	function the program will jump to the hardware reset vector.
//
void (*resetFunc)( void ) = 0;


//**********************************************************************
//	setup
//----------------------------------------------------------------------
//
void setup()
{
	g_bIsProgMode = false;

#ifdef DEBUGGING_PRINTOUT
	g_ulPrintStatusTimer	= 0L;

	g_clDebugging.Init();

	delay( 100 );

	g_clDebugging.PrintTitle( VERSION_MAIN, VERSION_MINOR );
	g_clDebugging.PrintInfoLine( infoLineInit );
#endif

	//----	LNCV: Check and Init  ----------------------------------
	g_clLncvStorage.CheckEEPROM( VERSION_NUMBER );
	
	delay( 500 );

	g_clLncvStorage.Init();

	delay( 500 );

	//----	I/O control  -------------------------------------------
	g_clControl.Init();

	delay( 100 );

	//----	loconet  -----------------------------------------------
	g_clMyLoconet.Init();

	delay( 100 );

	//----	some setup tests  --------------------------------------

	delay( 1000 );

	//----	Prepare Display  ---------------------------------------
#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintTitle( VERSION_MAIN, VERSION_MINOR );
	g_clDebugging.PrintInfoLine( infoLineFields );
#endif
}


//**********************************************************************
//	loop
//
void loop()
{
	box_state_t	theState;


	//==================================================================
	//	Read Inputs
	//	-	Input signals
	//	-	Loconet messages
	//
	g_clControl.ReadInputs();

#if defined( COMMAND_CONNECTION_LOCONET )
	g_clMyLoconet.CheckForMessage();


	//==================================================================
	//	Auswertung der Daten und logische Verknüpfungen abarbeiten.
	//

	//--------------------------------------------------------------
	//	Programmier-Modus
	//
	if( g_clMyLoconet.IsProgMode() != g_bIsProgMode )
	{
		if( g_bIsProgMode )
		{
			resetFunc();
		}
		else
		{
			g_bIsProgMode = true;

			g_clControl.LedSlow();
		}
	}
#endif


	//==================================================================
	//	Die State-Maschinen abarbeiten
	//
	theState = g_clStateMachine.CheckState();


	//==================================================================
	//	print actual status
	//
#ifdef DEBUGGING_PRINTOUT
	if( millis() > g_ulPrintStatusTimer )
	{
		g_clDebugging.PrintStatus(	theState,
										g_clControl.IsPermissionGranted()
									||	g_clMyLoconet.IsPermissionGranted(),
									g_clControl.IsKeyIn(),
									g_clControl.IsButtonPressed(),
									g_clControl.IsServoInLockPosition() );
									
		g_ulPrintStatusTimer = millis() + cg_ulInterval_500_ms;
	}
#endif
}
