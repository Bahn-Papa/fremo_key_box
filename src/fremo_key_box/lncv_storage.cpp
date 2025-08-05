//##########################################################################
//#
//#		LncvStorageClass
//#
//#	This class manages the LNCVs (L oco N et C onfiguration V ariables)
//#	These variables will be saved in the EEPROM.
//#
//#	To simplify access, the variables were arranged one after the other
//#	without gaps. This makes it very easy to read and write the variables
//#	with a tool.
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	8		from: 05.08.2025
//#
//#	Implementation:
//#		-	add LNCV #10 to configure the delay time for sending
//#			state of the key after booting.
//#			add definition
//#				LNCV_ADR_SEND_BOOT_STATE_DELAY
//#			add member variable
//#				m_uiSendBootStateDelayTime
//#			change in functions
//#				Constructor()
//#				Init()
//#				CheckEEPROM()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	7		from: 02.08.2025
//#
//#	Implementation:
//#		-	add a second LNCV address for the key state
//#			add new definition
//#				LNCV_ADR_KEY_STATE_2
//#			change in functions
//#				CheckEEPROM()
//#				IsValidLNCVAddress()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	6		from: 02.08.2025
//#
//#	Implementation:
//#		-	avoid compiler warnings
//#			changes in function
//#				CheckEEPROM()
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	5		from: 13.11.2022
//#
//#	Implementation:
//#		-	remove functions
//#				GetKeyStateAddress()
//#				GetPermissionAddress()
//#				GetSendDelayTime()
//#			remove variables
//#				m_uiKeyStateAddress
//#				m_uiPermissionAddress
//#				m_uiSendDelay
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	4		from: 13.11.2022
//#
//#	Implementation:
//#		-	move definitions from .cpp file to .h file
//#		-	add version number into EEPROM
//#		-	remove functions
//#				GetServoLockPosition()
//#				GetServoUnlockPosition()
//#			remove variables 
//#				m_uiServoLockPosition
//#				m_uiServoUnlockPosition
//#
//#-------------------------------------------------------------------------
//#
//#	File version:	3		from: 28.02.2022
//#
//#	Implementation:
//#		-	add configurable values for servo lock and unlock position
//#		-	change variable names to my new naming convention
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
//#		-	first version
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

#ifdef DEBUGGING_PRINTOUT
#include "debugging.h"
#endif

#include "lncv_storage.h"


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

LncvStorageClass	g_clLncvStorage = LncvStorageClass();


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

//----------------------------------------------------------------------
//	delay times
//
#define	MIN_SEND_DELAY_TIME						5
#define DEFAULT_SEND_DELAY_TIME					10
#define DEFAULT_SEND_BOOT_STATE_DELAY_TIME		2000


////////////////////////////////////////////////////////////////////////
//	CLASS: LncvStorageClass
//

//**********************************************************************
//	Constructor
//----------------------------------------------------------------------
//
LncvStorageClass::LncvStorageClass()
{
	m_uiSendBootStateDelayTime = 0;
}


//**********************************************************************
//	CheckEEPROM
//----------------------------------------------------------------------
//	This function checks if the EEPROM is empty (0xFF in the cells).
//	If so, then the EEPROM will be filled with default config infos
//	and all addresses will be set to zero.
//
void LncvStorageClass::CheckEEPROM( uint16_t uiVersionNumber )
{
	uint16_t	uiAddress	= ReadLNCV( LNCV_ADR_MODULE_ADDRESS );

#ifdef DEBUGGING_PRINTOUT
	uint16_t	uiArticle	= ReadLNCV( LNCV_ADR_ARTIKEL_NUMMER );

	g_clDebugging.PrintStorageCheck( uiAddress, uiArticle );
#endif

	if( (0xFFFF == uiAddress) || (0x0000 == uiAddress) )
	{
		//----------------------------------------------------------
		//	the EEPROM is empty, so write default config info ...
		//

#ifdef DEBUGGING_PRINTOUT
		g_clDebugging.PrintStorageDefault();
#endif

		WriteLNCV( LNCV_ADR_MODULE_ADDRESS, 0x0001 );
		WriteLNCV( LNCV_ADR_ARTIKEL_NUMMER,	ARTIKEL_NUMMER );
		WriteLNCV( LNCV_ADR_VERSION_NUMBER, uiVersionNumber );

		WriteLNCV( LNCV_ADR_CONFIGURATION, 0 );
		WriteLNCV( LNCV_ADR_SEND_DELAY, DEFAULT_SEND_DELAY_TIME );
		WriteLNCV( LNCV_ADR_SERVO_LOCK_POSITION, SERVO_LOCK_POS );
		WriteLNCV( LNCV_ADR_SERVO_UNLOCK_POSITION, SERVO_UNLOCK_POS );
		WriteLNCV( LNCV_ADR_KEY_PERMISSION, 0 );
		WriteLNCV( LNCV_ADR_KEY_STATE, 0 );
		WriteLNCV( LNCV_ADR_KEY_STATE_2, 0 );
		WriteLNCV( LNCV_ADR_SEND_BOOT_STATE_DELAY, DEFAULT_SEND_BOOT_STATE_DELAY_TIME );
	}
	else
	{
		WriteLNCV( LNCV_ADR_VERSION_NUMBER, uiVersionNumber );
	}
}


//**********************************************************************
//	Init
//----------------------------------------------------------------------
//	This function will read the saved informations from the EEPROM.
//
void LncvStorageClass::Init( void )
{
#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintStorageRead();
#endif

	//--------------------------------------------------------------
	//	read config information
	//
//	m_uiConfiguration		= ReadLNCV( LNCV_ADR_CONFIGURATION );

	m_uiSendBootStateDelayTime = ReadLNCV( LNCV_ADR_SEND_BOOT_STATE_DELAY );
	
	if( 0 == m_uiSendBootStateDelayTime )
	{
		m_uiSendBootStateDelayTime = DEFAULT_SEND_BOOT_STATE_DELAY_TIME;
	}
}


//**********************************************************************
//	IsValidLNCVAdress
//
bool LncvStorageClass::IsValidLNCVAddress( uint16_t Adresse )
{
	if( LNCV_ADR_KEY_STATE_2 >= Adresse )
	{
		return( true );
	}

	return( false );
}


//**********************************************************************
//	ReadLNCV
//
uint16_t LncvStorageClass::ReadLNCV( uint16_t Adresse )
{
	uint16_t	value;

	//--------------------------------------------------------------
	//	because of uint16 values the address has to be shifted
	//	by '1' (this will double the address).
	//
	value = eeprom_read_word( (uint16_t *)(Adresse << 1) );

	return( value );
}


//**********************************************************************
//	WriteLNCV
//
void LncvStorageClass::WriteLNCV( uint16_t Address, uint16_t Value )
{
	//--------------------------------------------------------------
	//	because of uint16 values the address has to be shifted
	//	by '1' (this will double the address).
	//
	uint16_t *	puiAdr	= (uint16_t *)(Address << 1);
	uint16_t	uiValue	= eeprom_read_word( puiAdr );
	
	if( uiValue != Value )
	{
		eeprom_write_word( puiAdr, Value );
	}
}


//--------------------------------------------------------------------------
#endif		//	defined( COMMAND_CONNECTION_LOCONET )
