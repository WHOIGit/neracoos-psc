/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			init.h
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
* This file consists of the init.c header 
*
*************************************************************************/
#define FCY 16000000UL  	//FCY = Fosc/2 for delay timing
#include <libpic30.h>		//for __delay_ms()
#include "try.h"
#include "adc.h"

#ifndef devboard
	#define devboard
#endif

#ifndef USE_AND_OR /* Format for AND_OR based bit setting */
	#define USE_AND_OR
#endif 

#include "p24fxxxx.h"
#include "io_config.h"
#include "uart.h"
#include "ports.h"
#include <stdio.h>
#include <stdlib.h>

//For UART

#define ON 1
#define OFF 0

#define RLY 0	//for controlling SS1_RLY_CE
#define LED 1	//for controlling SS1_LED_CE

#define ADC_Reading_VBUS ADC1BUFD
#define SCAN_MASK 0x0010		//AN4 used

//unsigned int channel = ADC_CH0_POS_SAMPLEA_AN4;

extern unsigned int FWRevision;

extern unsigned int error_flag_1;
extern unsigned int error_flag_2;
extern unsigned int error_flag_3;
extern unsigned int error_flag_4;

extern unsigned int override_flag;
extern int ST_override;
extern char *pWrite;
extern char *pRead;
extern char *UART1_RXDATA;
extern char UART1_TXDATA[2048];
extern char *UART2_RXDATA;
extern char UART2_TXDATA[64];

extern int ledflag;
extern int rlystat;
extern int datastreamcnt;
extern int datastreamcnt_loc;
extern int inhibitdatastream;
extern int brd_config;

extern double batt_chg_status;
extern int secondcnt;
//Coulomb Count for each battery string:
extern double fracc1,fracc2,fracc3,fracc4;
extern long intcc1,intcc2,intcc3,intcc4;
extern double ValleyBottom;

//HV CVRT
extern double cvtr_cvl_enab,cvtr_cvl_disab;
extern double chrg_thrshld;
extern int swgf_dly;

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
extern const int CVTR_SWGF_DLY_REG;

//prototypes
void init();
void UARTInit();
void INT1Init();
void ICNInit();
void EEPROMInit();
void I2C1Init();
void I2C2Init();
void I2C3Init();
void RTCLKInit();
void SPI1Init();
void SPI2Init();
void ANAInit();
void USBInit();
void debuglamptest();
void RestoreRlyState();
void UART1Init();
void UART2Init();

extern void ConfigIntptUART1(unsigned int config);
extern void OpenUART1port(unsigned int config1,unsigned int config2, unsigned int ubrg);

//extern int i2c_getstat(int i2c_ch, int SlaveAddress, int regaddr, int length);
extern int i2c_getstat1(int i2c_ch, int SlaveAddress, int regaddr, int length, unsigned char *i2cdata);
extern int SPI1tx(unsigned int data, int CE_type);

//extern void i2c_putstat(int i2c_ch, int SlaveAddress, int regaddr);
extern void i2c_putstat1(int i2c_ch, int SlaveAddress, int regaddr, unsigned char *I2CWrite, int wrt_cnt);
extern void chksumgen();
extern int chksumchk(char *chksum);
extern double get_bt_temp(const long batt_flag);
extern long get_battchrg_level(int regaddr, double *fralevel);
extern double get_valleybtm(int regaddr);
extern int allPVConfig();
extern int get_override_state();
extern int set_override_state();
extern void helplist();
extern void ClrResetFlag();
extern int hex2dec(char data);
extern int word2dec(char *data);
extern int hexbyte2dec(char data);

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
