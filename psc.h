/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			psc.h
* Dependencies:
* Processor:       	PIC24FJ256GB106
* Compiler:			MPLAB C30 V3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev. 	Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1			Joseph Zhu    	10/28/2010		Initial PSC Firmware (STD) 
* 2 		Joseph Zhu		08/09/2011		Special Build for Power Bench Test
*											change the FULLCHARGE in user.h
* 2.1		Joseph Zhu		08/11/2011		Update the SOC calculation formula
*											in init.c and command.c
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES
* This file consists of all psc parameters which shared with all
* modules
*
*************************************************************************/

#include "p24fxxxx.h"
#include "user.h"
#include "uart.h"
#include "PPS.h"
#include "io_config.h"
#include "init.h"
#include "stdio.h"
#include "string.h"
#include "userTypeDefs.h"
#include "adc.h"

unsigned int FWRevision = 16;		//hard coded Firmware Revision   Rev. 6 Oct 2013 fixed Rx lockup bug and CVTR temperature reading
								// Version 8 July 2014 fixed battery charging test to SOC || Vmain. And blanked out CVTR data and fault flags when no board present
								// Version 9 August 2014 added HP functionality and general cleanup 
								// Version 10 Oct 2014 added Fuel Cell functionality
								// Version 11 fixed HVCVTR/external charger bug and minor fixes 
								// Version 14 same as v12, removed automatic relay control, enabled 'bypass' mode, removed fuel cell functionality, tried to keep everything else the same
								// Version 15 Dec 2019 modified connect all behavior, external charge behavior, add leds_on and leds_off commands, set_dsf command
								// change to internal timer interrupt by default. Change to 111111 baud.
								//Version 15a March 2020 hard code "external pv"
								//Version 16a April 2020 added failsafe voltage cutoff, checked at 1hZ. Filtered out voltage readings above 80v in chk_bp
								//            also commented out unused code and fixed compilation warnings. Created rev_date below and moved boot message to psc.c
char rev_letter[2] = "a";
char rev_date[9] = "04/23/20";

const int MAXEEPROMBYTES = 255;

volatile int DatastreamFlag = 0;	//flag for CN53
volatile int DataavailableFlag = 0;	//flag for RX data received

unsigned int override_flag = 0x1FF;

double totalinputcurrent = 0.0;
int ST_override = OFF;
int rlystat;
int ledflag = 0;
int inhibitdatastream = OFF;	//data stream is permitted by default
int datastreamcnt = 5;			//datastream frequency default once every 5 sec
unsigned int error_flag_1 = 0;
unsigned int error_flag_2 = 0;
unsigned int error_flag_3 = 0;
unsigned int error_flag_4 = 0;
double batt_chg_status;			//the percentage of battery fully charged
int secondcnt = 0;				//Minute Count, for battery coulomb cnt save 
int minutecnt = 0;				//Hour Count, for Elapsed timer
//int CONVSTATEFLAG = 0x0; 		//bit 0 is CVC, bit 1 is CVR
int CONVPWRSTAT = 0x0;			//CVTR Power State flag. 0 means the CVTR is powered off
int CONVOUTSTAT = 0x0;			//CVTR Output State flag, 0 means the CVTR output is disabled
int CVTRINTLCKSTAT = 0x1;			//CVTR Output Interlock flag, 1 means interlock is in and no output
int CVTR_SWGF_TEST = 0x0;		//CVTR Sea Water Ground Fault Test Flag. 0 means test is stopped
double bp_v = 0.0;				//buoy power voltage level!
int swgf_dly = 2;				//swgf test delay in sec
double V_ERR = 80.0;            //the maximum voltage before we call it an error and set voltage to -1 in chk_bp
double DCVLMAX = 0.5;			//the maximum delta between CVL Enable and CVL Disable limits
double CVLMIN = 18.0;			//the minimum value for the CVL Disable limit
double CVLMAX = 36.0;			//the maximum value for the CVL ENable Limit
double SOCMIN = 50.0;			//the minimum recommended SOC value for the charge cycle threshold
double VMAX = 31.0;               //maximum system voltage before we start disconnecting relays
int failsafe_flag = 0;            //flag so that we only emergency disconnect relays once
int failsafe_enable = 1;          //enable for the voltage failsafe
long failsafe_count = 0;           //counter to run the failsafe in the main loop

unsigned char DataAvailable=0;
volatile char Rxdata1[100]={};		// Receive buffer for PlatCon serial interface
volatile char Rxdata2[200]={};		//Receive buffer for Fuel Cell 1 serial interface
char Txdata[50] = {};
char FC_data_buffer[200]={};

char UART1_TXDATA[2048]={};
char *UART1_RXDATA;
char UART2_TXDATA[64]={};
char *UART2_RXDATA;

LOGGER_STATUS       loggerStatus;
//volatile DWORD      currentTick;
//volatile LOG_DATA   logData[NUM_LOG_BUFFERS];
VOLUME_INFO         volume;
char filename[11];

//prototype
void datastream(int inhibit);
void enable_int();
void serviceCN53();
void setdsf();
void set_timing_external(int clock_source);

extern void response();
extern void update_elapsed_time();
extern void rtclk_reset();
extern void chksumgen();
extern void MonitorMedia();
extern void MonitorVBUS(unsigned int ADCResult);
extern void voltage_failsafe();
