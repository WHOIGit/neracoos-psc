/**********************************************************************
*    WHOI/Raytheon, 2010
*
* FileName:        	response.c
* Dependencies:     Header (.h) files if applicable, see code below
* Processor:       	PIC24FJ256GB106
* Compiler:        	MPLAB C30 v3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev 	Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1		Joseph Zhu    	10/28/2010		Initial File Creation 
* 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This routine contains the response to received UART1 RX data
**********************************************************************/
#include "response.h"
#define STOP 0
#define GO 1
#define OFF 0
#define ON 1

int response()
{
	char cmd[16]={}, arg1[256]={}, arg2[256]={}, arg3[20]={}, arg4[6]={}, arg5[4]={};
	char status[256], detail[256], incmd[256]={}, outcmd[256]={}, /*others[30]={},*/ temp[4]={};
	int numarg=0, cmdnum=0, srcnum=0, errcnt=0;
	char ERROR1[]=" error Incorrect Number of Arguments";
	char ERROR2[]=" error Command Checksum Error";
	char ERROR3[]=" error Invalid Source ID";
	char ERROR4[]=" error Invalid Device Control Command";
	//int SET = 1;
	//int GET = 0;
	extern char Rxdata1[100]; 
	char cmdstat[6];
	char id_message[100]/*,tm_message[100]*/;
	char data1[256]={};
	double bt_v,bt_t;
	
	
	numarg = sscanf(Rxdata1,"%s %s %s %s %s %s",cmd,arg1,arg2,arg3,arg4,arg5);	
	memset(Rxdata1,NULL,100);	
	
	if((strcmp(cmd,"connect")==0) | (strcmp(cmd,"disconnect")==0))
	{
		if(numarg != 3)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s",cmd,arg1);		
		if(!chksumchk(arg2,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		srcnum = src_id_chk(arg1);
		if(!srcnum)
		{
			reply(cmd,ERROR3,"");
			return 1;
		}
		connect(cmd,srcnum,cmdstat);
		reply(cmd,arg1,cmdstat);
		return 0;			
	}
	if(strcmp(cmd,"stop")==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		inhibitdatastream = ON;			//turn on data stream inhibitor
		reply(cmd," ok","");				
		return 0;		
	}
	if(strcmp(cmd,"go")==0)			
	{
		if(numarg != 3)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s",cmd,arg1);		
		if(!chksumchk(arg2,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		if((atoi(arg1)<1)|(atoi(arg1)>3600))  //datastream rate range: 1 to 3600
		{
			reply(cmd,arg1," error Invalid Rate");
			return 1;
		}
		inhibitdatastream = OFF;
		datastreamcnt = atoi(arg1);
		datastreamcnt_loc = datastreamcnt;	//perserve data stream count. The datasteamcnt varys in program
		reply(cmd,arg1," ok");	
		return 0;		
	}
    /* ppw 10082026 - per Dan Bishop, no support on NERACOOS PSC
	if(strcmp(cmd,"override_off")==0)	
	{
		if(numarg != 3)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s",cmd,arg1);		
		if(!chksumchk(arg2,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		srcnum = src_id_chk(arg1);
		if(!srcnum)
		{
			reply(cmd,ERROR3,"");
			return 1;
		}
		update_override_flag(srcnum);
		set_override_state();		//update EERPOM Override State flag
		reply(cmd,arg1," ok");
		return 0;			
	}
    */
	if(strcmp(cmd,"psc_reset")==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		update_override_flag(10);	//soft reset to default override
		set_override_state();
		inhibitdatastream = OFF;		//reset to default
		//...							//anything else needs to be reset? relay?
		reply(cmd," ok","");			//<--put in comm.c
		return 0;		
	}
    /* ppw 10082026 - per Dan Bishop, no support on NERACOOS PSC
	if(strcmp(cmd,"set_time")==0)
	{
		if(numarg != 4)
		{			
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s %s",cmd,arg1,arg2);		
		if(!chksumchk(arg3,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		sprintf(arg1,"%s %s",arg1,arg2);
		setgettime(cmd,SET,arg1,cmdstat);
		reply(cmd,cmdstat,"");				
		return 0;		
	}
	if(strcmp(cmd,"get_time")==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");		
			return 1;
		}
		setgettime(cmd, GET, tm_message ,cmdstat);	
		reply(cmd,tm_message,"");	
		return 0;		
	}
    */
	if(strcmp(cmd,"get_id")==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		get_id(id_message);				
		reply(cmd,id_message,"");				
		return 0;		
	}
    /* ppw 10082026 per Dan Bishop, no support in neracoos PSC 
	if(strcmp(cmd,"set_id")==0)
	{
		if(numarg != 5)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s %s %s",cmd,arg1,arg2,arg3);		
		if(!chksumchk(arg4,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		if(strlen(arg1)!=16)
		{
			reply(cmd," error The Part Number has to consist of 16 letters/digits","");
			return 1;
		}
		if(strlen(arg2)!=12)
		{
			reply(cmd," error The Serial Number has to consist of 12 letters/digits","");
			return 1;
		}
		if(strlen(arg3)>2) 
		{
			reply(cmd, " error The HW Rev consists of no more than 2 digits or two letters",""); 
			return 1;
		}
		set_id(arg1,strlen(arg1),arg2,strlen(arg2),arg3,cmdstat);				
		reply(cmd,cmdstat,"");				
		return 0;		
	}
    */
	if(strcmp(cmd,"pv_status")==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		chk_pv(status,detail);
		reply(cmd,detail,"");		
//		reply(cmd," ok","");		
		return 0;		
	}
	if(strcmp(cmd,"bt_status")==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		chk_bt(status,detail,&bt_v, &bt_t);
		reply(cmd,detail,"");		
//		reply(cmd," ok","");		
		return 0;		
	}
	if(strcmp(cmd,"hyd_status")==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		chk_hyd(status);
		reply(cmd,status,"");		
//		reply(cmd," ok","");		
		return 0;		
	}
    /* ppw 10082026 per Dan Bishop, no support in NERACOOS PSC
	if(strcmp(cmd,"wnd_sp")==0)
	{
		if(numarg != 3)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s",cmd,arg1);		
		if(!chksumchk(arg2,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		wd_speed(arg1,cmdstat);			
		reply(cmd,cmdstat,"");			
		return 0;		
	}
    */
	if(strcmp(cmd,"psc")==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		datastream(2);					//<--streamdata once no matter what
		reply(cmd," ok","");		
		return 0;		
	}
	if(strcmp(cmd,"help")==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		helplist();						
		return 0;		
	}
	if(strcmp(cmd,"?")==0)
	{
		helplist();
		return 0;
	}
    /* ppw 10082026 per Dan Bishop, no support for NERACOOS PSC
	if(strcmp(cmd,"cvt_status") == 0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		chk_conv(status,detail,others);
		reply(cmd,detail,"");
		return 0;
	}
    */
	if(strcmp(cmd,"eeprom_write") == 0)
	{
		if(numarg != 4)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
//		sprintf(incmd,"%s %s %s",cmd,arg1,arg2);	//no chksum for this command	
//		if(!chksumchk(arg3,incmd))
//		{
//			reply(cmd,ERROR2,"");
//			return 1;
//		}
		eepromWrite(arg1,arg2);
		if(strcmp(arg1,"24") == 0)		//note: when clear coulombcnts in EEPROM, clear 32 bytes starting at location 24 (decimal)
		{
			resetcoulombcnts();
		}
		reply(cmd," ok","");
		return 0;
	}
	if(strcmp(cmd,"eeprom_read") == 0)
	{
		if(numarg != 4)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if((atoi(arg2)+atoi(arg1)) > MAXEEPROMBYTES)
		{
			reply(cmd," Warning: There are only 256 locations! Only the contents from the valid locations are shown.","");
		}
//		sprintf(incmd,"%s %s %s",cmd,arg1,arg2);		
//		if(!chksumchk(arg3,incmd))
//		{
//			reply(cmd,ERROR2,"");
//			return 1;
//		}
		eepromRead(arg1,arg2,data1);
		reply(cmd,data1,"");
		return 0;
	}
	/*if((strcmp(cmd,"pv1cntl_status")==0)		//not used anymore
		| (strcmp(cmd,"pv2cntl_status")==0)
		| (strcmp(cmd,"pv3cntl_status")==0)
		| (strcmp(cmd,"pv4cntl_status")==0))
	{
		if(numarg != 2) 
		{
			reply(cmd,ERROR1,"");
			return 1;
		}		
		if(!chksumchk(arg1,cmd)) 
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		get_pvcntlstat(cmd,data1,detail);
		reply(cmd,data1,"");
		return 0;
	}
	if((strcmp(cmd,"pv1cntl_cmd")==0)			//not used anymore
		|(strcmp(cmd,"pv2cntl_cmd") == 0)
		|(strcmp(cmd,"pv3cntl_cmd") == 0)
		|(strcmp(cmd,"pv4cntl_cmd") == 0))
	{
		if(numarg != 3) 
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s",cmd,arg1);		
		if(!chksumchk(arg2,incmd)) 
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		if(!cmd_pvcntl(cmd,arg1))
			reply(cmd," ok","");
		else 
			reply(cmd," error","");
		return 0;
	}*/
	if((strcmp(cmd, "get_ccnt")) == 0)		//for debug only
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");		
			return 1;
		}
		EEPROMInit();	
		reply(cmd," ok","");	
		return 0;		
	}
	if((strcmp(cmd, "coulomb_rst")) == 0)	//for debug only
	{
				if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");		
			return 1;
		}
		resetcoulombcnts();
		reply(cmd," ok","");	
		return 0;		
	}
	if((strcmp(cmd,"clr_rst"))==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		ClrResetFlag();
		reply(cmd," ok","");
		return 0;	
	}
	if((strcmp(cmd, "cvt")) == 0)
	{
		if(numarg !=3)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s",cmd,arg1);		
		if(!chksumchk(arg2,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		cmdnum = valid_src_cmd(arg1);
		if(!cmdnum)
		{
			reply(outcmd,ERROR4,"");
			return 1;
		}
		cvt_ctrl(cmdnum,cmdstat);
		reply(cmd,arg1,cmdstat);
		return 0;
	}
	if((strcmp(cmd,"set_cvl"))==0)
	{
		if(numarg !=4)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s %s",cmd,arg1,arg2);		
		if(!chksumchk(arg3,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		if((atof(arg1))-(atof(arg2)) < DCVLMAX)
		{
			sprintf(status," error CVL Enable Limit has to be %0.2f volts over CVL Disable Limit",DCVLMAX);
			reply(cmd,status,"");
			return 1;
		}
		if((atof(arg1)>100.00) | (atof(arg1)<18.49))
		{
			sprintf(status," error CVL Enable Limit can not be higher then 100.00 volts or lower than 18.5 volts ");
			reply(cmd,status,"");
			return 1;
		}
		setgetcvl(1,arg1,arg2,cmdstat);
		reply(cmd," ok","");
		return 0;
	}
	if((strcmp(cmd,"get_cvl"))==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		setgetcvl(0,data1,status,cmdstat);
		reply(cmd,data1,"");
		return 0;
	}
if((strcmp(cmd,"set_chrg"))==0)
	{
		if(numarg !=3)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s",cmd,arg1);		
		if(!chksumchk(arg2,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		if( atof(arg1) < SOCMIN)
		{
			sprintf(status," Warning: Setting charge threshold below %0.2f can affect system performance",SOCMIN);
			reply(cmd,status,"");
			
		}
		if((atof(arg1)>99.9) | (atof(arg1)<10.0))
		{
			sprintf(status," Error: Charge cycle threshold cannot be set higher then 99.9 or lower than 10.0");
			reply(cmd,status,"");
			return 1;
		}
		setgetchrg(1,arg1,cmdstat);
		reply(cmd," ok","");
		return 0;
	}
	if((strcmp(cmd,"get_chrg"))==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		setgetchrg(0,data1,cmdstat); //AHF removed status as third argument 
		reply(cmd,data1,"");
		return 0;
	}
	if((strcmp(cmd,"set_swgf_dly"))==0)
	{
		if(numarg != 3)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s",cmd,arg1);		
		if(!chksumchk(arg2,incmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		swgf_dly = atoi(arg1);
		//write to the cvt reg of swgf_dly
		sprintf(temp,"%c",swgf_dly);
		i2c_putstat1(CVTR_CH,CVTR_SADDR,CVTR_SWGF_DLY_REG,(unsigned char *)temp,1);
		
		reply(cmd," ok","");
		return 0;
	}
	if((strcmp(cmd,"get_swgf_dly"))==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		sprintf(data1," %d",swgf_dly);
		reply(cmd,data1,"");
		return 0;
	}
	if((strcmp(cmd,"swgf_on"))==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		cvt_ctrl(6,cmdstat);
		reply(cmd,cmdstat,"");		// Changed for rev 9
		return 0;
	}
	if((strcmp(cmd,"swgf_off"))==0)		// Added for rev 9
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		cvt_ctrl(7,cmdstat);
		reply(cmd,cmdstat,"");		
		return 0;
	}
	if(strcmp(cmd,"set_cvt") == 0)	//set_cvt <start location> <data written to reg(s) "," deliminated> <chksum>
	{
		if(numarg != 4)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s %s",cmd,arg1,arg2);	//no chksum for this command	
		if(!chksumchk(arg3,incmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		setgetcvtregister(1,arg1,data1,arg2);		//data1 here is a filler
		reply(cmd,data1,"");
		return 0;
	}
	if(strcmp(cmd,"get_cvt") == 0)		//get_cvt <start location> <length> <chksum>
	{
		if(numarg != 4)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if((atoi(arg2)+atoi(arg1)) > 75)
		{
			reply(cmd," Warning: There are only 75 registers! Only the contents from the valid registers are shown.","");
		}
		sprintf(incmd,"%s %s %s",cmd,arg1,arg2);		
		if(!chksumchk(arg3,incmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		setgetcvtregister(0,arg1,arg2,data1);
		reply(cmd,data1,"");
		return 0;
	}
	if((strcmp(cmd,"get_cvt_err"))==0)
	{
		if(numarg != 2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{
			reply(cmd,ERROR2,"");
			return 1;
		}
		errcnt = displaycvterrdetail();
		sprintf(data1," Total CVTR Error Count=%d",errcnt);
		reply(cmd,data1,"");
		return 0;
	}
	if((strcmp(cmd, "cvt_reset")) == 0)
	{
		if(numarg !=2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		cvt_ctrl(5,cmdstat);
		reply(cmd,"",cmdstat);
		return 0;
	}
    if((strcmp(cmd, "leds_on")) == 0) //turn board mounted LEDs on
	{
		if(numarg !=2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		if(leddrive(1)) //leddrive returns 1 with success - continuing on with the logic from above statements, error returns 1, but response() in main isn't checked.
		{
		reply(cmd," ok","");
		return 0;
		}
		else
		{
		reply(cmd," leddrive error","");
		return 1;
		}
	}
    if((strcmp(cmd, "leds_off")) == 0) //turn board mounted LEDs off
	{
		if(numarg !=2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		if(leddrive(0)) //leddrive returns 1 with success 
		{
		reply(cmd," ok","");
		return 0;
		}
		else
		{
		reply(cmd," leddrive error","");
		return 1;
		}
	}
    if((strcmp(cmd, "set_dsf")) == 0) //set DatastreamFlag to manually poll the i2c sensors
	{                                 //Use if both internal and external interrupts fail - Give PSC a "go 1" to log all data from the port
		if(numarg !=2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		setdsf();
        reply(cmd," ok","");
		return 0;
	}
    if(strcmp(cmd,"set_timing_ext")==0)	//A value of 1 configures interrupt priority so that the external interrupt is used - system defaults to internal, value of 0
	{
		if(numarg != 3)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		sprintf(incmd,"%s %s",cmd,arg1);		
		if(!chksumchk(arg2,incmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		if((atoi(arg1)!=0) & (atoi(arg1)!=1))  //flag can be 0/1
		{
			reply(cmd,arg1," error invalid state");
			return 1;
		}
		set_timing_external(atoi(arg1));
		reply(cmd,arg1," ok");	
		return 0;		
	}
    if((strcmp(cmd, "reset_rtc")) == 0) //resets the real time clock alarm flags
	{                                 
		if(numarg !=2)
		{
			reply(cmd,ERROR1,"");
			return 1;
		}
		if(!chksumchk(arg1,cmd))
		{	
			reply(cmd,ERROR2,"");
			return 1;
		}
		rtclk_reset();
        reply(cmd," ok","");
		return 0;
	}
    if((strcmp(cmd, "set_failsafe_en")) == 0)
    {
        if(numarg != 3) 
        {
            reply(cmd, ERROR1, "");
            return 1;
        }
        sprintf(incmd, "%s %s", cmd, arg1);
        if (!chksumchk(arg2, incmd)) 
        {
            reply(cmd, ERROR2, "");
            return 1;
        }
        if ((atoi(arg1) != 0) & (atoi(arg1) != 1)) //flag can be 0/1
        {
            reply(cmd, arg1, " error invalid state");
            return 1;
        }
        failsafe_enable = atoi(arg1);
        reply(cmd, arg1, " ok");
        return 0;		
        
    }
    reply(cmd," error Invalid Command","");
    return 1;
}

void datastream(int inhibit)
{
	char message[400]={};
	char bp_status[60];
	char int_temp[8];
	char ex_status[60],pv_status[200],pv_detail[200],hyd_status[200];
	char bt_status[200],bt_detail[200],psch_status[200],cvt_errflag[9],swgf_status1[50];
	double bt_v=0,bt_t=0;
	char cv_status[200],cv_detail[200];
	//char debugmsg[50];	//only for debuging purpose, do not need when it is real

	EnableADC1;		//start to measure the Fused 5V sense
	totalinputcurrent = 0.0;
	chk_bp(bp_status,int_temp); //check Buoy Power (obviously)
	chk_ext_pwr(ex_status);
	chk_pv(pv_status,pv_detail);
	chk_conv(cv_status,cv_detail,swgf_status1);
	chk_psch(psch_status);
	chk_bt(bt_status,bt_detail,&bt_v,&bt_t);
    chk_hyd(hyd_status);

	sprintf(cvt_errflag," %02x%02x%02x%02x",cvtrbuffer[58],cvtrbuffer[59],cvtrbuffer[56],cvtrbuffer[57]);

	sprintf(message,"%s:%s %.1f %04x %04x%04x %04x%04x",CMD_psc,bp_status,batt_chg_status,
					override_flag,error_flag_2,error_flag_1,error_flag_4,error_flag_3);
	strcat(message,pv_status);
//	strcat(message,cv_status);		//moved on 2/11/2013 per Steve Lerner's request to the end of the msg
	strcat(message,bt_status);
	strcat(message,ex_status);
	strcat(message,psch_status);
	strcat(message,int_temp); 	
    strcat(message,hyd_status);
	
	//if(brd_config)
	//{
	//	strcat(message,cv_status);			//Write real CVTR data if board is present
	//	strcat(message,cvt_errflag);
	//}
	//else
	//{
	//	strcat(message," swg 0 0.00 0.00 cvt 0 0.0 0.0 0 0.0");		//or else put dummy data in here since there is no CVTR board 
	//	strcat(message," 00000000");
	//}
    
	//below is for debuging purpose only. Take them out when it is for real!
//	sprintf(debugmsg," %ld %ld %ld %ld %d %d",intcc1, intcc2, intcc3, intcc4, OverAsbChgVolt, OverCondChgVolt);
//	strcat(message,debugmsg);
	//end of debuging

    if(inhibit == 0)
    {
		if((--datastreamcnt_loc) == 0)
		{
			datastreamcnt_loc = datastreamcnt;	
			strcpy(UART1_TXDATA,message);
			tx_comm();
		}
    }
	if(inhibit == 2)		//stream data under "psc ff" command
	{
		strcpy(UART1_TXDATA, message);
		tx_comm();
	}

}

void helplist()
{
	char message1[2048]={};

	strcat(message1, "HELP COMMAND LIST:\r\n");
	
	strcat(message1, "\r\nCommon PSC Commands \r\n");
	strcat(message1, "    connect or disconnect <src_id> <chksum>\r\n      <scr_id> = pv1, pv2, pv3, pv4, cvt, all\r\n");
	strcat(message1, "    stop <chksum>\r\n"); 
	strcat(message1, "    go <output_rate> <chksum>\r\n      <output_rate> = n, => data streams once in n second(s)\r\n");
	//strcat(message1, "    override_off <src_id> <chksum>\r\n      <scr_id> = pv1, pv2, pv3, pv4, cvt, all\r\n");
	strcat(message1, "    swgf_on <chksum>\r\n");
	strcat(message1, "    swgf_off <chksum>\r\n");
	
	strcat(message1, "\r\nPSC Board Commands \r\n");
	strcat(message1, "    psc_reset <chksum>\r\n");
	strcat(message1, "    clr_rst <chksum>\r\n");
	strcat(message1, "    psc <chksum>\r\n");
	//strcat(message1, "    get_time <chksum>\r\n");
	//strcat(message1, "    set_time <yyyy/mm/dd  hr:mn:ss> <chksum>\r\n");
	strcat(message1, "    get_id <chksum>\r\n");
	strcat(message1, "    bt_status <chksum>\r\n");
 	strcat(message1, "    pv_status <chksum>\r\n");
    strcat(message1, "    hyd_status <chksum>\r\n");
	strcat(message1, "    eeprom_read <start addr> <number of bytes to read> <chksum>\r\n");
	strcat(message1, "    eeprom_write <start addr> <comma-separated bytes> <chksum>\r\n");
	strcat(message1, "    get_chrg <chksum>\r\n");
	strcat(message1, "    set_chrg <depth of discharge percentage> <chksum>\r\n");
	//strcat(message1, "    get_cvl <chksum>\r\n");
	//strcat(message1, "    set_cvl <enablelvl> <disablelvl> <chksum>\r\n");
	//strcat(message1,  "    set_id <PN> <SN> <hwrev> <chksum>\r\n");		//for production FAT only
	strcat(message1, "    leds_on <chksum> Turn board mounted LEDs on\r\n");
	strcat(message1, "    leds_off <chksum> Turn board mounted LEDs off\r\n");
	strcat(message1, "    set_dsf <chksum> Set DatastreamFlag to manually poll the i2c sensors. Use if both internal and external interrupts fail\r\n");
	strcat(message1, "    set_timing_ext <0/1> <chksum> A value of 1 configures interrupt priority so that the external interrupt is used - system defaults to internal\r\n");
	strcat(message1, "    reset_rtc <chksum> Resets the RTC alarm flags manually\r\n");
    
    /*
	strcat(message1, "\r\nCVTR Board Commands\r\n");
	strcat(message1, "    get_swgf_dly <chksum>\r\n");
	strcat(message1, "    set_swgf_dly <dwell time in seconds> <chksum>\r\n");	
	strcat(message1, "    cvt_reset <chksum>\r\n");
	strcat(message1, "    cvt_status <chksum>\r\n");
	strcat(message1, "    cvt <cmd> <chksum>\r\n      <cmd> = enable_in, disable_in, enable_out, disable_out\r\n");
	strcat(message1, "    get_cvt <start location> <number of bytes> <chksum>\r\n");
	strcat(message1, "    set_cvt <start addr in CVTR buffer> <comma-separated bytes> <chksum>\r\n");
	strcat(message1, "    get_cvt_err <chksum>\r\n");
    */
	strcpy(UART1_TXDATA,message1);
	tx_comm();
}

void reply(char *cmd, char *device, char *status)
{
	char message[512];

	//format UART1_TXDATA, generate chksum, tx the reply
	if(strlen(status) == 0)
	{
		sprintf(message,"%s:%s",cmd,device);
	}
	else
	{
		sprintf(message,"%s: %s %s",cmd,device,status);
	}
	strcpy(UART1_TXDATA ,message);
	tx_comm(); 	
}

int update_override_flag(int srcnum)	//update EEPROM override flag when OVERRIDE_OFF CMD received
{
	switch(srcnum)
	{
		case 1:		//PV1, bit 2
			override_flag = override_flag & 0x01FB;
			return 0;
		case 2:		//PV2, bit 3
			override_flag = override_flag & 0x01F7;
			return 0;
		case 3:		//PV3, bit 4
			override_flag = override_flag & 0x01EF;
			return 0;
		case 4:		//PV4, bit 5
			override_flag = override_flag & 0x01DF;
			return 0;
		case 9:		//CVT, bit 8
			override_flag = override_flag & 0x00FF;
			return 0;
		case 10:	//reset, --> reset override_flag to all overrided
			override_flag = 0x1ff;
			return 0;
		case 11:	//all, clear every bit
			override_flag = override_flag & 0x0000;
			return 0;
		default:
			return -1;
	}
}	

int src_id_chk(char *srcid)
{
	if(strcmp(srcid,"pv1")==0)return 1;
	if(strcmp(srcid,"pv2")==0)return 2;
	if(strcmp(srcid,"pv3")==0)return 3;
	if(strcmp(srcid,"pv4")==0)return 4;
	//if(strcmp(srcid,"cvt")==0)return 9;			//for 24V-300V Converter Input control
	if(strcmp(srcid,"reset")==0)return 10;		//for PSC reset command
	if(strcmp(srcid,"all")==0)return 11;
    if(strcmp(srcid,"hyd")==0) return 12; // ppw 08032026
	return 0;	
}

int valid_src_cmd(char *command)
{
	if(!strcmp(command,"on")) return 1;
	if(!strcmp(command,"enable_in")) return 1;		//this is for cvtr 
	if(!strcmp(command,"off")) return 2;
	if(!strcmp(command,"disable_in")) return 2;		//this is for cvtr
	if(!strcmp(command,"enable_out")) return 3;		//below are for cvtr specific
	if(!strcmp(command,"disable_out")) return 4;
	if(!strcmp(command,"reset")) return 5;
	if(!strcmp(command,"swgftst")) return 6;
	return 0;
}

int displaycvterrdetail()
{
	int i, j, cnt = 0;

	for (j=0;j<4;j++)
	{
		for (i=0;i<8;i++)
		{
			if((cvtrbuffer[56+j]>>i)&0x1)
			{
				strcpy(UART1_TXDATA, cvt_err_detail[i+8*j]);
				tx_comm(); 	
				cnt++;
			}
		}	
	}
	return cnt;
}

void voltage_failsafe()
{
    char bp_status[60];
    char int_temp[8];
    char cmdstat[6];
    chk_bp(bp_status,int_temp); //bp_status: sprintf(bp_status, " %.2f %.2f", bp_v, bp_i);
    //bp_v (type double) is updated in chk_bp, and it's global so we can use it directly.
    if(bp_v >= VMAX)
    {
        if(!failsafe_flag)
        {
            connect("disconnect",11,cmdstat);  //disconnect all command. 11 corresponds to all, it is referenced in a lookup table in src_id_chk
            reply("PSC failsafe disconnect","all",cmdstat);
        }
        failsafe_flag=1;
    }
    else 
    {
        failsafe_flag=0;
    }
}
