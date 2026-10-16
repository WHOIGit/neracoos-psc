/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			user.h
* Dependencies:
* Processor:       	PIC24FJ256GB106
* Compiler:			MPLAB C30 V3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev. 	Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1		Joseph Zhu    	10/28/2010		Initial File Creation 
* 2		Joseph Zhu		8/10/2011		change FULLCHARGE to 210AmpH for PBT
* 2-3	Joseph Zhu		8/16/2011		change FULLCHARGE to 180AmpH for PBT
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES
* This file consists of all user defined parameters which shared with all
* modules
*
*************************************************************************/

 #if defined(__PIC24FJ256GB106__)
/*************** COFIGURATION **************************************
*	Watchdog Timer Disabled with 1:128 Prescaler and 1:512 Postscale
*		WDTO is about 2 sec.
*	Two Speed Start-up enabled
*	Oscillator Selection: HS oscillator ( 32MHz crystal )
*	Clock switching and clock monitor both enabled
********************************************************************/
_CONFIG2(IESO_ON & PLLDIV_DIV2 & PLL_96MHZ_ON & FNOSC_FRCPLL & OSCIOFNC_ON ) 
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF & FWPSA_PR128 & WDTPS_PS512 & ICS_PGx1)
// _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & WINDIS_OFF) 
// _CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & DISUVREG_OFF & PLLDIV_DIV2 & PLL_96MHZ_ON & IOL1WAY_OFF)
#endif 

//others
#define ON 1
#define OFF 0

#define GO 1
#define STOP 0

//below line is for the AST2 PSC
//const long FULLCHARGE = 2638080;		//this is the total coulombs of 4 strings of battery at 8amp-22.9hour rate
// ppw 08122026 - TBD: Dan or Alex, change this (I simply divided by 2)
const long FULLCHARGE = 1319040;
//below line is for the Power Bench Test special build at 210 AmpH
//const long FULLCHARGE = 756000;		//this is the total coulombs of 1 string of battery at 10amp-21hour rate
//below line is for the Power Bench Test Special build at 180 AmpH
//const long FULLCHARGE = 648000;			//this is the total coulombs of 1 string of battery at 8amp-22.9hour rate

int OverAsbChgVolt = 0;		//default to FALSE
int OverCondChgVolt = 0;	//default to FALSE
int OverAsbChgCnt = 0; 
int OverCondChgCnt = 0;

int ADCFinished = 0;

//constant strings:
const char Firmware_Revision[] = "Rev -";
const char CMD_connect[] = "connect";
const char CMD_disconnect[] = "disconnect";
const char CMD_stop[] = "stop";
const char CMD_go[] = "go";
const char CMD_override[] = "override";
const char CMD_psc_reset[] = "psc_reset";
const char CMD_set_time[] = "set_time";
const char CMD_get_time[] = "get_time";
const char CMD_version[] = "version";
const char CMD_pv_status[] = "pv_status";
const char CMD_bt_status[] = "bt_status";
const char CMD_wnd_sp[] = "wnd_sp";
const char CMD_psc[] = "psc";
const char CMD_help[] = "help";

const char ID_pv1[] = "pv1";
const char ID_pv2[] = "pv2";
const char ID_pv3[] = "pv3";
const char ID_pv4[] = "pv4";
const char ID_bt1[] = "bt1";
const char ID_bt2[] = "bt2";
const char ID_conv[] = "conv";
const char ID_ext_pwr[] = "ext";
const char ID_psc_temp[] = "psc_temp";
const char ID_psc_htl[] = "int";
const char ID_buoy_pwr[] = "buoy_pwr";
const char ID_override[] = "override";
const char ID_hyd[] = "hyd";

const char ST_status[] = "status";
const char help_string[] = "command list";

const char ST_ok[] = "ok";
const char ST_error[] = "error";

//limits and thresholds
const double BATT_TEMP_LMT = 50.00;		//battery overtemp threshold in degree C
const double CVTR_TEMP_LMT = 90.00;		//HV PWR CVTR Overtemp Threshold in degree C
const double EXTPWR_PRESENT = 50.00;		//External Power Present current threshold in mA

//PSC Hardware Profile

