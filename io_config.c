/**********************************************************************
*    WHOI/Raytheon, 2010
*
* FileName:        	io_config.c
* Dependencies:     Header (.h) files if applicable, see code below
* Processor:       	PIC24FJ256GB106
* Compiler:        	MPLAB C30 v3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev. 	Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1		Joseph Zhu    	10/28/2010		Initial File Creation 
* 
* MPSC Rework:
* REV 1 Dan B/Paul W    10/19/2026          REV01 version for New Mini-PSC CCA
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This routine consists of the PPS peripheral pin assignments
*
**********************************************************************/
#include "io_config.h"

void io_config()
{

	//unlock PPS assignment
	__builtin_write_OSCCONL(OSCCON & 0xBF);	
	
	//Assign UART 1 signals onto pins using PPS 
	iPPSInput(IN_FN_PPS_U1RX,IN_PIN_PPS_RP26);			//U1RX to pin 5 (RP26)
	iPPSInput(IN_FN_PPS_U2RX,IN_PIN_PPS_RP21);			//U2RX to pin 4 (RP21)
	iPPSOutput(OUT_PIN_PPS_RP27,OUT_FN_PPS_U1TX);		//U1TX to pin 8 (RP27)
	iPPSOutput(OUT_PIN_PPS_RP19,OUT_FN_PPS_U2TX);		//U2TX to pin 6 (RP19)  

	//lock PPS Assignment
	__builtin_write_OSCCONL(OSCCON | 0x40);
}


