/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			try.h
* Dependencies:
* Processor:		PIC24F
* Compiler:			MPLAB C30 V3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author			Data			Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Joseph Zhu		10/28/2010		Ininitial File Creation
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES
* This file consists of the try.c header for simulation purpose
*
*************************************************************************/
#define FCY 16000000UL  	//FCY = Fosc/2 for delay timing
#include <libpic30.h>		//for __delay_ms()

#ifndef USE_AND_OR /* Format for AND_OR based bit setting */
	#define USE_AND_OR
#endif 

#include "p24fxxxx.h"
#include "io_config.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi.h"

#define ON 1
#define OFF 0

extern unsigned int override_flag;
extern int ST_override;
extern char *pWrite;
extern char *pRead;
extern char *UART1_RXDATA;
extern char UART1_TXDATA[2048];

extern int ledflag;
extern int rlystat;
extern int datastreamcnt;
extern int datastreamcnt_loc;
extern double bp_v;

extern const long FULLCHARGE;
extern unsigned int PV1MASK;
extern unsigned int PV2MASK;
extern unsigned int PV3MASK;
extern unsigned int PV4MASK;
extern unsigned int ALLMASK;
extern unsigned int STDMASK;

void init();
void UARTInit();
void EEPROMInit();
void I2C1Init();
void I2C2Init();
void RTCLKInit();
void SPI1Init();
void SPI2Init();
void debuglamptest();
int get_override_flag();
int set_override_flag();
void resetcontrol();
int try();					//development only, delete!
void putsSPI1(unsigned int length, unsigned int *wrptr);
unsigned int ReadSPI11(void);
unsigned int getsSPI11(unsigned int length, unsigned int *rdptr, unsigned int spi_data_wait);

//extern double batt_temp[360];
extern char cvt_err_detail[32][50];

//extern void i2c_getstat(int i2c_ch, int SlaveAddress, int regaddr, int length);
extern int i2c_getstat1(int i2c_ch, int SlaveAddress, int regaddr, int length, unsigned char *i2cdata);
//extern void i2c_putstat(int i2c_ch, int SlaveAddress, int regaddr);
extern void i2c_putstat1(int i2c_ch, int SlaveAddress, int regaddr, unsigned char *I2CWrite, int wrt_cnt);
extern void chksumgen();
extern int chksumchk(char *chksum);
extern double get_bt_temp(const long batt_flag);
extern char *chk_ext_pwr();
extern int set_battchrg_level1(int regaddr, long intlevel, double fralevel);
extern long get_battchrg_level(int regaddr, double *fralevel);
extern double UpdateCoulombCount(double btx_i, long *intccx, double *fraccx);
extern void update_elapsed_time();
extern void set_elapsed_time(unsigned long etime);
extern unsigned long get_elapsed_time();
extern int rlydrive(unsigned int mask, int state);
extern void eepromWrite(char *location, char *data);
extern void eepromRead(char *location, char *length, char *data);
extern void tx_comm();
extern void chk_wt_shunt(int wt);
extern int rlydrive(unsigned int mask, int state);
extern void flagRelayFault(double curr, double volt, unsigned int pwrmask,unsigned int fltmask);

//for group 1 fault:
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
//for group 2 fault:
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
