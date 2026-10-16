/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			chksum.h
* Dependencies:
* Processor:       	PIC24FJ256GB106
* Compiler:			MPLAB C30 V3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev. 	Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1		Joseph Zhu    	10/28/2010		Initial File Creation 
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES
* This file consists of the comm.c header 
*
*************************************************************************/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define uint16_t unsigned int

extern char UART1_TXDATA[2048];
extern char *UART1_RXDATA;

//prototype
int chksumchk(char *chksum, char *cmd);
void chksumgen();
