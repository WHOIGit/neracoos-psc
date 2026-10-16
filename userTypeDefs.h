/***********************************************************************
* 	WHOI/Raytheon, 2010
************************************************************************
* FileName:			userTypeDefs.h
* Dependencies:
* Processor:       	PIC24FJ256GB106
* Compiler:			MPLAB C30 V3.24 or higher
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Rev. 	Author        	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 1		Joseph Zhu    	10/28/2010		Initial File Creation 
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES
* This file consists of all user defined structure parameters which shared 
* with all modules
*
*************************************************************************/
typedef struct 
{
    WORD        vid;
    WORD        pid;
} THUMB_DRIVE_ID;
    
typedef struct _VOLUME_INFO
{
    char        label[12];
    BYTE        valid;
} VOLUME_INFO;

typedef struct _LOGGER_STATUS
{
    #if defined(__C30__)
        union
        {
            BYTE    value;
            struct
            {    
                BYTE        mediaPresent            : 1;
                BYTE        cannotInitialize        : 1;
                BYTE        overcurrentStateUSB     : 1;
                BYTE        readingPotentiometer    : 1;
                BYTE        readingTemperature      : 1;
            };
        };
    #endif
} LOGGER_STATUS;

