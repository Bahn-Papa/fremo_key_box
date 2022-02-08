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
//#	File version: 0.03	vom: 28.02.2022
//#
//#	Implementation:
//#		-	add configurable values for servo lock and unlock position
//#		-	change variable names to my new naming convention
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
//#	File version:	 0.01	Date: 21.01.2022
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
//	my artikle number
#define ARTIKEL_NUMMER	1511


//----------------------------------------------------------------------
//	address definitions for config informations
//
#define LNCV_ADR_MODULE_ADDRESS			0
#define LNCV_ADR_ARTIKEL_NUMMER			1
#define LNCV_ADR_CONFIGURATION			2
#define LNCV_ADR_SEND_DELAY				3
#define LNCV_ADR_SERVO_LOCK_POSITION	4
#define LNCV_ADR_SERVO_UNLOCK_POSITION	5


//----------------------------------------------------------------------
//	address definitions for messages
//
#define	LNCV_ADR_KEY_PERMISSION			6
#define	LNCV_ADR_KEY_STATE				7


//----------------------------------------------------------------------
//	mask definitions related to messages
//
#define	MASK_KEY_PERMISSION				0x01
#define	MASK_KEY_STATE					0x02


//---------------------------------------------------------------------
//	Servo default position values
//
#define SERVO_LOCK_POS		3999	//	2 ms pulse
#define SERVO_UNLOCK_POS	1999	//	1 ms pulse


//----------------------------------------------------------------------
//	delay times
//
#define	MIN_SEND_DELAY_TIME			   5
#define DEFAULT_SEND_DELAY_TIME		  10


////////////////////////////////////////////////////////////////////////
//	CLASS: LncvStorageClass
//

//**********************************************************************
//	Constructor
//----------------------------------------------------------------------
//
LncvStorageClass::LncvStorageClass()
{
}


//**********************************************************************
//	CheckEEPROM
//----------------------------------------------------------------------
//	This function checks if the EEPROM is empty (0xFF in the cells).
//	If so, then the EEPROM will be filled with default config infos
//	and all addresses will be set to zero.
//
void LncvStorageClass::CheckEEPROM( void )
{
	uint8_t	byte0 = eeprom_read_byte( (uint8_t *)0 );
	uint8_t	byte1 = eeprom_read_byte( (uint8_t *)1 );

#ifdef DEBUGGING_PRINTOUT
	g_clDebugging.PrintStorageCheck( byte0, byte1 );
#endif

	if( (0xFF == byte0) && (0xFF == byte1) )
	{
		//----------------------------------------------------------
		//	the EEPROM is empty, so write default config info ...
		//

#ifdef DEBUGGING_PRINTOUT
		g_clDebugging.PrintStorageDefault();
#endif

		WriteLNCV( LNCV_ADR_MODULE_ADDRESS, 0x0001 );
		WriteLNCV( LNCV_ADR_ARTIKEL_NUMMER,	ARTIKEL_NUMMER );
		WriteLNCV( LNCV_ADR_CONFIGURATION, 0 );
		WriteLNCV( LNCV_ADR_SEND_DELAY, DEFAULT_SEND_DELAY_TIME );
		WriteLNCV( LNCV_ADR_SERVO_LOCK_POSITION, SERVO_LOCK_POS );
		WriteLNCV( LNCV_ADR_SERVO_UNLOCK_POSITION, SERVO_UNLOCK_POS );
		WriteLNCV( LNCV_ADR_KEY_PERMISSION, 0 );
		WriteLNCV( LNCV_ADR_KEY_STATE, 0 );
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
	m_uiArticleNumber		= ReadLNCV( LNCV_ADR_ARTIKEL_NUMMER );
	m_uiModuleAddress		= ReadLNCV( LNCV_ADR_MODULE_ADDRESS );
//	m_uiConfiguration		= ReadLNCV( LNCV_ADR_CONFIGURATION );
	m_uiServoLockPosition	= ReadLNCV( LNCV_ADR_SERVO_LOCK_POSITION );
	m_uiServoUnlockPosition	= ReadLNCV( LNCV_ADR_SERVO_UNLOCK_POSITION );
	m_uiPermissionAddress	= ReadLNCV( LNCV_ADR_KEY_PERMISSION );
	m_uiKeyStateAddress		= ReadLNCV( LNCV_ADR_KEY_STATE );

	//--------------------------------------------------------------
	//	read send delay time
	//	and make sure it is not shorter than MIN_SEND_DELAY_TIME ms
	//
	m_uiSendDelay = ReadLNCV( LNCV_ADR_SEND_DELAY );

	if( MIN_SEND_DELAY_TIME > m_uiSendDelay )
	{
		m_uiSendDelay = MIN_SEND_DELAY_TIME;
	}
}


//**********************************************************************
//	IsValidLNCVAdress
//
bool LncvStorageClass::IsValidLNCVAddress( uint16_t Adresse )
{
	if( LNCV_ADR_KEY_STATE >= Adresse )
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
void LncvStorageClass::WriteLNCV( uint16_t Adresse, uint16_t Value )
{
	//--------------------------------------------------------------
	//	because of uint16 values the address has to be shifted
	//	by '1' (this will double the address).
	//
	eeprom_write_word( (uint16_t *)(Adresse << 1), Value );
}


//--------------------------------------------------------------------------
#endif		//	defined( COMMAND_CONNECTION_LOCONET )
