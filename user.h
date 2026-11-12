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
* MPSC Rework:
* REV 1 Dan B/Paul W    10/19/2026          REV01 version for New Mini-PSC CCA

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

//const long FULLCHARGE = 1319040;


int ADCFinished = 0;

//constant strings:
const char Firmware_Revision[] = "Rev -";
const char CMD_connect[] = "connect";
const char CMD_disconnect[] = "disconnect";
const char CMD_stop[] = "stop";
const char CMD_go[] = "go";
const char CMD_override[] = "override";
const char CMD_psc_reset[] = "psc_reset";
const char CMD_version[] = "version";
const char CMD_pv_status[] = "pv_status";
const char CMD_bt_status[] = "bt_status";
//const char CMD_psc[] = "psc";
const char CMD_psc[] = "MPSC";
const char CMD_help[] = "help";

const char ID_pv1[] = "pv1";
const char ID_pv2[] = "pv2";
const char ID_pv3[] = "pv3";
const char ID_pv4[] = "pv4";
const char ID_bt1[] = "bt1";
const char ID_bt2[] = "bt2";
const char ID_ext_pwr[] = "ext";
const char ID_psc_temp[] = "psc_temp";
const char ID_psc_htl[] = "int";
const char ID_buoy_pwr[] = "buoy_pwr";
const char ID_hyd[] = "hyd";

const char ST_status[] = "status";
const char help_string[] = "command list";

const char ST_ok[] = "ok";
const char ST_error[] = "error";

//limits and thresholds
const double BATT_TEMP_LMT = 50.00;		//battery overtemp threshold in degree C

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
const int HYDi2c_CH         = 3;        

const int HYD_SADDR         = 0x0A;

//FAULT TABLE
//for group 1 fault:
const unsigned int BT11OVERTEMP = 0x0001;	//Battery 1 of String 1 Overtemp
const unsigned int BT12OVERTEMP = 0x0002;	//Battery 2 of String 1 Overtemp
const unsigned int BT21OVERTEMP = 0x0004;	//Battery 1 of String 2 Overtemp
const unsigned int BT22OVERTEMP = 0x0008;	//Battery 2 of String 2 Overtemp

const unsigned int BT1CHGSENFLT = 0x1000;	//Battery String 1 Charging Sensor Fault
const unsigned int BT1DCHGSNFLT = 0x2000;	//Battery String 1 Disharging Sensor Fault
const unsigned int BT2CHGSENFLT = 0x4000;	//Battery String 2 Charging Sensor Fault
const unsigned int BT2DCHGSNFLT = 0x8000;	//Battery String 2 Disharging Sensor Fault

//for group 2 fault
const unsigned int PV1SENSORFLT = 0x0010;	//PV1 Sensor Fault
const unsigned int PV2SENSORFLT = 0x0020;	//PV2 Sensor Fault
const unsigned int PV3SENSORFLT = 0x0040;	//PV3 Sensor Fault
const unsigned int PV4SENSORFLT = 0x0080;	//PV4 Sensor Fault

const unsigned int EXTPWRSENFLT = 0x1000;	//External Charging Power Sensor Fault
const unsigned int PSCHTLSENFLT	= 0x2000;	//PSC Hotel Power Sensor Fault
const unsigned int PSCIOVERTEMP = 0x4000;	//PSC Internal Overtemp

//for group 3 fault:
const unsigned int BUOYPWSENFLT = 0x0001;	//24V Buoy Power Sensor Fault
const unsigned int BPWROVERVOLT = 0x0002;	//24V Buoy Power Over Voltage Fault
const unsigned int BPWUNDERVOLT = 0x0004;	//24V Buoy Power Under Voltage Fault

const unsigned int PSCRESETFLG	= 0x4000;	//PSC Hard Reset Flag
const unsigned int PSCPORSTFLG  = 0x8000;	//PSC Power On Reset Flag
//for group 4 fault:

// H2 sensor faults - ppw 07312026
const unsigned int HYDSENSORFLT = 0x1000;
// H2 status word 1, sensor faults
const unsigned int HYDH2HI      = 0x0001;   // Hydrogen above 3%
const unsigned int HYDCOHI      = 0x0002;   // CO above 100 ppm
// H2 status word 2 faults
const unsigned int HYDVOLTALM   = 0x0004;   // voltage in alarm
const unsigned int HYDH2HIHI    = 0x0008;   // Hydrogen above 10%

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

