/**********************************************************************
*    WHOI/Raytheon, 2010
*
* FileName:        	commands.c
* Dependencies:     Header (.h) files if applicable, see code below
* Processor:       	PIC24FJ256GB106
* Compiler:        	MPLAB C30 v3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev. 	Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1		Joseph Zhu    	10/28/2010		Initial File Creation 
* 15    Alex Franks     11/25/2019      Changed (dis)connect all behavior to only change pv and wt states
*                                       Added leddrive function
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES: 
* This routine consists of executions of the commands received from
* UART1 serial port RX input
**********************************************************************/
#include "commands.h"
#include "io_config.h"
	
void connect(char *cmd, int srcnum, char *cmdstat)
{	
	int cntl = 0;

//connect/disconnect device, check the device status, evaluate status data and return ok or error
	if(strcmp(cmd,"connect")==0) cntl = 1;		//cntl change to 1 for connect, otherwise for disconnect cntl = 0

	switch(srcnum)
	{
		case 1:
			if(rlydrive(PV1MASK, cntl)) 		//for pv1
			{
				override_flag = override_flag | 0x0004;
			}
			break;
		case 2:
			if(rlydrive(PV2MASK, cntl))		//for pv2
			{
				override_flag = override_flag | 0x0008;
			}
			break;
		case 3:
			if(rlydrive(PV3MASK, cntl))		//for pv3
			{
				override_flag = override_flag | 0x0010;
			}
			break;
		case 4:
			if(rlydrive(PV4MASK, cntl))		//for pv4
			{
				override_flag = override_flag | 0x0020;
			}
			break;
		case 9:									//For cvtr, connect/disconnect command turn on/off cvtr inptu and output
			switchcvtrrelay(cntl);				//turn on/off the CVTR Input Relay with read-mod-write
		//	CONVOUTCNTL |= !cntl;				//enable/disable the HV Converter HV Output (via hardwire)
			CONVOUTCNTL = cntl;				//enable/disable the HV Converter HV Output (via hardwire) - changed for Rev. 8
			CONVOUTSTAT = cntl;				// update state bit
			override_flag = override_flag | 0x0100;	//once the CPM command this, it has the control right
			break;
		default:
			break;
	}
	set_override_state();
	strcpy(cmdstat,"ok");
}

void setgettime(char *cmd, int IO, char *newtime, char *status)
{
	char day=1;					//not used 
	int yyyy,mm,dd,hh,mtmt,ss;
	char message[19]={};
	char timedata[8]={};

	switch(IO)
	{
		case 0:			//get time
			i2c_getstat1(RTCLKi2c_CH,RTCLK_SADDR,0x0,7,(unsigned char *)timedata);	
			sprintf(newtime," 20%02x/%02x/%02x %02x:%02x:%02x",timedata[6],timedata[5],timedata[4],
															  timedata[2],timedata[1],timedata[0]);
			strcpy(status," ok");		
			break;
		case 1:			//set time
			sscanf(newtime,"%d/%d/%d %d:%d:%d",&yyyy,&mm,&dd,&hh,&mtmt,&ss);
			if((mm>12)|(dd>31)|(hh>24)|(mtmt>60)|(ss>60)|(yyyy<0)|(mm<0)|(dd<0)|(hh<0)|(mtmt<0)|(ss<0)) 
			{
				strcpy(status," error");
				break;
			}
			sprintf(message,"%c%c%c%c%c%c%c",dec2char(ss),dec2char(mtmt),dec2char(hh),dec2char(day),dec2char(dd),dec2char(mm),dec2char(yyyy-2000));
			i2c_putstat1(RTCLKi2c_CH,RTCLK_SADDR,0x0,(unsigned char *)message,7);		
//			ClrResetFlag();		//put here for the time being. Removed after rev 4.2
			strcpy(status," ok");		
			break;
		default:
			break;
	}
}

int logfilename(char *timestamp)		//log file changes once a day
{
	char year,month,date,day=1,hour,minute,second;
	char newfilename[11]={};
	char timedata[8]={};
	
	i2c_getstat1(RTCLKi2c_CH,RTCLK_SADDR,0x0,7,(unsigned char *)timedata);	
	sscanf(timedata,"%c%c%c%c%c%c%c",&second,&minute,&hour,&day,&date,&month,&year);
	sprintf(timestamp,"20%02x/%02x/%02x %02x:%02x:%02x",year,month,date,hour,minute,second);
	sprintf(newfilename,"%c%c%c.dat",year,month,date);
	if(strcmp(newfilename,filename)!=0)				
	{
		strcpy(filename,newfilename);			//new and old filename does not match. Generate new filename
		return 1;
	}
	else
	{
		return 0;						//otherwise, day ain't over yet, use the old filename
	}
}	

