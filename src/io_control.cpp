//##########################################################################
//#
//#		IO_ControlClass
//#
//#-------------------------------------------------------------------------
//#	This class operates the Inputs and Outputs, like LEDs, relais etc.
//#	There will be functions to set Outputs and read Inputs.
//#
//#-------------------------------------------------------------------------
//#	Version: 1.02	vom: 07.01.2022
//#	
//#	Umsetzung:
//#		-	Anpassung an Platine Version 6
//#			Neue Inputs zur Konfiguration über DIP-Switches
//#		-	Neue Funktionen zur Abfrage der DIP-Switches
//#
//#-------------------------------------------------------------------------
//#	Version: 1.01	vom: 14.09.2021
//#
//#	Fehlerkorrektur:
//#		-	KEY_LED an das endgültige Layout der Platine angepasst.
//#
//#-------------------------------------------------------------------------
//#	Version: 1.0	vom: 12.08.2021
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

#include <Arduino.h>

#include "io_control.h"
#include "entprellung.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define	READ_INPUT_COUNT	6


#if PLATINE_VERSION == 6


	//----	PORT B  ----------------------------------------------------
	//		PB0		Loconet Receive (LN_RX)
	//		PB1		CONFIG_1
	//		PB2		CONFIG_2
	//		PB3		KEY_LED
	//		PB4		CONFIG_3
	//		PB5		not used
	//		PB6		Oscillator 1
	//		PB7		Oscillator 2
	//
	#define	CONFIG_1			PB1
	#define	CONFIG_2			PB2
	#define KEY_LED				PB3
	#define	CONFIG_3			PB4

	#define PORT_B_FREE_BITS	 (1 << PB5)
	#define PORT_B_OUTPUTS		 (1 << KEY_LED)
	#define PORT_B_INPUTS		((1 << CONFIG_1) | (1 << CONFIG_2) | (1 << CONFIG_3))
	
	//----	PORT C  ----------------------------------------------------
	//		PC0		BLOCK_ENABLE (BL_DRV_EN / LED_BLUE)
	//		PC1		LED_YELLOW
	//		PC2		LED_RED
	//		PC3		LED_GREEN
	//		PC4		I2C:SDA
	//		PC5		I2C:SCL
	//		PC6		not implemented
	//		PC7		not implemented

	//----	PORT D  ----------------------------------------------------
	//		PD0		Block Rx (BL_RX)
	//		PD1		Block Tx (BL_TX)
	//		PD2		BLK_ON_OFF
	//		PD3		KEY_REL
	//		PD4		KEY_IN
	//		PD5		CONTACT
	//		PD6		BLOCK_DETECT
	//		PD7		Loconet Send (LN_TX)
	//
	#define	BLOCK_ON_OFF	PD2
	#define	KEY_REL			PD3
	#define	KEY_IN			PD4
	#define CONTACT			PD5
	#define	BLOCK_DETECT	PD6

	#define	PORT_D_OUTPUTS	 (1 << KEY_REL)
	#define	PORT_D_INPUTS	((1 << BLOCK_ON_OFF) | (1 << KEY_IN) | (1 << CONTACT))



#elif PLATINE_VERSION == 5


	//----	PORT B  ----------------------------------------------------
	//		PB0		Loconet Receive (LN_RX)
	//		PB1		I2C_INTA
	//		PB2		I2C_INTB
	//		PB3		KEY_LED
	//		PB4		not used
	//		PB5		not used
	//		PB6		Oscillator 1
	//		PB7		Oscillator 2
	//
	#define KEY_LED				PB3

	#define PORT_B_FREE_BITS	((1 << PB1) | (1 << PB2) | (1 << PB4) | (1 << PB5))
	#define PORT_B_OUTPUTS		 (1 << KEY_LED)
	
	//----	PORT C  ----------------------------------------------------
	//		PC0		BLOCK_ENABLE (BL_DRV_EN / LED_BLUE)
	//		PC1		LED_YELLOW
	//		PC2		LED_RED
	//		PC3		LED_GREEN
	//		PC4		I2C:SDA
	//		PC5		I2C:SCL
	//		PC6		not implemented
	//		PC7		not implemented

	//----	PORT D  ----------------------------------------------------
	//		PD0		Block Rx (BL_RX)
	//		PD1		Block Tx (BL_TX)
	//		PD2		BLK_ON_OFF
	//		PD3		KEY_REL
	//		PD4		KEY_IN
	//		PD5		CONTACT
	//		PD6		BLOCK_DETECT
	//		PD7		Loconet Send (LN_TX)
	//
	#define	BLOCK_ON_OFF	PD2
	#define	KEY_REL			PD3
	#define	KEY_IN			PD4
	#define CONTACT			PD5
	#define	BLOCK_DETECT	PD6

	#define	PORT_D_OUTPUTS	 (1 << KEY_REL)
	#define	PORT_D_INPUTS	((1 << BLOCK_ON_OFF) | (1 << KEY_IN) | (1 << CONTACT))



