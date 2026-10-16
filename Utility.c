/**********************************************************************
*    WHOI/Raytheon, 2010
*
* FileName:        	Utility.c
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
* This routine collects all the useful tools
**********************************************************************/
#include "utility.h"

int read_pvcntlreg(int code,int SS)
{
	unsigned int config1 = SPI_MODE8_ON|SPI_CKE_ON|MASTER_ENABLE_ON|SEC_PRESCAL_4_1|PRI_PRESCAL_1_1;
	unsigned int config2 = 0x0;
	unsigned int config3 = SPI_ENABLE;
	int i;
	unsigned int Receivedata[3] ={}, data=0, cmd[3]={};

	OpenSPI2(config1,config2,config3);
	switch (SS)
	{
		case 1:
			SS21_PV1 = 0;
			break;
		case 2:
			SS22_PV2 = 0;
			break;
		case 3:
			SS23_PV3 = 0;
			break;
		case 4:
			SS24_PV4 = 0;
			break;
		default:
			break;
	}
	cmd[0] = code;
	cmd[1]=0x0;
	cmd[2]=0x0;

	__delay_us(1);
	for(i=0;i<3;i++)
	{
		while(SPI2STATbits.SPITBF);
		SPI2BUF = cmd[i];
		while(!SPI2STATbits.SPIRBF);
		Receivedata[i] = SPI2BUF;
	}

	__delay_us(1);
	SS21_PV1 = 1;
	SS22_PV2 = 1;
	SS23_PV3 = 1;
	SS24_PV4 = 1;
	__delay_us(1);
	CloseSPI2();

	data = (Receivedata[1]<<0x8) + Receivedata[2];

	return data;
}

void write_pvcntlreg(int pvcntlcmd, int slaveselect)
{
	unsigned int config1 = SPI_MODE8_ON|SPI_SMP_ON|SPI_CKE_ON|MASTER_ENABLE_ON|SEC_PRESCAL_4_1|PRI_PRESCAL_1_1;
	unsigned int config2 = 0x0;
	unsigned int config3 = SPI_ENABLE;
	char Receivedata;
	unsigned int i, cmd[3]={};

	OpenSPI2(config1,config2,config3);
	switch (slaveselect)
	{
		case 1:
			SS21_PV1 = 0;
			break;
		case 2:
			SS22_PV2 = 0;
			break;
		case 3:
			SS23_PV3 = 0;
			break;
		case 4:
			SS24_PV4 = 0;
			break;
		default:
			break;
	}
	__delay_us(1);
	cmd[0] = pvcntlcmd;
	for(i=0;i<3;i++)
	{
		while(SPI2STATbits.SPITBF);
		SPI2BUF = cmd[i];
		while(!SPI2STATbits.SPIRBF);
		Receivedata = SPI2BUF;
	}

	__delay_us(1);
	SS21_PV1 = 1;
	SS22_PV2 = 1;
	SS23_PV3 = 1;
	SS24_PV4 = 1;
	__delay_us(1);
	CloseSPI2();
}

int SPI1tx(unsigned int data, int CE_type)
{
	unsigned int config1 = SPI_MODE16_ON|SPI_SMP_ON|SPI_CKE_ON|MASTER_ENABLE_ON|SEC_PRESCAL_4_1|PRI_PRESCAL_1_1;
	unsigned int config2 = 0x0;
	unsigned int config3 = SPI_ENABLE;

	unsigned int Receivedata;

	if(CE_type==LED)
	{
		SS12_LED = 0;
	}
	else
	{
		SS11_RLY = 0;
	}
	__delay_us(1);

	OpenSPI1(config1, config2, config3);
	while(SPI1STATbits.SPITBF);
	SPI1BUF = data;
	while(!SPI1STATbits.SPIRBF);
	Receivedata = SPI1BUF;

	__delay_us(1);
	SS12_LED = 1;
	SS11_RLY = 1;

	__delay_us(1);
	CloseSPI1();
	return 1;
}

int SPI2tx(unsigned int data)		//--this needs to be fixed
{
	unsigned int config1 = SPI_SMP_ON|SPI_CKE_ON|SLAVE_ENABLE_ON|MASTER_ENABLE_ON|SEC_PRESCAL_4_1|PRI_PRESCAL_1_1;
	unsigned int config2 = 0x0;
	unsigned int config3 = SPI_ENABLE;

	OpenSPI2(config1, config2, config3);
	WriteSPI2(data);
	CloseSPI2();
	return 1;
}

