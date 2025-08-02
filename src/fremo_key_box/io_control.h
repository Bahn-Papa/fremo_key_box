
#pragma once

//##########################################################################
//#
//#		IO_ControlClass
//#
//#	This class operates the ports of the Atmel chip.
//#	There are functions to
//#		-	check digital inputs
//#		-	set digital outputs
//#		-	position a servo
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	5		vom: 02.08.2025
//#
//#	Implementation:
//#		-	read servo position at init
//#			add new member variables
//#				m_uiLockPos
//#				m_uiUnlockPos
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	4		vom: 28.01.2022
//#
//#	Implementation:
//#		-	change the handling of 'Permission Granted'
//#			the state machine must be able to clear the permission,
//#			so add new function ClearPermisson()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	3		vom: 28.01.2022
//#
//#	Implementation:
//#		-	add function IsPermissionGranted()
//#			this function is used if no LN is available (debug)
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	2		vom: 23.01.2022
//#
//#	Implementation:
//#		-	under development
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	1		vom: 21.01.2022
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

#include <stdint.h>


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================


//=====================================================================
//
//		K O N S T A N T E
//
//=====================================================================

extern const uint32_t	cg_ulInterval_20_ms;
extern const uint32_t	cg_ulInterval_100_ms;
extern const uint32_t	cg_ulInterval_500_ms;
extern const uint32_t	cg_ulInterval_2_s;


////////////////////////////////////////////////////////////////////////
//	CLASS:	IO_ControlClass
//
//	This class operates the ports of the Atmel chip.
//	There are functions to
//		-	check digital inputs
//		-	set digital outputs
//		-	position a servo
//
class IO_ControlClass
{
	public:
		IO_ControlClass();

		void Init( void );
		void ReadInputs( void );

		void LedOn( void );
		void LedOff( void );

		inline void LedFast( void )
		{
			m_bLedFast = true;
		};

		inline void LedSlow( void )
		{
			m_bLedSlow = true;
		};

		void SetServoToLockPosition( void );
		void SetServoToUnlockPosition( void );

		bool IsLedOn( void );
		bool IsKeyIn( void );
		bool IsButtonPressed( void );
		bool IsServoInLockPosition( void );

		inline bool IsPermissionGranted( void )
		{
			return( m_bPermissionGranted );
		}

		inline void ClearPermission( void )
		{
			m_bPermissionGranted = false;
		}

	private:
		uint16_t	m_uiLockPos;
		uint16_t	m_uiUnlockPos;
		bool		m_bLedFast;
		bool		m_bLedSlow;
		bool		m_bPermissionGranted;
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern IO_ControlClass	g_clControl;
