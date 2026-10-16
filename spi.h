#ifndef __SPI_H
#define __SPI_H

/******************************************************************************
 *
 *                  SPI PERIPHERAL LIBRARY HEADER FILE
 *
 ******************************************************************************
 * FileName:        spi.h
 * Dependencies:    See include below
 * Processor:       PIC24
 * Compiler:        MPLAB C30
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *****************************************************************************/

#include "PIC24F_periph_features.h"

//This preprocessor conditional statement is to avoid unintended linking for unsuppported devices.
#if defined (spi_v1_1) || defined (spi_v1_2) || defined (spi_v1_3) || defined (LIB_BUILD) 


#define SPI1STAT_VALUE         0x0000
#define SPI2STAT_VALUE         0x0000
#define SPI3STAT_VALUE         0x0000

#define SPI1BUF_VALUE          0x0000
#define SPI2BUF_VALUE          0x0000
#define SPI3BUF_VALUE          0x0000

#define SPI1CON1_VALUE         0x0000
#define SPI2CON1_VALUE         0x0000
#define SPI3CON1_VALUE         0x0000

#define SPI1CON2_VALUE         0x0000
#define SPI2CON2_VALUE         0x0000
#define SPI3CON2_VALUE         0x0000


#ifndef USE_AND_OR /* Format for AND_OR based bit setting */

/* SPIxCON REGISTER */
#define  DISABLE_SCK_PIN     	0xffff /* Internal SPI clock is diabled, pin functions as I/O */
#define  ENABLE_SCK_PIN      	0xefff /*Internal SPI clock is enabled */

#define  DISABLE_SDO_PIN        0xffff /* SDO pin is not used by module   */
#define  ENABLE_SDO_PIN         0xf7ff /* SDO pin is  used by module      */

#define  SPI_MODE16_ON          0xffff /* Communication is word wide      */
#define  SPI_MODE16_OFF         0xfbff /* Communication is byte wide      */
#define  SPI_MODE8_ON           SPI_MODE16_OFF

#define  SPI_SMP_ON             0xffff /* Input data sampled at end of data output time */
#define  SPI_SMP_OFF            0xfdff /* Input data sampled at middle of data output time */

#define  SPI_CKE_ON             0xffff /* Transmit happens from active clock state to idle clock state*/
#define  SPI_CKE_OFF            0xfeff /* Transmit happens on transition from idle clock state to active clock state */

#define  SLAVE_ENABLE_ON        0xffff /* Slave Select enbale               */
#define  SLAVE_ENABLE_OFF       0xff7f /* Slave Select not used by module   */

#define  CLK_POL_ACTIVE_LOW     0xffff /* Idle state for clock is high, active is low */
#define  CLK_POL_ACTIVE_HIGH    0xffbf /* Idle state for clock is low, active is high */

#define  MASTER_ENABLE_ON       0xffff /* Master Mode              */
#define  MASTER_ENABLE_OFF      0xffdf /* Slave Mode               */

#define  SEC_PRESCAL_1_1        0xffff /* Secondary Prescale 1:1   */
#define  SEC_PRESCAL_2_1        0xfffb /* Secondary Prescale 2:1   */
#define  SEC_PRESCAL_3_1        0xfff7 /* Secondary Prescale 3:1   */
#define  SEC_PRESCAL_4_1        0xfff3 /* Secondary Prescale 4:1   */
#define  SEC_PRESCAL_5_1        0xffef /* Secondary Prescale 5:1   */
#define  SEC_PRESCAL_6_1        0xffeb /* Secondary Prescale 6:1   */
#define  SEC_PRESCAL_7_1        0xffe7 /* Secondary Prescale 7:1   */
#define  SEC_PRESCAL_8_1        0xffe3 /* Secondary Prescale 8:1   */

#define  PRI_PRESCAL_1_1        0xffff /* Primary Prescale 1:1     */
#define  PRI_PRESCAL_4_1        0xfffe /* Primary Prescale 4:1     */
#define  PRI_PRESCAL_16_1       0xfffd /* Primary Prescale 16:1    */
#define  PRI_PRESCAL_64_1       0xfffc /* Primary Prescale 64:1    */