int i2c_getstat1(int i2c_ch, int SlaveAddress, int regaddr, int length, unsigned char *pRead)
{
	char addr_cmd[10];
	char read_cmd[3];
	int status;
	unsigned int conf1, conf2;

	conf1 = I2C_ON;
	conf2 = I2C_BRG;

	sprintf(addr_cmd,"%c%c", (SlaveAddress|0),regaddr);
	sprintf(read_cmd,"%c",(SlaveAddress|1));

	switch(i2c_ch)
	{
		case 1:
			OpenI2C1(conf1, conf2);
			StartI2C1();
			if(ChkI2C1Idle(10))
			{
				CloseI2C1();
				return -2;
			}

			status = MasterI2C1puts((unsigned char *)addr_cmd, 2, 2000);
			if(status)
			{
				CloseI2C1();
				return status;			//report error
			}
			if((I2C1STAT & 0x8000) == 0x8000)
			{
				CloseI2C1();
				return -3;				//invalid slave address fault
			}

			StartI2C1();
			if(ChkI2C1Idle(10))
			{
				CloseI2C1();
				return -2;
			}

			status = MasterI2C1puts((unsigned char *)read_cmd, 1, 2000);
			if(status)
			{
				CloseI2C1();
				return status;			//report error
			}
			if((I2C1STAT & 0x8000) == 0x8000)
			{
				CloseI2C1();
				return -4;				//invalid read command fault
			}

			status = MasterI2C1gets(length, pRead, 2000); //read x length of bytes
			if(status)
			{
				CloseI2C1();
				return status; 			//report error
			}
			if((I2C1STAT & 0x8000) == 0x8000)
			{
				CloseI2C1();
				return -5;				//invalid read data fault
			}

			StopI2C1();
			if(ChkI2C1Idle(10))
			{
				CloseI2C1();
				return -2;
			}

			CloseI2C1();
			break;
		case 2:
			OpenI2C2(conf1, conf2);
			StartI2C2();
			if(ChkI2C2Idle(10))
			{
				CloseI2C2();
				return -2;
			}

			status = MasterI2C2puts((unsigned char *)addr_cmd, 2, 2000);
			if(status)
			{
				CloseI2C2();
				return status;			//report error
			}
			if((I2C2STAT & 0x8000) == 0x8000)
			{
				CloseI2C2();
				return -3;				//invalid slave address fault
			}

			StartI2C2();
			if(ChkI2C2Idle(10))
			{
				CloseI2C2();
				return -2;
			}

			status = MasterI2C2puts((unsigned char *)read_cmd, 1, 2000);
			if(status)
			{
				CloseI2C2();
				return status;			//report error
			}
			if((I2C2STAT & 0x8000) == 0x8000)
			{
				CloseI2C2();
				return -4;				//invalid read command fault
			}

			status = MasterI2C2gets(length, pRead, 2000); //read x length of bytes
			if(status)
			{
				CloseI2C2();
				return status; 			//report error
			}
			if((I2C2STAT & 0x8000) == 0x8000)
			{
				CloseI2C2();
				return -5;				//invalid read data fault
			}

			if(ChkI2C2Idle(10))
			{
				CloseI2C2();
				return -2;
			}
			StopI2C2();
			if(ChkI2C2Idle(10))
			{
				CloseI2C2();
				return -2;
			}

			CloseI2C2();
			break;
		case 3:
			//OpenI2C3(conf1, conf2);
			OpenI2C3(conf1, 0x9D); // ppw 08052026
			StartI2C3();
			if(ChkI2C3Idle(10))
			{
				CloseI2C3();
				return -2;
			}

			status = MasterI2C3puts((unsigned char *)addr_cmd, 2, 2000);
			if(status)
			{
				CloseI2C3();
				return status;			//report error
			}
//			if((I2C3STAT & 0x8000) == 0x8000)
//			{
//				CloseI2C3();
//				return -3;				//invalid slave address fault
//			}

			RestartI2C3();
			if(ChkI2C3Idle(10))
			{
				CloseI2C3();
				return -2;
			}

			status = MasterI2C3puts((unsigned char *)read_cmd, 1, 2000);
			if(status)
			{
				CloseI2C3();
				return status;			//report error
			}
//			if((I2C3STAT & 0x8000) == 0x8000)
//			{
//				CloseI2C3();
//				return -4;				//invalid read command fault
//			}

			status = MasterI2C3gets(length, pRead, 2000); //read x length of bytes
			if(status)
			{
				CloseI2C3();
				return status; 			//report error
			}
//			if((I2C3STAT & 0x8000) == 0x8000)
//			{
//				CloseI2C3();
//				return -5;				//invalid read data fault
//			}

			StopI2C3();
			if(ChkI2C3Idle(10))
			{
				CloseI2C3();
				return -2;
			}

			CloseI2C3();
			break;
		default:
			break;
	}
	return 0;
}

