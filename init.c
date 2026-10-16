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
	U1BAUDSEL_TRIS = 0x1;
	SS11_RLY_TRIS = 0;
	SS12_LED_TRIS = 0;

	SS11_RLY = 0x1;
	SS12_LED = 0x1;

	SS21_PV1_TRIS = 0;
	SS22_PV2_TRIS = 0;
	SS23_PV3_TRIS = 0;
	SS24_PV4_TRIS = 0;

	FUSED5V_TRIS = 1;

	TC_SHDN_TRIS = 0;
	TC_SHDN = 1;
	TC_FAULT_TRIS = 1;

	//set PSC spare to output
	TEST1_TRIS = 0x0;
	TEST2_TRIS = 0x0;
	TEST3_TRIS = 0x0;
 	TEST4_TRIS = 0x0;
	TEST5_TRIS = 0x0;
	TEST6_TRIS = 0x0;

	CONVOUTCNTL_TRIS = 0x0;
	CONVRST_TRIS = 0x0;
	CONVOUTCNTL = 0x0;		//by default, the CVTR output is disabled, 0=disabled, 1=output enabled
	CONVRST = 0x0;			//by default, the CVTR is initially powered off. For Mod CVTR, RST is Reset! logic high to reset

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
    
	//initialize peripherals
//	intcc1 = 123;
//	fracc1 = 0.910;
//
//	try();		//for debug use only
//	debuglamptest();
	UART1Init();
	UART2Init();
	INT1Init();
	I2C1Init();
	I2C2Init();
    I2C3Init();
	EEPROMInit();
	RTCLKInit();
	SPI1Init();
	SPI2Init();
	ANAInit();
	//USBInit();
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

void EEPROMInit()
{
	double soc1, soc2;
	double StringFullCharge;
	unsigned char i2cdata[15];
	unsigned char i2cdataSend[1]={};

	StringFullCharge = FULLCHARGE/2;
	//get the stored Coulomb Counts (cleared after FAT or revised after Ext Chrg) and calculate the battery bank SOC:

	intcc1 = get_battchrg_level(BATTLVL1_REGADDR,&fracc1);
	intcc2 = get_battchrg_level(BATTLVL2_REGADDR,&fracc2);

	soc1 = 100*(StringFullCharge + (intcc1+fracc1))/StringFullCharge;
	soc2 = 100*(StringFullCharge + (intcc2+fracc2))/StringFullCharge;
	batt_chg_status = (soc1+ soc2)/StringFullCharge;
 	if(batt_chg_status > 100.0)
	{
		batt_chg_status = 100.0;   	//battery bank average state of charge in percentage
	}
	if(batt_chg_status < 0.0)
	{
		batt_chg_status = 0.0;
	}

	//get the stored Charge Threshold Level
	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,CHRGCTRL_REGADDR,2,i2cdata);
	chrg_thrshld = hexbyte2dec(i2cdata[1]) + hexbyte2dec(i2cdata[0])*0.01;

	//get the stored CVTR Output Disable Threshold Level
	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,CVTRCVL_REGADDR,4,i2cdata);
	cvtr_cvl_enab = hexbyte2dec(i2cdata[1]) + hexbyte2dec(i2cdata[0])*0.01;
	cvtr_cvl_disab = hexbyte2dec(i2cdata[3]) + hexbyte2dec(i2cdata[2])*0.01;
	//get the store SWGF delay from CVTR registers
	if(i2c_getstat1(CVTR_CH,CVTR_SADDR,CVTR_SWGF_DLY_REG,2,i2cdata))
	{
		swgf_dly = 1;
	}
	else
	{
		swgf_dly = (hexbyte2dec(i2cdata[1]) * 100)+ hexbyte2dec(i2cdata[0]);
	}

	//get the stored board configuration
	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,BRDCONF_REGADDR,1, (unsigned char *)i2cdata);
	brd_config = i2cdata[0]&0x01;

	//PSC 2.0: ensure external PV controllers are being used in bypass mode:
	//PVCONFIG_REGADDR is the address which checks the PV's bitmask for 'bypass' or 'Boost' mode.
	// Bypass Mode: Power comes in through bypass input to relay
	// Boost Mode:  Power comes into boost chips.  (for PSC 2.0 we are in bypass mode, as these chips are catching on fire in boost mode.)
	i2cdataSend[0] = PV1MASK + PV2MASK + PV3MASK + PV4MASK;  //0x3C3C  This puts us in bypass mode (PVConfig is the function that checks this)
	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,PVCONFIG_REGADDR,i2cdataSend,1);
	//Hard coded onmask and offmask in commands.c, so we won't be checking the PVCONFIG_REGADDR. AHF 3/2/20
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

