
#pragma once

//##########################################################################
//#
//#		MyLoconetClass
//#
//#	This class will handle all loconet messages and belongings
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	 0.01	vom: 21.01.2022
//#
//#	Implementation:
//#		-	Initial version
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include "compile_options.h"


#if defined( COMMAND_CONNECTION_LOCONET )

#include <stdint.h>


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================


///////////////////////////////////////////////////////////////////////
//	CLASS:	MyLoconetClass
//
//	This class will handle all loconet belongings.
//
class MyLoconetClass
{
	public:
		MyLoconetClass();

		void Init( void );
		void CheckForMessageAndStoreInDataPool( void );
		void LoconetReceived( uint16_t adr, uint8_t dir );
		void SendKeyRemoved( bool bRemoved );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern MyLoconetClass		g_clMyLoconet;


//--------------------------------------------------------------------------
#endif		//	defined( COMMAND_CONNECTION_LOCONET )
