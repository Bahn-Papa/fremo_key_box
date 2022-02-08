
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
		void		CheckEEPROM( void );
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

		//----------------------------------------------------------
		//
		inline uint16_t GetServoLockPosition( void )
		{
			return( m_uiServoLockPosition );
		};

		//----------------------------------------------------------
		//
		inline uint16_t GetServoUnlockPosition( void )
		{
			return( m_uiServoUnlockPosition );
		};

	private:
		uint16_t	m_uiArticleNumber;
		uint16_t	m_uiModuleAddress;
//		uint16_t	m_uiConfiguration;
		uint16_t	m_uiPermissionAddress;
		uint16_t	m_uiKeyStateAddress;
		uint16_t	m_uiSendDelay;
		uint16_t	m_uiServoLockPosition;
		uint16_t	m_uiServoUnlockPosition;
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern LncvStorageClass		g_clLncvStorage;


//--------------------------------------------------------------------------
#endif		//	defined( COMMAND_CONNECTION_LOCONET )
