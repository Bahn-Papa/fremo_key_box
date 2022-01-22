
#pragma once

//##########################################################################
//#
//#		IO_ControlClass
//#
//#	Diese Klasse verwaltet die Ports des Atmel Chips.
//#	Es werden Funktionen zur Verfügung gestellt, um
//#		-	diverse Ausgänge zu schalten
//#		-	diverse Eingänge auszuwerten
//#
//#-------------------------------------------------------------------------
//#
//#	Version: 1.01	vom: 07.01.2022
//#
//#	Umsetzung:
//#		-	Anpassung an Platine Version 6
//#			Einbindung der DIP-Switches für die Konfiguration
//#		-	Neue Funktionen zur Abfrage der DIP-Switches
//#
//#-------------------------------------------------------------------------
//#	Version: 1.0	vom: 01.08.2021
//#
//#	Umsetzung:
//#		-	Initialisierung
//#		-	Test
//#		-	Ein- und Ausschalten
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

//----	LEDs  ----------------------------------------------------------
#define LED_YELLOW		1
#define LED_RED			2
#define LED_GREEN		3

//----	Pseudonym (Alias)  ---------------------------------------------
#define LED_PROG_MODE				LED_YELLOW
#define LED_UEBERTRAGRUNGSSTOERUNG	LED_RED
#define LED_ANRUECKMELDER			LED_GREEN


///////////////////////////////////////////////////////////////////////
//	CLASS:	LedControlClass
//
class IO_ControlClass
{
	public:
		IO_ControlClass();

		void Init( void );
		void Test( uint16_t delayTime );
		void ReadInputs( void );

		void LedOn( uint8_t leds );
		void LedOff( uint8_t leds );

		bool IsLedOn( uint8_t led );

		void BlockEnable( void );
		void BlockDisable( void );

		bool IsBlockDetect( void );

		void KeyLedOn( void );
		void KeyLedOff( void );

		bool IsKeyLedOn( void );

		void KeyRelaisOn( void );
		void KeyRelaisOff( void );

		bool IsReset( void );
		bool IsBlockOnOff( void );
		bool IsKeyIn( void );
		bool IsContact( void );

		bool IsConfigKey( void );
		bool IsConfigKeyByBox( void );
		bool IsConfigRichtungsbetrieb( void );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern IO_ControlClass	g_clControl;