void SPI1Init()
{
	SPI1_SDI_AD = 1;
	SS12_LED = 1;
	SS11_RLY = 1;
}

void SPI2Init()
{
	SS21_PV1 = 1;
	SS22_PV2 = 1;
	SS23_PV3 = 1;
	SS24_PV4 = 1;
}

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

void USBInit()
{

}

void debuglamptest()
{
		SPI1tx(0x00FF,LED);
		__delay_ms(500);
		SPI1tx(0xFF00,LED);
}

void resetcontrol()
{
	//if it is POR
	if((RCONbits.POR == 1) && (RCONbits.BOR == 1) && (RCONbits.EXTR == 0))
	{
		//get override flag from EEPROM
		get_override_state();
		rlydrive(ALLMASK, 0);
		//get relay state from EEPROM
		//restore relay state accordingly
	//	RestoreRlyState();

		//reset datastream frequency to the default setting (DONE AFTER THIS MODULE).
		//set PSC Power On Reset Flag
		error_flag_3 |= PSCPORSTFLG;
	}
	//if it is hardreset
	if((RCONbits.POR == 1) && (RCONbits.BOR == 1) && (RCONbits.EXTR == 1))
	{
		//set PSC Reset Flag
		error_flag_3 |= PSCRESETFLG;

		//shunt both wind turbines
		//turn all relay off
		//	RestoreRlyState();
		rlydrive(ALLMASK, 0);    // For fuel cell come up with all relays off
		//store relay state to EEPROM
		//reset override flag to 0x3ff
		//store override flag to EEPROM
		//reset datastream frequency to the default setting.
	}
}

void RestoreRlyState()
{
	int storedrlystate, rlycntl, ledcntl,pvconfig_mask;
	char rly[2];

	//reading the relay on/off states from EEPROM
	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,RLYST_REGADDR,1,(unsigned char *)rly); 	//get from I2C channel 1, slave address 0xA0, register addr 0x01h, read one byte of relay state.
	storedrlystate = rly[0]&0xff;
	pvconfig_mask=allPVConfig();
	rlycntl = storedrlystate & ~pvconfig_mask; 		//<--fixed for external PV cntl config!
	ledcntl = ((~storedrlystate)<< 8) | (storedrlystate&0xFF);
	rlystat = ledcntl;
	__delay_ms(40);
	SPI1tx(rlycntl,RLY);			//restore the relay state
	SPI1tx(ledcntl,LED);			//update the LED indicators
	__delay_ms(40);
	SPI1tx(0x0,RLY);				//remove the driver control
}

int try()
{
//Simulate Battery Charging coulomb Count:
//	double input = 30145.9;
//	double bt_v = 25.00, bt_t = 27.0;
//	double bt1_i=8099,bt2_i=8099,bt3_i=8099,bt4_i=8099;
//	char bt_status[120],bt_detail[120];
//	chk_bt(bt_status,bt_detail,&bt_v,&bt_t);
//	long intpart = -123;
//	double frapart = -0.012;

//	*intpart = -123;
//	*frapart = -0.001;

//	UpdateCoulombCount(input,&intcc1,&fracc1);
//	soc1 = 100*(FULLCHARGE/4 + (intcc1+fracc1))/FULLCHARGE/4;
//	set_battchrg_level1(3,intcc1,fracc1);
//	override_flag = 0x3c;
//	rlystat = 0x03;

//	ValleyBottom = -650490;
//	while(1)
//	{
//		batterycharging(bt_t,bt_v,bt1_i);
//		secondcnt++;
//	}

//Simulate Power Source Fuse Blown Flag:
//	bp_v = 24;
//	rlystat = 0xff;
//	flagRelayFault(5,30,WT1MASK,WT1RELAYFLT);

//Simulate Large Current Charge/Discharge
//	double vbsimvalue = -123.839;
//	set_valleybtm(VALLEYBTM_REGADDR,vbsimvalue);

//	ValleyBottom = get_valleybtm(VALLEYBTM_REGADDR);

//	char tempstr[50]={};

//	strcpy(tempstr,cvt_err_detail[1]);
	return 0;
}
