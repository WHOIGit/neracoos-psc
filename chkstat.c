/**********************************************************************
*    WHOI/Raytheon, 2010
*
* FileName:        	chkstat.c
* Dependencies:     Header (.h) files if applicable, see code below
* Processor:       	PIC24FJ256GB106
* Compiler:        	MPLAB C30 v3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev. 	Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1		Joseph Zhu    	10/28/2010		Initial File Creation 
* 15    Alex Franks     11/25/2019      Changed logic of external charge function to only (dis)connect PV and WT inputs and not reconnect anything
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This routine consists of the current/voltage/temperature sensing 
* measurements
**********************************************************************/
#include "chkstat.h"
#include "io_config.h"
//#include "commands.h"

unsigned char *pRead;
unsigned char *pWrite;
int i,i2c_channel;
char rx_data[15]; 
char *end;

extern int PWRIN_STATE[];

void chk_bt(char *bt_status,char *bt_detail, double *bt_v, double *bt_t)
{
	double bt1_i, bt1_v, bt1_t, bt2_i, bt2_v, bt2_t;	
	double btc_i, btc_v, btc_t, btd_i, btd_v, btd_t;	
	char bt1_status[30], bt2_status[30];
	char bt1_detail[50], bt2_detail[50];
	unsigned int length = 6;
	int regaddr = 0,n = 0,m = 0;
	unsigned char i2cdata[15];

	bt_v = 0;
	bt_t = 0;
    //check status of bt1 charging
	if(i2c_getstat1(BATTERYi2c_CH,BATT1_C_SADDR,regaddr,length,i2cdata))
	{
		error_flag_1 |= BT1CHGSENFLT; 	//update error_flag
		btc_i = 0.0;
		btc_v = -1.0;
		btc_t = 0.0;
	}  
	else
	{
		error_flag_1 &= ~BT1CHGSENFLT; 	//update error_flag
		btc_i = get_data1(i2cdata, 0, deltaI/Rsenbt/1000.0);	//in mA
		btc_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
		btc_t = get_bt_temp(i2cdata, 4, BT12OVERTEMP, BATTTEMP);	//S1B2 temp in degree C
	}

    //check status of bt1 discharging
	if(i2c_getstat1(BATTERYi2c_CH,BATT1_D_SADDR,regaddr,length,i2cdata))
	{
		error_flag_1 |= BT1DCHGSNFLT;					//update error_flag
		btd_i = 0.0;
		btd_v = -1.0;
		btd_t = 0.0;
	}  
	else
	{
		error_flag_1 &= ~BT1DCHGSNFLT; 				//update error_flag
		btd_i = get_data1(i2cdata, 0, deltaI/Rsenbt/1000.0);		//in mA
		btd_v = get_data1(i2cdata, 2, deltaV/1000.0);				//in V
		btd_t = get_bt_temp(i2cdata, 4, BT11OVERTEMP, BATTTEMP);		//S1B1 temp in degree C
	}
	
	sprintf(bt1_detail, "%s %.2f %.2f %.2f %.2f %.2f %.2f", ID_bt1, btd_t, btc_t, btc_v, btc_i, btd_v, btd_i);	

	bt1_i = (btd_i - btc_i);										
	if((abs(bt1_i)>=BATTSTRINGCURRENTFULLSCALE)||((error_flag_1 & BT1CHGSENFLT)>>12)||((error_flag_1 & BT1DCHGSNFLT)>>13))
	{
		if(!pwrsenflt())	//no other power sensor error!
		{	
			bt1_i = totalinputcurrent / 4.0;
		}
		else				//otherwise, coulomb counting has null effect
		{
			ValleyBottom = FULLCHARGE * (-0.9);		//set at 90% of DoD
		}	
	}	 

	bt1_v = batterystringvoltage(btd_v, btc_v);
	bt1_t = max(btd_t,btc_t);
	if(bt1_v != -1.0)
	{
		*bt_v += bt1_v;
		n++;
	}
	*bt_t += bt1_t;
	m++;

	sprintf(bt1_status, "%s %.2f %.2f %.2f", ID_bt1, bt1_t, bt1_v, bt1_i);

	//check status of bt2 charging
	if(i2c_getstat1(BATTERYi2c_CH,BATT2_C_SADDR,regaddr,length,i2cdata))
	{
		error_flag_1 |= BT2CHGSENFLT; 				//update error_flag
		btc_i = 0.0;
		btc_v = -1.0;
		btc_t = 0.0;
	}  
	else
	{
		error_flag_1 &= ~BT2CHGSENFLT; 				//update error_flag
		btc_i = get_data1(i2cdata, 0, deltaI/Rsenbt/1000.0);		//in mA
		btc_v = get_data1(i2cdata, 2, deltaV/1000.0);				//in V
		btc_t = get_bt_temp(i2cdata, 4, BT22OVERTEMP, BATTTEMP);		//S2B2 temp in C
	}

    //check status of bt2 discharging
	if(i2c_getstat1(BATTERYi2c_CH,BATT2_D_SADDR,regaddr,length,i2cdata))
	{
		error_flag_1 |= BT2DCHGSNFLT;					//update error_flag
		btd_i = 0.0;
		btd_v = -1.0;
		btd_t = 0.0;
	}  
	else
	{
		error_flag_1 &= ~BT2DCHGSNFLT;					//update error_flag
		btd_i = get_data1(i2cdata, 0, deltaI/Rsenbt/1000.0);			//in mA
		btd_v = get_data1(i2cdata, 2, deltaV/1000.0);					//in V
		btd_t = get_bt_temp(i2cdata, 4, BT21OVERTEMP, BATTTEMP);			//S2B1 temp in C
	}

	sprintf(bt2_detail, "%s %.2f %.2f %.2f %.2f %.2f %.2f", ID_bt2, btd_t, btc_t, btc_v, btc_i, btd_v, btd_i);	
	
	bt2_i = (btd_i - btc_i);										
	if((abs(bt2_i)>=BATTSTRINGCURRENTFULLSCALE)||((error_flag_1 & BT2CHGSENFLT)>>14)||((error_flag_1 & BT2DCHGSNFLT)>>15))
	{
		if(!pwrsenflt())
		{	
			bt2_i = totalinputcurrent / 4.0;
		}
		else
		{
			ValleyBottom = FULLCHARGE * (-0.1);		//set at 90% of DoD
		}	
	}	 
	bt2_v = batterystringvoltage(btd_v, btc_v);
	bt2_t = max(btd_t,btc_t);
	if(bt2_v != -1.0)
	{
		*bt_v += bt2_v;
		n++;
	}
	*bt_t += bt2_t;
	m++;

	sprintf(bt2_status, "%s %.2f %.2f %.2f", ID_bt2, bt2_t, bt2_v, bt2_i);
    
   	sprintf(bt_status," %s %s", bt1_status, bt2_status);

	sprintf(bt_detail," %s %s", bt1_detail, bt2_detail);

	batterycharging( bt1_t, bp_v, bt1_i, bt2_i );	
}

