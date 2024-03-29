//##########################################################################
//#
//#		MyLoconetClass
//#
//#	This class will handle all loconet messages and belongings
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	3		from: 13.11.2022
//#
//#	Implementation:
//#		-	add member variables
//#				m_uiKeyStateAddress
//#				m_uiPermissionAddress
//#				m_uiSendDelay
//#		-	add handling of new member variables
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	2		from: 23.01.2022
//#
//#	Implementation:
//#		-	under development
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	1		from: 21.01.2022
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

#include <Arduino.h>
#include <LocoNet.h>


#ifdef DEBUGGING_PRINTOUT
#include "debugging.h"
#endif

#include "my_loconet.h"
#include "lncv_storage.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define LOCONET_TX_PIN			7

#define SWITCH_TO_RED			0
#define SWITCH_TO_GREEN			1


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

MyLoconetClass	 g_clMyLoconet		= MyLoconetClass();

LocoNetCVClass	 g_clLNCV;
lnMsg			*g_pLnPacket;

uint16_t	g_uiArticleNumber;
uint16_t	g_uiModuleAddress;


//==========================================================================
//
//		C L A S S   F U N C T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////////
//	CLASS: MyLoconetClass
//

//******************************************************************
//	Constructor
//------------------------------------------------------------------
//
MyLoconetClass::MyLoconetClass()
{
}


//******************************************************************
//	Init
//------------------------------------------------------------------
//
void MyLoconetClass::Init( void )
{
	m_bIsProgMode			= false;
	m_bPermissionGranted	= false;

	g_uiArticleNumber		= g_clLncvStorage.ReadLNCV( LNCV_ADR_ARTIKEL_NUMMER );
	g_uiModuleAddress		= g_clLncvStorage.ReadLNCV( LNCV_ADR_MODULE_ADDRESS );

	m_uiPermissionAddress	= g_clLncvStorage.ReadLNCV( LNCV_ADR_KEY_PERMISSION );
	m_uiKeyStateAddress		= g_clLncvStorage.ReadLNCV( LNCV_ADR_KEY_STATE );
	m_uiSendDelay			= g_clLncvStorage.ReadLNCV( LNCV_ADR_SEND_DELAY );

	LocoNet.init( LOCONET_TX_PIN );
}


//******************************************************************
//	CheckForAndHandleMessage
//------------------------------------------------------------------
//	This function will check if a new loconet message is available
//	and if so will call the desired function to process the message.
//
void MyLoconetClass::CheckForMessage( void )
{
	g_pLnPacket = LocoNet.receive();

	if( g_pLnPacket )
	{
		if( !LocoNet.processSwitchSensorMessage( g_pLnPacket ) )
		{
			g_clLNCV.processLNCVMessage( g_pLnPacket );
		}
	}
}


//******************************************************************
//	LoconetReceived
//------------------------------------------------------------------
//	This function checks if the received message is for 'us'.
//	This is done by checking whether the address of the message
//	matches the stored address.
//
void MyLoconetClass::LoconetReceived( uint16_t adr, uint8_t dir )
{
	if( m_uiPermissionAddress == adr )
	{
		if( SWITCH_TO_RED == dir )
		{
			m_bPermissionGranted = false;
		}
		else
		{
			m_bPermissionGranted = true;
		}
	}
}


//******************************************************************
//	SendKeyRemoved
//------------------------------------------------------------------
//
void MyLoconetClass::SendKeyRemoved( bool bRemoved )
{
	uint16_t	adr		= m_uiKeyStateAddress;
	uint8_t		dir;

	//---------------------------------------------------------
	//	send the message only if there is an address for it
	//
	if( 0 < adr )
	{
		if( bRemoved )
		{
			dir = SWITCH_TO_RED;
		}
		else
		{
			dir = SWITCH_TO_GREEN;
		}

		LocoNet.requestSwitch( adr, 1, dir );

#ifdef DEBUGGING_PRINTOUT
//		g_clDebugging.PrintReportSwitchMsg( adr, dir );
#endif

		//----	wait befor sending the next message  ------
		//
		delay( m_uiSendDelay );

		LocoNet.requestSwitch( adr, 0, dir );
	}
}


//==========================================================================
//
//		L O C O N E T   C A L L B A C K   F U N C T I O N S
//
//==========================================================================