int i2c_putstat1(int i2c_ch, int SlaveAddress, int regaddr,unsigned char *I2CWrite, int wrt_cnt)
{
	char addr_cmd[10];
	unsigned int status;

	sprintf(addr_cmd,"%c%c", (SlaveAddress|0),regaddr);

	switch(i2c_ch)
	{
		case 1:
			OpenI2C1(I2C_ON, I2C_BRG);
			StartI2C1();
			if(ChkI2C1Idle(10))
			{
				CloseI2C1();
				return -2;
			}

			status = MasterI2C1puts((unsigned char *)addr_cmd, 2, 2000);
			if(status)
			{
				CloseI2C1();
				return status;			//report error
			}
			if((I2C1STAT & 0x8000) == 0x8000)
			{
				CloseI2C1();
				return -3;				//invalid slave address fault
			}

			status = MasterI2C1puts(I2CWrite, wrt_cnt, 2000); 	//write data
			if(status)
			{
				CloseI2C1();
				return status;			//report error
			}
			if((I2C1STAT & 0x8000) == 0x8000)
			{
				CloseI2C1();
				return -4;				//invalid write data fault
			}

			StopI2C1();
			if(ChkI2C1Idle(10))
			{
				CloseI2C1();
				return -2;
			}

			CloseI2C1();
			break;
		case 2:
			OpenI2C2(I2C_ON, I2C_BRG);
			StartI2C2();
			if(ChkI2C2Idle(10))
			{
				CloseI2C2();
				return -2;
			}

			status = MasterI2C2puts((unsigned char *)addr_cmd, 2, 2000);
			if(status)
			{
				CloseI2C2();
				return status;			//report error
			}
			if((I2C2STAT & 0x8000) == 0x8000)
			{
				CloseI2C2();
				return -3;				//invalid slave address fault
			}

			status = MasterI2C2puts(I2CWrite, wrt_cnt, 2000); 	//write data
			if(status)
			{
				CloseI2C2();
				return status;			//report error
			}
			if((I2C2STAT & 0x8000) == 0x8000)
			{
				CloseI2C2();
				return -4;				//invalid write data fault
			}

			StopI2C2();
			if(ChkI2C2Idle(10))
			{
				CloseI2C2();
				return -2;
			}

			CloseI2C2();
			break;
		case 3:
			OpenI2C3(I2C_ON, 0x9D); // ppw 08052026
			StartI2C3();
			if(ChkI2C3Idle(10))
			{
				CloseI2C3();
				return -2;
			}

			status = MasterI2C3puts((unsigned char *)addr_cmd, 2, 2000);
			if(status)
			{
				CloseI2C3();
				return status;			//report error
			}
			if((I2C3STAT & 0x8000) == 0x8000)
			{
				CloseI2C3();
				return -3;				//invalid slave address fault
			}

			status = MasterI2C3puts(I2CWrite, wrt_cnt, 2000); 	//write data
			if(status)
			{
				CloseI2C3();
				return status;			//report error
			}
			if((I2C3STAT & 0x8000) == 0x8000)
			{
				CloseI2C3();
				return -4;				//invalid write data fault
			}

			StopI2C3();
			if(ChkI2C3Idle(10))
			{
				CloseI2C3();
				return -2;
			}

			CloseI2C3();
			break;
		default:
			break;
	}
	return 0;
}