/*SPIXCON2 REGISTER */
#define  FRAME_ENABLE_ON        0xffff /* Frame SPI support enable        */
#define  FRAME_ENABLE_OFF       0x7fff /* Frame SPI support Disable       */

#define  FRAME_SYNC_INPUT       0xffff /* Frame sync pulse Input (slave)  */
#define  FRAME_SYNC_OUTPUT      0xbfff /* Frame sync pulse Output (master)*/

#define  FRAME_POL_ACTIVE_HIGH  0xffff /* Frame sync pulse is active-high*/
#define  FRAME_POL_ACTIVE_LOW   0xdfff /* Frame sync pulse is active-low */

#define  FRAME_SYNC_EDGE_COINCIDE 0xffff /* Frame sync pulse coincides with first bit clock */
#define  FRAME_SYNC_EDGE_PRECEDE  0xfffd /* Frame sync pulse precedes first bit clock */

#define  FIFO_BUFFER_ENABLE  	0xffff /* FIFO buffer enabled */
#define  FIFO_BUFFER_DISABLE 	0xfffe /* FIFO buffer enabled */

/* SPIxSTAT REGISTER */
#define  SPI_ENABLE             0xffff /* Enable module */
#define  SPI_DISABLE            0x7fff /* Disable module */

#define  SPI_IDLE_CON           0xdfff /* Continue module operation in idle mode */
#define  SPI_IDLE_STOP          0xffff /* Discontinue module operation in idle mode */

#define  SPI_RX_OVFLOW_CLR      0xffbf /* Clear receive overflow bit.*/

#define FIFO_BUF_LEN_1      	0xfff8 /* FIFO buffer length 1 words */
#define FIFO_BUF_LEN_2      	0xfff9 /* FIFO buffer length 2 words */
#define FIFO_BUF_LEN_3      	0xfffa /* FIFO buffer length 3 words */
#define FIFO_BUF_LEN_4      	0xfffb /* FIFO buffer length 4 words */
#define FIFO_BUF_LEN_5      	0xfffc /* FIFO buffer length 5 words */
#define FIFO_BUF_LEN_6      	0xfffd /* FIFO buffer length 6 words */
#define FIFO_BUF_LEN_7      	0xfffe /* FIFO buffer length 7 words */
#define FIFO_BUF_LEN_8      	0xffff /* FIFO buffer length 8 words */

#define  SPI_INT_EN             0xffff /* SPI Interrupt Enable     */
#define  SPI_INT_DIS            0xfff7 /* SPI Interrupt Disable    */

#define  SPI_INT_PRI_0          0xfff8 /* SPI Interrupt Prior Level_0 */
#define  SPI_INT_PRI_1          0xfff9 /* SPI Interrupt Prior Level_1 */
#define  SPI_INT_PRI_2          0xfffa /* SPI Interrupt Prior Level_2 */
#define  SPI_INT_PRI_3          0xfffb /* SPI Interrupt Prior Level_3 */
#define  SPI_INT_PRI_4          0xfffc /* SPI Interrupt Prior Level_4 */
#define  SPI_INT_PRI_5          0xfffd /* SPI Interrupt Prior Level_5 */
#define  SPI_INT_PRI_6          0xfffe /* SPI Interrupt Prior Level_6 */
#define  SPI_INT_PRI_7          0xffff /* SPI Interrupt Prior Level_7 */

#else /* Format for backward compatibility (AND based bit setting). */

/* SPIXCON1: SPIx CONTROL REGISTER 1 */
#define  DISABLE_SCK_PIN        0x1000 /* SCK pin is not used by module */
#define  ENABLE_SCK_PIN         0x0000 /* SCK pin is  used by module */
#define  SCK_PIN_MASK           (~DISABLE_SCK_PIN)

#define  DISABLE_SDO_PIN        0x0800 /* SDO pin is not used by module */
#define  ENABLE_SDO_PIN         0x0000 /* SDO pin is  used by module */
#define  SDO_PIN_MASK           (~DISABLE_SDO_PIN)