#elif PLATINE_VERSION == 4


	//----	PORT B  ----------------------------------------------------
	//		PB0		Loconet Receive (LN_RX, !!  von Hand neu verdrahtet  !! )
	//		PB1		KEY_IN
	//		PB2		CONTACT
	//		PB3		not used
	//		PB4		not used
	//		PB5		not used
	//		PB6		Oscillator 1
	//		PB7		Oscillator 2
	//
	#define	KEY_IN				PB1
	#define	CONTACT				PB2

	#define PORT_B_FREE_BITS	((1 << PB3) | (1 << PB4) | (1 << PB5))
	#define PORT_B_INPUTS		((1 << KEY_IN) | (1 << CONTACT))

	//----	PORT C  ----------------------------------------------------
	//		PC0		BLOCK_ENABLE (BL_DRV_EN / LED_BLUE)
	//		PC1		LED_YELLOW
	//		PC2		LED_RED
	//		PC3		LED_GREEN
	//		PC4		I2C:SDA
	//		PC5		I2C:SCL
	//		PC6		not implemented
	//		PC7		not implemented
	//

	//----	PORT D  ----------------------------------------------------
	//		PD0		Block Rx (BL_RX)
	//		PD1		Block Tx (BL_TX)
	//		PD2		BLK_ON_OFF
	//		PD3		KEY_LED
	//		PD4		Loconet Send (LN_TX)
	//		PD5		BLOCK_DETECT
	//		PD6		not used
	//		PD7		KEY_REL ( !! von Hand neu verdrahtet !! )
	//
	#define BLOCK_ON_OFF		PD2
	#define KEY_LED				PD3
	#define	BLOCK_DETECT		PD5
	#define	KEY_REL				PD7

	#define PORT_D_FREE_BITS	 (1 << PD6)
	#define	PORT_D_OUTPUTS		((1 << KEY_LED) | (1 << KEY_REL))
	#define PORT_D_INPUTS		 (1 << BLOCK_ON_OFF)



#else	//	Version 3 und vorherige Versionen


	//----	PORT B  ----------------------------------------------------
	//		PB0		Loconet Receive (LN_RX)
	//		PB2		Block Send mirror (BL_TX)
	//		PB6		Oscillator 1
	//		PB7		Oscillator 2
	//
	#define PORT_B_FREE_BITS	((1 << PB1) | (1 << PB3) | (1 << PB4) | (1 << PB5))
	#define	BLOCK_TX_MIRROR		PB2

	//----	PORT C  ----------------------------------------------------
	//		PC0		BLOCK_ENABLE (BL_DRV_EN / LED_BLUE)
	//		PC1		LED_YELLOW
	//		PC2		LED_RED
	//		PC3		LED_GREEN
	//		PC4		I2C:SDA
	//		PC5		I2C:SCL
	//

	//----	PORT D  ----------------------------------------------------
	//		PD0		Block Rx (BL_RX)
	//		PD1		Block Tx (BL_TX)
	//		PD5		BLOCK_DETECT
	//		PD7		Loconet Send (LN_TX)
	//
	#define PORT_D_FREE_BITS	((1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PD6))
	#define	BLOCK_DETECT		PD5


#endif


//---------------------------------------------------------------------
//	The following definitions are identical to all board versions.
//

//----	Port B  ---------------------------------------------------


//----	Port C  ---------------------------------------------------
//	LED definitions are located in the header file 'io_control.h'
//
#define BLOCK_ENABLE	PC0

#define ALL_LEDS	((1 << LED_YELLOW) | (1 << LED_RED) | (1 << LED_GREEN))


//----	Port D  ---------------------------------------------------


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

IO_ControlClass	g_clControl	= IO_ControlClass();



#if PLATINE_VERSION == 6

//----------------------------------------------------------------------
//	for board version 6 Inputs are connected to Port B and Port D
//
EntprellungClass	g_clPortB( 0x00 );
EntprellungClass	g_clPortD( (1 << BLOCK_ON_OFF) );



#elif PLATINE_VERSION == 5

//----------------------------------------------------------------------
//	for board version 5 all Inputs are connected to Port D
//
EntprellungClass	g_clPortD( (1 << BLOCK_ON_OFF) );



