/**********************************************************************
*    WHOI/Raytheon, 2010
*
* FileName:        	chksum.c
* Dependencies:     Header (.h) files if applicable, see code below
* Processor:       	PIC24FJ256GB106
* Compiler:        	MPLAB C30 v3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev. 	Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1		Joseph Zhu    	10/28/2010		Initial File Creation 
* 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This routine consists of the message check sum function. A veriant
* of the Modular Sum Error check scheme is used 
**********************************************************************/
#include "chksum.h"
#define uint16_t unsigned int

void chksumgen()
{
	uint16_t chksum = 0;
	int i;
//	char debugstring[1024]={};

	for(i=0;i<strlen(UART1_TXDATA);i++)
	{
		chksum += UART1_TXDATA[i];	
	}  
	sprintf(UART1_TXDATA,"%s %04x\r\n",UART1_TXDATA,chksum);  
//	strcpy(debugstring,UART1_TXDATA); 
}

int chksumchk(char *chksum, char *cmd)
{
	uint16_t chksumd = 0;
	int i;
	char chksums[5],chksumi[5];
	
	if(strcmp(chksum,"ff")==0) return 1;
	if(strcmp(chksum,"FF")==0) return 1;

	switch(strlen(chksum)) 
	{
		case 1:
			sprintf(chksumi,"000%s",chksum);
			break;
		case 2:
			sprintf(chksumi,"00%s",chksum);
			break;
		case 3:
			sprintf(chksumi,"0%s",chksum);
			break;
		case 4:
			strcpy(chksumi,chksum);
			break;
		default:
			return 0;
	} 

	//calculate UART_RXDATA (without " <chksum> <cr><lf>") checksum and compare with chksum
	for(i=0;i<strlen(cmd);i++)
	{
			chksumd +=cmd[i];
	}	
	sprintf(chksums,"%04x",chksumd);
 	if(strcmp(chksums,chksumi)!=0) return 0;
	return 1;
}