#define  SPI_MODE16_ON          0x0400 /* Communication is word wide */
#define  SPI_MODE8_ON           0x0000 /* Communication is byte wide */
#define  SPI_MODE16_OFF         SPI_MODE8_ON  
#define  SPI_MODE_MASK          (~SPI_MODE16_ON)

#define  SPI_SMP_ON             0x0200 /* Input data sampled at end of data output time */
#define  SPI_SMP_OFF            0x0000 /* Input data sampled at middle of data output time */
#define  SPI_SMP_MASK           (~SPI_SMP_ON)

#define  SPI_CKE_ON             0x0100 /* Transmit happens from active clock state to idle clock state*/
#define  SPI_CKE_OFF            0x0000 /* Transmit happens on transition from idle clock state to active clock state */
#define  SPI_CKE_MASK           (~SPI_CKE_ON)

#define  SLAVE_ENABLE_ON        0x0080 /* Slave Select enbale */
#define  SLAVE_ENABLE_OFF       0x0000 /* Slave Select not used by module */
#define  SLAVE_ENABLE_MASK      (~SLAVE_ENABLE_ON)

#define  CLK_POL_ACTIVE_LOW     0x0040 /* Idle state for clock is high, active is low */
#define  CLK_POL_ACTIVE_HIGH    0x0000 /* Idle state for clock is low, active is high */
#define  CLK_POL_ACTIVE_MASK    (~CLK_POL_ACTIVE_LOW)

#define  MASTER_ENABLE_ON       0x0020 /* Master Mode */
#define  MASTER_ENABLE_OFF      0x0000 /* Slave Mode */
#define  MASTER_ENABLE_MASK     (~MASTER_ENABLE_ON)

#define  SEC_PRESCAL_1_1        0x001c /* Secondary Prescale 1:1   */
#define  SEC_PRESCAL_2_1        0x0018 /* Secondary Prescale 2:1   */
#define  SEC_PRESCAL_3_1        0x0014 /* Secondary Prescale 3:1   */
#define  SEC_PRESCAL_4_1        0x0010 /* Secondary Prescale 4:1   */
#define  SEC_PRESCAL_5_1        0x000c /* Secondary Prescale 5:1   */
#define  SEC_PRESCAL_6_1        0x0008 /* Secondary Prescale 6:1   */
#define  SEC_PRESCAL_7_1        0x0004 /* Secondary Prescale 7:1   */
#define  SEC_PRESCAL_8_1        0x0000 /* Secondary Prescale 8:1   */
#define  SEC_PRESCAL_MASK       (~SEC_PRESCAL_1_1)

#define  PRI_PRESCAL_1_1        0x0003 /* Primary Prescale 1:1     */
#define  PRI_PRESCAL_4_1        0x0002 /* Primary Prescale 4:1     */
#define  PRI_PRESCAL_16_1       0x0001 /* Primary Prescale 16:1    */
#define  PRI_PRESCAL_64_1       0x0000 /* Primary Prescale 64:1    */
#define  PRI_PRESCAL_MASK       (~PRI_PRESCAL_1_1)

/* SPIxSTAT REGISTER */
#define  SPI_ENABLE             0x8000 /* Enable module */
#define  SPI_DISABLE            0x0000 /* Disable module */
#define  SPI_ENBL_DSBL_MASK     (~SPI_ENABLE)

#define  SPI_IDLE_STOP          0x2000 /* Discontinue module operation in idle mode */
#define  SPI_IDLE_CON           0x0000 /* Continue module operation in idle mode */
#define  SPI_IDLE_MASK          (~SPI_IDLE_STOP)

#define  SPI_RX_OVFLOW          0x0040 /* SPI receive overflow bit enabled */
#define  SPI_RX_OVFLOW_CLR      0x0000 /* Clear receive overflow bit */

/* SPIxCON2: SPIx CONTROL REGISTER 2 */
#define  FRAME_ENABLE_ON        0x8000 /* Frame SPI support enable */
#define  FRAME_ENABLE_OFF       0x0000 /* Frame SPI support Disable */
#define  FRAME_ENABLE_MASK      (~FRAME_ENABLE_ON)