void get_id(char *id_message)
{
	//char PN[20];		//AHF these are unused
	//char SN[20];        
	unsigned char CVTR_REV; //,FW_REV;

	//get the PSC and CVTR FW Revs 
	CVTR_REV = cvtrbuffer[65];
	if(brd_config)
		sprintf(id_message," PSC_fw_rev:%d CVTR_fw_rev:%d",FWRevision,CVTR_REV);
	else
		sprintf(id_message," PSC_fw_rev:%d CVTR_fw_rev: No board installed",FWRevision);
}

void set_id(char *PN, int pnstrlen, char *SN, int snstrlen, char *HW_REV, char *cmdstat)		//this function should be used only during FAT with EEPROM WP jumper on
{
	//set the PSC PN, SN, HW Rev, and FW Rev to EEPROM 
	
	//for PN:
//	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,PN_REGADDR,(unsigned char *)PN,8);
//	__delay_ms(5);
//	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,PN_REGADDR+8,(unsigned char *)PN+8,8);
//	__delay_ms(5);
	
	//for SN:
//	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,SN_REGADDR,(unsigned char *)SN,8);
//	__delay_ms(5);
//	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,SN_REGADDR+8,(unsigned char *)SN+8,4);
//	__delay_ms(5);
	
	//for Hardware Rev:
//	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,HWREV_REGADDR,(unsigned char *)HW_REV,strlen(HW_REV));
//	__delay_ms(5);	

//	strcpy(cmdstat," ok");
}

//void wd_speed(char *speed,char *cmdstat)
//{
//	//store wind speed and return ok or error accordingly
//	//what we are going to do with the wind speed data??
//	if((atoi(speed)<0)|(atoi(speed)>1000))
//	{
//		strcpy(cmdstat,"error");
//	}
//	else
//	{
//		strcpy(cmdstat,"ok");
//	}
//}

//*****************************************************************************
//		Relay switching Routine
/******************************************************************************
	Summary:
	This routine turn on or off the relay of designated power source input per 
	mask. 

	Precondition:
	None
	
	Parameter:
	unsigned int mask		- mask to determine which relay we are examing
	int state				- the state (on or off) the relay should be switched to

	Return Value:
	int						- return 1 if successful
*******************************************************************************/
int rlydrive(unsigned int mask, int state)
{
	int rlycntl, ledcntl;
	char relaystate[2]="0";
	int offmask = 0xC33C,onmask = 0x3CC3; //AHF 2/28/20 hardcoding this to prevent hard reset issues


	switch(state)
	{
		case 0:				//FOR TURN OFF THE RELAYS OR RESET
			rlycntl = mask & offmask;
			rlystat = (~(mask) & rlystat) & 0x00FF; //update relay states
			break;
		case 1:				//FOR TURN ON THE RELAYS OR SET
			rlycntl = mask & onmask;
			rlystat = (mask | rlystat) & 0x00FF;  	//update relay states
			break;
		default:
			rlycntl = 0x0;
			break;
	}
	ledcntl = ((~rlystat)<<8)|(rlystat&0x00FF);  

	if(!SPI1tx(rlycntl, RLY)) return 0;		//send rlycntl out to rly drivers
	if(!SPI1tx(ledcntl, LED)) return 0;		//update LED indicators	
	sprintf(relaystate,"%c",rlystat);
	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,RLYST_REGADDR,(unsigned char *)relaystate,1);		//write back the relay states to EEPROM, improve: error trap
	__delay_ms(40);		//it take about 35ms to switch on the relay
	if(!SPI1tx(0x0,RLY)) return 0;			//stop driving the coils 
    
	if(!ledflag)
	{
	    if(!leddrive(ledflag)) return 0;			//AHF turn LEDs off
	}
    
	return 1;
}

int get_override_state()
{
	char OFlag[3];

	//get the override state from EEPROM 
	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,OVERRIDE_REGADDR,2,(unsigned char *)OFlag);	
	override_flag = ((OFlag[1]&0xff)<<8) | (OFlag[0]&0xff);
	return 0;
}

