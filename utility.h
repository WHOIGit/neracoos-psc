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
extern const unsigned int EEPROMACCFLT;	//EEPROM Access Fault
extern const unsigned int RTMCLKACCFLT;	//Real Time Clock IC Access Fault
extern const unsigned int EXTPWRSENFLT;	//External Charging Power Sensor Fault
extern const unsigned int PSCHTLSENFLT;	//PSC Hotel Power Sensor Fault
extern const unsigned int PSCIOVERTEMP;	//PSC Internal Overtemp
extern const unsigned int CONVINPTFUSE;	//24v-300v Converter Input Fuse Blown
//for group 3 fault:
extern const unsigned int BUOYPWSENFLT;	//24V Buoy Power Sensor Fault
extern const unsigned int BPWROVERVOLT;	//24V Buoy Power Over Voltage Fault
extern const unsigned int BPWUNDERVOLT;	//24V Buoy Power Under Voltage Fault
extern const unsigned int FUSED5VFUSE;  //Fused 5V power Fuse Blown
extern const unsigned int PV1RELAYFLT;	//PV1 Control Relay Fault
extern const unsigned int PV2RELAYFLT;	//PV2 Control Relay Fault
extern const unsigned int PV3RELAYFLT;	//PV3 Control Relay Fault
extern const unsigned int PV4RELAYFLT;	//PV4 Control Relay Fault
extern const unsigned int CVTRSWGFLT;	//HV Power CVTR Sea Water Ground Fault
extern const unsigned int CVTRGENFLT;	//HV Power CVTR General Fault
extern const unsigned int PSCRESETFLG;	//PSC Hard Reset Flag
extern const unsigned int PSCPORSTFLG;	//PSC Power On Reset Flag
//for group 4 fault:
extern const unsigned int PV1FUSEFLT;	//PV1 Fuse Fault
extern const unsigned int PV2FUSEFLT;	//PV2 Fuse Fault
extern const unsigned int PV3FUSEFLT;	//PV3 Fuse Fault
extern const unsigned int PV4FUSEFLT;	//PV4 Fuse Fault
extern const unsigned int CVTFCSHTDWN;	//CVT Force Shut Down by PSC indiator

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
extern double TempIndex[180];
extern double bp_v;
extern double fracc1,fracc2,fracc3,fracc4;
extern long intcc1,intcc2,intcc3,intcc4;
extern double ValleyBottom;
extern double valleybottomlimit[100];
extern double overshoot[100];

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
extern const int EEPROMi2c_CH;
extern const int EEPROM_SADDR;
extern const int RLYST_REGADDR;
extern const int OVERRIDE_REGADDR;
extern const int BRDCONF_REGADDR;
extern const int PVCONFIG_REGADDR;
extern const int ELPSDTM_REGADDR;
extern const int CVTRCVL_REGADDR;
extern const int BATTLVL1_REGADDR;
extern const int VALLEYBTM_REGADDR;
extern const int CHRGCTRL_REGADDR;
extern const int PN_REGADDR;
extern const int SN_REGADDR;
extern const int HWREV_REGADDR;
extern const int FWREV_REGADDR;
extern const int CVTR_CH;
extern const int CVTR_SADDR;
extern const int CVTR_REG1;
extern const int CVTR_INTLCK_V_REG;
extern const int CVTR_OI_REG;
extern const int CVTR_OV_REG;
extern const int CVTR_CTRL_REG;

extern LOGGER_STATUS       loggerStatus;
//extern volatile DWORD      currentTick;
//extern volatile LOG_DATA   logData[NUM_LOG_BUFFERS];
extern VOLUME_INFO         volume;

//prototypes
int SPI1tx(unsigned int data, int CE_type);
int SPI2tx(unsigned int data);
int i2c_getstat(int i2c_ch, int SlaveAddress, int regaddr, int length);
int i2c_getstat1(int i2c_ch, int SlaveAddress, int regaddr, int length,unsigned char *I2CRead);
//int i2c_putstat(int i2c_ch, int SlaveAddress, int regaddr);
int i2c_putstat1(int i2c_ch, int SlaveAddress, int regaddr,unsigned char *I2CWrite, int wrt_cnt);
int hex2dec(char hex);
int bin2dec(char binary);
int bcd2dec(unsigned char bcd);
int dec2bcd(int dec);
int bcd2hex(int bcd);
char dec2char(int dec);
int word2dec(char *data);
int hexbyte2dec(char data);
//double get_data(int base, double resolution);
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
void update_elapsed_time();
double get_bt_temp(unsigned char *i2cdata, int baseaddress, const long batt_flag,int type);
void rtclk_reset();
void ConfigIntptUART1(unsigned int config);
void OpenUART1port(unsigned int config1,unsigned int config2, unsigned int ubrg);
void UpdateCoulombCount(double btx_i, long *intccx, double *fraccx);
int temp2index(double btx_t);
double overshootlookup(double valleybottom);
unsigned long get_elapsed_time();
void set_elapsed_time(unsigned long etime);
void MonitorMedia();
int read_pvcntlreg(int code,int SS);
void write_pvcntlreg(int pvcntlcmd, int SS);
double batterystringvoltage(double v1, double v2);
//double max(double t1, double t2);
void resetcoulombcnts();
void cntstore(int regaddr, double intvalue, double fravalue);
void ClrResetFlag();
char cvltoi2cdata(char *input);

extern void remap_led_ce();
extern void remap_rly_ce();
extern void chksumgen();
extern int device_state(unsigned int mask);
extern void flagRelayFault(double curr, double volt, unsigned int pwrmask,unsigned int fltmask);
extern int rlydrive(unsigned int mask, int state);
