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
* MPSC Rework:
* REV 1 Dan B/Paul W    10/19/2026          REV01 version for New Mini-PSC CCA
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This routine collects all the useful tools
**********************************************************************/
#include "utility.h"

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

			status = MasterI2C3gets(length, pRead, 2000); //read x length of bytes
			if(status)
			{
				CloseI2C3();
				return status; 			//report error
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
	extern double BATT_TEMP_LMT; //, CVTR_TEMP_LMT;

	index = (get_data1(i2cdata, baseaddress, 1)-180)/10;		//index of batt_temp table.
    //DMB: Not sure why they don't just do a floating point calculation instead of a lookup, but 
    //when the sensor is disconnected the output is -9.31 which could still be considered a valid temperature,
    // so substituting -100 instead so it is absolutely clear
	if(index<0) result = (batt_temp[0]); //Highest temperature
	//else if(index>392) result = (batt_temp[392]); //DMB: Lowest temperature (when sensor is unplugged)
    else if(index>=391) result = -100; //DMB: substitute -100 so it is clear that temp probe is disconnected
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

double batterystringvoltage(double v1, double v2)
{
	if(v1 == -1.00) return v2;
	if(v2 == -1.00) return v1;
	return ((v1+v2)/2.0);
}