int set_override_state()
{
	char OFlag[3];

	//save the override state flag to EEPROM
	OFlag[1] = (override_flag&0xff00)>>8;
	OFlag[0] = (override_flag&0xff);
	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,OVERRIDE_REGADDR,(unsigned char *)OFlag,2);				//write back the relay states to EEPROM, 
	return 0;												//should return i2c_putstat return!!
}

int set_battchrg_level1(int regaddr, long intlevel, double fralevel)
{
	cntstore(regaddr, intlevel, fralevel);
	return 0;
}

long get_battchrg_level(int regaddr, double *fralevel)		//get stored battery string coulomb count from EEPROM
{
	long intlevel; 
	double sign = 1;  
	char coulombcnts[5]={};
		
	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,regaddr,5, (unsigned char *)coulombcnts);
	if(((coulombcnts[0]>>8)&0x1) == 1)
	{
		coulombcnts[0] &= 0x7F;
		sign = -1;
	}
	intlevel = (bcd2dec(coulombcnts[0])*100000);
	intlevel += (bcd2dec(coulombcnts[1])*1000);
	intlevel +=  bcd2dec(coulombcnts[2])*10;
	intlevel +=  bcd2dec(coulombcnts[3])/10;
	*fralevel = (bcd2dec(coulombcnts[3])%10)*100;
	*fralevel += bcd2dec(coulombcnts[4]);
	*fralevel = *fralevel/1000;
	intlevel *= sign;
	*fralevel *= sign;
	return intlevel;
}

void set_valleybtm(int regaddr, double valleybottom)
{
	double intvalue;
	double fravalue;

	fravalue = modf(valleybottom,&intvalue);	
	cntstore(regaddr,intvalue,fravalue);
}

double get_valleybtm(int regaddr)
{
	double valleybottom;
	long cnt_int;
	double cnt_fra;
	
	cnt_int = get_battchrg_level(regaddr,&cnt_fra);
	valleybottom = cnt_int + cnt_fra;
	return valleybottom;
}

