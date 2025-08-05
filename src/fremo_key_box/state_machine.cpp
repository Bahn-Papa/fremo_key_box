//##########################################################################
//#
//#		StateMachineClass
//#
//#	This class contains the state machine for 'FREMO Key Box'
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	5		vom: 05.08.2025
//#
//#	Implementation:
//#		-	add new state STATE_KEY_LOCKED_BOOT
//#			changes in function
//#				CheckState()
//#
//#	Bug Fix:
//#		-	handling of LED and permission of key corrected as stated
//#			in e-mail from Rlaf S.
//#		-	add use of new 'SendBootStateDelayTime'
//#			changes in function
//#				CheckState()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	4		vom: 02.08.2025
//#
//#	Bug Fix:
//#		-	remove the repeatedly sending of key state messages
//#		-	when key was removed and button released switch off the LED
//#			changes in function
//#				CheckState()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	3		vom: 28.01.2022
//#
//#	Implementation:
//#		-	finished the sequence of the state machine
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
//#		-	initial version
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include "compile_options.h"

#include <Arduino.h>

#ifdef DEBUGGING_PRINTOUT
#include "debugging.h"
#endif

#include "lncv_storage.h"
#include "io_control.h"
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

StateMachineClass	g_clStateMachine	= StateMachineClass();


//==========================================================================
//
//		C L A S S   F U N C T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////
//	CLASS: StateMachineClass
//

//******************************************************************
//	Constructor
//------------------------------------------------------------------
//	set the internal variables to their initial value
//	the initial state is 'STATE_POWER_ON'.
//
StateMachineClass::StateMachineClass()
{
	m_eState		= STATE_POWER_ON;
	m_eOldState		= STATE_POWER_ON;
	m_ulDelayMillis	= 0;
}


//******************************************************************
//	CheckState
//------------------------------------------------------------------
//	This function is the heart of the state machine.
//	The data from the data pool and the current state are used
//	to check whether a state change should take place.
//------------------------------------------------------------------
//	Principle of the state machine:
//	-	when entering a state (m_eOldState != m_eState)
//		actions are executed that are to be executed,
//		regardless of the previous state
//	-	in the state data from the data pool is checked
//		to find out whether a state change should take place.
//	-	if a state change should take place execute all actions
//		that are specific for this state change only.
//------------------------------------------------------------------
//	RETURN value
//		The actual or new state will be returned
//
box_state_t StateMachineClass::CheckState( void )
{
	switch( m_eState )
	{
		//----------------------------------------------------------
		case STATE_POWER_ON:
			if( g_clControl.IsKeyIn() )
			{
				g_clControl.LedOff();
				g_clControl.SetServoToLockPosition();

				m_ulDelayMillis	= millis() + g_clLncvStorage.GetSendBootStateDelayTime();
			}
			else if( (0L < m_ulDelayMillis) && (millis() > m_ulDelayMillis) )
			{
				m_ulDelayMillis = 0L;

				m_eState = STATE_KEY_LOCKED;

				g_clMyLoconet.SendKeyRemoved( false );
			}
			else
			{
				m_eState = STATE_KEY_OUT_BOOT;
			}
			break;

		//----------------------------------------------------------
		case STATE_KEY_LOCKED_BOOT:
			if( m_eOldState != m_eState )
			{
				m_eOldState	= m_eState;

				g_clControl.LedOff();
				g_clControl.SetServoToLockPosition();

				m_ulDelayMillis	= millis() + g_clLncvStorage.GetSendBootStateDelayTime();
			}
			else if( (0L < m_ulDelayMillis) && (millis() > m_ulDelayMillis) )
			{
				m_ulDelayMillis = 0L;

				g_clMyLoconet.SendKeyRemoved( false );
			}
			else if(	g_clMyLoconet.IsPermissionGranted()
					||	g_clControl.IsPermissionGranted()	)
			{
				m_eState = STATE_PERMISSION_GRANTED;
			}
			break;

		//----------------------------------------------------------
		case STATE_KEY_OUT_BOOT:
			if( m_eOldState != m_eState )
			{
				m_eOldState	= m_eState;

				g_clControl.LedSlow();

				m_ulDelayMillis	= millis() + g_clLncvStorage.GetSendBootStateDelayTime();
			}
			else if( (0L < m_ulDelayMillis) && (millis() > m_ulDelayMillis) )
			{
				m_ulDelayMillis = 0L;

				g_clMyLoconet.SendKeyRemoved( true );
			}
			else if( g_clControl.IsKeyIn() )
			{
				m_eState = STATE_KEY_LOCKED_PRE;
			}
			break;

		//----------------------------------------------------------
		case STATE_KEY_OUT:
			if( m_eOldState != m_eState )
			{
				m_eOldState	= m_eState;

				g_clControl.LedOn();

				g_clMyLoconet.SendKeyRemoved( true );
			}
			else if( g_clControl.IsKeyIn() )
			{
				m_eState = STATE_KEY_LOCKED_PRE;
			}
			break;

		//----------------------------------------------------------
		case STATE_KEY_LOCKED_PRE:
			if( m_eOldState != m_eState )
			{
				m_eOldState = m_eState;

				m_ulDelayMillis = millis() + cg_ulInterval_500_ms;
//				m_ulDelayMillis = millis() + cg_ulInterval_2_s;
			}
			else if( (0 < m_ulDelayMillis) && (millis() > m_ulDelayMillis) )
			{
				m_ulDelayMillis = 0L;

				g_clMyLoconet.SendKeyRemoved( false );

				m_eState = STATE_KEY_LOCKED;
			}
			break;

		//----------------------------------------------------------
		case STATE_KEY_LOCKED:
			if( m_eOldState != m_eState )
			{
				m_eOldState = m_eState;

				g_clControl.ClearPermission();
				g_clMyLoconet.ClearPermission();

				g_clControl.LedOff();
				g_clControl.SetServoToLockPosition();
			}
			else if(	g_clMyLoconet.IsPermissionGranted()
					||	g_clControl.IsPermissionGranted()	)
			{
				m_eState = STATE_PERMISSION_GRANTED;
			}
			break;

		//----------------------------------------------------------
		case STATE_PERMISSION_GRANTED:
			if( m_eOldState != m_eState )
			{
				m_eOldState = m_eState;
				
				g_clControl.LedOn();
			}
			else if( !(		g_clMyLoconet.IsPermissionGranted()
						||	g_clControl.IsPermissionGranted()	) )
			{
				m_eState = STATE_KEY_LOCKED;
			}
			else if( g_clControl.IsButtonPressed() && g_clControl.IsKeyIn() )
			{
				m_eState = STATE_KEY_UNLOCKED;
			}
			break;

		//----------------------------------------------------------
		case STATE_KEY_UNLOCKED:
			if( m_eOldState != m_eState )
			{
				m_eOldState = m_eState;

				g_clControl.LedOn();

				g_clControl.SetServoToUnlockPosition();
				g_clMyLoconet.SendKeyRemoved( true );
			}
			else if( !g_clControl.IsKeyIn() )
			{
				m_eState = STATE_KEY_OUT;
			}
			else if( !g_clControl.IsButtonPressed() )
			{
				g_clMyLoconet.SendKeyRemoved( false );

				m_eState = STATE_KEY_LOCKED;
			}
			break;
	}

	return( m_eState );
}