int hex2dec(char input)		// Convert hex or decimal char to decimal int
{
	switch (input)
	{
		case 0:
		case '0':
			return 0;
		case 1:
		case '1':
			return 1;
		case 2:
		case '2':
			return 2;
		case 3:
		case '3':
			return 3;
		case 4:
		case '4':
			return 4;
		case 5:
		case '5':
			return 5;
		case 6:
		case '6':
			return 6;
		case 7:
		case '7':
			return 7;
		case 8:
		case '8':
			return 8;
		case 9:
		case '9':
			return 9;
		case 10:
		case 'a':
		case 'A':
			return 10;
		case 11:
		case 'b':
		case 'B':
			return 11;
		case 12:
		case 'c':
		case 'C':
			return 12;
		case 13:
		case 'd':
		case 'D':
			return 13;
		case 14:
		case 'e':
		case 'E':
			return 14;
		case 15:
		case 'f':
		case 'F':
			return 15;
		default:
			return -1;
	}
}

int bin2dec(char binary)
{
	int decimal = 0;
	int j, digit;

	for(j=0;j<8;j++)
	{
		if((binary>>j)&0x1)
		{
			digit = 1;
		}
		else
		{
			digit = 0;
		}
		decimal += digit * pow(2,j);
	}
	return decimal;
}

int dec2bcd(int dec)
{
	int bcd;
	bcd = ((dec/10)<<4)+(dec%10);
	return bcd;
}

char dec2char(int dec)		// Convert integer to BCD byte
{
	char bcdchar;
	bcdchar = (char)((dec/10)<<4)+(dec%10);
	return bcdchar;
}

int bcd2hex(int bcd)
{
	int hex;
	hex = ((bcd/16)*10)+(bcd%16);
	return hex;
}

int bcd2dec(unsigned char bcd)
{
	unsigned int dec;
	dec = ((bcd>>4)*16) + (bcd%16);
	return dec;
}

int word2dec(char *data)		// Convert BCD char string to decimal
{
	int dec;
	char tempdata[3]={};

	sprintf(tempdata,data);
	if(strlen(tempdata)<2)
		dec=hex2dec(tempdata[0]);
	else
		dec = hex2dec(tempdata[0])*16 + hex2dec(tempdata[1]);
	return dec;
}

int hexbyte2dec(char data)		// Convert BCD byte to decimal int
{
	int dec=0;
	dec = hex2dec((data>>4)&0xf)*10+hex2dec(data &0xf);
	return dec;
}

double get_data1(unsigned char *i2cdata, int iBase, double resolution)
{
	unsigned char data1_h=0, data1_l=0;
	double sdata = 0;

	data1_l = (i2cdata[iBase]);
	data1_h = (i2cdata[iBase+1]) & 0xf0;
	sdata = ((data1_l << 4) + (data1_h>>4)) * resolution;

	return sdata;
}

int ChkI2C1Idle(int dly_cnt)
{
	int wait=0;

	//check if I2C1 Bus is Inactive
	while(I2C1CONbits.SEN || I2C1CONbits.RSEN || I2C1CONbits.PEN || I2C1CONbits.RCEN ||
          	I2C1CONbits.ACKEN || I2C1STATbits.TRSTAT)
	{
		if(wait < dly_cnt)
			wait++;
		else
			return -2;			//timeout! bus is not in idle state
		__delay_us(10);
	}
	return 0;					//return bus is in idle state
}

int ChkI2C2Idle(int dly_cnt)
{
	int wait=0;

	//check if I2C2 Bus is Inactive
	while(I2C2CONbits.SEN || I2C2CONbits.RSEN || I2C2CONbits.PEN || I2C2CONbits.RCEN ||
          	I2C2CONbits.ACKEN || I2C2STATbits.TRSTAT)
	{
		if(wait < dly_cnt)
			wait++;
		else
			return -2;			//timeout! bus is not in idle state
		__delay_us(10);
	}
	return 0;					//return bus is in idle state
}

int ChkI2C3Idle(int dly_cnt)
{
	int wait=0;

	//check if I2C3 Bus is Inactive
	while(I2C3CONbits.SEN || I2C3CONbits.RSEN || I2C3CONbits.PEN || I2C3CONbits.RCEN ||
          	I2C3CONbits.ACKEN || I2C3STATbits.TRSTAT)
	{
		if(wait < dly_cnt)
			wait++;
		else
			return -2;			//timeout! bus is not in idle state
		__delay_us(10);
	}
	return 0;					//return bus is in idle state
}

