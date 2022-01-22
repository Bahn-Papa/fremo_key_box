
#pragma once

//##########################################################################
//#
//#		compile_options.h
//#
//#	This file contains compile options:
//#		-	DEBUGGING_PRINTOUT
//#			If defined, debug info and/or text is shown on an OLED display
//#
//#		-	COMMAND_CONNECTION_LOCONET
//#			If defined, then commands to receive / revoke unlock permission
//#			are send over loconet
//#
//#		-	COMMAND_CONNECTION_DIN_CONNECTOR
//#			If defined, then commands to receive / revoke unlock permission
//#			will be handled by hardware with the help of a DIN connector.
//#
//##########################################################################


//==========================================================================
//
//		C O D E   S E L E C T I O N   D E F I N I T I O N S
//
//==========================================================================

#define DEBUGGING_PRINTOUT

//--------------------------------------------------------------------------
//	!!	ATTENTION	!!
//	Please uncomment only ONE of the following lines
//
#define COMMAND_CONNECTION_LOCONET
//define COMMAND_CONNECTION_DIN_CONNECTOR