int batterycharging(double bt_t, double bt_v, double bt1_i, double bt2_i)
{
	double soc1, soc2;
	//int cntl;	//,wt=0;
	int index;
	double StringFullCharge,TotalCoulombCnt,OverShoot,delta_cc;
	//int temp;

	StringFullCharge = FULLCHARGE/2;
	//calculate battery bank state of charge in %:
	UpdateCoulombCount(bt1_i,&intcc1,&fracc1);						//convert battery string current into coulomb count
	soc1 = 100*(StringFullCharge + (intcc1+fracc1))/StringFullCharge;

	UpdateCoulombCount(bt2_i,&intcc2,&fracc2);
	soc2 = 100*(StringFullCharge + (intcc2+fracc2))/StringFullCharge;

	delta_cc = intcc1+fracc1 + intcc2+fracc2;
	TotalCoulombCnt =FULLCHARGE + delta_cc;			
	batt_chg_status = (soc1 + soc2)/2;			

//	if(batt_chg_status > 100.0)								// Changed to display true SOC above 100% July 2014 per H2 mitigation protocol
//	{
//		batt_chg_status = 100.0;
//	}
	if(batt_chg_status< 0.0)
	{
		batt_chg_status = 0.0;
	}

    ValleyBottom = MIN(ValleyBottom,TotalCoulombCnt-FULLCHARGE);
	OverShoot = overshootlookup(ValleyBottom);
/*
	// Count down to Fuel Cell powerdown
	if(FCShtdwnDlyCnt)		// If delay counter is zero skip this
	{
		if(!FC_ResponseFlag && !(FCShtdwnDlyCnt%10))		// If no response yet and it's on the 10th second
		{
			strcpy(UART2_TXDATA ,"set fccmgr.state 0\r\n");	// Resend stop command to FC
			IEC1bits.U2TXIE = 1;
			error_flag_4 |= FC1COMFLT;	//turn on the FC1 communication error flag
		}
		FCShtdwnDlyCnt--;	// or decrement the counter
		if(FCShtdwnDlyCnt == 0)	// and if it's down to zero
		{
			rlydrive(FC2MASK, 0); //  Turn off fuel cell no matter the state of override (contact closure)
			rlydrive(FC1MASK, 0); //  Turn off fuel cell no matter the state of override	(Power down)
			FC_ResponseFlag = 0;		// Turn off response flag
			error_flag_4 &= ~FC1COMFLT;	//turn off the FC1 communication error flag
		}
	}	
*/
	//compare if the battery voltage exceed conditioning or absorption voltage:
    // ppw 08112026 - TBD Have Dan or Alex check these
	index = temp2index(bt_t);
	if(bt_v > Absorption_Volt[index])
	{
		if(OverAsbChgCnt > 20)										//observe Over Voltage for 1 minute before acting after 60 consective overvoltage - changed to 20 July 2014 per H2 mitigation protocol
		{
			OverAsbChgVolt = TRUE;
		}
		else 
		{
			OverAsbChgCnt++;
		}
	}
	else
	{
		OverAsbChgCnt = 0;
		OverAsbChgVolt = FALSE;
	}

	if(bt_v > Conditioning_Volt[index])
	{
		if(OverCondChgCnt > 60)
		{
			OverCondChgVolt = TRUE;
		}
		else 
		{
			OverCondChgCnt++;
		}
	}
	else
	{
		OverCondChgCnt = 0;
		OverCondChgVolt = FALSE;
	}

	//once a minute check the following conditions:
	if(secondcnt==60)				//If one minute is up: save Coulomb count to EEPROM
	{
	/*
	sprintf(UART2_TXDATA ,"#DBG 60 delta_cc %f tcc %f vb %f os %f OC %u  \r\n", delta_cc   , ValleyBottom , TotalCoulombCnt ,OverShoot ,OverAsbChgVolt   );   IEC1bits.U2TXIE = 1;	// Send stop command to FC
					
		if((TotalCoulombCnt < (FULLCHARGE * chrg_thrshld/100.0)) && (((override_flag & 0x400)>>10)!=1))	   //When total coulomb cnt is less than Charge threshold and not in ext pwr lockout
		{
			cntl = 1;											//connect the external power sources to charge the battery
			switch((override_flag & 0x3))						//this switch block for wind turbine shunting circuit control
			{
				case 0:		//none of the WT relays are overriden
					WT1_ENB = !cntl;
					WT2_ENB = !cntl;
					break;
				case 1:		//WT1 is overriden. 
					WT2_ENB = !cntl;
					break;
				case 2:		//WT2 is overriden
					WT1_ENB = !cntl;
					break;
				default:	//both are overriden or other conditions, do nothing
					break;
			}
			__delay_ms(4); 
			temp = (override_flag <<8)|(override_flag);
			rlydrive((STDMASK & ~temp), cntl);
			__delay_ms(100);
			rlydrive((FC1MASK & ~temp), 1); 			// Turn on the fuel cell (power up)
			rlydrive((FC2MASK & ~temp), 1); 			// Turn on the fuel cell (contact closure) 
		}
	
		if((delta_cc > abs((ValleyBottom * (OverShoot-1)))) || (OverAsbChgVolt == TRUE))	//   changed AND to OR for SOC and Vmain test July 2014 per H2 mitigation protocol
		{
			cntl = 0;							// Disconnect PV and WT input power sources regardless of override state. Changed July 2014 per H2 mitigation protocol
			WT1_ENB = !cntl;
			WT2_ENB = !cntl;
			__delay_ms(4);
			rlydrive((STDMASK ), cntl); 
			
			strcpy(UART2_TXDATA ,"set fccmgr.state 0\r\n");	// Send stop command to FC
			IEC1bits.U2TXIE = 1;
			
			FCShtdwnDlyCnt = 119;	//Set FC time delay to 2 min
			

			resetcoulombcnts();
		}

		if(TotalCoulombCnt > (FULLCHARGE * 1.20) )	// SOC > 120% - absolute cut off condition!! Changed July 2014 per H2 mitigation protocol
		{
			cntl = 0;							//disconnect PV and WT input power sources
			WT1_ENB = !cntl;
			WT2_ENB = !cntl;
			__delay_ms(4);
			rlydrive((STDMASK ), cntl); 

			strcpy(UART2_TXDATA ,"set fccmgr.state 0\r\n");	// Send stop command to FC
			IEC1bits.U2TXIE = 1;
		
			FCShtdwnDlyCnt = 119;	//Set FC time delay to 2 min
			resetcoulombcnts();
		}

	//calculate CDS here <-- not implemented here!
*/
		//set the delta coulomb cnt in EEPROM
		set_battchrg_level1(BATTLVL1_REGADDR,intcc1,fracc1);
		set_battchrg_level1(BATTLVL2_REGADDR,intcc2,fracc2);
		set_valleybtm(VALLEYBTM_REGADDR,ValleyBottom);

		minutecnt++;			
		secondcnt = 0;				//clear 1 minute count
	}	
		
	if(minutecnt==60)
	{
		update_elapsed_time();		//elapsed timer ticks on the hour
		minutecnt = 0;				//clear 1 hour count
	}
	return 0;
}

