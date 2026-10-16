/**********************************************************************
*    Woods Hole Oceanographic Institution/Raytheon, 2010
*
* FileName:        	psc.c
* Dependencies:     Header (.h) files if applicable, see code below
* Processor:       	PIC24FJ256GB106
* Compiler:        	MPLAB C30 v3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev.		Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1			Joseph Zhu    	10/28/2010		Initial PSC Firmware (STD)
* 2 		Joseph Zhu		08/09/2011		Special Build for Power Bench Test
*											change the FULLCHARGE in user.h
* 2.1		Joseph Zhu		08/11/2011		Update the SOC calculation formula
*											in init.c and command.c
* 14		John Reine		07/11/2018		PSC 2.0				
											-- commented out 'automatic' functionality
* 15    Alex Franks     12/14/2019          Changed default interrupt to internal timer based
*                                           Default system interrupts now reset instead of while(1) nop
*                                           Uart1 RX routine set to correct buffer size (100)
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code Tested on:
* WHOI PSC Firmware Development Board with PIC24FJ256GB106 controller
* The Processor runs on 32 Mhz external crystal, communicates over
* UART1 in 115200 baud rate at 8N1 with no hardware flow control
**********************************************************************/
#define USE_AND_OR
#define SYSCLK				32000000	//External OSC Frequency
#define HW_BOARD_VERSION	1			//HW VERSION -- Dev Board
#define RX_BUFFER_SIZE  100
//#define devboard			//<--delete this when not using PSC Firmware Develop Board

#include "psc.h"

//*************** Interrupt Service routine for UART1 Transmission *************************************
void __attribute__ ((interrupt,no_auto_psv)) _U1TXInterrupt(void)       //platcon
{
  static unsigned int i=0;

  if(UART1_TXDATA[i]!=NULL)
  {
   	while(BusyUART1());								//wait till the UART is busy
   	WriteUART1((unsigned int)UART1_TXDATA[i++]);		//Transmit the data
  }
  else
  {
	i=0;
	IEC0bits.U1TXIE = 0;		//disable the UART TX interrupt after end of complete transmission
   }
}

//*************** Interrupt Service routine for UART1 reception *************************************
void __attribute__ ((interrupt,no_auto_psv)) _U1RXInterrupt(void)       //platcon
{
	static unsigned int j=0; 
 	U1RX_Clear_Intr_Status_Bit;  					//clear the interrupt status of UART1 RX     
    
   	while(!DataRdyUART1());							//wait for data reception on RX
	Rxdata1[j] = ReadUART1();						//Read the data from UART buffer
   	if(Rxdata1[j] == '\n')
	{
   		DataavailableFlag = 1;						//Set the DataAvailability flag after complete reception
		j=0;
	}
	else if(Rxdata1[j] == '\b')
	{
		if(j>0)	{j--;}
	}
	else
	{
		if(j<RX_BUFFER_SIZE) {j++;}
	}
        if(U1STAbits.OERR == 1)     // If Rx FIFO overflows
        {
            j=0;                        // reset buffer index
            //memset(Rxdata1,NULL,100);     // clear buffer
            U1STAbits.OERR = 0;         //  reset overflow error flag
            UART1Init();                 // reinitialize
        }
}
//*************** Interrupt Service routine for UART2 Transmission *************************************
void __attribute__ ((interrupt,no_auto_psv)) _U2TXInterrupt(void)       //fuel cell
{
  static unsigned int i=0;

  if(UART2_TXDATA[i]!=NULL)
  {
   	while(BusyUART2());								//wait till the UART is busy
   	WriteUART2((unsigned int)UART2_TXDATA[i++]);		//Transmit the data
  }
  else
  {
	i=0;
	IEC1bits.U2TXIE = 0;		//disable the UART TX interrupt after end of complete transmission
   }
}