#define  FRAME_SYNC_INPUT       0x4000 /* Frame sync pulse Input (slave)  */
#define  FRAME_SYNC_OUTPUT      0x0000 /* Frame sync pulse Output (master)*/
#define  FRAME_SYNC_MASK        (~FRAME_SYNC_INPUT)

#define  FRAME_SYNC_ACTIVE_HIGH 0x2000 /* Frame sync pulse Input (slave)  */
#define  FRAME_SYNC_ACTIVE_LOW  0x0000 /* Frame sync pulse Output (master)*/
#define  FRAME_SYNC_POL_MASK    (~FRAME_SYNC_ACTIVE_HIGH)

#define  SPI_FRM_PULSE_FIRST_CLK 0x0002 /* frame pulse coincides with the first bit clock */
#define  SPI_FRM_PULSE_PREV_CLK  0x0000 /* frame pulse precedes the first bit clock */
#define  SPI_FRM_PULSE_MASK     (~SPI_FRM_PULSE_FIRST_CLK)

#define  SPI_ENH_BUFF_ENABLE    0x0001 /* enable enhanced buffer */
#define  SPI_ENH_BUFF_DISABLE   0x0000 /* disable enhanced buffer */
#define  SPI_ENH_BUFF_MASK      (~SPI_ENH_BUFF_ENABLE)

/* SPI Interrupt defines */
#define  SPI_INT_EN             0x0008 /* SPI Interrupt Enable     */
#define  SPI_INT_DIS            0x0000 /* SPI Interrupt Disable    */
#define  SPI_INT_MASK           (~SPI_INT_EN)

#define  SPI_FAULT_INT_EN       0x0080 /* SPI Fault Interrupt Enable     */
#define  SPI_FAULT_INT_DIS      0x0000 /* SPI Fault Interrupt Disable    */
#define  SPI_FAULT_INT_MASK     (~SPI_FAULT_INT_EN)

#define  SPI_INT_PRI_0          0x0000 /* SPI Interrupt Prior Level_0 */
#define  SPI_INT_PRI_1          0x0001 /* SPI Interrupt Prior Level_1 */
#define  SPI_INT_PRI_2          0x0002 /* SPI Interrupt Prior Level_2 */
#define  SPI_INT_PRI_3          0x0003 /* SPI Interrupt Prior Level_3 */
#define  SPI_INT_PRI_4          0x0004 /* SPI Interrupt Prior Level_4 */
#define  SPI_INT_PRI_5          0x0005 /* SPI Interrupt Prior Level_5 */
#define  SPI_INT_PRI_6          0x0006 /* SPI Interrupt Prior Level_6 */
#define  SPI_INT_PRI_7          0x0007 /* SPI Interrupt Prior Level_7 */
#define  SPI_INT_PRI_MASK       (~SPI_INT_PRI_7)  

#define  SPI_FAULT_INT_PRI_0    0x0000 /* SPI Fault Interrupt Prior Level_0 */
#define  SPI_FAULT_INT_PRI_1    0x0010 /* SPI Fault Interrupt Prior Level_1 */
#define  SPI_FAULT_INT_PRI_2    0x0020 /* SPI Fault Interrupt Prior Level_2 */
#define  SPI_FAULT_INT_PRI_3    0x0030 /* SPI Fault Interrupt Prior Level_3 */
#define  SPI_FAULT_INT_PRI_4    0x0040 /* SPI Fault Interrupt Prior Level_4 */
#define  SPI_FAULT_INT_PRI_5    0x0050 /* SPI Fault Interrupt Prior Level_5 */
#define  SPI_FAULT_INT_PRI_6    0x0060 /* SPI Fault Interrupt Prior Level_6 */
#define  SPI_FAULT_INT_PRI_7    0x0070 /* SPI Fault Interrupt Prior Level_7 */
#define  SPI_FAULT_INT_PRI_MASK (~SPI_FAULT_INT_PRI_7)

#endif /* USE_AND_OR */

