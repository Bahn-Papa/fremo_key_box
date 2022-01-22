
#pragma once

//##########################################################################
//#
//#		StateMachineClass
//#
//#	This class contains the state machine for 'FREMO Key Box'
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

#include <stdint.h>


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

typedef enum box_state
{
	STATE_POWER_ON				= 0,
	STATE_PERMISSION_GRANTED,
	STATE_KEY_OUT_BOOT,
	STATE_KEY_OUT,
	STATE_KEY_LOCKED_PRE,
	STATE_KEY_LOCKED,
	STATE_KEY_UNLOCKED

} box_state_t;


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================


///////////////////////////////////////////////////////////////////////
//	CLASS:	StateMachineClass
//
//	This class contains the state machine for 'FREMO Key Box'
//
class StateMachineClass
{
	private:
		box_state_t		m_eState;
		box_state_t		m_eOldState;
		uint32_t		m_ulDelayMillis;

	public:
		//*********************************************************
		//	Constructor
		//
		StateMachineClass();

		//*********************************************************
		//	GetState
		//---------------------------------------------------------
		//	The function will return the actual state.
		//
		inline box_state_t GetState( void )
		{
			return( m_eState );
		};
		
		//*********************************************************
		//	CheckState
		//---------------------------------------------------------
		//	The function will perform state changes depending
		//	of the internal data stored in the 'data pool'
		//
		box_state_t CheckState( void );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern StateMachineClass	g_clStateMachine;