#elif PLATINE_VERSION == 4

//----------------------------------------------------------------------
//	for board version 4 Inputs are connected to Port B and Port D
//
EntprellungClass	g_clPortB( 0x00 );
EntprellungClass	g_clPortD( (1 << BLOCK_ON_OFF) );



#else	//	Version 3 und vorherige Versionen

//----------------------------------------------------------------------
//	for older board versions there is only one Input relevant and
//	this Input is connected to Port D
//
EntprellungClass	g_clPortD( 0x00 );

#endif


//==========================================================================
//
//		C L A S S   F U N C T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////
//	CLASS: IO_ControlClass
//

//******************************************************************
//	Constructor
//
IO_ControlClass::IO_ControlClass()
{
}


//******************************************************************
//	Init
//------------------------------------------------------------------
//	here for all Ports the relevant I/O pins will be configured.
//	Not used I/O's will be set to Input with PullUp.
//	all connected devices (like LED's) will set to 'off'.
//
void IO_ControlClass::Init( void )
{
	//----	set unused pins to input with pullup  ------------------
	//
	DDRB	&= ~PORT_B_FREE_BITS;
	PORTB	|=  PORT_B_FREE_BITS;

#ifdef PORT_D_FREE_BITS
	DDRD	&= ~PORT_D_FREE_BITS;
	PORTD	|=  PORT_D_FREE_BITS;
#endif


	//----	Port B  ------------------------------------------------
	//
#ifdef PORT_B_INPUTS
	DDRB	&= ~PORT_B_INPUTS;		//	configure as Input
	PORTB	|=  PORT_B_INPUTS;		//	Pull-Up on
#endif

#ifdef PORT_B_OUTPUTS
	DDRB	|= PORT_B_OUTPUTS;		//	configure as Output
	PORTB	|= PORT_B_OUTPUTS;		//	switch off
#endif

	//----	Port C  ------------------------------------------------
	//
	//----	Pins auf Ausgang setzen  -------------------------------
	DDRC |= ((1 << BLOCK_ENABLE) | ALL_LEDS);
	
	LedOff( ALL_LEDS );
	BlockDisable();

	//----	Port D  ------------------------------------------------
	//
	DDRD	&= ~(1 << BLOCK_DETECT);	//	configure as Input
	
#ifdef PORT_D_INPUTS
	DDRD	&= ~PORT_D_INPUTS;			//	configure as Input
	PORTD	|=  PORT_D_INPUTS;			//	Pull-Up on
#endif

#ifdef PORT_D_OUTPUTS
	DDRD	|= PORT_D_OUTPUTS;			//	configure as Output
	PORTD	|= PORT_D_OUTPUTS;			//	switch off
#endif

	//----	Read actual Inputs  ------------------------------------
	//
	for( uint8_t idx = 0 ; idx < READ_INPUT_COUNT ; idx++ )
	{
		delay( 20 );

		ReadInputs();
	}

	delay( 20 );
}


//******************************************************************
//	LedTest
//
void IO_ControlClass::Test( uint16_t delayTime )
{
	//----	Blau EIN, alle anderen aus  ------------------------
	BlockEnable();
	delay( delayTime );

	//----	Blau AUS, Gelb EIN  --------------------------------
	BlockDisable();
	LedOn( 1 << LED_YELLOW );
	delay( delayTime );
	
	//----	Gelb AUS, Rot EIN  ---------------------------------
	LedOff( 1 << LED_YELLOW );
	LedOn( 1 << LED_RED );
	delay( delayTime );
	
	//----	Rot AUS, Grün EIN  ---------------------------------
	LedOff( 1 << LED_RED );
	LedOn( 1 << LED_GREEN );
	delay( delayTime );
	
	//----	Grün AUS, Test beendet  ----------------------------
	LedOff( 1 << LED_GREEN );
}


//******************************************************************
//	ReadInputs
//------------------------------------------------------------------
//	This function should be called at regular intervals,
//	for example: every 20 ms
//
void IO_ControlClass::ReadInputs( void )
{
	g_clPortD.Work( PIND );

#if PLATINE_VERSION > 5
	g_clPortB.Work( PINB );
	
#elif PLATINE_VERSION == 4
	g_clPortB.Work( PINB );
#endif
}


//******************************************************************
//	LedOn
//
void IO_ControlClass::LedOn( uint8_t leds )
{
	PORTC &= ~leds; 
}


//******************************************************************
//	LedOff
//
void IO_ControlClass::LedOff( uint8_t leds )
{
	PORTC |= leds;
}