void chk_pv(char *pv_status, char *pv_detail)
{
	double pv_i, pv_v;	
	char pv1_status[30], pv2_status[30], pv3_status[30], pv4_status[30];
	char pv1_detail[50], pv2_detail[50], pv3_detail[50], pv4_detail[50];
    char tempdata[100];
	unsigned int length = 6;
	int regaddr = 0;
	unsigned char i2cdata[15];

	//Check the PV sensing data from PSC board.

	//check status of pv1
	if(i2c_getstat1(PVi2c_CH,PV1_SADDR,regaddr,length,i2cdata))
	{
		error_flag_2 |= PV1SENSORFLT;		//update error_flag
		pv_i = 0.0;
		pv_v = -1.0;
	}
	else
	{
		error_flag_2 &= ~PV1SENSORFLT;		//update error_flag
		pv_i = get_data1(i2cdata, 0, deltaI/Rsenpv/1000.0);		//in mA
		pv_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
	}
	sprintf(pv1_status, "%s %d %.2f %.2f", ID_pv1, device_state(PV1MASK), pv_v, pv_i); 
    get_pvcntlstat("pv1_cntl",tempdata,pv1_detail);
	totalinputcurrent -= pv_i;
	flagRelayFault(pv_i,pv_v,PV1MASK,PV1RELAYFLT);

	//check status of pv2
	if(i2c_getstat1(PVi2c_CH,PV2_SADDR,regaddr,length,i2cdata))
	{
		error_flag_2 |= PV2SENSORFLT;		//update error_flag
		pv_i = 0.0;
		pv_v = -1.0;
	}
	else
	{
		error_flag_2 &= ~PV2SENSORFLT;		//update error_flag
		pv_i = get_data1(i2cdata, 0, deltaI/Rsenpv/1000.0);		//in mA
		pv_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
	}
	sprintf(pv2_status, "%s %d %.2f %.2f", ID_pv2, device_state(PV2MASK), pv_v, pv_i);
    get_pvcntlstat("pv2_cntl",tempdata,pv2_detail);
	totalinputcurrent -= pv_i;
	flagRelayFault(pv_i,pv_v,PV2MASK,PV2RELAYFLT);
    
	//check status of pv3
	if(i2c_getstat1(PVi2c_CH,PV3_SADDR,regaddr,length,i2cdata))
	{
		error_flag_2 |= PV3SENSORFLT;		//update error_flag
		pv_i = 0.0;
		pv_v = -1.0;
	}
	else
	{
		error_flag_2 &= ~PV3SENSORFLT;		//update error_flag
		pv_i = get_data1(i2cdata, 0, deltaI/Rsenpv/1000.0);		//in mA
		pv_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
	}
	sprintf(pv3_status, "%s %d %.2f %.2f", ID_pv3, device_state(PV3MASK), pv_v, pv_i);
    get_pvcntlstat("pv3_cntl",tempdata,pv3_detail);
	totalinputcurrent -= pv_i;
	flagRelayFault(pv_i,pv_v,PV3MASK,PV3RELAYFLT);

	//check status of pv4
	if(i2c_getstat1(PVi2c_CH,PV4_SADDR,regaddr,length,i2cdata))
	{
		error_flag_2 |= PV4SENSORFLT;		//update error_flag
		pv_i=0.0;
		pv_v=-1.0;
	}
	else
	{
		error_flag_2 &= ~PV4SENSORFLT;		//update error_flag
		pv_i = get_data1(i2cdata, 0, deltaI/Rsenpv/1000.0);		//in mA
		pv_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
	}
	sprintf(pv4_status, "%s %d %.2f %.2f", ID_pv4, device_state(PV4MASK), pv_v, pv_i);
    get_pvcntlstat("pv4_cntl",tempdata,pv4_detail);
	totalinputcurrent -= pv_i;
	flagRelayFault(pv_i,pv_v,PV4MASK,PV4RELAYFLT);

	sprintf(pv_status," %s %s %s %s",pv1_status, pv2_status, pv3_status, pv4_status);
    sprintf(pv_detail," %s %s %s %s %s %s %s %s",pv1_status,pv1_detail,pv2_status,pv2_detail,pv3_status,pv3_detail,pv4_status,pv4_detail);
}

