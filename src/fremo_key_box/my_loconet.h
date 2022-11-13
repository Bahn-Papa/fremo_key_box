
#pragma once

//##########################################################################
//#
//#		MyLoconetClass
//#
//#	This class will handle all loconet messages and belongings
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
		void LoconetReceived( uint16_t adr, uint8_t dir );
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
		bool		m_bIsProgMode;
		bool		m_bPermissionGranted;
		uint16_t	m_uiPermissionAddress;
		uint16_t	m_uiKeyStateAddress;
		uint16_t	m_uiSendDelay;
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern MyLoconetClass		g_clMyLoconet;


//--------------------------------------------------------------------------
#endif		//	defined( COMMAND_CONNECTION_LOCONET )
