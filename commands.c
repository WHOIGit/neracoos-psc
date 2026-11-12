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
* 
* MPSC Rework:
* REV 1 Dan B/Paul W    10/19/2026          REV01 version for New Mini-PSC CCA
*
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
		case 1: //PV1
            PV_IN_CH_ON[0]=cntl;
            EN_PV_CH1 = cntl;   //Set CH1 output control bit
			break;
		case 2: //PV2
            PV_IN_CH_ON[1]=cntl;
            EN_PV_CH2 = cntl;   //Set CH2 output control bit
			break;
		case 3:
            PV_IN_CH_ON[2]=cntl;
            EN_PV_CH3 = cntl;   //Set CH3 output control bit
			break;
		case 4:
            PV_IN_CH_ON[3]=cntl;
            EN_PV_CH4 = cntl;   //Set CH4 output control bit
			break;
        case 11: //all ON or OFF
            PV_IN_CH_ON[0]=cntl;
            EN_PV_CH1 = cntl;
            PV_IN_CH_ON[1]=cntl;
            EN_PV_CH2 = cntl;
            PV_IN_CH_ON[2]=cntl;
            EN_PV_CH3 = cntl;
            PV_IN_CH_ON[3]=cntl;
            EN_PV_CH4 = cntl;
            break;
            
		default:
			break;
	}
	strcpy(cmdstat,"ok");
}

void get_id(char *id_message)
{	
        sprintf(id_message," CCA_REV: %s MPSC_fw_rev: %.2d ", board_rev, MPSC_FWRevision);
}

#if 0
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
#endif

