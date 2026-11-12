/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			response.h
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
* This file consists of the response.c header 
*
*************************************************************************/
#define FCY 16000000UL  	//FCY = Fosc/2 for delay timing
#include <libpic30.h>		//for __delay_ms()
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "uart.h"
#include "userTypeDefs.h"
#include "io_config.h"
#include "adc.h"

#define V_FLAG 0x01
#define T_FLAG 0x02
#define H_FLAG 0x04

extern int inhibitdatastream;
extern int datastreamcnt;
int datastreamcnt_loc;

extern int DatastreamFlag; //debug only remove after

extern char UART1_TXDATA[2048];
extern char *UART1_RXDATA;
extern char UART2_TXDATA[64];
extern char *UART2_RXDATA;

extern char CMD_psc[];

extern unsigned int error_flag_1;
extern unsigned int error_flag_2;
extern unsigned int error_flag_3;
extern unsigned int error_flag_4;

extern double totalinputcurrent;

extern double bp_v;			//buoy power voltage level
extern double VMAX;			//maximum system voltage before we start disconnecting relays
extern int failsafe_flag;   //flag so that we only emergency disconnect relays once

extern int failsafe_check;
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
extern const int HYDi2c_CH; // ppw 08032026
extern const int HYD_SADDR;
extern const int BUOYPWRi2c_CH;
extern const int BUOYPWR_SADDR;
extern const int RTCLKi2c_CH;
extern const int RTCLK_SADDR;
extern const int PSCHTLi2c_CH;
extern const int PSCHTL_SADDR;
extern const int EEPROMi2c_CH;
extern const int EEPROM_SADDR;

//prototypes
int response();
void datastream(int inhibit);
void helplist();
void reply(char *cmd, char *device, char *status);

int src_id_chk(char *srcid);
int valid_src_cmd(char *command);

void failsafe();

extern int chksumchk(char *chksum, char *cmd);
extern void connect(char *cmd, int srcnum, char *cmdstat);
extern void datastream(int inhibit);
extern void get_id(char *id_message);

extern void chk_bt(char *bt_status, char *bt_detail, double *bt_v, double *bt_t);
extern void chk_pv(char *pv_status,char *detail);
extern void chk_bp(char *bp_status, char *psc_t);
extern void chk_ext_pwr(char *ex_status);
extern void chk_psch(char *psch_status);

extern void chk_hyd(char *hyd_status, double *H);
extern int cal_hyd(void);

extern void tx_comm();

extern int i2c_putstat1(int i2c_ch, int SlaveAddress, int regaddr,unsigned char *I2CWrite, int wrt_cnt);

extern void rtclk_reset();