int MasterI2C1puts(unsigned char *wrptr, int wrt_cnt, int dly_cnt)
{
	int wait = 0;

	while(wrt_cnt)								//transmit data until wrt_cnt is up
	{
		if(MasterputcI2C1(*wrptr) == -1)		//write a byte use MasterWriteI2C1
			return -3;							//return with write collison error

		while(I2C1STATbits.TBF)					//while data is not transmitted
		{
			__delay_us(10);						//insert 10us wait
			if(wait<dly_cnt)
				wait++;							//check if data is transmitted
			else
				return -4;						//tiemout! data not transmit, return transmit error
		}

		if((ChkI2C1Idle(10)))					//bus would not idle!
			return -2;							//return with bus error
		wrptr++;
		wrt_cnt--;
	}
	return 0;									//return with no error
}

int MasterI2C2puts(unsigned char *wrptr, int wrt_cnt, int dly_cnt)
{
	int wait = 0;

	while(wrt_cnt)								//transmit data until wrt_cnt is up
	{
		if(MasterputcI2C2(*wrptr) == -1)		//write a byte use MasterWriteI2C1
			return -3;							//return with write collison error

		while(I2C2STATbits.TBF)					//while data is not transmitted
		{
			__delay_us(10);						//insert 10us wait
			if(wait<dly_cnt)
				wait++;							//check if data is transmitted
			else
				return -4;						//tiemout! data not transmit, return transmit error
		}

		if((ChkI2C2Idle(10)))					//bus would not idle!
			return -2;							//return with bus error
		wrptr++;
		wrt_cnt--;
	}
	return 0;									//return with no error
}

int MasterI2C3puts(unsigned char *wrptr, int wrt_cnt, int dly_cnt)
{
	int wait = 0;

	while(wrt_cnt)								//transmit data until wrt_cnt is up
	{
		if(MasterputcI2C3(*wrptr) == -1)		//write a byte use MasterWriteI2C1
			return -3;							//return with write collison error

		while(I2C3STATbits.TBF)					//while data is not transmitted
		{
			__delay_us(10);						//insert 10us wait
			if(wait<dly_cnt)
				wait++;							//check if data is transmitted
			else
				return -4;						//tiemout! data not transmit, return transmit error
		}

		if((ChkI2C3Idle(10)))					//bus would not idle!
			return -2;							//return with bus error
		wrptr++;
		wrt_cnt--;
	}
	return 0;									//return with no error
}

int MasterI2C1gets(unsigned int length, unsigned char * rdptr, int dly_cnt)
{
    int wait = 0;

    while(length)                    /* Receive the number of bytes specified by length */
    {
        I2C1CONbits.RCEN = 1;
        while(!DataRdyI2C1())
        {
            if(wait < dly_cnt)
                wait++ ;
            else
            return(length);          /* Time out, return number of byte/word to be read */
        }
        wait = 0;
        *rdptr = I2C1RCV;             /* save byte received */
        rdptr++;
        length--;
        if(length == 0)              /* If last char, generate NACK sequence */
        {
            I2C1CONbits.ACKDT = 1;
            I2C1CONbits.ACKEN = 1;
        }
        else                         /* For other chars,generate ACK sequence */
        {
            I2C1CONbits.ACKDT = 0;
            I2C1CONbits.ACKEN = 1;
        }
        while(I2C1CONbits.ACKEN == 1)    /* Wait till ACK/NACK sequence is over */
		{
			if(wait < dly_cnt)
				wait++;
			else
				return -5;
		}
		wait = 0;
    }
    return 0;
}

int MasterI2C2gets(unsigned int length, unsigned char * rdptr, int dly_cnt)
{
    int wait = 0;

    while(length)                    /* Receive the number of bytes specified by length */
    {
        I2C2CONbits.RCEN = 1;
        while(!DataRdyI2C2())
        {
            if(wait < dly_cnt)
                wait++ ;
            else
            	return(length);          /* Time out, return number of byte/word to be read */
        }
        wait = 0;
        *rdptr = I2C2RCV;             /* save byte received */
        rdptr++;
        length--;
        if(length == 0)              /* If last char, generate NACK sequence */
        {
            I2C2CONbits.ACKDT = 1;
            I2C2CONbits.ACKEN = 1;
        }
        else                         /* For other chars,generate ACK sequence */
        {
            I2C2CONbits.ACKDT = 0;
            I2C2CONbits.ACKEN = 1;
        }
        while(I2C2CONbits.ACKEN == 1)   /* Wait till ACK/NACK sequence is over */
		{
			if(wait < dly_cnt)
				wait++;
			else
				return -5;
		}
		wait = 0;
    }
    return 0;
}