void chk_ext_pwr(char *ex_status)
{
	double ex_i, ex_v;	
	unsigned int length = 6;
	int regaddr = 0, i;
	extern const double EXTPWR_PRESENT;
	unsigned char i2cdata[15];

	//if the External Charging Power is sensed, turn off all power input source relay!
	if(i2c_getstat1(EXTPWRi2c_CH,EXTPWR_SADDR,regaddr,length,i2cdata))
	{
		error_flag_2 |= EXTPWRSENFLT;		//update error_flag
		ex_i = 0.0;
		ex_v = -1.0;
	}
	else
	{	
		error_flag_2 &= ~EXTPWRSENFLT;		//update error_flag
		ex_i = get_data1(i2cdata, 0, deltaI/Rsenex/1000.0);			//in mA
		ex_v = get_data1(i2cdata, 2, deltaV/1000.0);				//in V
		if(ex_i > EXTPWR_PRESENT)						//sense ext_pwr input presents
		{
			if((override_flag & 0x0400) == 0)			//if it is not in external power override mode
			{
				rlydrive(ALLMASK, 0);					//turn off all relays! regardless of override
				override_flag |= 0x0400;				//set external power flag (bit #10)
				//CONVOUTCNTL = 0x0;						//disable the CVTR output (AHF commented these two lines out to keep CVT status the same)
				//CONVRST = 0x0;							//power down the CVTR
			}
		}
		else											//if the ext pwr is not present
		{
			if((override_flag & 0x0400) != 0)			//if it was in external power override mode
			{
				override_flag &= 0xFBFF;				//clear external power flag
				//RestoreRlyState();						//restore the state of relays (AHF: don't do this)
				regaddr = BATTLVL1_REGADDR;
				//if not assume full charge by ext, then add routine to calculate SOC!!!!!
				for(i=0;i<4;i++)
				{
					set_battchrg_level1(regaddr+i*4,0,0.000);			//assume external power removed because battery is fully charged
				}
				secondcnt = 0;
				batt_chg_status = 100;
			}
			else										//it was not in external power override mode, and is not now
			{
				//do nothing for now	
			}
		}
		
	}
	sprintf(ex_status, " %s %.2f %.2f", ID_ext_pwr, ex_v, ex_i);
//ACTION INSERT HERE (if ST_override is OFF)
}

