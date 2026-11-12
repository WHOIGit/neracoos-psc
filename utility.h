/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			utility.h
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
* This file consists of the comm.c header 
*
*************************************************************************/
#define FCY 16000000UL  	//FCY = Fosc/2 for delay timing
#include <libpic30.h>		//for __delay_ms()

#define USE_AND_OR
#include "spi.h"
#include "i2c.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "io_config.h"
#include "userTypeDefs.h"

//define I2C baud rate
#define Fsck 400000			//400kHz I2C
#define I2C_BRG 0x25		

#define RLY 0	//for controlling SS1_RLY_CE
#define LED 1	//for controlling SS1_LED_CE

#define BATTTEMP 0
#define CVTRTEMP 1

#define deltaT 0.5

#define MAX_ALLOWED_CURRENT 500         // Maximum power the USB device can sink (in mA)

// These thumb drives appear to have electrical issues that makes their
// operation at full speed marginal.
THUMB_DRIVE_ID      problemThumbDrives[] = { {0x0930, 0x6545} };
#define PROBLEM_THUMB_DRIVE_COUNT   (sizeof(problemThumbDrives) / sizeof(THUMB_DRIVE_ID))

//for group 1
extern const unsigned int BT11OVERTEMP;	//Battery 1 of String 1 Overtemp
extern const unsigned int BT12OVERTEMP;	//Battery 2 of String 1 Overtemp
extern const unsigned int BT21OVERTEMP;	//Battery 1 of String 2 Overtemp
extern const unsigned int BT22OVERTEMP;	//Battery 2 of String 2 Overtemp
extern const unsigned int BATTSTR1FUSE;	//Battery String 1 Fuse Blown
extern const unsigned int BATTSTR2FUSE;	//Battery String 2 Fuse Blown
extern const unsigned int BT1CHGSENFLT;	//Battery String 1 Charging Sensor Fault
extern const unsigned int BT1DCHGSNFLT;	//Battery String 1 Disharging Sensor Fault
extern const unsigned int BT2CHGSENFLT;	//Battery String 2 Charging Sensor Fault
extern const unsigned int BT2DCHGSNFLT;	//Battery String 2 Disharging Sensor Fault
//for group 2
extern const unsigned int PV1SENSORFLT;	//PV1 Sensor Fault
extern const unsigned int PV2SENSORFLT;	//PV2 Sensor Fault
extern const unsigned int PV3SENSORFLT;	//PV3 Sensor Fault
extern const unsigned int PV4SENSORFLT;	//PV4 Sensor Fault

extern const unsigned int EXTPWRSENFLT;	//External Charging Power Sensor Fault
extern const unsigned int PSCHTLSENFLT;	//PSC Hotel Power Sensor Fault
extern const unsigned int PSCIOVERTEMP;	//PSC Internal Overtemp

//for group 3 fault:
extern const unsigned int BUOYPWSENFLT;	//24V Buoy Power Sensor Fault
extern const unsigned int BPWROVERVOLT;	//24V Buoy Power Over Voltage Fault
extern const unsigned int BPWUNDERVOLT;	//24V Buoy Power Under Voltage Fault

extern const unsigned int PV1RELAYFLT;	//PV1 Control Relay Fault
extern const unsigned int PV2RELAYFLT;	//PV2 Control Relay Fault
extern const unsigned int PV3RELAYFLT;	//PV3 Control Relay Fault
extern const unsigned int PV4RELAYFLT;	//PV4 Control Relay Fault

extern const unsigned int PSCRESETFLG;	//PSC Hard Reset Flag
extern const unsigned int PSCPORSTFLG;	//PSC Power On Reset Flag
//for group 4 fault:

extern unsigned int PV1MASK;
extern unsigned int PV2MASK;
extern unsigned int PV3MASK;
extern unsigned int PV4MASK;
extern unsigned int ALLMASK;
extern unsigned int STDMASK;

extern char *pRead;
extern char *pWrite;
extern double batt_temp[324];
extern unsigned int error_flag_1;
extern unsigned int error_flag_2;
extern unsigned int error_flag_3;
extern unsigned int error_flag_4;

extern double bp_v;

//PSC Hardware Profile
extern const int BATTERYi2c_CH;
extern const int BATT1_C_SADDR;
extern const int BATT1_D_SADDR;
extern const int BATT2_C_SADDR;
extern const int BATT2_D_SADDR;
extern const int EXTPWRi2c_CH;
extern const int EXTPWR_SADDR;
extern const int PVi2c_CH;
extern const int PV1_SADDR;
extern const int PV2_SADDR;
extern const int PV3_SADDR;
extern const int PV4_SADDR;
extern const int BUOYPWRi2c_CH;
extern const int BUOYPWR_SADDR;
extern const int RTCLKi2c_CH;
extern const int RTCLK_SADDR;
extern const int PSCHTLi2c_CH;
extern const int PSCHTL_SADDR;


//prototypes

//int SPI2tx(unsigned int data);
int i2c_getstat(int i2c_ch, int SlaveAddress, int regaddr, int length);
int i2c_getstat1(int i2c_ch, int SlaveAddress, int regaddr, int length,unsigned char *I2CRead);

int i2c_putstat1(int i2c_ch, int SlaveAddress, int regaddr,unsigned char *I2CWrite, int wrt_cnt);
int hex2dec(char hex);
int bin2dec(char binary);
int bcd2dec(unsigned char bcd);
int dec2bcd(int dec);
int bcd2hex(int bcd);
char dec2char(int dec);
int word2dec(char *data);
int hexbyte2dec(char data);

double get_data1(unsigned char *i2cdata, int base, double resolution);
int ChkI2C1Idle(int dly_cnt);
int MasterI2C1puts(unsigned char *wrptr, int wrt_cnt, int dly_cnt);
int MasterI2C1gets(unsigned int length, unsigned char *rdptr, int dly_cnt);
int ChkI2C2Idle(int dly_cnt);
int MasterI2C2puts(unsigned char *wrptr, int wrt_cnt, int dly_cnt);
int MasterI2C2gets(unsigned int length, unsigned char *rdptr, int dly_cnt);
int ChkI2C3Idle(int dly_cnt);
int MasterI2C3puts(unsigned char *wrptr, int wrt_cnt, int dly_cnt);
int MasterI2C3gets(unsigned int length, unsigned char *rdptr, int dly_cnt);
void tx_comm();

double get_bt_temp(unsigned char *i2cdata, int baseaddress, const long batt_flag,int type);
void rtclk_reset();
void ConfigIntptUART1(unsigned int config);
void OpenUART1port(unsigned int config1,unsigned int config2, unsigned int ubrg);

double batterystringvoltage(double v1, double v2);

extern void chksumgen();

