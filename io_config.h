/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			io_config.h
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
* MPSC Rework:
* REV 1 Dan B/Paul W    10/19/2026          REV01 version for New Mini-PSC CCA
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES
* This file consists of the IO definitions 
*
*************************************************************************/
#include "PPS.h"

#define psc

//define IO for PSC 

    //for baud rate selector input PIN47
	#define U1BAUDSEL PORTCbits.RC13		
	#define U1BAUDSEL_TRIS TRISCbits.TRISC13

    //MPSC PFET Enable channels for PV Inputs
    //PV1 Enable PIN 62
    #define EN_PV_CH1 PORTEbits.RE2
    #define EN_PV_CH1_TRIS TRISEbits.TRISE2
    //PV2 Enable PIN 63
    #define EN_PV_CH2 PORTEbits.RE3
    #define EN_PV_CH2_TRIS TRISEbits.TRISE3
    //PV3 Enable PIN 64
    #define EN_PV_CH3 PORTEbits.RE4
    #define EN_PV_CH3_TRIS TRISEbits.TRISE4
    //PV4 Enable PIN 1
    #define EN_PV_CH4 PORTEbits.RE5
    #define EN_PV_CH4_TRIS TRISEbits.TRISE5

    //LED_EN Output
    #define LED_EN PORTDbits.RD1
    #define LED_EN_TRIS TRISDbits.TRISD1


//prototypes
void io_config();