void chk_bp(char *bp_status, char *psc_t)
{
	double bp_i,psc_temp;
	unsigned int length = 6;
	int regaddr = 0;
	unsigned char i2cdata[15];
	
	//check PSC 24V Buoy output power stat
	if(i2c_getstat1(BUOYPWRi2c_CH,BUOYPWR_SADDR,regaddr,length,i2cdata))
	{
		error_flag_3 |= BUOYPWSENFLT;		//update error_flag
		bp_i = 0.0;
		bp_v = -1.0;
	}
	else
	{	
		error_flag_3 &= ~BUOYPWSENFLT;		//update error_flag
		bp_i = get_data1(i2cdata, 0, (deltaI/Rsenbuoy)/1000.0);		//in mA
		bp_v = get_data1(i2cdata, 2, deltaV/1000.0);				//in V
		psc_temp = (get_data1(i2cdata, 4, deltaT)-500)/Dt;		//in degree C {T = [steps*0.5(mV)-500(mV)]/10.0(mV/C)}
	}
	totalinputcurrent += bp_i;
        if(bp_v >= V_ERR)
        {
            error_flag_3 |= BUOYPWSENFLT;		//update error_flag
            bp_v = -1.0;
        }
	sprintf(bp_status, " %.2f %.2f", bp_v, bp_i);
	sprintf(psc_t," %.2f",psc_temp);

//ACTION INSERT BELOW (if ST_override is OFF)

	//manage High Voltage Power Converter:
	if((bp_v > cvtr_cvl_enab)&(!((override_flag>>8)&0x5)))		//if CVTR is not override & system is not charged by external power & 24VDC bus level exceed cvrt enable_level
	{
		switchcvtrrelay(1);	//turn on the CVTR Input Relay with read-mod-write
		CONVOUTCNTL = 1;	//enable the CVTR output
	}
	if((bp_v < cvtr_cvl_disab)&(!((override_flag>>8)&0x1)))		//if CVTR is not override & 24VDC bus level below cvrt disable_level
	{
		switchcvtrrelay(0);		//turn off the CVTR Input Relay with read-mod-write
		CONVOUTCNTL = 0;		//disable the CVTR output
	}
	if(bp_v < CVLMIN)											//if the 24VDC bus level is below the DC-DC Converter Brick input low limit, turn off cvtr
	{
		switchcvtrrelay(0);		//turn off the CVTR Input Relay with read-mod-write
		CONVOUTCNTL = 0;		//disable the CVTR output
		if((override_flag >> 8)&0x1)	//if the override control is on for CVT (or CPM had the control)
		{
			error_flag_4 |= CVTFCSHTDWN;	//turn on the CVT Force Shut down flag
		}
		else
		{
			error_flag_4 &= ~CVTFCSHTDWN;	//otherwise, turn off the CVT Force Shut Down flag
		}	
	}
}

