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
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES
* This file consists of the IO definitions 
*
*************************************************************************/
#include "PPS.h"

//#define devboard
#define psc

//define IO for PSC 
	//define digital discrete input
		//for baud rate selector input
	#define U1BAUDSEL PORTCbits.RC13		
	#define U1BAUDSEL_TRIS TRISCbits.TRISC13
	#define TEST7_TRIS TRISEbits.TRISE2	//Used as HV_OUT_INTLCK in Demo
	#define TEST7 PORTEbits.RE2
	#define TC_FAULT_TRIS TRISBbits.TRISB3
	#define TC_FAULT PORTBbits.LATB3

	//define digital discrete output
		//for Slave Select output
	#define SS11_RLY_TRIS TRISBbits.TRISB9
	#define SS11_RLY LATBbits.LATB9
	#define SS12_LED_TRIS TRISBbits.TRISB14
	#define SS12_LED LATBbits.LATB14

	#define SPI1_SDI_AD AD1PCFGLbits.PCFG6

	#define SS21_PV1_TRIS TRISDbits.TRISD4
	#define SS21_PV1 LATDbits.LATD4
	#define SS22_PV2_TRIS TRISDbits.TRISD5
	#define SS22_PV2 LATDbits.LATD5
	#define SS23_PV3_TRIS TRISDbits.TRISD0
	#define SS23_PV3 LATDbits.LATD0
	#define SS24_PV4_TRIS TRISDbits.TRISD11 
	#define SS24_PV4 LATDbits.LATD11
	
	#define FUSED5V_TRIS TRISBbits.TRISB4

//	#define TEST7_TRIS TRISEbits.TRISE2	//commented out, used as input in Demo
//	#define TEST7 LATEbits.LATE2
	#define TEST8_TRIS TRISEbits.TRISE3	//used in demo as 300V_DIS_LED
	#define TEST8 LATEbits.LATE3	
	#define TEST9_TRIS TRISEbits.TRISE4	//used in demo as HV_INPUT_CTRL_LED
	#define TEST9 LATEbits.LATE4		
	#define TEST10_TRIS TRISEbits.TRISE5	//used in demo as SOMETHING_LED
	#define TEST10 LATEbits.LATE5		

	#define TC_SHDN_TRIS TRISBbits.TRISB10		//spare UART transceiver control lines
	#define TC_SHDN LATBbits.LATB10

	//define all unused pin to digital output
	#define TEST1_TRIS TRISBbits.TRISB11
	#define TEST1 LATBbits.LATB11
	#define TEST2_TRIS TRISBbits.TRISB12
	#define TEST2 LATBbits.LATB12
	#define TEST3_TRIS TRISBbits.TRISB13
	#define TEST3 LATBbits.LATB13
	#define TEST4_TRIS TRISBbits.TRISB15
	#define TEST4 LATBbits.LATB15
	#define TEST5_TRIS TRISFbits.TRISF3
	#define TEST5 LATFbits.LATF3
	#define TEST6_TRIS TRISCbits.TRISC14
	#define TEST6 LATCbits.LATC14
//	#define TEST7_TRIS TRISEbits.TRISE2	//used in demo as 300V_DIS_LED
//	#define TEST7 LATEbits.LATE2
//	#define TEST8_TRIS TRISEbits.TRISE3	//used in demo as HV_INPUT_CTRL_LED
//	#define TEST8 LATEbits.LATE3	
//	#define TEST9_TRIS TRISEbits.TRISE4	//used in demo as SOMETHING_LED
//	#define TEST9 LATEbits.LATE4		
//	#define TEST10_TRIS TRISEbits.TRISE5	//used as HV_INPUT_INTLCK input in demo
//	#define TEST10 LATEbits.LATE5		


//define Digital Output common to Devboard and PSC 
#define CONVOUTCNTL LATFbits.LATF0
#define CONVRST LATFbits.LATF1

#define CONVOUTCNTL_TRIS TRISFbits.TRISF0
#define CONVRST_TRIS TRISFbits.TRISF1

//prototypes
void io_config();
void remap_led_ce();
void remap_rly_ce();
void remap_pv1_ce();
void remap_pv2_ce();
void remap_pv3_ce();
void remap_pv4_ce();