const int BATTERYi2c_CH 	= 1;
const int BATT1_C_SADDR 	= 0xD0;
const int BATT1_D_SADDR 	= 0xCE;
const int BATT2_C_SADDR 	= 0xD4;
const int BATT2_D_SADDR 	= 0xD2;
const int EXTPWRi2c_CH 		= 1;
const int EXTPWR_SADDR 		= 0xDE;
const int PVi2c_CH 			= 2;
const int PV1_SADDR 		= 0xD2;
const int PV2_SADDR 		= 0xD4;
const int PV3_SADDR 		= 0xD6;
const int PV4_SADDR 		= 0xD8;
const int BUOYPWRi2c_CH 	= 2;
const int BUOYPWR_SADDR 	= 0xDE;
const int RTCLKi2c_CH 		= 2;
const int RTCLK_SADDR 		= 0xD0;
const int PSCHTLi2c_CH 		= 2;
const int PSCHTL_SADDR 		= 0xCE;
const int HYDi2c_CH         = 3;        // ppw 07302026 - Add support for i2c hydrogen sensor on i2c3, addr 05
//const int HYD_SADDR         = 0x05;
const int HYD_SADDR         = 0x0A;
const int EEPROMi2c_CH 		= 1;
const int EEPROM_SADDR 		= 0xA0;
const int RLYST_REGADDR 	= 0x01;		// Relay state status 0x01 => FC2-FC1-PV4-PV3-PV2-PV1-WT2-WT1, 0x02 => X-X-X-X-X-X-X-CVTR
const int OVERRIDE_REGADDR 	= 0x03;		// PSC override flags 0x03 => FC2-FC1-PV4-PV3-PV2-PV1-WT2-WT1, 0x04 => X-X-X-X-X-EXTPWR-X-CVTR
const int BRDCONF_REGADDR 	= 0x06;		// Used to store board configuration - CVTR installed?
const int PVCONFIG_REGADDR	= 0x07;		// PV config for internal or external controller (1 byte)
const int ELPSDTM_REGADDR 	= 0x10;		// 3 bytes
const int CVTRCVL_REGADDR	= 0x14;		// Converter enable and disable voltage thresholds (4 bytes)
const int BATTLVL1_REGADDR 	= 0x18;		// Batt 1 coulomb count (5 bytes)
const int BATTLVL2_REGADDR 	= 0x20;		// Batt 2 coulomb count	(5 bytes)
const int VALLEYBTM_REGADDR = 0x38;		// Max coulomb discharge (5 bytes)
const int CHRGCTRL_REGADDR	= 0x3D;		// Threshold for starting recharge cycle (2 bytes)
const int PN_REGADDR 		= 0x40;		// PSC part number (16 bytes)
const int SN_REGADDR 		= 0x60;		// PSC serial number (12 bytes)
const int HWREV_REGADDR 	= 0x70;		// PSC hardware revision (1 byte)
const int FWREV_REGADDR 	= 0x72;		// PSC firmware revision (1 byte)
const int CVTR_CH			= 3;
const int CVTR_SADDR		= 0x90;
const int CVTR_REG1			= 0x00;
const int CVTR_INTLCK_V_REG = 0x24;		//or 36 decimal
const int CVTR_OI_REG		= 0x2C;		//or 44 decimal
const int CVTR_OV_REG		= 0x2E;		//or 46 decimal
const int CVTR_CTRL_REG		= 0x3D;		//or 61 decimal
const int CVTR_SWGF_DLY_REG = 0x42;		//or 66 decimal
const int CVTR_SWGFREADING_P_REG = 0X30;//or 48 decimal      changed ver. 9  - read from updating registers
const int CVTR_SWGFREADING_N_REG = 0x32;//or 50 decimal

