/**********************************************************************
*    WHOI/Raytheon, 2010
*
* FileName:        	usb_function.c
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
* 
*
**********************************************************************/
#include "usb_function.h"

/****************************************************************************
  Function:
    void MonitorVBUS( void )

  Description:
    This routine monitors VBUS to check for overcurrent conditions.  
    The valid range for Vbus is 4.4V - 5.25V. If we are in an overcurrent 
	condition, Vbus will be lower than 4.4V. Full range on the A/D is 0x3FF 
    for 3.3V. So any value lower than 0xnnn on AN4 is overcurrent or fused-5V
	fuse is blown, which will flag the fused_5V fuse blown in error_flag.  
	Otherwise, the current level is fine.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    TRUE    - An overcurrent situation exists. flag error-flag.
    FALSE   - Normal USB operation.

  Remarks:
    Since Vbus monitoring in the PSC can only monitor the USB 5V power and there is 
	no control over the Fused_5V to the USB Bus, there is no action taken to shut 
	off the 5V USB power. Warning: The source of the Fused_5V can source way more than 
	the 500mA called by the USB standard. 
  ***************************************************************************/
#define OVERCURRENT_RESET_VOLTAGE   4800ul        // In millivolts <--threshold used to reset flag
#define OVERCURRENT_TRIP_VOLTAGE    4400ul        // In millivolts <--threshold used to set flag
#define SYSTEM_VOLTAGE              3300ul        // In millivolts
#define OVERCURRENT_RESET_READING   (1023ul * (OVERCURRENT_RESET_VOLTAGE/2) / SYSTEM_VOLTAGE)
#define OVERCURRENT_TRIP_READING    (1023ul * (OVERCURRENT_TRIP_VOLTAGE/2) / SYSTEM_VOLTAGE)

void MonitorVBUS(unsigned int ADCResult )
{
	double ADCReading=0;

	ADCReading = ADCResult*3.3/1023.0;
    if (ADCResult < OVERCURRENT_TRIP_READING)
    {
		//voltage sag below threshold, raise the fused_5V fuse blown flag
		error_flag_3 |= FUSED5VFUSE;
    }
    else if (ADCResult > OVERCURRENT_RESET_READING)
    {
		//voltage above power-good level, clear the fused_5V fuse blown flag
		error_flag_3 &= ~FUSED5VFUSE;
	}
}