//*****************************************************************************
//		Check PV Controller Configuration
/******************************************************************************
	Summary:
	This routine reads the PV controller configuration options from EEPROM 
	PVCONFIG location to determine if the PV Controllers are internal on PSC board
	or external eval boards. 

	Precondition:
	None
	
	Parameter:
	unsigned char pv_config	- byte from PVCONFIG in EEPROM
	int mask				- mask for checking which PV (there are four of them)

	Return Value:
	1 means the masked PV Controller is external
	0 means the masked PV controller is internal
*******************************************************************************/
/*int PVConfig(int mask)
{
	char pv_config[4]={};

	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,PVCONFIG_REGADDR,1,(unsigned char *)pv_config);
	if(((int) pv_config[0] & mask)!= 0)return 0;
	return 1;
}
*/
//*****************************************************************************
//		Check All PV Controller Configuration
/******************************************************************************
	Summary:
	This routine reads the PV controller configuration options from EEPROM 
	PVCONFIG location to determine if the PV Controllers are internal on PSC board
	or external eval boards. 

	Precondition:
	None
	
	Parameter:
	unsigned char pv_config	- byte from PVCONFIG in EEPROM
	int newmask				- mask from EEPROM for all four PV cntl config,1=ext, 0=int

	Return Value:
	newmask
*******************************************************************************/
int allPVConfig()
{
	char pv_config[4];
	int newmask = 0;

	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,PVCONFIG_REGADDR,1,(unsigned char *)pv_config);
	newmask = ((int)pv_config[0]&0x3C);	
	return newmask;
}
//*****************************************************************************
//		get PV Controller status data from PV Controller Status registers
/******************************************************************************
	Summary:
	This routine reads the PV controller status registers over the PV Controller's
	SPI bus and report back the status data

	Precondition:
	None
	
	Parameter:
	char *cmd				- the command for PV1~4
	char *status_data		- mask for checking which PV (there are four of them)

	Return Value:
	status_data
*******************************************************************************/
void get_pvcntlstat(char *cmd, char *status_data, char *pvcntlstatus)
{
	int data;
	int SS;
	
	SS=hex2dec(cmd[2]);	
	data = read_pvcntlreg(5,SS);		//input voltage level from Boost Chip register
	sprintf(pvcntlstatus,"%.2f",(double)data);
//	sprintf(status_data,"%s    Vin=%d\r\n,   ",status_data,data);
//	sprintf(pvcntlstatus,"%s Vin=%f (V)\r\n",status_data,data*0.0012*22.82);
//	sprintf(pvcntlstatus,"%s Vin=%f (V)\r\n",status_data,data*(pvcntl_ref/1023)*(pvcntl_R1+pvcntl_R2)/pvcntl_R2);
	data = read_pvcntlreg(4,SS);		//input current level from boost chip register
	sprintf(pvcntlstatus,"%s %.2f",pvcntlstatus,(double)data);
//	sprintf(pvcntlstatus,"%d",data);	
//	sprintf(status_data,"\r\n    Iin=%d\r\n",data);	
	data = read_pvcntlreg(6,SS);		//boost chip internal pwm 
	sprintf(pvcntlstatus,"%s %d",pvcntlstatus,data);
//	sprintf(status_data,"%s    PWM=%d\r\n",status_data,data);
	data = read_pvcntlreg(7,SS);		//boost chip status
	sprintf(pvcntlstatus,"%s %d",pvcntlstatus,data);
//	sprintf(status_data,"%s status=%d\r\n",status_data,data);
}