int MasterI2C3gets(unsigned int length, unsigned char *rdptr, int dly_cnt)
{
    int wait = 0;

    while(length)                    /* Receive the number of bytes specified by length */
    {
        I2C3CONbits.RCEN = 1;
        while(!DataRdyI2C3())
        {
            if(wait < dly_cnt)
                wait++ ;
            else
            	return(length);          /* Time out, return number of byte/word to be read */
        }
        wait = 0;
        *rdptr = I2C3RCV;             /* save byte received */
        rdptr++;
        length--;
        if(length == 0)              /* If last char, generate NACK sequence */
        {
            I2C3CONbits.ACKDT = 1;
            I2C3CONbits.ACKEN = 1;
        }
        else                         /* For other chars,generate ACK sequence */
        {
            I2C3CONbits.ACKDT = 0;
            I2C3CONbits.ACKEN = 1;
        }
        while(I2C3CONbits.ACKEN == 1)    /* Wait till ACK/NACK sequence is over */
		{
			if(wait < dly_cnt)
				wait++;
			else
				return -5;
		}
		wait = 0;
		__delay_us(10);					//give it a little time for the next byte
    }
    return 0;
}

void tx_comm()
{

	chksumgen();				//Append chksum to UART_DATA
//	U1STAbits.UTXEN = 1;		//Enable UART Transmit for TX UART1_TXDATA
	IEC0bits.U1TXIE = 1;
}

void update_elapsed_time()
{
	long etime;

	//read elapsed time from EEPROM, add 1 and write it back
	etime = get_elapsed_time();
	etime++;
	set_elapsed_time(etime);
}

//*****************************************************************************
//		Elapsed Time read from and store to the EEPROM
/******************************************************************************
	Summary:
	These routines are used to read/write the elapsed time from/to the EEPROM.
	The elapsed time are stored in EEPROM in BCD format with range from 0 to
	999999. Each count represents an hour elapsed.

	Precondition:
	none,

	Parameter:
	unsigned long etime		- the elapsed time write to EEPROM in set_elapsed_time

	Return Value:
	unsigned long etime		- the read out from the EEPROM by get_elapsed_time
*******************************************************************************/
unsigned long get_elapsed_time()
{
	char etimeout[3]={};
	unsigned long etime = 0;

	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,ELPSDTM_REGADDR,3,(unsigned char *)etimeout);
	etime = bcd2hex((int)etimeout[0])*10000 + bcd2hex((int)etimeout[1])*100 + bcd2hex((int)etimeout[2]);
	return etime;
}

void set_elapsed_time(unsigned long etime)
{
	char etimein[4]={};

	sprintf(etimein,"%c%c%c",dec2char(etime/10000),dec2char((etime%10000)/100),dec2char(etime%100));
	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,ELPSDTM_REGADDR,(unsigned char *)etimein,3);
	__delay_ms(5);
}

//*****************************************************************************
//		Measure Battery String Temperature
/******************************************************************************
	Summary:
	This routine measures the battery string temperature and flag it if it is
	hotter than 50 degree C.

	Precondition:
	get_data1,

	Parameter:
	unsigned char *i2cdata		- the raw data read out from LTC4151, AD port
	const long batt_flag		- Battery String Overtemp flag string

	Return Value:
	double battery string temperature
*******************************************************************************/
double get_bt_temp(unsigned char *i2cdata, int baseaddress, const long batt_flag, int type)
{
	int index;
	double result;
	extern double BATT_TEMP_LMT, CVTR_TEMP_LMT;

	index = (get_data1(i2cdata, baseaddress, 1)-180)/10;		//index of batt_temp table.
	if(index<0) result = (batt_temp[0]);
	else if(index>392) result = (batt_temp[392]);
	else result = (batt_temp[index]);
	switch(type)
	{
		case BATTTEMP:
			if(result > BATT_TEMP_LMT)
			{
				error_flag_1 |= batt_flag;
			}
			else
			{
				error_flag_1 &= ~batt_flag;
			}
			break;
		case CVTRTEMP:
			if(result > CVTR_TEMP_LMT)
			{
				error_flag_3 |= batt_flag;
			}
			else
			{
				error_flag_3 &= ~batt_flag;
			}
			break;
		default:
			break;
	}
	return result;
}