//FAULT TABLE
//for group 1 fault:
const unsigned int BT11OVERTEMP = 0x0001;	//Battery 1 of String 1 Overtemp
const unsigned int BT12OVERTEMP = 0x0002;	//Battery 2 of String 1 Overtemp
const unsigned int BT21OVERTEMP = 0x0004;	//Battery 1 of String 2 Overtemp
const unsigned int BT22OVERTEMP = 0x0008;	//Battery 2 of String 2 Overtemp
const unsigned int BATTSTR1FUSE = 0x0100;	//Battery String 1 Fuse Blown
const unsigned int BATTSTR2FUSE = 0x0200;	//Battery String 2 Fuse Blown
const unsigned int BT1CHGSENFLT = 0x1000;	//Battery String 1 Charging Sensor Fault
const unsigned int BT1DCHGSNFLT = 0x2000;	//Battery String 1 Disharging Sensor Fault
const unsigned int BT2CHGSENFLT = 0x4000;	//Battery String 2 Charging Sensor Fault
const unsigned int BT2DCHGSNFLT = 0x8000;	//Battery String 2 Disharging Sensor Fault
//for group 2 fault
const unsigned int PV1SENSORFLT = 0x0010;	//PV1 Sensor Fault
const unsigned int PV2SENSORFLT = 0x0020;	//PV2 Sensor Fault
const unsigned int PV3SENSORFLT = 0x0040;	//PV3 Sensor Fault
const unsigned int PV4SENSORFLT = 0x0080;	//PV4 Sensor Fault
const unsigned int EEPROMACCFLT = 0x0400;	//EEPROM Access Fault
const unsigned int RTMCLKACCFLT	= 0x0800;	//Real Time Clock IC Access Fault
const unsigned int EXTPWRSENFLT = 0x1000;	//External Charging Power Sensor Fault
const unsigned int PSCHTLSENFLT	= 0x2000;	//PSC Hotel Power Sensor Fault
const unsigned int PSCIOVERTEMP = 0x4000;	//PSC Internal Overtemp
const unsigned int CONVINPTFUSE = 0x8000;	//24v-300v Converter Input Fuse Blown
//for group 3 fault:
const unsigned int BUOYPWSENFLT = 0x0001;	//24V Buoy Power Sensor Fault
const unsigned int BPWROVERVOLT = 0x0002;	//24V Buoy Power Over Voltage Fault
const unsigned int BPWUNDERVOLT = 0x0004;	//24V Buoy Power Under Voltage Fault
const unsigned int FUSED5VFUSE  = 0x0008;	//Fused 5V power Fuse Blown
const unsigned int PV1RELAYFLT	= 0x0040;	//PV1 Relay Contact Fault
const unsigned int PV2RELAYFLT 	= 0x0080;	//PV2 Relay Contact Fault
const unsigned int PV3RELAYFLT	= 0x0100;	//PV3 Relay Contact Fault
const unsigned int PV4RELAYFLT 	= 0x0200;	//PV4 Relay Contact Fault
const unsigned int CVTRSWGFLT	= 0x1000;	//HV Power CVTR Sea Water Ground Fault
const unsigned int CVTRGENFLT	= 0x2000;	//HV Power CVTR General Fault
const unsigned int PSCRESETFLG	= 0x4000;	//PSC Hard Reset Flag
const unsigned int PSCPORSTFLG  = 0x8000;	//PSC Power On Reset Flag
//for group 4 fault:
const unsigned int PV1FUSEFLT   = 0x0004;
const unsigned int PV2FUSEFLT   = 0x0008;
const unsigned int PV3FUSEFLT   = 0x0010;
const unsigned int PV4FUSEFLT   = 0x0020;
const unsigned int CVTFCSHTDWN	= 0x0040;	//CVT Force Shut Down by PSC indiator
// H2 sensor faults - ppw 07312026
const unsigned int HYDSENSORFLT = 0x1000;
// H2 status word 1, sensor faults
const unsigned int HYDH2HI      = 0x0001;   // Hydrogen above 3%
const unsigned int HYDCOHI      = 0x0002;   // CO above 100 ppm
// H2 status word 2 faults
const unsigned int HYDVOLTALM   = 0x0004;   // voltage in alarm
const unsigned int HYDH2HIHI    = 0x0008;   // Hydrogen above 10%

//Coulomb Count for each battery string:
double fracc1,fracc2,fracc3,fracc4;
long intcc1,intcc2,intcc3,intcc4;
double ValleyBottom;
double chrg_thrshld;	// State of charge threshold to initiate battery recharge

//HV CVTR 
double cvtr_cvl_enab, cvtr_cvl_disab;	//cvt enable and disable threshold levels
int brd_config;