void chk_psch(char *psch_status)
{
	double psch_i, psch_v;				//, devbrd_temp;
	unsigned int length = 6;
	int regaddr = 0;
	unsigned char i2cdata[15];

	//check PSC hotel power stat

	if(i2c_getstat1(PSCHTLi2c_CH,PSCHTL_SADDR,regaddr,length,i2cdata))
	{
		error_flag_2 |= PSCHTLSENFLT;		//update error_flag
		psch_i = 0.0;
		psch_v = -1.0;
	}
	else
	{	
		error_flag_2 &= ~PSCHTLSENFLT;		//update error_flag
		psch_i = get_data1(i2cdata, 0, (deltaI/Rsenpsch)/1000.0);		//in mA
		psch_v = get_data1(i2cdata, 2, deltaV/1000.0);					//in V
		#ifdef devboard //next line for DEV Board only:
		devbrd_temp = (get_data1(i2cdata, 4, deltaT)-500)/Dt;		//in degree C {T = [steps*0.5(mV)-500(mV)]/10.0(mV/C)}
		#endif
	}
	totalinputcurrent += psch_i;
	sprintf(psch_status, " %s %.2f %.2f", ID_psc_htl, psch_v, psch_i);
	#ifdef devboard
	sprintf(psch_status,"%s %.2f",psch_status, devbrd_temp);
	#endif
//ACTION INSERT HERE (if ST_override is OFF)	
}

// ppw 08032026

void chk_hyd(char *hyd_status)
{
    //char hyd_status[30];
	unsigned int length = 14;
	int regaddr = 0;
	unsigned char i2cdata[15];
    double hyd_pct;
    
    if (i2c_getstat1(HYDi2c_CH, HYD_SADDR, regaddr, length, i2cdata))
    {
        error_flag_2 |= HYDSENSORFLT;
        hyd_pct = -1.0;
    }
    else
    {
        error_flag_2 &= HYDSENSORFLT;
        hyd_pct = (double)(((int) (i2cdata[2]) * 256 + 
                            (int)(i2cdata[3]))) / 10000.0;
    }
    int dev_state = ((int) i2cdata[7]) + (((int) i2cdata[8]) * 256);
	sprintf(hyd_status, " %s %d %.2f", ID_hyd, dev_state, hyd_pct); 
}

