

#ifndef YUENJEE_H__
#define YUENJEE_H__

#include "stm32f10x.h"


//-------------------------------------------------------------------------------------------------------
// Flash store area

#define	  SN_ADDR  	0x0801E000
#define   ID_ADDR   0x0801E400 
#define   TR_ADDR   0x0801F000 
#define ADC1_Address    ((u32)0x4001244C)


//-------------------------------------------------------------------------------------------------------
typedef unsigned char       BOOL;
typedef unsigned char       bool;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
// Unsigned numbers
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned long       UINT32;

// Signed numbers
typedef signed char         INT8;
typedef signed short        INT16;
typedef signed long         INT32;

typedef uint64_t    		u64;
typedef uint64_t 		    UINT64;

//-------------------------------------------------------------------------------------------------------
// Common values

#define BACK_SUCCESS       0   /*³É¹¦*/
#define BACK_ERROR         1   /*´íÎó*/
#define BACK_TIMEOUT       2   /*³¬Ê±*/


#define WORK_MODE      	0x11   
#define CHECK_MODE      0x12  
#define ADVER_MODE      0x13   


#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef NULL
    #define NULL 0
#endif

#ifndef MSB
    #define MSB 0x80
#endif
//-------------------------------------------------------------------------------------------------------

typedef enum SYS_STATUS_
{
	System_OK = 0,
	Sensor_OK = 0x10,
  Sensor_Lost = 0x12,
	Sensor_Write_Error = 0x14,
	Sensor_Read_Error = 0x16,
	Sensor_Data_Error = 0x18,
	Flash_OK = 0x20,
	Flash_Erase_Error = 0x22,
	Flash_Write_Error = 0x24,
	Flash_Read_Error = 0x26,
	
	Power_Get_Error = 0x32,
}SYS_STATUS;

#endif