//LOOKUP TABLE
//Battery Temperature (in C) Sensing Lookup Table[index = (A/D step-180)/10]:
double batt_temp[392] = 
{99.95,99.16,98.37,97.60,96.83,96.06,95.31,94.56,93.82,93.08,
92.36,91.64,90.92,90.21,89.51,88.82,88.13,87.45,86.77,86.10,
85.44,84.79,84.14,83.49,82.85,82.22,81.60,80.98,80.36,79.75,
79.15,78.55,77.96,77.38,76.80,76.22,75.65,75.09,74.53,73.98,
73.43,72.89,72.35,71.82,71.29,70.77,70.26,69.74,69.24,68.74,
68.24,67.75,67.26,66.78,66.30,65.82,65.36,64.89,64.43,63.98,
63.52,63.08,62.64,62.20,61.76,61.33,60.91,60.49,
60.06,59.65,59.24,58.83,58.43,58.03,57.64,57.25,56.86,56.48,
56.10,55.72,55.35,54.98,54.62,54.26,53.90,53.54,53.19,52.84,
52.50,52.16,51.82,51.48,51.15,50.82,50.49,50.17,49.85,49.53,
49.22,48.91,48.60,48.29,47.99,47.69,47.39,47.09,46.80,46.51,
46.22,45.94,45.65,45.37,45.10,44.82,44.55,44.28,44.01,43.74,
43.48,43.21,42.95,42.70,42.44,42.19,41.93,41.69,41.44,41.19,
40.95,40.71,40.47,40.23,39.99,39.76,39.52,39.29,39.06,38.83,
38.61,38.38,38.16,37.94,37.72,37.50,37.28,37.06,36.85,36.63,
36.42,36.21,36.00,35.79,35.59,35.38,35.18,34.97,34.77,34.57,
34.37,34.17,33.97,33.78,33.58,33.38,33.19,33.00,32.81,32.61,
32.42,32.23,32.05,31.86,31.67,31.48,31.30,31.11,30.93,30.74,
30.56,30.38,30.20,30.02,29.83,29.65,29.47,29.30,29.12,28.94,
28.76,28.58,28.41,28.23,28.05,27.88,27.70,27.53,27.35,27.18,
27.00,26.83,26.66,26.48,26.31,26.14,25.96,25.79,25.62,25.45,
25.27,25.10,24.93,24.76,24.59,24.41,24.24,24.07,23.90,23.73,
23.56,23.38,23.21,23.04,22.87,22.70,22.53,22.35,22.18,22.01,
21.84,21.66,21.49,21.32,21.15,20.97,20.80,20.63,20.45,20.28,
20.11,19.93,19.76,19.58,19.41,19.23,19.06,18.88,18.71,18.53,
18.35,18.18,18.00,17.82,17.64,17.47,17.29,17.11,16.93,16.75,
16.57,16.39,16.21,16.03,15.85,15.67,15.49,15.31,15.12,14.94,
14.76,14.58,14.39,14.21,14.02,13.84,13.65,13.47,13.28,13.09,
12.91,12.72,12.53,12.35,12.16,11.97,11.78,11.59,11.40,11.21,
11.02,10.83,10.64,10.45,10.25,10.06,9.873,9.680,9.487,9.293,
9.099,8.905,8.710,8.515,8.319,8.123,7.927,7.731,7.534,7.337,
7.140,6.942,6.744,6.546,6.348,6.149,5.950,5.751,5.551,5.352,
5.152,4.951,4.751,4.550,4.350,4.149,3.947,3.746,3.545,3.343,
3.141,2.939,2.737,2.535,2.332,2.130,1.927,1.725,1.522,1.319,
1.117,0.914,0.711,0.508,0.305,0.103,-0.09,-0.30,-0.50,-0.70,
-0.91,-1.11,-1.31,-1.51,-1.71,-1.92,-2.12,-2.32,-2.52,-2.72,
-2.92,-3.12,-3.32,-3.52,-3.72,-3.92,-4.12,-4.32,-4.52,-4.72,
-4.92,-5.11,-5.31,-5.51,-5.70,-5.90,-6.09,-6.29,-6.48,-6.68,
-6.87,-7.06,-7.25,-7.44,-7.63,-7.82,-8.01,-8.20,-8.39,-8.58,
-8.76,-8.95,-9.13,-9.31};