//*************** Interrupt Service routine for UART2 reception *************************************
void __attribute__ ((interrupt,no_auto_psv)) _U2RXInterrupt(void)       //fuel cell
{
	static unsigned int j=0; 
 	U2RX_Clear_Intr_Status_Bit;  					//clear the interrupt status of UART2 RX     
    
   	while(!DataRdyUART2());							//wait for data reception on RX
	Rxdata2[j] = ReadUART2();						//Read the data from UART buffer

	if(Rxdata2[j] == '\n')
	{
		j=0;
	}
	else if(Rxdata2[j] == '\b')
	{
		if(j>0)	j--;
	}
	else
	{
		if(j<200) j++;
	}

	if(U2STAbits.OERR == 1)     // If Rx FIFO overflows
    {
        j=0;                        // reset buffer index
        //memset(Rxdata2,NULL,200);     // clear buffer
        U2STAbits.OERR = 0;         //  reset overflow error flag
        UART2Init();                 // reinitialize
    }
}
//**************** Interrupt Service routine for INT1 reception *************************************
void __attribute__ ((interrupt,no_auto_psv)) _INT1Interrupt(void)
{                                                   //this ISR is for the RTC 1 second external interrupt
    IFS1bits.INT1IF = 0;                        	//clear the interrupt flag
    DatastreamFlag = 1;                          	//set Data Stream Flag
    rtclk_reset();                                    //reset the RTC alarm flags so they can trigger an interrupt
}
//**************** Interrupt Service routine for internal 1 sec timer *******************************
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{                                               //this ISR is for the INTERNAL 1 second timer interrupt
    IFS0bits.T3IF = 0;                        	//clear the interrupt flag
    DatastreamFlag = 1;                        	//set Data Stream Flag
}
//**************** PSC command to set data stream flag manually **************************************
void setdsf()
{
    DatastreamFlag=1;                           //set Data Stream Flag
}
//**************** PSC command to pick interrupt source *********************************************
void set_timing_external(int clock_source)
{
    switch(clock_source) //A value of 1 configures interrupt priority so that the external interrupt is used - system defaults to internal in main with a value of 0
	{
		case 0:
            IPC5bits.INT1IP = 0x0; //external clock interrupt disabled
            IPC2bits.T3IP = 0x4; //internal timer priority 4
			break;
		case 1:
            IPC2bits.T3IP = 0x0; //internal timer interrupt disabled
            IPC5bits.INT1IP = 0x4; //external clock interrupt priority 4
            rtclk_reset();      //reset the RTC alarm flags so they can trigger an interrupt
			break;
		default:
			break;
	}
}
//**************** Interrupt Service routine for A/D ************************************************
void __attribute__((__interrupt__, auto_psv)) _ADC1Interrupt( void )
{
	ADC1_Clear_Intr_Status_Bit;
	ADCFinished = 1;
}

/*******************************************************************************
Function:       void __attribute__((__interrupt__, auto_psv)) _DefaultInterrupt(void)

This is just here to catch any interrupts that we see during debugging.

*******************************************************************************/

void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void)
{
  strcpy(UART1_TXDATA, "!!! Default interrupt handler !!!\r\n" );
  tx_comm();
  asm("RESET");
}

void __attribute__((interrupt, auto_psv)) _OscillatorFail(void)
{
  strcpy(UART1_TXDATA, "!!! Oscillator Fail interrupt handler !!!\r\n" );
  tx_comm();
  asm("RESET");
}
void __attribute__((interrupt, auto_psv)) _AddressError(void)
{
  strcpy(UART1_TXDATA, "!!! Address Error interrupt handler !!!\r\n" );
  tx_comm();
  asm("RESET");
}
void __attribute__((interrupt, auto_psv)) _StackError(void)
{
  strcpy(UART1_TXDATA, "!!! Stack Error interrupt handler !!!\r\n" );
  tx_comm();
  asm("RESET");
}
void __attribute__((interrupt, auto_psv)) _MathError(void)
{
  strcpy(UART1_TXDATA, "!!! Math Error interrupt handler !!!\r\n" );
  tx_comm();
  asm("RESET");
}

//****************** Main PSC Routine ***************************************************************
int main(void)
{
	io_config();									//assign IOs
        sprintf(Txdata, "WHOI PSC 2.0 %s Rev %d%s",rev_date,FWRevision,rev_letter);
	strcpy(UART1_TXDATA,Txdata);
	chksumgen();

	init();											//initialization and setup

	ClrWdt();

	set_timing_external(0);                                //default to internal timer based interrupt
	datastream(inhibitdatastream);					//get status data and TX via UART1
	//sprintf(UART2_TXDATA ,"#DBG boot fc  uart2  \r\n");
	//				IEC1bits.U2TXIE = 1;


	while(1)
	{
		if(DatastreamFlag) 							//1Hz Data Stream Interrupted
		{
			datastream(inhibitdatastream);
			DatastreamFlag = 0;
			secondcnt++;
			ClrWdt();
		}
		if(ADCFinished == 1)
		{
			DisableADC1;
			MonitorVBUS(ADC1BUF0);
			ADCFinished = 0;
			ClrWdt();
		}
		if(DataavailableFlag)						//UART1 RX Command Received
		{
			response();
			DataavailableFlag = 0;
			ClrWdt();
		}
                //AHF check voltage here, if 30V, disconnect input relays and be verbose about it
                //We could add a check in the datastream function where it calls chk_bp, but if the interrupts fail
                //The check won't be executed
                //Therefore call chk_bp via new function in response.c
                failsafe_count++;
                if(failsafe_enable & (failsafe_count >= 500000)) //500000 is roughly once per second. User can set failsafe_enable via serial/UCMD in response.c
                {                                                //using a manual counter here instead of a timer/interrupt for less moving parts
                    voltage_failsafe();                          //just in case "all else fails"
                    failsafe_count = 0;
                }
	}
}

/*************************************************************************************************
*	EOF
**************************************************************************************************/
