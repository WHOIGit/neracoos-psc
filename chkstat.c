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
* 
* MPSC Rework:
* REV 1 Dan B/Paul W    10/19/2026          REV01 version for New Mini-PSC CCA
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This routine consists of the current/voltage/temperature sensing 
* measurements
**********************************************************************/
#include "chkstat.h"
#include "io_config.h"


unsigned char *pRead;
unsigned char *pWrite;
int i,i2c_channel;
char rx_data[15]; 
char *end;

extern int PWRIN_STATE[];

void chk_bt(char *bt_status,char *bt_detail, double *bt_v, double *bt_t) //DMB: bt_v and bt_t used for maximum values only
{
	double bt1_i, bt1_v, bt1_t, bt2_i, bt2_v, bt2_t;	
	double btc_i, btc_v, btc_t, btd_i, btd_v, btd_t;	
	char bt1_status[30], bt2_status[30];
	char bt1_detail[50], bt2_detail[50];
	unsigned int length = 6;
	int regaddr = 0; //,n = 0,m = 0;
	unsigned char i2cdata[15];

	*bt_v = 0;
	*bt_t = 0;
    
    //check status of bt1 charging
	if(i2c_getstat1(BATTERYi2c_CH,BATT1_C_SADDR,regaddr,length,i2cdata))
	{
		btc_i = 0.0;
		btc_v = -1.0;
		btc_t = 0.0;
	}  
	else
	{
		btc_i = get_data1(i2cdata, 0, deltaI/Rsenbt/1000.0);	//in mA
		btc_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
		btc_t = get_bt_temp(i2cdata, 4, BT12OVERTEMP, BATTTEMP);	//S1B2 temp in degree C
	}

    //check status of bt1 discharging
	if(i2c_getstat1(BATTERYi2c_CH,BATT1_D_SADDR,regaddr,length,i2cdata))
	{
		btd_i = 0.0;
		btd_v = -1.0;
		btd_t = 0.0;
	}  
	else
	{
		btd_i = get_data1(i2cdata, 0, deltaI/Rsenbt/1000.0);		//in mA
		btd_v = get_data1(i2cdata, 2, deltaV/1000.0);				//in V
		btd_t = get_bt_temp(i2cdata, 4, BT11OVERTEMP, BATTTEMP);		//S1B1 temp in degree C
	}
	
	sprintf(bt1_detail, "%s %.2f %.2f %.2f %.2f %.2f %.2f", ID_bt1, btd_t, btc_t, btc_v, btc_i, btd_v, btd_i);	

	bt1_i = (btd_i - btc_i);
    
    if ((btd_v > 0) && (btc_v > 0)) //DMB: Since this is averaged (not just max-checked) we need to check is valid first
	bt1_v = batterystringvoltage(btd_v, btc_v); //If both good then average
    else if (btd_v > 0) //If only discharge value good use that
    bt1_v = btd_v;
    else if (btc_v > 0)  //If only charge value good so use that
    bt1_v = btc_v;
   
	bt1_t = max(btd_t,btc_t);
#if 0	
    if(bt1_v != -1.0)
	{
		*bt_v += bt1_v;
		n++;
	}
	*bt_t += bt1_t;
	m++;
#endif
	sprintf(bt1_status, "%s %.2f %.2f %.2f", ID_bt1, bt1_t, bt1_v, bt1_i);

	//check status of bt2 charging
	if(i2c_getstat1(BATTERYi2c_CH,BATT2_C_SADDR,regaddr,length,i2cdata))
	{
		btc_i = 0.0;
		btc_v = -1.0;
		btc_t = 0.0;
	}  
	else
	{
		btc_i = get_data1(i2cdata, 0, deltaI/Rsenbt/1000.0);		//in mA
		btc_v = get_data1(i2cdata, 2, deltaV/1000.0);				//in V
		btc_t = get_bt_temp(i2cdata, 4, BT22OVERTEMP, BATTTEMP);		//S2B2 temp in C
	}

    //check status of bt2 discharging
	if(i2c_getstat1(BATTERYi2c_CH,BATT2_D_SADDR,regaddr,length,i2cdata))
	{
		btd_i = 0.0;
		btd_v = -1.0;
		btd_t = 0.0;
	}  
	else
	{
		btd_i = get_data1(i2cdata, 0, deltaI/Rsenbt/1000.0);			//in mA
		btd_v = get_data1(i2cdata, 2, deltaV/1000.0);					//in V
		btd_t = get_bt_temp(i2cdata, 4, BT21OVERTEMP, BATTTEMP);			//S2B1 temp in C
	}

	sprintf(bt2_detail, "%s %.2f %.2f %.2f %.2f %.2f %.2f", ID_bt2, btd_t, btc_t, btc_v, btc_i, btd_v, btd_i);	
	
	bt2_i = (btd_i - btc_i);
    
    if ((btd_v > 0) && (btc_v > 0)) //DMB: Since this is averaged (not just max-checked) we need to check is valid first
	bt2_v = batterystringvoltage(btd_v, btc_v); //If both good then average
    else if (btd_v > 0) //If only discharge value good use that
    bt2_v = btd_v;
    else if (btc_v > 0)  //If only charge value good so use that
    bt2_v = btc_v;
   
	bt2_t = max(btd_t,btc_t);
    
#if 0
	if(bt2_v != -1.0)
	{
		*bt_v += bt2_v;
		n++;
	}
	*bt_t += bt2_t;
	m++;
#endif
    
    //DMB: bt_t and bt_v are only used to check maximum levels for safety
    *bt_t = max(bt1_t, bt2_t);
    *bt_v = max(bt1_v, bt2_v);
    
	sprintf(bt2_status, "%s %.2f %.2f %.2f", ID_bt2, bt2_t, bt2_v, bt2_i);
    
   	sprintf(bt_status," %s %s", bt1_status, bt2_status);

	sprintf(bt_detail," %s %s", bt1_detail, bt2_detail);
}