//*****************************************************************************
//		PV Controller turn-on/shut-off command 
/******************************************************************************
	Summary:
	This routine sends the PV controller output control command over the PV 
	Controller's SPI bus. 

	Precondition:
	None
	
	Parameter:
	char *command			- command string entered by CPM
	char *type				- PV Controller Command type: on or off

	Return Value:		
	return 0				- no error
*******************************************************************************/
/*int cmd_pvcntl(char *command, char *type)
{
	char cmd1,cmd2,cmd3,*temp;
	int SS,pvcntlcmd;

	sscanf(command,"%c%c%c%s",&cmd1,&cmd2,&cmd3,temp);
	SS=hex2dec(cmd3);
	if(strcmp(type,"on") == 0)pvcntlcmd = 0x03;
	if(strcmp(type,"off")== 0)pvcntlcmd = 0x02;
	write_pvcntlreg(pvcntlcmd,SS);

	return 0;
}
*/
//*****************************************************************************
//		write data to EEPROM 
/******************************************************************************
	Summary:
	This routine writes to the EEPROM to the loaction spectifed. The command format
	is as in this example:
		eeprom_write 5 12@07@FF@ac@2d@8A <checksum>\r\n
	where: 	eeprom_write 		<-- command string
			5 					<-- decimal number for location 
			12@07@FF@ac@2d@8A 	<-- data to be stored from location 5	

	Precondition:
	None
	
	Parameter:
	char *location			- the start address of the data to be stored in EEPROM
	char *data				- data in hex byte with @ deliminators

	Return Value:
	none
*******************************************************************************/
void eepromWrite(char *location, char *data)
{
	int regaddr,datalength=0;
	char *temp;
        unsigned char gooddata[3] = {};
        unsigned char lastdata[3] = {};
	char delim[2] =",";
//	char delim[2] ="@";

	regaddr = atoi(location);
	temp = strtok(data,delim);
	sprintf((char *)gooddata,"%c",word2dec(temp));
	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,regaddr,gooddata,1);
	__delay_ms(5);
	while(temp != NULL)
	{
		temp = strtok(NULL,delim);
		sprintf((char *)gooddata,"%c",word2dec(temp));
		datalength++;
		i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,regaddr+datalength,1,lastdata);
		i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,regaddr+datalength,gooddata,1);
		__delay_ms(5);
	}
	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,regaddr+datalength,lastdata,1);
}

//*****************************************************************************
//		read data from EEPROM 
/******************************************************************************
	Summary:
	This routine reads data from the EEPROM from the loaction spectifed. 
	The command format is as in this example:
		eeprom_read 5 9 <checksum>\r\n
	where: 	eeprom_read 		<-- command string
			5 					<-- decimal number for starting location 
			9				 	<-- number of bytes to be read (decimal)	

	Precondition:
	None
	
	Parameter:
	char *location			- the start address of the data to be stored in EEPROM
	char *length			- number of bytes to be read from EEPROM (decimal)

	Return Value:
	char *data1				- resulting data (bytes in hex) 
*******************************************************************************/
void eepromRead(char *location, char *length, char *data1)
{
	int i;
	char temp[256]={};
	int readlength = 0;

	if((atoi(length)+atoi(location)) > MAXEEPROMBYTES)
	{
		readlength = MAXEEPROMBYTES - atoi(location);
	}
	else
	{
		readlength = atoi(length);
	}
	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,atoi(location),readlength,((unsigned char *)data1));
	//need to convert string to hex with space in between	
	for(i=0;i<atoi(length);i++)
	{
		sprintf(temp,"%s %02X",temp,data1[i]&0xFF);
	}
	strcpy(data1,temp);			
}

