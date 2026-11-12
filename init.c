/**********************************************************************
*    WHOI/Raytheon, 2010
*
* FileName:        	init.c
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
* This is the initialization routine which sets up the discrete IOs,
*	initializes the prepherals, and restores the critical parameters
**********************************************************************/
#include "init.h"

void init()
{
	//initialize IO:

    //Set as Input
	U1BAUDSEL_TRIS = 1;
	
    //Set Enables as Outputs
    EN_PV_CH1_TRIS = 0;
    EN_PV_CH2_TRIS = 0;
    EN_PV_CH3_TRIS = 0;
    EN_PV_CH4_TRIS = 0;

    //Set LED_EN as output
    LED_EN_TRIS = 0;
    
	secondcnt = 0;
    
	//AHF initialize timer for internal interrupt
	//borrowed from MPIC code, timers.c

	T2CONbits.TON = 0;
	T2CON = 0;          //Stops any 16/32-bit Timer2 operation
	T3CON = 0;          //Stops any 16-bit Timer3 operation
	TMR3 = 0;           //Clear contents of the timer3 register
	TMR2 = 0;           //Clear contents of the timer2 register
	PR3 = 0b11110100;         //set 32 bit period register to (FCY-1)
	PR2 = 0b0010001111111111; //111101000010001111111111 = 15999999
	IFS0bits.T3IF = 0;  //Clear the Timer3 interrupt status flag
	IEC0bits.T3IE = 1;  //Enable Timer3 interrupts
	T2CONbits.T32 = 1;  //Enable 32 bit mode
	T2CONbits.TON = 1;  //Start 32-bit timer
    
	UART1Init();
	UART2Init();
	INT1Init();
	I2C1Init();
	I2C2Init();
    I2C3Init();
//	EEPROMInit();
//	RTCLKInit();
	ANAInit();
	resetcontrol();			//<-- determine which reset and acting accordingly

	//initialziation parameters
	datastreamcnt_loc = datastreamcnt;
}

void UART1Init()
{
	int BAUDRATE = 35;				//FOR 111111 BAUDRATE to match MPIC
	//int BAUDRATE = 51;              //AHF for 19200 with UART_BRGH_SIXTEEN
	CloseUART1();					//disable UART1

    CLKDIV = 0x0000;    			// Set PLL prescaler (1:1), for using Internal RC to generate 32MHz System OSC

	if(U1BAUDSEL == 0) 				//if 9600 baud rate jumper is installed
	{
		BAUDRATE = 416;				//uart1 baud rate sets to 9600 baud
	//	inhibitdatastream = ON;		//stop the auto-datastream
	}

	/*********************** UART 1 Configuration ********************
	*		111111 baud to match MPIC (0.33% err)
	*		8 bit transmission/reception
	*		No parity bit
	*		1 stop bit
	******************************************************************/
	//setup UART1 (both RX and TX interrupt priority are set to 6)
    ConfigIntUART1(UART_RX_INT_EN |UART_RX_INT_PR6 | UART_TX_INT_EN |UART_TX_INT_PR6);
    OpenUART1(UART_EN | UART_BRGH_FOUR , UART_TX_ENABLE, BAUDRATE); //AHF use UART_BRGH_SIXTEEN for 19200, change to UART_BRGH_FOUR for 115200 or 111111
}

void UART2Init()
{
	int BAUDRATE = 416;				//FOR 9600 BAUDRATE

	CloseUART2();					//disable UART1

    CLKDIV = 0x0000;    			// Set PLL prescaler (1:1), for using Internal RC to generate 32MHz System OSC

//	inhibitdatastream = ON;		//stop the auto-datastream


	/*********************** UART 2 Configuration ********************
	*		9600 baud
	*		8 bit transmission/reception
	*		No parity bit
	*		1 stop bit
	******************************************************************/
	//setup UART2 (both RX and TX interrupt priority are set to 7)
    ConfigIntUART2(UART_RX_INT_EN |UART_RX_INT_PR7 | UART_TX_INT_EN |UART_TX_INT_PR7);
    OpenUART2(UART_EN | UART_BRGH_FOUR , UART_TX_ENABLE, BAUDRATE);
}

void INT1Init()
{
    CloseINT1();											//Disable INT1 if enabled previously
	ConfigINT1(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_5); 	//set priority to 5
}
void I2C1Init()
{

}

void I2C2Init()
{

}

void I2C3Init()
{
    
}
#if 0
void RTCLKInit()
{
	unsigned char CntlStatData[2]={0x07,0x40};
	unsigned char Alarm1Setting[4]={0x80,0x80,0x80,0x80};
	unsigned char DefaultDate[7]={0x10,0x23,0x18,0x06,0x23,0x05,0x04};	//Hobbes Birthday

	//setup alarm to 1Hz output
	i2c_putstat1(RTCLKi2c_CH,RTCLK_SADDR,0x07,Alarm1Setting,4);
	//setup control and status reg
	i2c_putstat1(RTCLKi2c_CH,RTCLK_SADDR,0x0e,CntlStatData,2);
	//setup default time and date
	i2c_putstat1(RTCLKi2c_CH,RTCLK_SADDR,0x00,DefaultDate,7);
}
#endif
void ANAInit()
{
	unsigned int channel,config1,config2,config3,configportl,configporth=0,configscanl=0,configscanh=0;

	CloseADC10();	//Turn off ADC in case if it was operational previously

	/************** ADC configuration ***********************************************************
	*
	*	Use AN4 channel for siganl sampling
	*	ADC uses internal RC as source of clock
	*	Auto scan is enabled
	*	conversion clcok is selected to 254Tcy
	*	Sampling interval is selected to 17 TAD
	*	ADC interrupt after EACH conversions
	*******************************************************************************************/
	channel = ADC_CH0_POS_SAMPLEA_AN4;
	config1 = ADC_MODULE_OFF | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON ;
	config2 = ADC_SCAN_ON | ADC_INTR_EACH_CONV ;
	config3 = ADC_SAMPLE_TIME_17 | ADC_CONV_CLK_254Tcy;
	configportl = 0x0000;
	configscanl = ADC_SCAN_AN4 ;
	OpenADC10(config1,config2,config3,configportl,configporth,configscanl,configscanh);
	ConfigIntADC10(ADC_INT_ENABLE|ADC_INT_PRI_5); 		//configure ADC interrupt}

	SetChanADC10(channel);    	//Set the channel to AN4
}


void resetcontrol()
{
	//if it is POR
	if((RCONbits.POR == 1) && (RCONbits.BOR == 1) && (RCONbits.EXTR == 0))
	{
		error_flag_3 |= PSCPORSTFLG;
	}
	//if it is hardreset
	if((RCONbits.POR == 1) && (RCONbits.BOR == 1) && (RCONbits.EXTR == 1))
	{
		//set PSC Reset Flag
		error_flag_3 |= PSCRESETFLG;
	}
}

