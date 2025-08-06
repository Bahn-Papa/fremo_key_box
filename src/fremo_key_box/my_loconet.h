
#pragma once

//##########################################################################
//#
//#		MyLoconetClass
//#
//#	This class will handle all loconet messages and belongings
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	6		from: 06.08.2025
//#
//#	Improvement:
//#		-	improvement of message handling
//#			change in function
//#				LoconetReceived()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	5		from: 02.08.2025
//#
//#	Implementation:
//#		-	add a second LNCV address for the key state
//#			add member variable
//#				m_devKeyState_2
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	4		from: 02.08.2025
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
//#			change in function
//#				LoconetReceived()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	3		from: 28.01.2022
//#
//#	Implementation:
//#		-	add member variables
//#				m_uiKeyStateAddress
//#				m_uiPermissionAddress
//#				m_uiSendDelay
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	4		from: 08.02.2022
//#
//#	Bug Fix:
//#		-	change return type for function 'ClearPermission'
//#			from bool to void
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	3		from: 28.01.2022
//#
//#	Implementation:
//#		-	change the handling of 'Permission Granted'
//#			the state machine must be able to clear the permission,
//#			so add new function ClearPermisson()
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

#include <stdint.h>


//==========================================================================
//
//		T Y P E   D E F I N I T I O N S
//
//==========================================================================

typedef struct
{
	uint16_t	m_uiAddress;
	uint8_t		m_bFlags;

} device_t;


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
		void CheckForMessage( void );
		void LoconetReceived( bool isSensor, uint16_t adr, uint8_t dir );
		void SendKeyRemoved( bool bRemoved );

		inline void SetProgMode( bool bMode )
		{
			m_bIsProgMode = bMode;
		};

		inline bool IsProgMode( void )
		{
			return( m_bIsProgMode );
		};

		inline bool IsPermissionGranted( void )
		{
			return( m_bPermissionGranted );
		};

		inline void ClearPermission( void )
		{
			m_bPermissionGranted = false;
		}

	private:
		device_t	m_devPermission;
		device_t	m_devKeyState;
		device_t	m_devKeyState_2;
		bool		m_bIsProgMode;
		bool		m_bPermissionGranted;
		uint16_t	m_uiSendDelay;

		void SetDeviceInfo( device_t *pDevice, uint16_t uiInfo );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern MyLoconetClass		g_clMyLoconet;


//--------------------------------------------------------------------------
#endif		//	defined( COMMAND_CONNECTION_LOCONET )
