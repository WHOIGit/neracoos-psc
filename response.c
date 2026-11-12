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
* MPSC Rework:
* REV 1 Dan B/Paul W    10/19/2026          REV01 version for New Mini-PSC CCA
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

double H_val;

int response()
{
	char cmd[16]={}, arg1[256]={}, arg2[256]={}, arg3[20]={}, arg4[6]={}, arg5[4]={};
	char status[256], detail[256], incmd[256]={}; //, /*outcmd[256]={},*/ /*others[30]={},*/ temp[4]={};
	int numarg=0, /*cmdnum=0,*/ srcnum=0; //errcnt=0;
	char ERROR1[]=" error Incorrect Number of Arguments";
	char ERROR2[]=" error Command Checksum Error";
	char ERROR3[]=" error Invalid Source ID";
	extern char Rxdata1[100]; 
	char cmdstat[6];
	char id_message[100];
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

		reply(cmd," RESETTING...","");
        asm("RESET");
        
		return -1; //If we get this far something went wrong
	}

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
        reply(cmd,status,"");				
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
		chk_bt(status,detail, &bt_v, &bt_t);
		reply(cmd,detail,"");	//DMB Note: The command is for "status" 
                                //but what we are reporting for this command is detail (more info)
                                //status (consolidated info) is what is reported in the PSC: output in datastream()
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
		chk_hyd(status, &H_val);
		reply(cmd,status,"");		
		return 0;		
	}
    if(strcmp(cmd,"cal_hyd")==0)
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
		if(cal_hyd())
        {
            reply(cmd, "FAILED", "");
            return 1;
        }
        else
        reply(cmd, "SUCCESS", "");
        return 0;
    }
        
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
    
    reply(cmd," error Invalid Command","");
    return 1;
}

void datastream(int inhibit)
{
	char message[400]={};
    char f_safe_mess[20];
	//char bp_status[60];
	//char int_temp[8];
	char ex_status[60],pv_status[200],pv_detail[200],hyd_status[200];
	char bt_status[200],bt_detail[200],psch_status[200]; 
	double bt_v=0,bt_t=0;


	EnableADC1;		//start to measure the Fused 5V sense
	totalinputcurrent = 0.0;
	//chk_bp(bp_status,int_temp); // DMB: Buoy Power (For REV01 MPSC CCA this is coming from BT2 (J4)), on REV A it will have its own separate output
	chk_ext_pwr(ex_status);
	chk_pv(pv_status,pv_detail);
	chk_psch(psch_status); //hotel load
	chk_bt(bt_status,bt_detail, &bt_v, &bt_t);
    chk_hyd(hyd_status, &H_val);


	sprintf(message,"%s:",CMD_psc);

	strcat(message,pv_status);
	strcat(message,bt_status);
	strcat(message,ex_status);
	strcat(message,psch_status);
	//strcat(message,int_temp); 	
    strcat(message,hyd_status);
    
    sprintf(f_safe_mess, " fsafe_flag: 0x%.2X", failsafe_flag);
    strcat(message, f_safe_mess);
	
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
	strcat(message1, "    connect or disconnect <src_id> <chksum>\r\n      <scr_id> = pv1, pv2, pv3, pv4, all\r\n");
	strcat(message1, "    stop <chksum>\r\n"); 
	strcat(message1, "    go <output_rate> <chksum>\r\n      <output_rate> = n, => data streams once in n second(s)\r\n");
	strcat(message1, "\r\nPSC Board Commands \r\n");
	strcat(message1, "    psc_reset <chksum>\r\n");
	strcat(message1, "    psc <chksum>\r\n");
	strcat(message1, "    get_id <chksum>\r\n");
	strcat(message1, "    bt_status <chksum>\r\n");
 	strcat(message1, "    pv_status <chksum>\r\n");
    strcat(message1, "    hyd_status <chksum>\r\n");
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

int src_id_chk(char *srcid)
{
	if(strcmp(srcid,"pv1")==0)return 1;
	if(strcmp(srcid,"pv2")==0)return 2;
	if(strcmp(srcid,"pv3")==0)return 3;
	if(strcmp(srcid,"pv4")==0)return 4;
	if(strcmp(srcid,"reset")==0)return 10;		//for PSC reset command
	if(strcmp(srcid,"all")==0)return 11;
    if(strcmp(srcid,"hyd")==0) return 12; // ppw 08032026
	return 0;	
}

void failsafe() //DMB: Modified formerly "voltage_failsafe" to just "failsafe" 
                //to cover both over-voltage and excessive Hydrogen and temperature. As we are still 
                //testing the BLD1-I2C Compact (Nov 2026), we don't want the disconnect
                //to be absolute, so like the original function there is nothing stopping
                // a manual intervention to turn PV inputs back on if desired
{
    char bt_status_loc[60];
    double bt_v_loc, bt_t_loc;
    char detail_loc[256];
    char cmdstat_loc[60];
    char status[256];
    
    chk_bt(bt_status_loc,detail_loc, &bt_v_loc,  &bt_t_loc);
    chk_hyd(status, &H_val);
    
    if(bt_v_loc > 31.0)
    {
        if(failsafe_flag < 1)
        {
            connect("disconnect",11,cmdstat_loc);  //disconnect all command. 11 corresponds to all
            reply("PSC High Voltage failsafe disconnect","all",cmdstat_loc);
        }
        failsafe_flag|= V_FLAG;
    }
    else if (bt_t_loc > 50.0)
    {
        if(failsafe_flag < 1)
        {
            connect("disconnect",11,cmdstat_loc);  //disconnect all command. 11 corresponds to all
            reply("PSC High Temperature failsafe disconnect","all",cmdstat_loc);
        }
        failsafe_flag|= T_FLAG;
    }
    else if (H_val > 1.9)
    {
        if(failsafe_flag < 1)
        {
            connect("disconnect",11,cmdstat_loc);  //disconnect all command. 11 corresponds to all
            reply("PSC Excessive Hydrogen failsafe disconnect","all",cmdstat_loc);
        }
        failsafe_flag|= H_FLAG;
    }       
    else 
    {
        failsafe_flag=0; //RESET
    }
}