#if defined (spi_v1_1) || defined (spi_v1_2) || defined (spi_v1_3)  || defined (LIB_BUILD)
/**************************************************************************
Macro       : EnableIntSPI1
 
Description : Enables SPIx Transfer Complete Interrupt 
 
Arguments   : None
 
Remarks     : This macro sets SPI Interrupt Enable bit of Interrupt Enable 
              Control register.
***************************************************************************/ 
#define EnableIntSPI1                   (IEC0bits.SPI1IE = 1)

/**************************************************************************
Macro       : DisableIntSPI1

Description : This macro disables the SPI Interrupt
 
Arguments   : None
 
Remarks     : This macro clears SPI Interrupt Enable bit of Interrupt Enable 
              Control register.
***************************************************************************/ 
#define DisableIntSPI1                  (IEC0bits.SPI1IE = 0)

/**************************************************************************
Macro       : SetPriorityIntSPI1(priority)
 
Description : This macro sets the priority level for SPI interrupt
 
Arguments   : Priority - This input parameter is the level of interrupt priority
 
Remarks     : None 
***************************************************************************/
#define SetPriorityIntSPI1(priority)     (IPC2bits.SPI1IP = priority)

/*******************************************************************
Macro       : SPI1_Clear_Intr_Status_Bit

Description : Macro to Clear SPI Interrupt Status bit 

Arguments   : None 

Remarks     : None 
*******************************************************************/
#define SPI1_Clear_Intr_Status_Bit     (IFS0bits.SPI1IF = 0)

/**************************************************************************
Macro       : getcSPI1

Description : macro is identical to ReadSPI1,#define to ReadSPI1 in spi.h
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  getcSPI1   ReadSPI1
 
/**************************************************************************
Macro       : putcSPI1

Description : macro is identical to WriteSPI1,#define to WriteSPI1 in spi.h
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  putcSPI1   WriteSPI1
 
 
/**************************************************************************
Macro       : SPI1_Clear_Recv_OV

Description : macro clears Receive Overflow flag
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  SPI1_Clear_Recv_OV    SPI1STATbits.SPIROV = 0
 
/**************************************************************************
Macro       : SPI1_Tx_Buf_Full

Description : macro reads the status of Transmit Buffer.
              Returns 1 - Indicates buffer is full , Transmit is not yet stated
                      0 - Indicates buffer is empty , Transmit is complete
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  SPI1_Tx_Buf_Full    SPI1STATbits.SPITBF
 
 
/**************************************************************************
Macro       : SPI1_Rx_Buf_Full

Description : macro reads the status of Receive Buffer.
              Returns 1 - Indicates Receive complete
                      0 - Indicates not yet received 
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  SPI1_Rx_Buf_Full    SPI1STATbits.SPIRBF

  
/* CloseSPI. Disables SPI module */
    void __attribute__ ((section (".libperi"))) CloseSPI1() ;

/* ConfigINtSPI1. Configure Interrupt enable and priorities */
    void __attribute__ ((section(".libperi"))) ConfigIntSPI1(unsigned int config) ;

/* DataRdySPI */
    char __attribute__ ((section (".libperi"))) DataRdySPI1();   

/* getsSPI.Write string to SPIBUF */
    unsigned int __attribute__ ((section (".libperi")))getsSPI1(unsigned int length, unsigned int *rdptr,unsigned int spi_data_wait);

/* OpenSPI */
    void __attribute__ ((section (".libperi"))) OpenSPI1(unsigned int config1,unsigned int config2,unsigned int config3 );   

/* putsSPI Read string from SPIBUF */
    void __attribute__ ((section (".libperi"))) putsSPI1(unsigned int length, unsigned int *wrptr);

/* ReadSPI.Read byte/word from SPIBUF register */
    unsigned int __attribute__ ((section (".libperi"))) ReadSPI1();

/* WriteSPI. Write byte/word to SPIBUF register */
    void __attribute__ ((section (".libperi"))) WriteSPI1(unsigned int data_out);

#endif /* _SPI_1 */


