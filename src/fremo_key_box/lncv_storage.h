
#pragma once

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
#define LNCV_ADR_MODULE_ADDRESS				0
#define LNCV_ADR_ARTIKEL_NUMMER				1
#define LNCV_ADR_VERSION_NUMBER				2
#define LNCV_ADR_CONFIGURATION				3
#define LNCV_ADR_SEND_DELAY					4
#define LNCV_ADR_SERVO_LOCK_POSITION		5
#define LNCV_ADR_SERVO_UNLOCK_POSITION		6


//----------------------------------------------------------------------
//	address definitions for messages
//
#define	LNCV_ADR_KEY_PERMISSION				7
#define	LNCV_ADR_KEY_STATE					8


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


#if defined( COMMAND_CONNECTION_LOCONET )


////////////////////////////////////////////////////////////////////////
//	CLASS:	LncvStorageClass
//
class LncvStorageClass
{
	public:
		//----------------------------------------------------------
		//	Constructor
		//
		LncvStorageClass();

		//----------------------------------------------------------
		//
		void		CheckEEPROM( uint16_t uiVersionNumber );
		void		Init( void );
		bool		IsValidLNCVAddress( uint16_t Adresse );
		uint16_t	ReadLNCV(  uint16_t Adresse );
		void		WriteLNCV( uint16_t Adresse, uint16_t Value );

		//----------------------------------------------------------
		//
		inline uint16_t GetArticleNumber( void )
		{
			return( m_uiArticleNumber );
		};

		//----------------------------------------------------------
		//
		inline uint16_t GetModuleAddress( void )
		{
			return( m_uiModuleAddress );
		};

		//----------------------------------------------------------
		//
		inline uint16_t GetPermissionAddress( void )
		{
			return( m_uiPermissionAddress );
		};

		//----------------------------------------------------------
		//
		inline uint16_t GetKeyStateAddress( void )
		{
			return( m_uiKeyStateAddress );
		};

		//----------------------------------------------------------
		//
		inline uint16_t GetSendDelayTime( void )
		{
			return( m_uiSendDelay );
		};


	private:
		uint16_t	m_uiArticleNumber;
		uint16_t	m_uiModuleAddress;
//		uint16_t	m_uiConfiguration;
		uint16_t	m_uiPermissionAddress;
		uint16_t	m_uiKeyStateAddress;
		uint16_t	m_uiSendDelay;
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern LncvStorageClass		g_clLncvStorage;


//--------------------------------------------------------------------------
#endif		//	defined( COMMAND_CONNECTION_LOCONET )