void rtclk_reset()
{
	char flag[2]={0x0,0x0};

	i2c_putstat1(RTCLKi2c_CH,RTCLK_SADDR,0x0f,(unsigned char *)flag,1);
}

//*****************************************************************************
//		Configurate UART1
/******************************************************************************
	Summary:
	This routine configurates the UART1 port interrupt priority and enables it

	Precondition:
	None

	Parameter:
	unsigned int config		- configuration parameters

	Return Value:
	None
*******************************************************************************/
void ConfigIntptUART1(unsigned int config)
{
    /* clear IF flags */
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;

    /* set priority */
    IPC2bits.U1RXIP = 0x0007 & config;
    IPC3bits.U1TXIP = (0x0070 & config) >> 4;

    /* enable/disable interrupt */
    IEC0bits.U1RXIE = (0x0008 & config) >> 3;
    IEC0bits.U1TXIE = (0x0080 & config) >> 7;
}

//*****************************************************************************
//		Open UART1 port
/******************************************************************************
	Summary:
	This routine open up the UART1 port according to the input parameters

	Precondition:
	None

	Parameter:
	unsigned int config1	- UART1 control register setting
	unsigned int config2	- UART1 control/status register setting
	unsigned int ubrg		- UART1 baud rate setting

	Return Value:
	None
*******************************************************************************/
void OpenUART1port(unsigned int config1,unsigned int config2, unsigned int ubrg)
{
    U1BRG  = ubrg;     /* baud rate */
    U1MODE = config1;  /* operation settings */
    U1STA = config2;   /* TX & RX interrupt modes */
    while(U1STAbits.URXDA == 1) /* Clear Buffer*/
    {
        char Temp;
        Temp = U1RXREG;
    }
    IFS0bits.U1RXIF = 0;         /*clear interrupt flag*/
}

//*****************************************************************************
//		Coulomb Count update
/******************************************************************************
	Summary:
	This routine updates the Coulomb Counts once every second per current (mA)
	btx_i input. The coulombs of the past second is added to previously
	accumulated coulomb counts stored as the whole number part in intccx and
	the fraction part in fraccx.

	Precondition:
	None

	Parameter:
	double btx_i	- delta current in or out of the battery string x, signed,
						positive = battery discharge
						negative = battery charge
	long *intccx	- whole number part of the total Coulomb Count, signed
						positive = battery discharge
						negative = battery charge
	double *fraccx	- fraction part of the total Coulomb Count, also signed
						positive = battery discharge
						negative = battery charge

	Return Value:   - by referece > intccx and fraccx
*******************************************************************************/
void UpdateCoulombCount(double btx_i, long *intccx, double *fraccx)
{
	double fralevel;
	double intlevel, carry=0;

	fralevel = modf(((-1)*btx_i/1000),&intlevel);	//convert current to coulomb count (current and coulomb count has opposite signage)
	*fraccx = modf((*fraccx + fralevel),&carry);
	*intccx = *intccx + (long)intlevel + (long)carry;
}

//*****************************************************************************
//		Find index from temperature table
/******************************************************************************
	Summary:
	This routine finds the table index of the battery temperature table (TempIndex)
 	based on the temperature input.

	Precondition:
	None

	Parameter:
	double btx_t	- battery string x temperature sensing value

	Return Value:
	int index		- Table TempIndex index
*******************************************************************************/
int temp2index(double btx_t)		//look up index based on Temp input (1 degree F inaccuracy)
{
	int i;

	for(i=0;i<181;i++)
	{
		if((btx_t > TempIndex[i])&&(btx_t < TempIndex[i+1])) return i;
	}
	return i;
}