#if defined (spi_v1_2) || defined (spi_v1_3) || defined (LIB_BUILD)
/**************************************************************************
Macro       : EnableIntSPI2

Description : Enables SPIx Transfer Complete Interrupt 
 
Arguments   : None
 
Remarks     : This macro sets SPI Interrupt Enable bit of Interrupt Enable 
              Control register.
***************************************************************************/ 
#define EnableIntSPI2                    (IEC2bits.SPI2IE = 1)

/**************************************************************************
Macro       : DisableIntSPI2

Description : This macro disables the SPI Interrupt
 
Arguments   : None
 
Remarks     : This macro clears SPI Interrupt Enable bit of Interrupt Enable 
              Control register.
***************************************************************************/ 
#define DisableIntSPI2                  (IEC2bits.SPI2IE = 0)

/**************************************************************************
Macro       : SetPriorityIntSPI2(priority)

Description : This macro sets the priority level for SPI interrupt
 
Arguments   : Priority - This input parameter is the level of interrupt priority
 
Remarks     : None 
***************************************************************************/
#define SetPriorityIntSPI2(priority)     (IPC8bits.SPI2IP = priority)

/*******************************************************************
Macro       : SPI2_Clear_Intr_Status_Bit

Description : Macro to Clear external Interrupt Status bit 

Arguments   : None 

Remarks     : None 
*******************************************************************/
#define SPI2_Clear_Intr_Status_Bit     (IFS2bits.SPI2IF = 0)

/**************************************************************************
Macro       : getcSPI2

Description : macro is identical to ReadSPI2,#define to ReadSPI2 in spi.h
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
#define  getcSPI2   ReadSPI2

/**************************************************************************
Macro       : putcSPI2

Description : macro is identical to WriteSPI2,#define to WriteSPI2 in spi.h
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
#define  putcSPI2    WriteSPI2

/**************************************************************************
Macro       : SPI2_Clear_Recv_OV

Description : macro clears Receive Overflow flag
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  SPI2_Clear_Recv_OV    SPI2STATbits.SPIROV = 0
 
/**************************************************************************
Macro       : SPI2_Tx_Buf_Full

Description : macro reads the status of Transmit Buffer.
              Returns 1 - Indicates buffer is full , Transmit is not yet stated
                      0 - Indicates buffer is empty , Transmit is complete
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  SPI2_Tx_Buf_Full    SPI2STATbits.SPITBF
 
 
/**************************************************************************
Macro       : SPI2_Rx_Buf_Full
 
Description : macro reads the status of Receive Buffer.
              Returns 1 - Indicates Receive complete
                      0 - Indicates not yet received 
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  SPI2_Rx_Buf_Full    SPI2STATbits.SPIRBF


/* CloseSPI2.Disables SPI module */
    void  __attribute__ ((section (".libperi")))  CloseSPI2();

/* ConfigINtSPI2. Configures Interrupt enable and priorities */
    void  __attribute__ ((section(".libperi"))) ConfigIntSPI2(unsigned int config);

/* OpenSPI */
    void __attribute__ ((section(".libperi"))) OpenSPI2(unsigned int config1,unsigned int config2, unsigned int config3 );

/* DataRdySPI. Test if SPIBUF register is full */
    char  __attribute__ ((section (".libperi"))) DataRdySPI2();

/* getsSPI.Write string to SPIBUF */
    unsigned int __attribute__ ((section(".libperi"))) getsSPI2(unsigned int length, unsigned int *rdptr, unsigned int spi_data_wait);   

/* putsSPI. Read string from SPIBUF */
    void __attribute__ ((section(".libperi"))) putsSPI2(unsigned int length, unsigned int *wrptr);

/* ReadSPI.Read byte/word from SPIBUF register */
    unsigned int __attribute__ ((section (".libperi"))) ReadSPI2();

/* WriteSPI. Write byte/word to SPIBUF register */
    void __attribute__ ((section(".libperi"))) WriteSPI2( unsigned int data_out);
#endif /* _SPI_2 */