//Battery Temperature (in C) index table for looking up conditioning and absorption:
double TempIndex[181] =
{-40.00,-39.44,-38.89,-38.33,-37.78,-37.22,-36.67,-36.11,-35.56,
-35.00,-34.44,-33.89,-33.33,-32.78,-32.22,-31.67,-31.11,-30.56,
-30.00,-29.44,-28.89,-28.33,-27.78,-27.22,-26.67,-26.11,-25.56,
-25.00,-24.44,-23.89,-23.33,-22.78,-22.22,-21.67,-21.11,-20.56,
-20.00,-19.44,-18.89,-18.33,-17.78,-17.22,-16.67,-16.11,-15.56,
-15.00,-14.44,-13.89,-13.33,-12.78,-12.22,-11.67,-11.11,-10.56,
-10.00,-9.44,-8.89,-8.33,-7.78,-7.22,-6.67,-6.11,-5.56,-5.00,
-4.44,-3.89,-3.33,-2.78,-2.22,-1.67,-1.11,-0.56,0.00,0.56,1.11,
1.67,2.22,2.78,3.33,3.89,4.44,5.00,5.56,6.11,6.67,7.22,7.78,8.33,
8.89,9.44,10.00,10.56,11.11,11.67,12.22,12.78,13.33,13.89,14.44,
15.00,15.56,16.11,16.67,17.22,17.78,18.33,18.89,19.44,20.00,20.56,
21.11,21.67,22.22,22.78,23.33,23.89,24.44,25.00,25.56,26.11,26.67,
27.22,27.78,28.33,28.89,29.44,30.00,30.56,31.11,31.67,32.22,32.78,
33.33,33.89,34.44,35.00,35.56,36.11,36.67,37.22,37.78,38.33,38.89,
39.44,40.00,40.56,41.11,41.67,42.22,42.78,43.33,43.89,44.44,45.00,
45.56,46.11,46.67,47.22,47.78,48.33,48.89,49.44,50.00,50.56,51.11,
51.67,52.22,52.78,53.33,53.89,54.44,55.00,55.56,56.11,56.67,57.22,
57.78,58.33,58.89,59.44,60.00};

//battery conditioning voltage lookup table:
double Conditioning_Volt[181] = 
{36.10,36.04,35.98,35.92,35.86,35.80,35.74,35.68,35.62,35.56,
35.50,35.44,35.39,35.33,35.28,35.22,35.16,35.11,35.05,35.00,
34.94,34.89,34.83,34.78,34.72,34.67,34.62,34.56,34.51,34.45,
34.40,34.35,34.30,34.25,34.20,34.15,34.10,34.05,34.00,33.95,
33.90,33.85,33.80,33.76,33.71,33.66,33.61,33.56,33.52,33.47,
33.42,33.37,33.33,33.28,33.24,33.19,33.14,33.10,33.05,33.01,
32.96,32.92,32.88,32.83,32.79,32.75,32.71,32.67,32.62,32.58,
32.54,32.50,32.46,32.42,32.38,32.34,32.30,32.26,32.22,32.18,
32.14,32.10,32.07,32.03,32.00,31.96,31.92,31.89,31.85,31.82,
31.78,31.75,31.72,31.68,31.65,31.62,31.59,31.56,31.52,31.49,
31.46,31.43,31.40,31.37,31.34,31.31,31.28,31.25,31.22,31.19,
31.16,31.13,31.10,31.08,31.05,31.02,30.99,30.96,30.94,30.91,
30.88,30.86,30.83,30.81,30.78,30.76,30.74,30.71,30.69,30.66,
30.64,30.62,30.60,30.57,30.55,30.53,30.51,30.49,30.46,30.44,
30.42,30.40,30.38,30.37,30.35,30.33,30.31,30.29,30.28,30.26,
30.24,30.22,30.21,30.19,30.18,30.16,30.14,30.13,30.11,30.10,
30.08,30.07,30.06,30.04,30.03,30.02,30.01,30.00,29.98,29.97,
29.96,29.95,29.94,29.93,29.92,29.91,29.90,29.89,29.88,29.87,
29.86};

