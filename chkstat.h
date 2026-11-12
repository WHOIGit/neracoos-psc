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
* MPSC Rework:
* REV 1 Dan B/Paul W    10/19/2026          REV01 version for New Mini-PSC CCA

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

extern double bp_v;				//buoy power voltage level.

extern double V_ERR;            //the maximum voltage before we call it an error and set voltage to -1 in chk_bp

//extern const long FULLCHARGE;
extern const char ID_pv1[];
extern const char ID_pv2[];
extern const char ID_pv3[];
extern const char ID_pv4[];
extern const char ID_bt1[];
extern const char ID_bt2[];

extern const char ID_ext_pwr[];
extern const char ID_psc_temp[];
extern const char ID_psc_htl[];
extern const char ID_buoy_pwr[];

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

extern const unsigned int PSCRESETFLG;	//PSC Hard Reset Flag
extern const unsigned int PSCPORSTFLG;	//PSC Power On Reset Flag
//for group 4 fault:
extern const unsigned int PV1FUSEFLT;	//PV1 Fuse Fault
extern const unsigned int PV2FUSEFLT;	//PV2 Fuse Fault
extern const unsigned int PV3FUSEFLT;	//PV3 Fuse Fault
extern const unsigned int PV4FUSEFLT;	//PV4 Fuse Fault


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

extern int PV_IN_CH_ON[4];

extern unsigned int error_flag_1;
extern unsigned int error_flag_2;
extern unsigned int error_flag_3;
extern unsigned int error_flag_4;

extern int secondcnt;
extern double totalinputcurrent;

//prototype
void chk_bt(char *bt_status,char *bt_detail, double *bt_v, double *bt_t);
void chk_pv(char *pv_status, char *pv_detail);
void chk_ext_pwr(char *ex_status);
void chk_bp(char *bp_status, char *psc_t);
void chk_psch(char *psch_status);
void chk_hyd(char *hyd_status, double *H);
int  cal_hyd(void); //DMB 10-26-2026

int bin2dec(char binary);

extern int i2c_getstat1(int i2c_ch, int SlaveAddress, int regaddr, int length, unsigned char *I2CRead);
extern int i2c_putstat1(int i2c_ch, int SlaveAddress, int regaddr,unsigned char *I2CWrite, int wrt_cnt);

extern double get_data1(unsigned char *i2cdata, int base, double resolution);
extern double get_bt_temp(unsigned char *i2cdata, int baseaddress, const long batt_flag, int type);

extern double batterystringvoltage(double v1, double v2);

