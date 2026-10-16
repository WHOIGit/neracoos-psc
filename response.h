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

extern const int MAXEEPROMBYTES;

extern LOGGER_STATUS loggerStatus;
extern char filename[11];

extern int inhibitdatastream;
extern int datastreamcnt;
int datastreamcnt_loc;
extern int brd_config;

extern int elapsedtimecnt;

extern int DatastreamFlag; //debug only remove after
extern unsigned int override_flag;
extern long intcc1, intcc2, intcc3, intcc4; //debug logging only. do not need for real

extern char UART1_TXDATA[2048];
extern char *UART1_RXDATA;
extern char UART2_TXDATA[64];
extern char *UART2_RXDATA;
extern int ST_override;
extern char CMD_psc[];
extern char ID_override[];
extern char fuel_level[];
extern unsigned int error_flag_1;
extern unsigned int error_flag_2;
extern unsigned int error_flag_3;
extern unsigned int error_flag_4;
extern double batt_chg_status;
extern int OverAsbChgVolt;
extern int OverCondChgVolt;
extern double totalinputcurrent;
extern int CVTR_SWGF_TEST;
extern int swgf_dly;
extern unsigned char cvtrbuffer[75];		// fixed rev 9

extern double DCVLMAX;			//the maximum delta between CVL Enable and CVL Disable limits
extern double CVLMIN;			//the minimum value for the CVL Disable limit
extern double CVLMAX;			//the maximum value for the CVL ENable Limit
extern double SOCMIN;			// Minimum recommended charge cycle threshold
extern double bp_v;			//buoy power voltage level
extern double VMAX;			//maximum system voltage before we start disconnecting relays
extern int failsafe_flag;   //flag so that we only emergency disconnect relays once
extern int failsafe_enable; //enable for the voltage failsafe

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

extern char cvt_err_detail[32][50];
//prototypes
int response();
void datastream(int inhibit);
void helplist();
void reply(char *cmd, char *device, char *status);
int update_override_flag(int srcnum);
int src_id_chk(char *srcid);
int valid_src_cmd(char *command);
void datastreamlogger(char *message);
void eepromWrite(char *location, char *data);
void eepromRead(char *location, char *length, char *data);
void get_pvcntlstat(char *cmd, char *status_data, char *detail);
//int cmd_pvcntl(char *cmd, char *cmdtype);
void setgetcvl(int IO, char *enable_level, char *disable_level, char *status);
int displaycvterrdetail();
//void chksumgen_fc();
void voltage_failsafe();

extern int chksumchk(char *chksum, char *cmd);
extern void connect(char *cmd, int srcnum, char *cmdstat);
extern void datastream(int inhibit);
extern void setgettime(char *cmd, int IO, char *newtime, char *cmdstat);
extern void get_id(char *id_message);
extern void set_id(char *PN, int pnstrlen, char *SN, int snstrlen, char *HW_REV, char *cmdstat);
extern void chk_bt(char *bt_status, char *bt_detail, double *bt_v, double *bt_t);
extern void chk_pv(char *pv_status,char *detail);
extern void chk_bp(char *bp_status, char *psc_t);
extern void chk_ext_pwr(char *ex_status);
extern void chk_psch(char *psch_status);
//extern void chk_fl(char *fl_status);
extern void chk_hyd(char *hyd_status);
extern void chk_conv(char *conv_status, char *cvtr_detail, char *swgf_status);
extern void wd_speed(char *speed, char *cmdstat);
extern void tx_comm();
extern int set_override_state();
extern unsigned long get_elapsed_time();
extern void set_elapsed_time(unsigned long etime);
extern int logfilename(char *timestamp);
extern void resetcoulombcnts();
extern void EEPROMInit();
extern void ClrResetFlag();
extern void cvt_ctrl(int cmdnum, char *cmdstat);		
//extern FSFILE * FSfopen(const char * fileName, const char *mode);
extern int i2c_putstat1(int i2c_ch, int SlaveAddress, int regaddr,unsigned char *I2CWrite, int wrt_cnt);
extern void setgetcvtregister(int type, char *location, char *length, char *data);
//extern void fc_ctrl(int scr_id, int cmdnum, char *cmdstat);
extern int leddrive(int state);
extern void setdsf();
extern void set_timing_external(int state);
extern void rtclk_reset();
extern void setgetchrg(int IO, char *chrg_level, char *status);  // Set or read the charge cycle threshold