#if defined (spi_v1_3) || defined (LIB_BUILD)
/**************************************************************************
Macro       : EnableIntSPI3

Description : Enables SPIx Transfer Complete Interrupt 
 
Arguments   : None
 
Remarks     : This macro sets SPI Interrupt Enable bit of Interrupt Enable 
              Control register.
***************************************************************************/ 
#define EnableIntSPI3                    (IEC5bits.SPI3IE = 1)

/**************************************************************************
Macro       : DisableIntSPI3

Description : This macro disables the SPI Interrupt
 
Arguments   : None
 
Remarks     : This macro clears SPI Interrupt Enable bit of Interrupt Enable 
              Control register.
***************************************************************************/ 
#define DisableIntSPI3                  (IEC5bits.SPI3IE = 0)

/**************************************************************************
Macro       : SetPriorityIntSPI3(priority)

Description : This macro sets the priority level for SPI interrupt
 
Arguments   : Priority - This input parameter is the level of interrupt priority
 
Remarks     : None 
***************************************************************************/
#define SetPriorityIntSPI3(priority)     (IPC22bits.SPI3IP = priority)

/*******************************************************************
Macro       : SPI3_Clear_Intr_Status_Bit

Description : Macro to Clear external Interrupt Status bit 

Arguments   : None 

Remarks     : None 
*******************************************************************/
#define SPI3_Clear_Intr_Status_Bit     (IFS5bits.SPI3IF = 0)

/**************************************************************************
Macro       : getcSPI3

Description : macro is identical to ReadSPI3,#define to ReadSPI3 in spi.h
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
#define  getcSPI3    ReadSPI3

/**************************************************************************
Macro       : putcSPI3

Description : macro is identical to WriteSPI3,#define to WriteSPI3 in spi.h
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
#define  putcSPI3    WriteSPI3

/**************************************************************************
Macro       : SPI3_Clear_Recv_OV

Description : macro clears Receive Overflow flag
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  SPI3_Clear_Recv_OV    SPI3STATbits.SPIROV = 0
 
/**************************************************************************
Macro       : SPI3_Tx_Buf_Full
 
Description : macro reads the status of Transmit Buffer.
              Returns 1 - Indicates buffer is full , Transmit is not yet stated
                      0 - Indicates buffer is empty , Transmit is complete
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  SPI3_Tx_Buf_Full    SPI3STATbits.SPITBF 
 
/**************************************************************************
Macro       : SPI3_Rx_Buf_Full

Description : macro reads the status of Receive Buffer.
              Returns 1 - Indicates Receive complete
                      0 - Indicates not yet received 
 
Arguments   : None
 
Remarks     : None 
***************************************************************************/
 #define  SPI3_Rx_Buf_Full    SPI3STATbits.SPIRBF

     
/* CloseSPI3.Disables SPI module */
    void  __attribute__ ((section (".libperi")))  CloseSPI3();

/* ConfigINtSPI3. Configures Interrupt enable and priorities */
    void  __attribute__ ((section(".libperi"))) ConfigIntSPI3(unsigned int config);

/* OpenSPI */
    void __attribute__ ((section(".libperi"))) OpenSPI3(unsigned int config1,unsigned int config2, unsigned int config3 );

/* DataRdySPI. Test if SPIBUF register is full */
    char  __attribute__ ((section (".libperi"))) DataRdySPI3();

/* getsSPI.Write string to SPIBUF */
    unsigned int __attribute__ ((section(".libperi"))) getsSPI3(unsigned int length, unsigned int *rdptr, unsigned int spi_data_wait);   

/* putsSPI. Read string from SPIBUF */
    void __attribute__ ((section(".libperi"))) putsSPI3(unsigned int length, unsigned int *wrptr);

/* ReadSPI.Read byte/word from SPIBUF register */
    unsigned int __attribute__ ((section (".libperi"))) ReadSPI3();

/* WriteSPI. Write byte/word to SPIBUF register */
    void __attribute__ ((section(".libperi"))) WriteSPI3( unsigned int data_out);

//#else
//#warning "Selected device not supports this function"
#endif

#else		//This preprocessor conditional statement is to avoid unintended linking for unsuppported devices.
#warning "Does not build on this target"
#endif

#endif  /* __SPI_H */