//******************************************************************
//	BlockEnable
//
void IO_ControlClass::BlockEnable( void )
{
	PORTC |= (1 << BLOCK_ENABLE);
}


//******************************************************************
//	BlockDisable
//
void IO_ControlClass::BlockDisable( void )
{
	PORTC &= ~(1 << BLOCK_ENABLE);
}


//******************************************************************
//	IsLedOn
//
bool IO_ControlClass::IsLedOn( uint8_t leds )
{
	return( 0 == (PINC & leds) );
}


//******************************************************************
//	IsBlockDetect
//
bool IO_ControlClass::IsBlockDetect( void )
{
	return( 0 != g_clPortD.GetKeyState( 1 << BLOCK_DETECT ) );
}


//******************************************************************
//	KeyLedOn
//
void IO_ControlClass::KeyLedOn( void )
{
#if PLATINE_VERSION > 4
	PORTB &= ~(1 << KEY_LED);
#elif PLATINE_VERSION == 4
	PORTD &= ~(1 << KEY_LED);
#else
	return( false );
#endif 
}


//******************************************************************
//	KeyLedOff
//
void IO_ControlClass::KeyLedOff( void )
{
#if PLATINE_VERSION > 4
	PORTB |= (1 << KEY_LED);
#elif PLATINE_VERSION == 4
	PORTD |= (1 << KEY_LED);
#else
	return( false );
#endif 
}


//******************************************************************
//	IsKeyLedOn
//
bool IO_ControlClass::IsKeyLedOn( void )
{
#if PLATINE_VERSION > 4
	return( 0 == (PINB & (1 << KEY_LED)) );
#elif PLATINE_VERSION == 4
	return( 0 == (PIND & (1 << KEY_LED)) );
#else
	return( false );
#endif
}


//******************************************************************
//	KeyRelaisOn
//
void IO_ControlClass::KeyRelaisOn( void )
{
#if PLATINE_VERSION > 3
	PORTD &= ~(1 << KEY_REL);
#endif
}


//******************************************************************
//	KeyRelaisOff
//
void IO_ControlClass::KeyRelaisOff( void )
{
#if PLATINE_VERSION > 3
	PORTD |= (1 << KEY_REL);
#endif
}


//******************************************************************
//	IsReset
//
bool IO_ControlClass::IsReset( void )
{
#if PLATINE_VERSION > 3
	return( 0 != g_clPortD.GetKeyLong( 1 << BLOCK_ON_OFF ) );
#else
	return( false );
#endif
}


//******************************************************************
//	IsBlockOnOff
//
bool IO_ControlClass::IsBlockOnOff( void )
{
#if PLATINE_VERSION > 3
	return( 0 != g_clPortD.GetKeyShort( 1 << BLOCK_ON_OFF ) );
#else
	return( false );
#endif
}


//******************************************************************
//	IsKeyIn
//
bool IO_ControlClass::IsKeyIn( void )
{
#if PLATINE_VERSION > 4
	return( 0 != g_clPortD.GetKeyState( 1 << KEY_IN ) );
#elif PLATINE_VERSION == 4
	return( 0 != g_clPortB.GetKeyState( 1 << KEY_IN ) );
#else
	return( false );
#endif
}


//******************************************************************
//	IsContact
//
bool IO_ControlClass::IsContact( void )
{
#if PLATINE_VERSION > 4
	return( 0 != g_clPortD.GetKeyState( 1 << CONTACT ) );
#elif PLATINE_VERSION == 4
	return( 0 != g_clPortB.GetKeyState( 1 << CONTACT ) );
#else
	return( false );
#endif
}


//******************************************************************
//	IsConfigKey
//
bool IO_ControlClass::IsConfigKey( void )
{
#if PLATINE_VERSION > 5
	return( 0 != g_clPortB.GetKeyState( 1 << CONFIG_1 ) );
#else
	return( false );
#endif
}


//******************************************************************
//	IsConfigKeyByBox
//
bool IO_ControlClass::IsConfigKeyByBox( void )
{
#if PLATINE_VERSION > 5
	return( 0 != g_clPortB.GetKeyState( 1 << CONFIG_2 ) );
#else
	return( false );
#endif
}


//******************************************************************
//	IsConfigRichtungsbetrieb
//
bool IO_ControlClass::IsConfigRichtungsbetrieb( void )
{
#if PLATINE_VERSION > 5
	return( 0 != g_clPortB.GetKeyState( 1 << CONFIG_3 ) );
#else
	return( false );
#endif
}
