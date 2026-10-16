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
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This routine consists of the PPS peripheral pin assginments
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

	//Assign INT1 to Pin42 using PPS
    iPPSInput(IN_FN_PPS_INT1,IN_PIN_PPS_RP2);    		//INT1 to pin 42 (RP2) 
	
	//Assign SPI 1 signals onto pins using PPS
	iPPSInput(IN_FN_PPS_SDI1,IN_PIN_PPS_RP6);			//SDI 1 to pin 17 (RP6)
	iPPSOutput(OUT_PIN_PPS_RP7,OUT_FN_PPS_SDO1);		//SDO 1 to pin 18 (RP7)
	iPPSOutput(OUT_PIN_PPS_RP8,OUT_FN_PPS_SCK1OUT);		//SCK 1 to pin 21 (RP8)
//	iPPSOutput(OUT_PIN_PPS_RP9,OUT_FN_PPS_SS1OUT);		//SS 1 to pin 22 (RP9) as SS1_RLY_CE by default for Relay drivers
	//iPPSOutput(OUT_PIN_PPS_RP14,OUT_FN_PPS_SS1OUT);	//SS 1 to pin 29 (RP14) as SS1_LED_CE for LED drivers

	//Assign SPI 2 signals onto pins useing PPS
	iPPSInput(IN_FN_PPS_SDI2,IN_PIN_PPS_RP24);			//SDI 2 to pin 49 (RP24)
	iPPSOutput(OUT_PIN_PPS_RP23,OUT_FN_PPS_SDO2);		//SDO 2 to pin 50 (RP23)
	iPPSOutput(OUT_PIN_PPS_RP22,OUT_FN_PPS_SCK2OUT);	//SCK 2 to pin 51 (RP22)
//	iPPSOutput(OUT_PIN_PPS_RP25,OUT_FN_PPS_SS2OUT);		//SS 2 to pin 52 (RP25) as SS2_PV1_CE by default for PV1 controller
	//iPPSOutput(OUT_PIN_PPS_RP20,OUT_FN_PPS_SS2OUT);	//SS 2 to pin 53 (RP20) as SS2_PV2_CE for PV2 controller
	//iPPSOutput(OUT_PIN_PPS_RP11,OUT_FN_PPS_SS2OUT);	//SS 2 to pin 46 (RP11) as SS2_PV3_CE for PV3 controller
	//iPPSOutput(OUT_PIN_PPS_RP12,OUT_FN_PPS_SS2OUT);	//SS 2 to pin 45 (RP12) as SS2_PV4_CE for PV4 controller

	//lock PPS Assignment
	__builtin_write_OSCCONL(OSCCON | 0x40);
}

void remap_led_ce()
{
	__builtin_write_OSCCONL(OSCCON & 0xBF);	
	iPPSOutput(OUT_PIN_PPS_RP14,OUT_FN_PPS_SS1OUT);	//SS 1 to pin 29 (RP14) as SS1_LED_CE for LED drivers
	__builtin_write_OSCCONL(OSCCON | 0x40);
}	

void remap_rly_ce()
{
	__builtin_write_OSCCONL(OSCCON & 0xBF);	
	iPPSOutput(OUT_PIN_PPS_RP9,OUT_FN_PPS_SS1OUT);		//SS 1 to pin 22 (RP9) as SS1_RLY_CE by default for Relay drivers
	__builtin_write_OSCCONL(OSCCON | 0x40);
}

void remap_pv1_ce()
{
	__builtin_write_OSCCONL(OSCCON & 0xBF);	
	iPPSOutput(OUT_PIN_PPS_RP25,OUT_FN_PPS_SS2OUT);		//SS 2 to pin 52 (RP25) as SS2_PV1_CE by default for PV1 controller
	__builtin_write_OSCCONL(OSCCON | 0x40);
}	

void remap_pv2_ce()
{
	__builtin_write_OSCCONL(OSCCON & 0xBF);	
	iPPSOutput(OUT_PIN_PPS_RP20,OUT_FN_PPS_SS2OUT);		//SS 2 to pin 53 (RP20) as SS2_PV2_CE for PV2 controller
	__builtin_write_OSCCONL(OSCCON | 0x40);
}
void remap_pv3_ce()
{
	__builtin_write_OSCCONL(OSCCON & 0xBF);	
	iPPSOutput(OUT_PIN_PPS_RP11,OUT_FN_PPS_SS2OUT);		//SS 2 to pin 54 (RP11) as SS2_PV3_CE for PV3 controller
	__builtin_write_OSCCONL(OSCCON | 0x40);
}	

void remap_pv4_ce()
{
	__builtin_write_OSCCONL(OSCCON & 0xBF);	
	iPPSOutput(OUT_PIN_PPS_RP12,OUT_FN_PPS_SS2OUT);		//SS 2 to pin 45 (RP12) as SS2_PV4_CE for PV4 controller
	__builtin_write_OSCCONL(OSCCON | 0x40);
}