double overshootlookup(double valleybottom)
{
	int i;
	for(i=0;i<100;i++)
	{
		if((valleybottom < valleybottomlimit[i])&&(valleybottom > valleybottomlimit[i+1]))
		{
			return overshoot[i];
		}
	}
	return overshoot[i-1];
}
//*****************************************************************************
//		Flag Relay Fault
/******************************************************************************
	Summary:
	This routine exams the current sensing at the switch side of the relay. If
	a positive current flow of more than 50mA is detected while the relay is
	turned off, then this routine will set the fault flag to indicate the relay
	contacts might be stuck. (errors are not latching)

	Precondition:
	None

	Parameter:
	double curr		- Current flow out of the relay switch side
	unsigned int pwrmask	- mask to determine which relay we are examing
	unsigned int fltmask	- fault flag mask so the correct flag can be goggled

	Return Value:
	None
*******************************************************************************/
void flagRelayFault(double curr, double volt, unsigned int pwrmask, unsigned int fltmask)
{
	const double delta_v = 2.0;				//The voltage difference between sensing voltage, volt, and +24V Battery Bus voltage (in V)
	const double delta_i = 10.0;			//The current threshold, less than this, no current flow. (in mA)

	if(device_state(pwrmask) == 0)		//if the power source relay is off
	{
		if(curr > delta_i)		//Power Source Input relay is off yet there is still current input (> 10mA)
		{
			error_flag_3 |= fltmask;	//raise error flag for relay fault
		}
		else
		{
			error_flag_3 &= ~fltmask;	//clear error flag for relay fault
		}
	}
	else								//if the power source relay is on
	{
		if((bp_v + delta_v < volt)&&(curr < delta_i))	//if the power source input voltage is high than +24V and there is no current flowing
		{									//then the fuse is blown. flag it and shut off the input power source relay regardless of override state
			//shut off the relay per pwrmask
			//rlydrive(pwrmask,0);  //PSC 2.0 removed so relays are not doing anything active jreine
			//flag it use fltmask on error flag group 4 for PV and WT, Battery Fuse is not checked here
			error_flag_4 |= (fltmask>>4);	//using the relay fault flag to get fuse blown flag
		}
	}
}

//double max(double t1, double t2)
//{
//	if(t1>=t2) return t1;
//	return t2;
//}

double batterystringvoltage(double v1, double v2)
{
	if(v1 == -1.00) return v2;
	if(v2 == -1.00) return v1;
	return ((v1+v2)/2.0);
}

void resetcoulombcnts()
{
	intcc1 = 0;
	intcc2 = 0;
	intcc3 = 0;
	intcc4 = 0;
	fracc1 = 0.0;
	fracc2 = 0.0;
	fracc3 = 0.0;
	fracc4 = 0.0;
	ValleyBottom = 0.0;
}

void cntstore(int regaddr, double intvalue, double fravalue)
{
	int cntbyte5, cntbyte4, cntbyte3, cntbyte2, cntbyte1, cntbyte;
	char cntvalue[5];

	if(intvalue < 0)
	{
		intvalue *=(-1);
		fravalue *=(-1);
		cntbyte5 = intvalue/100000 + 80;
	}
	else
	{
		cntbyte5 = intvalue/100000;
	}
	cntbyte4 = ((long)intvalue %100000)/1000;
	cntbyte3 = ((long)intvalue %1000)/10;
	cntbyte2 =  ((long)intvalue % 10) * 10;
	cntbyte = (fravalue*1000) / 100;
	cntbyte1 = fmod((fravalue*1000),100);
	cntbyte2 += cntbyte;

	sprintf(cntvalue, "%c%c%c%c%c", dec2char(cntbyte5),dec2char(cntbyte4),dec2char(cntbyte3),dec2char(cntbyte2),dec2char(cntbyte1));
	i2c_putstat1(EEPROMi2c_CH,EEPROM_SADDR,regaddr,(unsigned char *)cntvalue,5);
	__delay_ms(5);
}

void ClrResetFlag()
{
	error_flag_3 &= ~PSCRESETFLG;
	error_flag_3 &= ~PSCPORSTFLG;
}

char cvltoi2cdata(char *input)
{
	char i2c_data;

	if(strlen(input)==2)
	{
		i2c_data=hex2dec(input[0])*16 + hex2dec(input[1]);
	}
	else
	{
		i2c_data=hex2dec(input[0]);
	}
	return i2c_data;
}
