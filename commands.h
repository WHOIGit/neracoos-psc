/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			commands.h
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
* This file consists of the commands.c header 
*
*************************************************************************/
#define FCY 16000000UL  	//FCY = Fosc/2 for delay timing

#include <libpic30.h>		//for __delay_ms()
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern unsigned int FWRevision;

#define MIN(a,b)    ((a) < (b) ? (a) : (b)) 
#define MAX(a,b)    ((a) > (b) ? (a) : (b)) 

#define EEPROMWP LATDbits.LATD11

/*rlycntl: 	using TWO MAX4823 to drive 8 latching relays, RLY1 for set 
coil, RLY2 for reset coil. when the control bit is zoro, the coil is not 
driven. below is relay control bit assignment: 
			s_wt1, s_wt2, s_pv1, s_pv2, s_pv3, s_pv4, s_fc1, s_fc2,
			r_wt1, r_wt2, r_pv1, r_pv2, r_pv3, r_pv4, r_fc1, r_fc2,
On firmware hard reset, none of the bits are driven to preserve the relay 
states.
*/
unsigned int rlycntl=0x0; 
/*rlystat: using two MAX4823 again to drive 16 LEDs. The green LEDs in LED1
represents the set coil state. The red LEDs in LED2 represents the reset coil
state. On Firmware hard reset, the led state will reflect the rlystat state 
saved in EEPROM. rlystat bits assign according to rlycntl assignment history.
*/
unsigned int PV1MASK = 0x0404;
unsigned int PV2MASK = 0x0808;
unsigned int PV3MASK = 0x1010;
unsigned int PV4MASK = 0x2020;
unsigned int STDMASK = 0x3f3f;		//only WTs and PVs
unsigned int CVIMASK = 0xCCCC;		//more of a flag than mask
unsigned int CVRMASK = 0xDDDD;		//more of a flag than mask
unsigned int ALLMASK = 0xFFFF;

#define RLY 0	//for controlling SS1_RLY_CE
#define LED 1	//for controlling SS1_LED_CE

extern int swgfcnt;
extern int swgf_dly;

const double pvcntl_R1=220000.0;
const double pvcntl_R2=11000.0;
const double pvcntl_ref=1.23;

extern const int MAXEEPROMBYTES;

extern char * pRead;
extern char * pWrite;

extern int ledflag;
extern int rlystat;
extern int override_flag;
extern unsigned int error_flag_1;
extern unsigned int error_flag_2;
extern unsigned int error_flag_3;
extern unsigned int error_flag_4;
//extern int CONVSTATEFLAG;
extern int CONVPWRSTAT;
extern int CONVOUTSTAT;
extern int CVTRINTLCKSTAT;
extern int CVTR_SWGF_TEST;
extern double cvtr_cvl_enab, cvtr_cvl_disab;
extern double chrg_thrshld;
extern unsigned char cvtrbuffer[75];

extern long FULLCHARGE;
extern double fracc1,fracc2,fracc3,fracc4;
extern long intcc1,intcc2,intcc3,intcc4;
extern double ValleyBottom;
extern int secondcnt;
extern int minutecnt;
extern double batt_chg_status;
extern double Absorption_Volt[180];
extern double Conditioning_Volt[180];
extern int OverAsbChgVolt;
extern int OverCondChgVolt;
extern int OverAsbChgCnt; 
extern int OverCondChgCnt;
extern char UART2_TXDATA[200];

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
extern const int FCGAUGE_REGADDR;
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

extern char filename[11];