void chk_pv(char *pv_status, char *pv_detail)
{
	double pv_i, pv_v;	
	char pv1_status[30], pv2_status[30], pv3_status[30], pv4_status[30];
	unsigned int length = 6;
	int regaddr = 0;
	unsigned char i2cdata[15];

	//Check the PV sensing data from PSC board.

	//check status of pv1
	if(i2c_getstat1(PVi2c_CH,PV1_SADDR,regaddr,length,i2cdata))
	{
		pv_i = 0.0;
		pv_v = -1.0;
	}
	else
	{
		pv_i = get_data1(i2cdata, 0, deltaI/Rsenpv/1000.0);		//in mA
		pv_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
	}
	sprintf(pv1_status, "%s %d %.2f %.2f", ID_pv1, PV_IN_CH_ON[0], pv_v, pv_i); 
	totalinputcurrent -= pv_i;

	//check status of pv2
	if(i2c_getstat1(PVi2c_CH,PV2_SADDR,regaddr,length,i2cdata))
	{
		pv_i = 0.0;
		pv_v = -1.0;
	}
	else
	{
		pv_i = get_data1(i2cdata, 0, deltaI/Rsenpv/1000.0);		//in mA
		pv_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
	}
	sprintf(pv2_status, "%s %d %.2f %.2f", ID_pv2, PV_IN_CH_ON[1], pv_v, pv_i);
	totalinputcurrent -= pv_i;
    
	//check status of pv3
	if(i2c_getstat1(PVi2c_CH,PV3_SADDR,regaddr,length,i2cdata))
	{
		pv_i = 0.0;
		pv_v = -1.0;
	}
	else
	{
		pv_i = get_data1(i2cdata, 0, deltaI/Rsenpv/1000.0);		//in mA
		pv_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
	}
	sprintf(pv3_status, "%s %d %.2f %.2f", ID_pv3, PV_IN_CH_ON[2], pv_v, pv_i);
	totalinputcurrent -= pv_i;

	//check status of pv4
	if(i2c_getstat1(PVi2c_CH,PV4_SADDR,regaddr,length,i2cdata))
	{
		pv_i=0.0;
		pv_v=-1.0;
	}
	else
	{
		pv_i = get_data1(i2cdata, 0, deltaI/Rsenpv/1000.0);		//in mA
		pv_v = get_data1(i2cdata, 2, deltaV/1000.0);			//in V
	}
	sprintf(pv4_status, "%s %d %.2f %.2f", ID_pv4, PV_IN_CH_ON[3], pv_v, pv_i);
	totalinputcurrent -= pv_i;

	sprintf(pv_status," %s %s %s %s",pv1_status, pv2_status, pv3_status, pv4_status);
}

