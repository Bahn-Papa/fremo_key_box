
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
			return( m_ui16ArticleNumber );
		};

		//----------------------------------------------------------
		//
		inline uint16_t GetModuleAddress( void )
		{
			return( m_ui16ModuleAddress );
		};

		//----------------------------------------------------------
		//
		inline uint16_t GetPermissionAddress( void )
		{
			return( m_ui16PermissionAddress );
		};

		//----------------------------------------------------------
		//
		inline uint16_t GetKeyStateAddress( void )
		{
			return( m_ui16KeyStateAddress );
		};

		//----------------------------------------------------------
		//
		inline uint16_t GetSendDelayTime( void )
		{
			return( m_ui16SendDelay );
		};

	private:
		uint16_t	m_ui16ArticleNumber;
		uint16_t	m_ui16ModuleAddress;
//		uint16_t	m_ui16Configuration;
		uint16_t	m_ui16PermissionAddress;
		uint16_t	m_ui16KeyStateAddress;
		uint16_t	m_ui16SendDelay;
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern LncvStorageClass		g_clLncvStorage;


//--------------------------------------------------------------------------
#endif		//	defined( COMMAND_CONNECTION_LOCONET )