void chk_conv(char *conv_status, char *cvtr_detail, char *swgf_status)
{
	int i,CONVSTATE=0;
	double cvi_v = 0.0, cvi_i = 0.0, cviv[5]={}, cvii[5]={}, cvraw_v = 0.0, cvraw_i = 0.0;
	double cvo_v = 0.0, cvo_i =0.0, cv_t = 0.0, cvtt[5]={};
	double swgf_read_p = 0.0, swgf_read_n = 0.0;

	if(i2c_getstat1(CVTR_CH,CVTR_SADDR,CVTR_REG1,75,cvtrbuffer))		// s/b 75 - fixed rev 9
	{
		error_flag_3 |= CVTRGENFLT;
	}
	else
	{
		error_flag_3 &= ~CVTRGENFLT;
		for(i=0;i<4;i++)
		{
			cviv[i+1] = get_data1(cvtrbuffer,CVTR_REG1+(i*4+2),deltaV/1000.0);				//in V
			cvi_v += cviv[i+1];
			cvii[i+1] = get_data1(cvtrbuffer,CVTR_REG1+(i*4),deltaI/Rsencvi/1000.0);		//in mA
			cvi_i += cvii[i+1];
			cvtt[i+1] = get_bt_temp(cvtrbuffer,CVTR_REG1+(i*6+20), CVTRGENFLT,CVTRTEMP);	    //in (degree) C
			cv_t += cvtt[i+1];
		}
		cvi_v = cvi_v/4;
		cv_t = cv_t/4;	
		cvo_v = (get_data1(cvtrbuffer,CVTR_OV_REG,deltaI/RsenHVI/1000.0) * RsenHVV);			//in V
		cvo_i = get_data1(cvtrbuffer,CVTR_OI_REG,deltaI/Rsencvo/1000.0);					//in mA
		//if the interlock is on (CVTRINTLCKSTAT = 1: Interlock is on or inhibit output; CVTRINTLCKSTAT = 0: output permitted)
		CVTRINTLCKSTAT = 0;			//assume there is no the interlock jumper, there won't be hv output
		if(get_data1(cvtrbuffer,CVTR_INTLCK_V_REG,deltaV/1000.0) > 20.0) CVTRINTLCKSTAT = 1;	//detect interlock jumper, there will be hv output!
		//if the converter brick input power is ordered?
		CONVPWRSTAT = 0;
		if((((cvtrbuffer[CVTR_CTRL_REG])>>3) & 0x01) == 1) CONVPWRSTAT = 1;
		//if the converter brick output enable is ordered?
		//CONVOUTSTAT = 0;
		//if((((cvtrbuffer[CVTR_CTRL_REG])>>4) & 0x01) == 1) CONVOUTSTAT = 1;

		swgf_read_p = get_data1(cvtrbuffer,CVTR_SWGFREADING_P_REG,1.0);
		swgf_read_n = get_data1(cvtrbuffer,CVTR_SWGFREADING_N_REG,1.0);
	
	}
	if((CONVPWRSTAT) & (CVTRINTLCKSTAT) & (abs(bp_v - cvi_v) > 20.0))
	{
		error_flag_2 |= CONVINPTFUSE;		// Either a blown fuse or there's no CVTR board (Sept. 2014)
	}
	else
	{
		error_flag_2 &= ~CONVINPTFUSE;
	}

	//Get conv status
	cvraw_v = get_data1(cvtrbuffer,54, deltaV/1000.0);
	cvraw_i = get_data1(cvtrbuffer,52,deltaI/RsenCVRAW/1000.0);

	sprintf(cvtr_detail," cvt %.2f %.2f %02x%02x%02x%02x cvi %d %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f cvo %d %.2f %.2f %d swgft %d %.2f %.2f", 
						cvraw_v, cvraw_i, cvtrbuffer[58],cvtrbuffer[59],cvtrbuffer[56],cvtrbuffer[57],
						CONVPWRSTAT,cvi_v, cvii[1], cvii[2], cvii[3], cvii[4], cvtt[1], cvtt[2], cvtt[3], cvtt[4],
						CONVOUTSTAT,cvo_v,cvo_i,CVTRINTLCKSTAT,CVTR_SWGF_TEST,swgf_read_p,swgf_read_n);
	
	CONVSTATE = CONVPWRSTAT & CONVOUTSTAT;
	sprintf(conv_status," swg %d %.2f %.2f cvt %d %.2f %.2f %d %.2f",CVTR_SWGF_TEST,swgf_read_p,swgf_read_n,CONVSTATE,cvo_v,cvo_i,CVTRINTLCKSTAT,cvtt[1]);
	sprintf(swgf_status," swg %d %.2f %.2f",CVTR_SWGF_TEST,swgf_read_p,swgf_read_n);
}

int device_state(unsigned int mask)
{
	int state;		//assume device relay is in OFF state

	if((state = (mask & rlystat) & 0xFF)==0)
		return 0;
	else
		return 1;
}

int pwrsenflt()
{
	if(((error_flag_2 & PV1SENSORFLT)>> 4)
	 ||((error_flag_2 & PV2SENSORFLT)>> 5)
	 ||((error_flag_2 & PV3SENSORFLT)>> 6)
	 ||((error_flag_2 & PV4SENSORFLT)>> 7)
//	 ||((error_flag_2 & SPCHTLSENFLT)>> 13)			//PSC Hotel Power Sensor Fault is not important
	 ||((error_flag_3 & BUOYPWSENFLT)))
	{
		return TRUE;		//other power current sensor fault(s) is/are ture
	}		
	else
	{
		return FALSE;		//no other power current sensor fault
	}	
}

