/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			chkstat.h
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
* This file consists of the chkstat.c header 
*
*************************************************************************/
#define FCY 16000000UL  	//FCY = Fosc/2 for delay timing
#include <libpic30.h>		//for __delay_ms()

#define USE_AND_OR
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "adc.h"

#define Rsenwt 0.005				//in Ohm, sense resistor of wind turbines
#define Rsenbt 0.005				//in Ohm, sense resistor of batteries
#define Rsenpv 0.0091				//in Ohm, sense resistor of pv panels
#define Rsenex 0.008				//in Ohm, sense resistor of ext charging power
#define Rsenbuoy 0.003			//in Ohm, sense resistor of buoy 24V bus
#define Rsenpsch 0.08			//in Ohm, sense resistor of PSC hotel power
#define Rsencvi 0.01			//in Ohm, sense resistor of the CVTR input current
#define Rsencvo 0.04			//in Ohm, sense resistor of the CVTR output current
#define RsenHVI 82.5			//in Ohm, sense resistor for the CVTR output HV current -- ?
#define RsenHVV 383.0			//in kOhm, sense resistor for the CVTR output HV Voltage cal -- ?
#define RsenCVRAW 0.002			//in Ohm,

#define deltaI 20				//LTC4151 current sense resolution in uV
#define deltaV 25				//LTC4151 voltage sense resolution in mV
#define deltaT 0.5				//LTC4151 aux voltage sense resolution in mV

#define Dt 10					//TC1047 temp sensor resolution in mV/C

#define BATTTEMP 0
#define CVTRTEMP 1

//const double BATTSTRINGCURRENTFULLSCALE = 81.92 / Rsenbt; //in mA, the LTC4151 current full scale is 81.92 mv
//const double BATTSTRINGCURRENTFULLSCALE = 81.9 / Rsenbt; //in mA, the LTC4151 current full scale is 81.92 mv -- changed to 81.9 on 8-22-2011 since the fullscale is 4095
const double BATTSTRINGCURRENTFULLSCALE = 4000; //4095 is too high. Need to consider the difference btw charge and discharge currents
unsigned char cvtrbuffer[75]={};

extern double bp_v;				//buoy power voltage level.
extern double ValleyBottom;

extern double DCVLMAX;			//the maximum delta between CVL Enable and CVL Disable limits
extern double CVLMIN;			//the minimum value for the CVL Disable limit
extern double CVLMAX;			//the maximum value for the CVL ENable Limit
extern double V_ERR;            //the maximum voltage before we call it an error and set voltage to -1 in chk_bp

extern const long FULLCHARGE;
extern const char ID_pv1[];
extern const char ID_pv2[];
extern const char ID_pv3[];
extern const char ID_pv4[];
extern const char ID_bt1[];
extern const char ID_bt2[];
extern const char ID_conv[];
extern const char ID_ext_pwr[];
extern const char ID_psc_temp[];
extern const char ID_psc_htl[];
extern const char ID_buoy_pwr[];
extern const char ID_override[];
extern const char ID_hyd[];      // ppw 08032026

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
extern const int HYDi2c_CH;          // ppw 08032026
extern const int HYD_SADDR;           // ppw 08032026
extern const int RLYST_REGADDR;
extern const int OVERRIDE_REGADDR;
extern const int BRDCONF_REGADDR;
extern const int PVCONFIG_REGADDR;
extern const int ELPSDTM_REGADDR;
extern const int CVTRCVL_REGADDR;
extern const int BATTLVL1_REGADDR;
extern const int BATTLVL2_REGADDR;
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
extern const int CVTR_SWGFREADING_P_REG;
extern const int CVTR_SWGFREADING_N_REG;

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

// hydrogen sensor faults - ppw 08032026
extern const unsigned int HYD3PCTFLT;    // Thermal runaway h2 > 3% fault
extern const unsigned int HYDVOLTAGELOW; // Voltage low fault
extern const unsigned int HYDVOLTAGEHI;  // Voltage high fault
extern const unsigned int HYD10PCTFLT;   // Thermal runaway h2 > 10% fault
extern const unsigned int HYDSENSORFLT;  // Sensor damage fault
extern const unsigned int HYDCHIPFLT;    // Chip short fault
extern const unsigned int HYDCOFLT;      // CO sensor fault

extern unsigned int PV1MASK;
extern unsigned int PV2MASK;
extern unsigned int PV3MASK;
extern unsigned int PV4MASK;
extern unsigned int CVIMASK;
extern unsigned int CVRMASK;
extern unsigned int ALLMASK;
extern unsigned int STDMASK;

extern int ledflag;
extern int rlystat;
extern int override_flag;
extern unsigned int error_flag_1;
extern unsigned int error_flag_2;
extern unsigned int error_flag_3;
extern unsigned int error_flag_4;
extern int CONVPWRSTAT;
extern int CONVOUTSTAT;
extern int CVTRINTLCKSTAT;
extern int CVTR_SWGF_TEST;

extern double batt_chg_status;
extern int secondcnt;
extern double totalinputcurrent;

extern double cvtr_cvl_enab, cvtr_cvl_disab;
extern int swgf_dly;
extern int swgfcnt;
//extern unsigned int channel;

//prototype
void chk_bt(char *bt_status,char *bt_detail, double *bt_v, double *bt_t);
void chk_pv(char *pv_status, char *pv_detail);
void chk_ext_pwr(char *ex_status);
void chk_bp(char *bp_status, char *psc_t);
void chk_psch(char *psch_status);
void chk_hyd(char *hyd_status); // ppw 08032026
void chk_conv(char *conv_status, char *cvtr_detail, char *swgf_status);
int bin2dec(char binary);
int device_state(unsigned int mask);
//int i2c_getstat(int i2c_ch, int SlaveAddress, int regaddr, int length);
void flagRelayFault(double curr, double volt, unsigned int pwrmask,unsigned int fltmask);
int pwrsenflt();

extern int i2c_getstat1(int i2c_ch, int SlaveAddress, int regaddr, int length, unsigned char *I2CRead);
//int i2c_putstat(int i2c_ch, int SlaveAddress, int regaddr);
//extern double get_data(int base, double resolution);
extern double get_data1(unsigned char *i2cdata, int base, double resolution);
extern double get_bt_temp(unsigned char *i2cdata, int baseaddress, const long batt_flag, int type);
extern void wnd_speed(char *speed, char *cmdstat);
extern void RestoreRlyState();
extern int rlydrive(unsigned int mask, int state);
extern int set_battchrg_level(int regaddr, double level);
extern int set_battchrg_level1(int regaddr, long intlevel, double fralevel);
extern int batterycharging(double bt_t, double bt_v, double bt1_i, double bt2_i);
extern double batterystringvoltage(double v1, double v2);
//extern double max(double t1, double t2);
extern void get_pvcntlstat(char *cmd, char *status_data,char *detail);
extern void switchcvtrrelay(int cntl);