//battery absorption voltage lookup table:
double Absorption_Volt[181] =
{33.76,33.70,33.64,33.58,33.52,33.46,33.40,33.34,33.28,33.22,
33.16,33.10,33.05,32.99,32.94,32.88,32.82,32.77,32.71,32.66,
32.60,32.55,32.49,32.44,32.38,32.33,32.28,32.22,32.17,32.11,
32.06,32.01,31.96,31.91,31.86,31.81,31.76,31.71,31.66,31.61,
31.56,31.51,31.46,31.42,31.37,31.32,31.27,31.22,31.18,31.13,
31.08,31.03,30.99,30.94,30.90,30.85,30.80,30.76,30.71,30.67,
30.62,30.58,30.54,30.49,30.45,30.41,30.37,30.33,30.28,30.24,
30.20,30.16,30.12,30.08,30.04,30.00,29.96,29.92,29.88,29.84,
29.80,29.76,29.73,29.69,29.66,29.62,29.58,29.55,29.51,29.48,
29.44,29.41,29.38,29.34,29.31,29.28,29.25,29.22,29.18,29.15,
29.12,29.09,29.06,29.03,29.00,28.97,28.94,28.91,28.88,28.85,
28.82,28.79,28.76,28.74,28.71,28.68,28.65,28.62,28.60,28.57,
28.54,28.52,28.49,28.47,28.44,28.42,28.40,28.37,28.35,28.32,
28.30,28.28,28.26,28.23,28.21,28.19,28.17,28.15,28.12,28.10,
28.08,28.06,28.04,28.03,28.01,27.99,27.97,27.95,27.94,27.92,
27.90,27.88,27.87,27.85,27.84,27.82,27.80,27.79,27.77,27.76,
27.74,27.73,27.72,27.70,27.69,27.68,27.67,27.66,27.64,27.63,
27.62,27.61,27.60,27.59,27.58,27.57,27.56,27.55,27.54,27.53,
27.52};

double valleybottomlimit[100] = 
{-6480,-12960,-19440,-25920,-32400,-38880,-45360,-51840,-58320,
-64800,-71280,-77760,-84240,-90720,-97200,-103680,-110160,-116640,
-123120,-129600,-136080,-142560,-149040,-155520,-162000,-168480,
-174960,-181440,-187920,-194400,-200880,-207360,-213840,-220320,
-226800,-233280,-239760,-246240,-252720,-259200,-265680,-272160,
-278640,-285120,-291600,-298080,-304560,-311040,-317520,-324000,
-330480,-336960,-343440,-349920,-356400,-362880,-369360,-375840,
-382320,-388800,-395280,-401760,-408240,-414720,-421200,-427680,
-434160,-440640,-447120,-453600,-460080,-466560,-473040,-479520,
-486000,-492480,-498960,-505440,-511920,-518400,-524880,-531360,
-537840,-544320,-550800,-557280,-563760,-570240,-576720,-583200,
-589680,-596160,-602640,-609120,-615600,-622080,-628560,-635040,
-641520,-648000};

double overshoot[100] = 
{1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.03,
1.03,1.03,1.03,1.03,1.04,1.04,1.04,1.04,1.04,1.05,1.05,1.05,1.05,
1.05,1.06,1.06,1.06,1.06,1.06,1.07,1.07,1.07,1.07,1.07,1.08,1.08,
1.08,1.08,1.08,1.09,1.09,1.09,1.09,1.09,1.10,1.10,1.10,1.10,1.10,
1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,
1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,
1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,
1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10};

//cvt errro message --> sequence: reg 56, 57, 58, 59
char cvt_err_detail[32][50] = 
{"+5V_PSC Overvoltage (>5.5VDC)", 
 "+5V_PSC Undervoltage (<4.5VDC)", 
 "Output Sensor Power Overvoltage (>9.45VDC)", 
 "Output Sensor Power Undervoltage (<8.64VDC)", 
 "P_SWG Sensor Power Overvoltage (>9.45VDC)", 
 "P_SWG Sensor Power Undervoltage (<8.64VDC)", 
 "N_SWG Sensor Power Overvoltage (>9.45VDC)", 
 "N_SWG Sensor Power Undervoltage (<8.64VDC)",
 "CVTR Board Overtemperature (>100C)",
 "",
 "",
 "",
 "Input 1 Fuse Blown (reserved for PSC input)",
 "Input 2 Fuse Blown (reserved for PSC input)",
 "Input 3 Fuse Blown (reserved for PSC input)",
 "Input 4 Fuse Blown (reserved for PSC input)",
 "Input 1 Sensor Fault",
 "Input 2 Sensor Fault",
 "Input 3 Sensor Fault",
 "Input 4 Sensor Fault",
 "Output Current Sensor Fault",
 "Output Voltage Sensor Fault",
 "P_SWGF Sensor Fault",
 "N_SWGF Sensor Fault",
 "RAW_24V Sensor Fault",
 "HTL_24V Sensor Fault",
 "Interlock Supply Sensor Fault",
 "Interlock Status 1 Sensor Fault",
 "Interlock Status 2 Sensor Fault",
 "Interlock Sensor Fault",
 "+Sea Water Ground Fault",
 "-Sea Water Ground Fault"};