void cvt_ctrl(int cmdnum, char *cmdstat)
{
	unsigned char i2cdata[2]={};

	switch(cmdnum)
	{
		case 1:		//enable cvt input relays
			switchcvtrrelay(1);				//turn on the CVTR Input Relay with read-mod-write
			break;		
		case 2:		//disable cvt input relays
			switchcvtrrelay(0);				//turn off the CVTR Input Relay with read-mod-write
			break;
		case 3:		//enable cvt output
			CONVOUTCNTL = 0x1;		
			break;
		case 4:		//disable cvt output
			CONVOUTCNTL = 0x0;
			break;
		case 5:		//reset cvt 
			CONVOUTCNTL = 0x0;					//reset means turn off output too!
			CONVRST = 0x1;						//Drive CVTR Reset Line
			__delay_ms(100);						//the input relays turned off by uC of CVTR
			CONVRST = 0x0;						//Clear CVTR Reset Line 
			break;
		case 6:		//"swgf_on" command issued - start the Sea Water Ground Fault Test
			if(i2c_getstat1(CVTR_CH,CVTR_SADDR,CVTR_CTRL_REG,1,i2cdata))
			{
				error_flag_3 |= CVTRGENFLT;
				strcpy(cmdstat,"error");
			}
			else
			{
				error_flag_3 &= ~CVTRGENFLT;
				i2cdata[0] |= 0x46;				//enable the swgf test bits  ver. 9 added P status bit
				i2c_putstat1(CVTR_CH,CVTR_SADDR,CVTR_CTRL_REG,i2cdata,1);
				CVTR_SWGF_TEST = 1;
				strcpy(cmdstat,"ok");
			}
			return;
		case 7:		//"swgf_off" command issued - stop the Sea Water Ground Fault Test - ver. 9 added 
			if(i2c_getstat1(CVTR_CH,CVTR_SADDR,CVTR_CTRL_REG,1,i2cdata))
			{
				error_flag_3 |= CVTRGENFLT;
				strcpy(cmdstat,"error");
			}
			else
			{
				error_flag_3 &= ~CVTRGENFLT;
				i2cdata[0] &= 0x39;				//disable the swgf status bits  
				i2cdata[0] |= 0x06;				//enable the swgf test bits so we drop into the CVTR shutoff routine
				i2c_putstat1(CVTR_CH,CVTR_SADDR,CVTR_CTRL_REG,i2cdata,1);
				CVTR_SWGF_TEST = 0;
				strcpy(cmdstat,"ok");
			}
			return;
		default:
			break;
	}
	override_flag = override_flag | 0x0100;		//give the CVTR output disable control back to CPM
	set_override_state();
	strcpy(cmdstat," ok");
}