void chk_ext_pwr(char *ex_status)
{
	double ex_i, ex_v;	
	unsigned int length = 6;
	int regaddr = 0;
	unsigned char i2cdata[15];

	if(i2c_getstat1(EXTPWRi2c_CH,EXTPWR_SADDR,regaddr,length,i2cdata))
	{
		ex_i = 0.0;
		ex_v = -1.0;
	}
	else
	{	
		ex_i = get_data1(i2cdata, 0, deltaI/Rsenex/1000.0);			//in mA
		ex_v = get_data1(i2cdata, 2, deltaV/1000.0);				//in V
	}
	sprintf(ex_status, " %s %.2f %.2f", ID_ext_pwr, ex_v, ex_i);
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
		bp_i = 0.0;
		bp_v = -1.0;
	}
	else
	{	
		bp_i = get_data1(i2cdata, 0, (deltaI/Rsenbuoy)/1000.0);		//in mA
		bp_v = get_data1(i2cdata, 2, deltaV/1000.0);				//in V
		psc_temp = (get_data1(i2cdata, 4, deltaT)-500)/Dt;		//in degree C {T = [steps*0.5(mV)-500(mV)]/10.0(mV/C)}
	}
	totalinputcurrent += bp_i;
        if(bp_v >= V_ERR)
        {
            bp_v = -1.0;
        }
	sprintf(bp_status, " %.2f %.2f", bp_v, bp_i);
	sprintf(psc_t," %.2f",psc_temp);
    
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
		psch_i = 0.0;
		psch_v = -1.0;
	}
	else
	{	
		psch_i = get_data1(i2cdata, 0, (deltaI/Rsenpsch)/1000.0);		//in mA
		psch_v = get_data1(i2cdata, 2, deltaV/1000.0);					//in V
	}
	totalinputcurrent += psch_i;
	sprintf(psch_status, " %s %.2f %.2f", ID_psc_htl, psch_v, psch_i);
}

void chk_hyd(char *hyd_status, double *H) //From BLD1-I2C-Compact
{
	unsigned int length = 14;
	int regaddr = 0x12;
	unsigned char i2cdata[15];
    double hyd_pct, temperature, humidity;
    
    if (i2c_getstat1(HYDi2c_CH, HYD_SADDR, regaddr, length, i2cdata))
    {
        hyd_pct = -100.0;
        temperature = -100;
        humidity = -1.0;
    }
    else
    {
        //DMB: Based on feedback from a application Engineer (Mumian) the value is just multipled by 0.01% to get value in percent)
        hyd_pct = (double)(((int) (i2cdata[2]) * 256 + 
                            (int)(i2cdata[3]))) *0.01;     
        
        //DMB (October 29, 2026) For now on REV01 we will use hard-coded offset, 
        //and just use the field data to see how much it changes in time, temperature and humidity
        //Fixed offset for Board Serial #001
        hyd_pct -= 1.76;
        
        //Temperature Calc: (Reading-10000)*0.01= Tenperature
        temperature = (double)( (((int) (i2cdata[0]) * 256 + 
                            (int)(i2cdata[1])) -10000)*0.01);
        
        //Humidity Calc: Reading * 0.01%
        humidity = (double)(((int) (i2cdata[4]) * 256 + 
                            (int)(i2cdata[5]))) *0.01;
    }
    *H = hyd_pct;
    int dev_state = ((int) i2cdata[7]) + (((int) i2cdata[8]) * 256);
	sprintf(hyd_status, " %s H: %.2f temp: %.2f hum: %.2f bld1_flag: 0x%04x", ID_hyd, hyd_pct, temperature, humidity, dev_state); 
}

//DMB: This has shown to be only moderately effective and for more extreme offsets, it does not retain after power cycle. Fixed offset in code is more effective for now (Nov 3,2026))
int cal_hyd(void) //DMB: Eliminates ~1% offset in BLD1-I2C-Compact (recommended by field engineer (Mumian))
{
    unsigned int length = 10;
	int regaddr = 0x15;
    int wr_length = 5;
    int write_regaddr = 0x14;
	unsigned char i2cdata[15];
    unsigned char i2c_send_data[5] = {0x02, 0x83, 0x52, 0x00, 0x00}; //DMB: First 3 are "password"
    int correction = (int) (3.75/0.012);
    int hold_val=0;

    //Read 10 byte s from ID 0x0A, register address 0x15
    if (i2c_getstat1(HYDi2c_CH, HYD_SADDR, regaddr, length, i2cdata))
    {
         return -1; //read failed
    }
     else
     {
         hold_val = (int) ((int) (i2cdata[0] * 256) + (int) i2cdata[1]);
         hold_val -= correction;
     }
    i2c_send_data[3] = (hold_val >> 8) & 0x00FF;
    i2c_send_data[4] = hold_val & 0x00FF;
    if (i2c_putstat1(HYDi2c_CH, HYD_SADDR, write_regaddr, i2c_send_data, wr_length))
    {
        return -1; //write failed
    }
    return 0; //success
}

