//##########################################################################
//#
//#	fremo_key_box		copyright: Michael Pfeil
//#
//#	This program controls the hardware fremo_key_box.
//#
//##########################################################################


#define VERSION_MAIN	0
#define	VERSION_MINOR	4


//##########################################################################
//#
//#		Version History:
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

bool	g_bIsProgMode;


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
	g_clDebugging.Init();
#endif

	g_clControl.Init();
	g_clMyLoconet.Init();

	//----	some setup tests  --------------------------------------
#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintTitle( VERSION_MAIN, VERSION_MINOR );
	g_clDebugging.PrintInfoLine( infoLineInit );
#endif

	delay( 200 );

	//----	LNCV: Check and Init  ----------------------------------
	g_clLncvStorage.CheckEEPROM();
	
	delay( 500 );

	g_clLncvStorage.Init();

	delay( 200 );

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
	//==================================================================
	//	Read Inputs
	//	-	Loconet messages
	//	-	Input signals
	//
	g_clMyLoconet.CheckForMessage();
	g_clControl.ReadInputs();


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


	//==================================================================
	//	Die State-Maschinen abarbeiten
	//
	g_clStateMachine.CheckState();


	//==================================================================
	//	print actual status
	//
#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintStatus(	g_clMyLoconet.IsPermissionGranted(),
								g_clControl.IsKeyIn(),
								g_clControl.IsButtonPressed(),
								g_clControl.IsServoInLockPosition() );
#endif
}