void setgetcvl(int IO, char *enable_level, char *disable_level, char *status)
{
	char *temp;
	char delim[2] =".";

	unsigned char i2cdata[5]={};

	switch(IO)
	{
		case 0:		//get cvl
			i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,CVTRCVL_REGADDR,4,i2cdata);
			cvtr_cvl_enab = hexbyte2dec(i2cdata[1]) + hexbyte2dec(i2cdata[0])*0.01;
			cvtr_cvl_disab = hexbyte2dec(i2cdata[3]) + hexbyte2dec(i2cdata[2])*0.01;
			sprintf(enable_level," %.2f %.2f",cvtr_cvl_enab,cvtr_cvl_disab);
			break;
		case 1:		//set cvl
			temp = strtok(enable_level,delim);
			i2cdata[1] = cvltoi2cdata(temp);
			temp = strtok(NULL,delim);
			i2cdata[0]=	hex2dec(temp[0])*16 +hex2dec(temp[1]);
			temp = strtok(disable_level,delim);
			i2cdata[3] = cvltoi2cdata(temp);
			temp = strtok(NULL,delim);
			i2cdata[2]=	hex2dec(temp[0])*16 +hex2dec(temp[1]);
			cvtr_cvl_enab = hexbyte2dec(i2cdata[1]) + hexbyte2dec(i2cdata[0])*0.01;
			cvtr_cvl_disab = hexbyte2dec(i2cdata[3]) + hexbyte2dec(i2cdata[2])*0.01;
			i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,CVTRCVL_REGADDR,i2cdata,4);
			break;
		default:
			break;
	}
}
void setgetchrg(int IO, char *chrg_level, char *status)  // Set or read the charge cycle threshold
{
	char *temp;
	char delim[2] =".";

	unsigned char i2cdata[5]={};

	switch(IO)
	{
		case 0:		//get cvl
			i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,CHRGCTRL_REGADDR,2,i2cdata);			// Get threshold from EEPROM
			chrg_thrshld = hexbyte2dec(i2cdata[1]) + hexbyte2dec(i2cdata[0])*0.1;		// Generate threshold value (**Global variable s/b a return arg)
			sprintf(chrg_level," %.1f",chrg_thrshld);
			break;
		case 1:		//set cvl
			temp = strtok(chrg_level,delim);		// Get 2 chars before decimal point
			i2cdata[1]=	hex2dec(temp[0])*16 +hex2dec(temp[1]);  // Convert first 2 chars into BCD value
			temp = strtok(NULL,delim);			// Get char after decimal
			i2cdata[0]=	hex2dec(temp[0]);		// Put char into 2nd half of BCD
			chrg_thrshld = hexbyte2dec(i2cdata[1]) + hexbyte2dec(i2cdata[0])*0.1;		// Generate threshold value
			
			i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,CHRGCTRL_REGADDR,i2cdata,2);		// Save threshold in EEPROM
			break;
		default:
			break;
	}
}
void setgetcvtregister(int type, char *location, char *length, char *data)
{
	int regaddr,i,readlength = 0;
	char temp[256]={};

	switch(type)
	{
		case 0:			//get
			memset(data,NULL,256);		//clear the data for fill with response later
			if((atoi(length)+atoi(location)) > 75)			// Changed from 70 to 75 for Rev 8
			{
				readlength = 75 - atoi(location);
			}
			else
			{
				regaddr = atoi(location);
				readlength = atoi(length);
			}
			if(i2c_getstat1(CVTR_CH,CVTR_SADDR,regaddr,readlength,((unsigned char *)temp)))
			{
				error_flag_3 |= CVTRGENFLT;
				strcpy(data," error, Cannot access CVTR");
			}
			else
			{
				error_flag_3 &= ~CVTRGENFLT;
				//convert string to hex with space in between	
				for(i=0;i<readlength;i++)
				{
					sprintf(data,"%s %02X",data,temp[i]&0xFF);
				}
//				strcpy(data,temp);	
			}		
			break;
		case 1:			//set
			regaddr = atoi(location);
			if(regaddr == CVTR_CTRL_REG)		
			{
				sprintf(temp,"%c",word2dec(data));
				i2c_putstat1(CVTR_CH,CVTR_SADDR,regaddr,(unsigned char *)temp,1);
//				if(((regaddr >>1) & 0x3)>0x1) CVTRINTLCKSTAT = 1;
				strcpy(length," ok");
				break;
			}
			else if(regaddr == CVTR_SWGF_DLY_REG)
			{
				sprintf(temp,"%c",word2dec(data));
				i2c_putstat1(CVTR_CH,CVTR_SADDR,regaddr,(unsigned char *)temp,1);	//Added for Rev 9
				strcpy(length," ok");
			}
			else if(regaddr == CVTR_SWGF_DLY_REG + 1)
			{
				sprintf(temp,"%c",word2dec(data));
				i2c_putstat1(CVTR_CH,CVTR_SADDR,regaddr,(unsigned char *)temp,1);	//Added for Rev 9
				strcpy(length," ok");
			}
			else
			{ 
				strcpy(length," error, read only register specified");	//Only reg .61, .66, .67 can be written
				break;
			}
		default:
			break;
	}
}

//*****************************************************************************
//		Routine to switch the HV Converter low voltage input power relays
/******************************************************************************
	Summary:
	These routines are used to send the command to HV Converter board via I2C bus
	to turn on/off the low voltage input power relays on the HV Converter board. 
	The routine performs a read-modify-write to flip the bit 3 of the HV Converter's 
	register 61 per cnrl state. cntl = 1 --> turn on the relays; cntl = 0 --> turn
 	off the relays.

	Precondition:
	none,

	Parameter:
	int cntrl				- The state to turn relay on or off
	 
	Return Value:
	None
*******************************************************************************/

void switchcvtrrelay(int cntl)
{
	unsigned char i2cdata[3]={};

	i2c_getstat1(CVTR_CH,CVTR_SADDR,CVTR_CTRL_REG,1,i2cdata);
	if(cntl == 1)
	{
		i2cdata[0] |= 0x08;		//set bit 3 of cvtr register 61
	}
	else
	{
		i2cdata[0] &= 0xF7;		//clear bit 3 of cvtr register 61
	}
	i2c_putstat1(CVTR_CH,CVTR_SADDR,CVTR_CTRL_REG,i2cdata,1);
}
int leddrive(int state)
{
	int ledcntl;
	ledcntl = ((~rlystat)<<8)|(rlystat&0x00FF);  
	switch(state)
	{
		case 0:
		    if(!SPI1tx(0x0,LED)) return 0;			//turn LEDs off
		    ledflag = 0;
		    return 1;
		case 1:
		    if(!SPI1tx(ledcntl, LED)) return 0;		//turn LEDs on
		    ledflag = 1;
		    return 1;
		default:
		    return 0;
	}
}
