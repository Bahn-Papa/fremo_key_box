
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
//#	File version: 0.02	vom: 23.01.2022
//#
//#	Implementation:
//#		-	under development
//#
//#-------------------------------------------------------------------------
//#
//#	File version: 0.01	vom: 21.01.2022
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
		
	private:
		bool	m_bLedFast;
		bool	m_bLedSlow;
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern IO_ControlClass	g_clControl;
