//##########################################################################
//#
//#		MyLoconetClass
//#
//#	This class will handle all loconet messages and belongings
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	5		from: 02.08.2025
//#
//#	Implementation:
//#		-	change handling of loconet addresses for switch and sensor
//#			messages. the info will now consists of an address and code part
//#			add member variables
//#				m_devKeyState
//#				m_devPermission
//#			remove member variables
//#				m_uiKeyStateAddress
//#				m_uiPermissionAddress
//#			add function
//#				SetDeviceInfo()
//#			change in functions
//#				Init()
//#				SendKeyRemoved()
//#				LoconetReceived()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	4		from: 18.11.2023
//#
//#	Implementation:
//#		-	switch to LocoNet library version 1.1.13
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

#define	DIR_RED						0
#define DIR_THROWN					0
#define DIR_GREEN					1
#define DIR_CLOSED					1

#define DEVICE_IS_INVERT			0x01
#define DEVICE_IS_SENSOR			0x02


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

	m_uiSendDelay			= g_clLncvStorage.ReadLNCV( LNCV_ADR_SEND_DELAY );

	SetDeviceInfo( &m_devPermission, g_clLncvStorage.ReadLNCV( LNCV_ADR_KEY_PERMISSION ) );
	SetDeviceInfo( &m_devKeyState,   g_clLncvStorage.ReadLNCV( LNCV_ADR_KEY_STATE      ) );

	LocoNet.init( LOCONET_TX_PIN );
}


//**********************************************************************
//	SetDeviceInfo (private)
//----------------------------------------------------------------------
//
void MyLoconetClass::SetDeviceInfo( device_t *pDevice, uint16_t uiInfo )
{
	uint16_t	address	= uiInfo / 10;

	uiInfo -= (address * 10);

	pDevice->m_uiAddress	= address;
	pDevice->m_bFlags		= (uint8_t)uiInfo;
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
void MyLoconetClass::LoconetReceived( bool isSensor, uint16_t adr, uint8_t dir, uint8_t /* output */ )
{
	bool	bIsGreen	= (dir != DIR_RED);

	//----------------------------------------------------------
	//	first check if this is one of our addresses
	//
	if( adr == m_devPermission.m_uiAddress )
	{
		//------------------------------------------------------
		//	yes it is
		//	then check if we are searching for
		//	a sensor message ('isSensor' == true) or
		//	a switch message ('isSensor' == false)
		//
		if( isSensor == (m_devPermission.m_bFlags & DEVICE_IS_SENSOR) )
		{
			//--------------------------------------------------
			//	we get to this code area only
			//	if 'isSensor' and the expression
			//	'm_devXXX.m_usFlags & DEVICE_IS_SENSOR' are both
			//		true	==>		sensor message
			//		false	==>		switch message
			//--------------------------------------------------
			//	Okay, address and message type are correct
			//	so process the message ...
			//

			//--------------------------------------------------
			//	Check if 'dir' should be inverted
			//
			if( 0 == (m_devPermission.m_bFlags & DEVICE_IS_INVERT) )
			{
				bIsGreen = !bIsGreen;
			}

			if( DIR_RED == bIsGreen )
			{
				m_bPermissionGranted = false;
			}
			else
			{
				m_bPermissionGranted = true;
			}
		}
	}
}


//******************************************************************
//	SendKeyRemoved
//------------------------------------------------------------------
//
void MyLoconetClass::SendKeyRemoved( bool bRemoved )
{
	uint16_t	adr		= m_devKeyState.m_uiAddress;
	uint8_t		dir;

	//---------------------------------------------------------
	//	send the message only if there is an address for it
	//
	if( 0 < adr )
	{
		if( bRemoved )
		{
			dir = DIR_RED;
		}
		else
		{
			dir = DIR_GREEN;
		}

		//-----------------------------------------------------
		//	Check if 'dir' should be inverted
		//
		if( 0 == (m_devKeyState.m_bFlags & DEVICE_IS_INVERT) )
		{
			dir = !dir;
		}

		//-----------------------------------------------------
		//	Check if this should be a sensor
		//	or a switch message
		//
		if( m_devKeyState.m_bFlags & DEVICE_IS_SENSOR )
		{
			//----	sensor message  ------------------------------------
			//
			LocoNet.reportSensor( adr, dir );

#ifdef DEBUGGING_PRINTOUT
//			g_clDebugging.PrintReportSensorMsg( adr, dir );
#endif
		}
		else
		{
			//----	switch message  ---------------------------
			//
			LocoNet.requestSwitch( adr, 1, dir );

#ifdef DEBUGGING_PRINTOUT
//			g_clDebugging.PrintReportSwitchMsg( adr, dir );
#endif

			//----	wait befor sending the next message  ------
			//
			delay( m_uiSendDelay );

			LocoNet.requestSwitch( adr, 0, dir );
		}
	}
}


//==========================================================================
//
//		L O C O N E T   C A L L B A C K   F U N C T I O N S
//
//==========================================================================


//**********************************************************************
//	notifySensor
//----------------------------------------------------------------------
//
void notifySensor( uint16_t Address, uint8_t State )
{
	g_clMyLoconet.LoconetReceived( true, Address, State, 0 );
}


//**********************************************************************
//	notifySwitchRequest
//----------------------------------------------------------------------
//
void notifySwitchRequest( uint16_t Address, uint8_t Output, uint8_t Direction )
{
	g_clMyLoconet.LoconetReceived( false, Address, Direction, Output );
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

//	g_clMyLoconet.SetProgMode( true );

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
			//------------------------------------------------------
			//	broadcast, so give Module Address back,
			//	but don't go into programming mode !!
			//
//			g_clMyLoconet.SetProgMode( true );

			ModuleAddress	= g_uiModuleAddress;
			retval			= LNCV_LACK_OK;
		}
		else if( ModuleAddress == g_uiModuleAddress )
		{
			//-----------------------------------------------------
			//	valid article number and valid module address,
			//	so switch to programming mode
			//
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
			//------------------------------------------------------
			//	valid article number and valid module address,
			//	so switch off programming mode
			//
			g_clMyLoconet.SetProgMode( false );
		}
	}
}


//**********************************************************************
//	notifyLNCVread
//----------------------------------------------------------------------
//
int8_t notifyLNCVread( uint16_t ArtNr, uint16_t Address, uint16_t &Value )
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
			if(		(LNCV_ADR_VERSION_NUMBER != Address)
				&&	(LNCV_ADR_ARTIKEL_NUMMER != Address) )
			{
				g_clLncvStorage.WriteLNCV( Address, Value );

				if( LNCV_ADR_MODULE_ADDRESS == Address )
				{
					g_uiModuleAddress = Value;
				}
			}

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