//for group 1
extern const unsigned int BT11OVERTEMP;	//Battery 1 of String 1 Overtemp
extern const unsigned int BT12OVERTEMP;	//Battery 2 of String 1 Overtemp
extern const unsigned int BT21OVERTEMP;	//Battery 1 of String 2 Overtemp
extern const unsigned int BT22OVERTEMP;	//Battery 2 of String 2 Overtemp
extern const unsigned int BATTSTR1FUSE;	//Battery String 1 Fuse Blown
extern const unsigned int BT1CHGSENFLT;	//Battery String 1 Charging Sensor Fault
extern const unsigned int BT1DCHGSNFLT;	//Battery String 1 Disharging Sensor Fault
extern const unsigned int BT2CHGSENFLT;	//Battery String 2 Charging Sensor Fault
extern const unsigned int BT2DCHGSNFLT;	//Battery String 2 Disharging Sensor Fault
//for group 2
extern const unsigned int PV1SENSORFLT;	//PV1 Sensor Fault
extern const unsigned int PV2SENSORFLT;	//PV2 Sensor Fault
extern const unsigned int PV3SENSORFLT;	//PV3 Sensor Fault
extern const unsigned int PV4SENSORFLT;	//PV4 Sensor Fault
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

//prototypes
void connect(char *cmd, int srcnum, char *cmdstat);
char *psc_reset();
void setgettime(char *cmd, int IO, char *newtime, char *cmdstat);
void get_id(char *id_message);
void set_id(char *PN, int pnstrlen, char *SN, int snstrlen, char *HW_REV, char *cmdstat);
//void wd_speed(char *speed, char *cmdstat);
int rlydrive(unsigned int mask, int state);
int SPI1tx(unsigned int data, int CE_type);
int get_override_state();
int set_override_state();
int set_battchrg_level(int regaddr, double level);
int set_battchrg_level1(int regaddr, long intlevel, double fralevel);
long get_battchrg_level(int regaddr, double *fralevel);
int batterycharging(double bt_t, double bt_v, double bt1_i, double bt2_i);
void set_valleybtm(int regaddr, double valleybottom);
int logfilename(char *timestamp);		
//int PVConfig(int mask);
int allPVConfig();
void get_pvcntlstat(char *cmd,char *status_data, char *detail);
//int cmd_pvcntl(char *cmd, char *cmdtype);
double get_valleybtm(int regaddr);
void cvt_ctrl(int cmdnum, char *cmdstat);
void setgetcvtregister(int type, char *location, char *length, char *data);
void switchcvtrrelay(int cntl);
void setgetchrg(int IO, char *chrg_level, char *status);  // Set or read the charge cycle threshold
int leddrive(int state);

extern int i2c_getstat(int i2c_ch, int SlaveAddress, int regaddr, int length);
extern int i2c_getstat1(int i2c_ch, int SlaveAddress, int regaddr, int length, unsigned char *I2CRead);
//extern int i2c_putstat(int i2c_ch, int SlaveAddress, int regaddr);
extern int i2c_putstat1(int i2c_ch, int SlaveAddress, int regaddr,unsigned char *I2CWrite,int wrt_cnt);
extern void reply(char *cmd, char *device, char *status);
extern int set_override_state();
extern int bcd2dec(int bcd);
extern int dec2bcd(int dec);
extern char dec2char(int dec);
extern int word2dec(char *data);
extern void UpdateCoulombCount(double btx_i, long *intccx, double *fraccx); //AHF changed *intccx to long from int
extern int temp2index(double btx_t);
extern double overshootlookup(double valleybottom);
extern void update_elapsed_time();
extern void eepromWrite(char *location, char *data);
extern void eepromRead(char *location, char *length, char *data);
extern void get_pvcntlstat(char *cmd, char *status_data, char *detail);
//extern int cmd_pvcntl(char *cmd, char *cmdtype);
extern int read_pvcntlreg(int code,int SS);
extern int hex2dec(char hex);
extern void write_pvcntlreg(int pvcntlcmd, int SS);
extern void chk_wt_shunt(int wt);
extern void resetcoulombcnts();
extern void cntstore(int regaddr, double intvalue, double fravalue);
extern void ClrResetFlag();
extern void setgetcvl(int IO, char *enable_level, char *disable_level, char *status);
extern int hexbyte2dec(char data);
extern char cvltoi2cdata(char *input);
extern int brd_config;