//**********************************************************************
//
void notifySwitchRequest( uint16_t Address, uint8_t /* Output */, uint8_t Direction )
{
#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintNotifyType( NT_Request );
#endif

	g_clMyLoconet.LoconetReceived( Address, Direction );
}


//**********************************************************************
//
void notifySwitchReport( uint16_t Address, uint8_t /* Output */, uint8_t Direction )
{
#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintNotifyType( NT_Report );
#endif

	g_clMyLoconet.LoconetReceived( Address, Direction );
}


//**********************************************************************
//
void notifySwitchState( uint16_t Address, uint8_t /* Output */, uint8_t Direction )
{
#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintNotifyType( NT_State );
#endif

	g_clMyLoconet.LoconetReceived( Address, Direction );
}


//**********************************************************************
//	notifyLNCVdiscover
//----------------------------------------------------------------------
//	we received a broadcast message, so give back article number
//	and module address.
//
int8_t notifyLNCVdiscover( uint16_t &ArtNr, uint16_t &ModuleAddress )
{
	ArtNr			 = g_uiArticleNumber;
	ModuleAddress	 = g_uiModuleAddress;

	g_clMyLoconet.SetProgMode( true );

#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintLncvDiscoverStart( false, ArtNr, ModuleAddress  );
#endif

	return( LNCV_LACK_OK );
}


//**********************************************************************
//	notifyLNCVprogrammingStart
//----------------------------------------------------------------------
//
int8_t notifyLNCVprogrammingStart( uint16_t &ArtNr, uint16_t &ModuleAddress )
{
	int8_t retval = -1;		//	default: ignore request
	
	if( g_uiArticleNumber == ArtNr )
	{
		if( 0xFFFF == ModuleAddress )
		{
			//----	broadcast, so give Module Address back  --------
			g_clMyLoconet.SetProgMode( true );

			ModuleAddress	= g_uiModuleAddress;
			retval			= LNCV_LACK_OK;
		}
		else if( ModuleAddress == g_uiModuleAddress )
		{
			//----  that's for me, so process it  ------------------
			g_clMyLoconet.SetProgMode( true );

			retval	= LNCV_LACK_OK;
		}
	}

#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintLncvDiscoverStart( true, ArtNr, ModuleAddress  );
#endif

	return( retval );
}


//**********************************************************************
//	notifyLNCVprogrammingStop
//----------------------------------------------------------------------
//
void notifyLNCVprogrammingStop( uint16_t ArtNr, uint16_t ModuleAddress )
{
#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintLncvStop();
#endif

	if( g_clMyLoconet.IsProgMode() )
	{
		if( 	(ArtNr			== g_uiArticleNumber)
			&&	(ModuleAddress	== g_uiModuleAddress) )
		{
			//----	for me, so switch prog mode off  ---------------
			g_clMyLoconet.SetProgMode( false );
		}
	}
}


//**********************************************************************
//	notifyLNCVread
//----------------------------------------------------------------------
//
int8_t notifyLNCVread( uint16_t ArtNr, uint16_t Address, uint16_t, uint16_t &Value )
{
	int8_t retval = -1;		//	default: ignore request

	if( g_clMyLoconet.IsProgMode() && (ArtNr == g_uiArticleNumber) )
	{
		if( g_clLncvStorage.IsValidLNCVAddress( Address ) )
		{
			Value	= g_clLncvStorage.ReadLNCV( Address );
			retval	= LNCV_LACK_OK;
		}
		else
		{
			retval = LNCV_LACK_ERROR_UNSUPPORTED;
		}
	}

#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintLncvReadWrite( true, Address, Value );
#endif

	return( retval );
}


//**********************************************************************
//	notifyLNCVwrite
//----------------------------------------------------------------------
//
int8_t notifyLNCVwrite( uint16_t ArtNr, uint16_t Address, uint16_t Value )
{
	int8_t retval = -1;		//	default: ignore request

	if( g_clMyLoconet.IsProgMode() && (ArtNr == g_uiArticleNumber) )
	{
		if( g_clLncvStorage.IsValidLNCVAddress( Address ) )
		{
			g_clLncvStorage.WriteLNCV( Address, Value );
			retval = LNCV_LACK_OK;
		}
		else
		{
			retval = LNCV_LACK_ERROR_UNSUPPORTED;
		}
	}

#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintLncvReadWrite( false, Address, Value );
#endif

	return( retval );
}


//--------------------------------------------------------------------------
#endif		//	defined( COMMAND_CONNECTION_LOCONET )
