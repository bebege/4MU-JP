/* ###################################################################
**     Filename    : main.c
**     Project     : KEAZ128
**     Processor   : SKEAZ128MLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-02-23, 12:00, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**tyutiiiiiituituitttttttttttttttttttttiiiiiiiii
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "CI2C1.h"
#include "Bit3.h"
#include "Bits1.h"
#include "AS1.h"
#include "TU3.h"
#include "CRC1.h"
#include "RS485EN.h"
#include "RS232.h"
#include "GPIO1.h"
#include "GPIO2.h"
#include "Bit1.h"
#include "Bit2.h"
#include "TI1.h"
#include "TU2.h"
#include "TU1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
//==============================================================================


/*int 4 bytes , short 2 bytes (+/-32767) , char=bool 1 byte, 

unsigned short int x = 0x9527;
// �Q�Υk�� 8 bits ���� low byte:
unsigned short int hb = x >> 8;  (high byte of 9527 = 95)
//TXD_Buf[3]=data>>8;		//high byte (choose high byte)
// �Q�ξB�n�L�o�� high byte ������:
unsigned short int lb = x & 0x00ff; (low byte of 9527 = 27)
// TXD_Buf[2]=data%256;	//low byte (choose low byte)
//unsigned char(1 byte)  a = 205;       // �Y 1100 1101, �۷�� a = 0xcd


*/

LDD_TError Error;
LDD_TDeviceData *ddBitsIO,*MyGPIO1Ptr, *MyGPIO2Ptr,*MyGPIO3Ptr,*MyTU1Ptr,*MyI2CPtr,*MyI2C2Ptr,
*bit1Ptr, *bit2Ptr,*bit3Ptr,*MyTI1Ptr, *bit1Ptr, *bit2Ptr,*MySerialPtr,*MyTU3Ptr;
LDD_TDeviceData *DeviceData=NULL ,*RS485ENPtr;

LDD_I2C_TBusState * I2CBusStatePtr;

 
  

volatile unsigned char q=0,id=1;
volatile unsigned char Count_1=0,Count_2=0,Count_3=0,Count_4=0,Count_6=0;
volatile unsigned short Count_0=0, Count_5=0; 


uint8_t OutData[4] = {0x00U, 0x01U, 0x02U, 0x03U};                /* Initialization of output data buffer */
uint8_t InpData[17];
uint8_t TXD_Buf[13];
uint8_t Send_Buf[15];
char xxxOutData[] = "Hello world";



uint8_t OutData2[32]={0x11U, 0x31U} ;             
uint8_t InpData2[2]= {0x00U, 0x01U};

volatile bool IIC_ReceivedFlg = FALSE;
volatile bool IIC_TransmittedFlg = FALSE;
volatile bool IIC2_TransmittedFlg = FALSE,IIC2_ReceivedFlg = FALSE;
volatile bool UART_TransmittedFlg= FALSE;
volatile bool UART_ReceivedFlg= FALSE;

volatile bool RS232_TransmittedFlg=0,RS232_ReceivedFlg=0;

volatile bool I2C_flag=0,alert_flag=0,Alert_LED_Flag=0,fetch_Ready_Flag=0,I2C_fault_Flag=0;
volatile bool UV_flag=0, OV_flag=0,SOC_low_flag=0,OCC_flag=0,OCD_flag=0,OT_flag=0,UT_flag=0;
volatile bool BMU_fault_Flag= 0 , Alarm_Flag=0, SUM_Safety_Flag=0;
volatile bool Init_fault_Flag= 0,recover_flag=0;


volatile bool I2C_statu, RS485_S_F=0;

#define  busy 1
#define idle 0

unsigned char  BMUID =	0;
unsigned char  BMU_NUMBERS=14;        //===================
unsigned char  SUMBMU_fault =0;
unsigned short  BMU_fault_Count[16];

unsigned short  Block_Volts_Data[16][8];
unsigned short  Block_minVolts_Data[16][1];
unsigned short  Block_maxVolts_Data[16][1];
unsigned short  Block_RSOC[11];   // 11 blocks

unsigned short  Buff_Data_V[5][2];
unsigned short  Buff_Data[16][8];

signed short  Block_Temp_Data[16][8];
signed short  Block_minTemp_Data[16][1];
signed short  Block_maxTemp_Data[16][1];
unsigned short  Block_Alert_Data[16][16];
unsigned short  Pack_maxVolt;
unsigned short  Pack_minVolt;
signed short  Pack_maxTemp;
signed short  Pack_minTemp;
signed short  Pack_Temp;

signed short  Module_Temp[16];

unsigned short Pack_Volt=0;
unsigned short P_Buff_Volt=0;

unsigned short  Pack_RSOC=1; 
unsigned short  Pack_ASOC=1; 
signed short  Pack_Current=1;
char IIC_OutData[4] = {0x02U, 0x01U, 0x02U, 0x03U};    /* Initialization of output data buffer */
char IIC_InpData[2];
char slaveaddress_data;

char Alarm[16][16];

	//Alarm[15] volatile short   OT1D ;   //THM1  support idle
	//Alarm[14]volatile short	OT1C;
		//[13] volatile short	OCD;
	//Alarm[12]volatile short	OCC;
			//volatile short	UTC;
			//volatile short	UTD;
			//volatile short	OT2D;
	 //Alarm[8]volatile short	OT2C;
			
	//Alarm[7]volatile short      CUV;
	//Alarm[6]volatile short	     COV;
			//volatile short	PF;
			//
			//
			//volatile short	AOCD;
			//volatile short	SCC;
	//Alarm[0] volatile short	SCD;

short  Safety;
		
  short CRC_Result=0;
/*
struct {
  byte Header;
  byte Command;
  word Data;
  byte Checksum;
} message;

  message.Header = 0x55;
  message.Command = 0x01;
  message.Data = 0x0A0B;
  message.Checksum = message.Header + message.Command + (message.Data & 0x00ff) + (message.Data >> 8);
*/

unsigned int Delay_ms=0;
 void Timer_Delay_ms(unsigned short delay)
 {
 	Delay_ms=delay;		//timer interrupt event trigger 100ms
 	while(Delay_ms){ }
 }
//=============================================
char BlockAddress( char ID)
//==================================== 
	{
	slaveaddress_data =0x10+ID;
	return slaveaddress_data;
	}

//=======================================================
short IIC_ReadWord(char SlaveAddress_7BITS,char CommandCode)
//=======================================================
{ 

		
    CI2C1_SelectSlaveDevice(MyI2CPtr, LDD_I2C_ADDRTYPE_7BITS,SlaveAddress_7BITS);
	
	IIC_OutData[0] = CommandCode;  //BQ34Z100 StateOfCharge                               /* Initialization of OutData buffer address */
	Error = CI2C1_MasterSendBlock(MyI2CPtr, &IIC_OutData, 1U, LDD_I2C_NO_SEND_STOP); /* Send OutData (1 byte) on the I2C bus stop condition and will not generate the end of the transmission */
	I2C_flag=1;           //start counter
	while (!IIC_TransmittedFlg) {                                   /* Wait until OutData are transmitted */
		CI2C1_Main(MyI2CPtr);
		I2C_flag=1;           //start counter
	}
	I2C_flag=0;
	Count_2=0;
	IIC_TransmittedFlg = FALSE;
	//---------------------------------------------------------------------------------------------------------
	char i=0;
	for(i=0;i<2;i++){ IIC_InpData[i]=0; }
	Error = CI2C1_MasterReceiveBlock(MyI2CPtr, &IIC_InpData, 2U, LDD_I2C_SEND_STOP); /* Receive InpData (2 bytes) from the I2C bus and generates a stop condition to end transmission */  
	
	I2C_flag=1;           //start counter
	while (!IIC_ReceivedFlg) {                                      /* Wait until InpData are received */
		CI2C1_Main(MyI2CPtr);
		I2C_flag=1;           //start counter
	}
	I2C_flag=0;
	Count_2=0;
	IIC_ReceivedFlg = FALSE;
		
	return (IIC_InpData[1]<<8|IIC_InpData[0]);  //return word data (low + high)

}

	
//========================================================================
void IIC_WriteWord(char SlaveAddress_7BITS,char CommandCode,unsigned short Data)
//========================================================================
{
	
	CI2C1_SelectSlaveDevice(MyI2CPtr, LDD_I2C_ADDRTYPE_7BITS,SlaveAddress_7BITS);
	OutData[0] = CommandCode;  //                               /* Initialization of OutData buffer */   
	OutData[1] = Data%256;  //                               /* Initialization of OutData buffer */   
	OutData[2] = Data>>8;  //                             /* Initialization of OutData buffer */   
	Error = CI2C1_MasterSendBlock(MyI2CPtr, &OutData, 3U, LDD_I2C_SEND_STOP); /* Send OutData (3 bytes) on the I2C bus stop condition and will not generate the end of the transmission */
	I2C_flag=1;           //start counter
	while (!IIC_TransmittedFlg) {                                   /* Wait until OutData are transmitted */
		CI2C1_Main(MyI2CPtr);
		I2C_flag=1;           //start counter
			}
	I2C_flag=0;
	Count_2=0;
	IIC_TransmittedFlg = FALSE;

	}

//===============================================================================
short  IIC_ReadBlock(char SlaveAddress_7BITS,char CommandCode)
//=========================================================================
{	
    CI2C1_SelectSlaveDevice(MyI2CPtr, LDD_I2C_ADDRTYPE_7BITS,SlaveAddress_7BITS);
	OutData[0] = CommandCode;  //                             /* Initialization of OutData buffer address */  
	//Error = TU1_Disable(MyTU1Ptr);  
	Error = CI2C1_MasterSendBlock(MyI2CPtr, &OutData, 1U, LDD_I2C_NO_SEND_STOP); /* Send OutData (1 byte) on the I2C bus stop condition and will not generate the end of the transmission */
	//Error = TU1_Enable(MyTU1Ptr);  
	I2C_flag=1;           //start counter
	while (!IIC_TransmittedFlg) {                                   /* Wait until OutData are transmitted */
	   CI2C1_Main(MyI2CPtr); 
	   I2C_flag=1;           //start counter
	}
		I2C_flag=0;
	Count_2=0;
	IIC_TransmittedFlg = FALSE;
	char i=0;
	for(i=0;i<17;i++){InpData[i]=0; }
	//---------------------------------------------------------------------------------------------------------
	Error = CI2C1_MasterReceiveBlock(MyI2CPtr, &InpData, 3U, LDD_I2C_SEND_STOP); /* Receive InpData (17 bytes) from the I2C bus and generates a stop condition to end transmission */  
	//Error = TU1_Enable(MyTU1Ptr);  
	I2C_flag=1;           //start counter
	while (!IIC_ReceivedFlg) 
	{                                      /* Wait until InpData are received */
		CI2C1_Main(MyI2CPtr);
		I2C_flag=1;           //start counter
			}
		I2C_flag=0;
	Count_2=0;
	IIC_ReceivedFlg = FALSE;
	//return (InpData[1]<<8|InpData[0]);  //return word data (low + high)
}

//================================================================
void balance(char BMUID,char map)
//================================================================
{
      //I2C_switch(0);
       MyI2CPtr = CI2C1_Init(NULL);
	//   	  Timer_Delay_ms(2);
   unsigned int tmp = ((unsigned int)map | 0x0500);  //0x0500 /on duration 5s/who	
	IIC_WriteWord( BlockAddress(BMUID),0xE6,tmp);
	//I2C_switch(1);
}


//================================================================
void get_Block_Volts(char BMUID)
//================================================================
{
   //I2C_switch(0);
	//BMU Request CMD (0xE0)
	//command data 0x0050 volts ; 0x0051 temperature (value-2731) ; 0x0052 status ;0x0053 gauge ;  
	// read register )0xE3; BMU Info Block (0xE3) 
       MyI2CPtr = CI2C1_Init(NULL);
IIC_WriteWord( BlockAddress(BMUID),0xE0,0x0050);	
Timer_Delay_ms(1);
IIC_ReadBlock(BlockAddress(BMUID),0xE3);

char i=0,j=1;	
					for( i=0;i<8 ;i=i+1)             //collect 8 words
					{						
					   Block_Volts_Data[BMUID][i]=InpData[j]<<8|InpData[j+1];
						j=j+2;					  				
					}

}
//===============================================================
void Find_max_min_Value(char BMUID)
//================================================================
{			/***
			Block_maxVolts_Data[BMUID][0]=Block_Volts_Data[BMUID][6];
			Block_minVolts_Data[BMUID][0]=Block_Volts_Data[BMUID][6];	
			char i=0;
								    for( i = 0; i < 8; i++){  //��X�̤j�� , 8 cells
							
								            if(Block_maxVolts_Data[BMUID][0] < Block_Volts_Data[BMUID][i]){
												if(Block_maxVolts_Data[BMUID][0]<6000)
								                      Block_maxVolts_Data[BMUID][0]= Block_Volts_Data[BMUID][i];
								            }    
								
										//��X�̤p�� , 8 cells
					  					if(Block_minVolts_Data[BMUID][0]  >Block_Volts_Data[BMUID][i] ){
											if( Block_Volts_Data[BMUID][i] >2000)
								                      Block_minVolts_Data[BMUID][0]  = Block_Volts_Data[BMUID][i];
								            }   
								    			
								    	}
									
				Block_maxTemp_Data[BMUID][0]=Block_Temp_Data[BMUID][0];  //
			   for( i = 1; i < 4; i++)
			   	{  //��X�̤j�� , 8 cells
			            if(   Block_maxTemp_Data[BMUID][0] < Block_Temp_Data[BMUID][i]    ){
			                      Block_maxTemp_Data[BMUID][0]= Block_Temp_Data[BMUID][i];
			            }       
			    }
				***/
		 
		//===find pack max value==============================

			  Pack_maxVolt= Block_Volts_Data[0][0] ;
			  Pack_minVolt=Block_Volts_Data[0][0];
				char  i=0;
			   for( i = 1; i < BMU_NUMBERS; i++)
			   	{  //��X�̤j�� 			   					   	
			            if(   Block_Volts_Data[i][0] >Pack_maxVolt  ) {
						if( Block_Volts_Data[i][0] < 8000)
					 Pack_maxVolt=Block_Volts_Data[i][0];			                 
			            	}
				  	  //��X�̤p�� 
			            if(  Block_Volts_Data[i][0] <Pack_minVolt     )  //fetch voltagesBlock_minVolts_Data[i][0] )
			            	{  if(Pack_minVolt>1000)
			                 Pack_minVolt  = Block_Volts_Data[i][0];
			            	}
			     }  
			    	
//==================temperature==========================
			i=0;
			   for( i = 0; i < BMU_NUMBERS; i++){  //��X�̤j�� 
			            if(Block_maxTemp_Data[i][0] >= Pack_maxTemp){
			                      Pack_maxTemp=Block_maxTemp_Data[i][0];
			            }       
			   
				  	  //��X�̤p�� 
			            if( Block_minTemp_Data[i][0]  <=Pack_minTemp   )  //fetch voltagesBlock_minVolts_Data[i][0] )
			            {
			            
			                 Pack_minTemp  = Block_minTemp_Data[i][0];
			            }    
			   	}

}
//================================================================
void get_Block_temp(char BMUID)
//================================================================
{
   //I2C_switch(0);
	//command data 0x0050 volts ; 0x0051 temperature ; 0x0052 alert ;  
	// read register )xE3;
MyI2CPtr = CI2C1_Init(NULL);
IIC_WriteWord( BlockAddress(BMUID),0xE0,0x0051);	
Timer_Delay_ms(1);
IIC_ReadBlock(BlockAddress(BMUID),0xE3);
char  i=0,j=1;

					for( i=0;i<4 ;i=i+1)             //collect 8 words, first ignore
						{	
						
						Buff_Data[BMUID][i]=(InpData[j]<<8|InpData[j+1]);  //
									j=j+2;
											 
						  Block_Temp_Data[BMUID][i]=(Buff_Data[BMUID][i])-2731;

							}

}
//================================================================
char TEMPERATURE(char BMUID)   //0x0D8
//================================================================
{	
       MyI2CPtr = CI2C1_Init(NULL);
	// Timer_Delay_ms(5);
	
	Module_Temp[BMUID]= IIC_ReadWord(BlockAddress(BMUID),0x08);
	
}

//================================================================
char RSOC(char BMUID)   //0x0D 
//================================================================
{	
       MyI2CPtr = CI2C1_Init(NULL);
	// Timer_Delay_ms(5);
	
	Pack_RSOC= IIC_ReadWord(BlockAddress(BMUID),0x0D);
	
}

//================================================================
char ASOC(char BMUID)   //0x0E
//================================================================
{	
       MyI2CPtr = CI2C1_Init(NULL);
	// Timer_Delay_ms(5);
	
	Pack_ASOC= IIC_ReadWord(BlockAddress(BMUID),0x0E);
	
}
//================================================================
signed int  CURRENT(char BMUID)   //0x0A 
//================================================================
{	MyI2CPtr = CI2C1_Init(NULL);
      //Timer_Delay_ms(5);
	Pack_Current=IIC_ReadWord(BlockAddress(BMUID),0x0A);   
}

//================================================================
void SafetyAlarm(char BMUID)   //0x51
//================================================================
{	MyI2CPtr = CI2C1_Init(NULL);
  
   	Safety=0x0000;
	Safety=IIC_ReadWord(BlockAddress(BMUID),0x51);   
	unsigned short mask = 0x0001; // ��l�B�n�O�ӳ̰������� 1 ��byte
	char i=0;
	
	  for ( i=0; i<16; ++i)    
    {		Alarm[BMUID][i]=0;  //init
    
        if ((Safety& mask) != 0)   // �p�G b �M mask �� AND �����G����0
        {   Alarm[BMUID][i]=1;  }    // �Y�N�� b ���� bit �� 1
        else                   // �ҥH��ܤ@�� 1
        {   Alarm[BMUID][i]=0; }    // �_�h��ܤ@�� 0
                               // �C���j�餤���� mask �k���@�� bit
        mask <<= 1;            // �N���F�U�� bit ���B�n
    }
	
		
}

//================================================================
void GPIO_STATUS()
//================================================================
{
if ((GPIO1_GetFieldValue(MyGPIO1Ptr,input1) ==1)&&(Alert_LED_Flag==0)&& (BMU_fault_Flag==0))  //start button on (N.O.)
	{
	GPIO2_ClearFieldBits(MyGPIO2Ptr, com4, 1);    //RLY   on
	Timer_Delay_ms(50);
	
						if (    GPIO1_GetFieldValue(MyGPIO1Ptr,input2)==0   ) //EMG button on (N.C.)
								{ 
								   GPIO2_ClearFieldBits(MyGPIO2Ptr, com3, 1);   // EMG LED on
								   GPIO2_SetFieldValue(MyGPIO2Ptr, com1, 1);   //green LED off 
									 GPIO2_SetFieldValue(MyGPIO2Ptr, com2, 1);   //red LED off
								}
												
	else {	GPIO2_SetFieldValue(MyGPIO2Ptr, com3, 1);   // EMG LED off
		GPIO2_ClearFieldBits(MyGPIO2Ptr, com1, 1);        //green LED on    
		fetch_Ready_Flag=1;  
		}
	}

if ((GPIO1_GetFieldValue(MyGPIO1Ptr,input1) ==0) )
		{	//start button off
		fetch_Ready_Flag=0;  
		GPIO2_SetFieldValue(MyGPIO2Ptr, com1, 1);   //green LED off  
    		GPIO2_SetFieldValue(MyGPIO2Ptr, com4, 1);   //RLY   off 
	       GPIO2_SetFieldValue(MyGPIO2Ptr, com3, 1);   // EMG LED off
	        GPIO2_SetFieldValue(MyGPIO2Ptr, com2, 1);   //red LED off
	       alert_flag=0;
	       Alarm_Flag=0;
		Alert_LED_Flag=0;
		SUMBMU_fault=0;
		BMU_fault_Flag=0;
		Count_4=0;
		Count_5=0;
		Count_6=0;
			
		}
}

#if 1
//========================================================================
void RS485_TX()
//========================================================================
{	
/*
struct {
  byte Address;
  byte Command;
  byte Byte_Count;
  unsigned short Pack_Volt;
} */
	TXD_Buf[0]= 0x00;
	TXD_Buf[1]=0x03;
	TXD_Buf[2]=0x0A;
	TXD_Buf[3]=Pack_Volt>>8;		  /* high byte */  
	TXD_Buf[4]=Pack_Volt%256; 		  /*  low byte */   ;
	TXD_Buf[5]= Pack_Current>>8;
	TXD_Buf[6]=Pack_Current%256;
	TXD_Buf[7]=Pack_maxTemp>>8;
	TXD_Buf[8]= Pack_maxTemp%256;;
	TXD_Buf[9]=Pack_RSOC;
	TXD_Buf[10]=Pack_ASOC;
	TXD_Buf[11]=Safety>>8;
	TXD_Buf[12]= Safety%256;
	char i=0;
	for(i=0;i<13;i++) {	Send_Buf[i]=TXD_Buf[i];	}
	
	//CRC1_GetBlockCRC(DeviceData, (LDD_TData*)  TXD_Buf, 16 , &CRC_Result);
	Send_Buf[13]=CRC_Result>>8;
	Send_Buf[14]=CRC_Result%256;
	//TXD_Buf[15]= 'P';
	
	RS485EN_SetVal(RS485ENPtr);
	Error = AS1_SendBlock(MySerialPtr, Send_Buf,15 ); /* Send block of characters */
	
	while (!UART_TransmittedFlg){
		AS1_Main(MySerialPtr);
		}
	UART_TransmittedFlg=FALSE;
}
#endif

//========================================================================
void UART_TX_2bytes(char command, signed short data)
//========================================================================
{
	TXD_Buf[0]=command;
	TXD_Buf[1]=data;	
	Error = RS232_SendBlock(MySerialPtr, TXD_Buf,1u); /* Send block of characters */
	while (!RS232_TransmittedFlg){	
		RS232_Main(MySerialPtr);
	}
	RS232_TransmittedFlg=FALSE;
}

//========================================================================
void I2C_switch(char k)
//========================================================================
{ 
        
	ddBitsIO = Bits1_Init(NULL);
	if(k==1)  {	
				Bits1_PutVal(ddBitsIO, 0b00u);       // Set "00" on the pins
		    		Bits1_SetDir(ddBitsIO, FALSE);   // Set input mode
				}
	if(k==0)
	{  
	  Bits1_SetDir(ddBitsIO, TRUE);   // Set output mode
	  Bits1_PutVal(ddBitsIO, 0b11u);       // Set "11" on the pins
	 	  
	  }	

}
//===================================================================
void Intial_parameter(char BMUID)
//======================================================================
{
MyI2CPtr = CI2C1_Init(NULL);
IIC_WriteWord( BlockAddress(BMUID),0xE0,0x0050);	
Timer_Delay_ms(2); //must 10ms
IIC_ReadBlock(BlockAddress(BMUID),0xE3);

char i=0,j=1;
			for( i=0;i<8 ;i=i+1)             //collect 16 words
			{						
					   Block_Volts_Data[BMUID][i]=InpData[j]<<8|InpData[j+1];
						j=j+2;
			 }

					for( i=0;i<16 ;i=i++)             //clear16 words
					{		
						InpData[i]=0;	
					}	
IIC_WriteWord( BlockAddress(BMUID),0xE0,0x0051);	
Timer_Delay_ms(1);
IIC_ReadBlock(BlockAddress(BMUID),0xE3);

i=0,j=1;
	for( i=0;i<4 ;i=i+1)             //collect 16 words
			{						
					   Block_Temp_Data[BMUID][i]=( InpData[j]<<8|InpData[j+1] ) -2731;
						j=j+2;
			 }
						for( i=0;i<16 ;i=i++)             //clear16 words
					{		
						InpData[i]=0;	
					}	
}

//===================================================================
void Get_parameter(char BMUID)
//======================================================================
{
MyI2CPtr = CI2C1_Init(NULL);
IIC_WriteWord( BlockAddress(BMUID),0xE0,0x0050);	
Timer_Delay_ms(2); //must 10ms
IIC_ReadBlock(BlockAddress(BMUID),0xE3);

if(I2C_fault_Flag==0){
char i=0,j=1;
			for( i=0;i<8 ;i=i+1)             //collect 16 words
			{						
					   Block_Volts_Data[BMUID][i]=InpData[j]<<8|InpData[j+1];
						j=j+2;
			 }

					for( i=0;i<16 ;i=i++)             //clear16 words
					{		
						InpData[i]=0;	
					}
}
IIC_WriteWord( BlockAddress(BMUID),0xE0,0x0051);	
Timer_Delay_ms(2);
IIC_ReadBlock(BlockAddress(BMUID),0xE3);

	if(I2C_fault_Flag==0){
	char i=0,j=1;
		for( i=0;i<4 ;i=i+1)             //collect 16 words
				{						
						   Block_Temp_Data[BMUID][i]=( InpData[j]<<8|InpData[j+1] ) -2731;
							j=j+2;
				 }
							for( i=0;i<16 ;i=i++)             //clear16 words
						{		
							InpData[i]=0;	
						}
				}
}
//===============================end of function================================================

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  MyTU1Ptr = TU1_Init((LDD_TUserData *)NULL);        /* Initialize the device */
  MyTU3Ptr = TU3_Init((LDD_TUserData *)NULL);        /* Initialize the device */
  MyTI1Ptr = TI1_Init((LDD_TUserData *)NULL);          
  MyGPIO1Ptr = GPIO1_Init((LDD_TUserData *)NULL);        /* Initialize the pins */
  MyGPIO2Ptr = GPIO2_Init((LDD_TUserData *)NULL);        /* Initialize the pins */
  bit1Ptr = Bit1_Init((LDD_TUserData *)NULL);       /* Initialize the pin */
  bit2Ptr = Bit2_Init((LDD_TUserData *)NULL);        /* Initialize the pin */
  bit3Ptr = Bit3_Init((LDD_TUserData *)NULL);        /* Initialize the pin */  //DI2C_PWR_ON
  ddBitsIO = Bits1_Init(NULL);
  MyI2CPtr = CI2C1_Init(NULL); 
  MySerialPtr = AS1_Init(NULL);                                     
  DeviceData = CRC1_Init(NULL);
  RS485ENPtr = RS485EN_Init((LDD_TUserData *)NULL); 

  MySerialPtr = RS232_Init(NULL);  
  
  GPIO2_SetFieldValue(MyGPIO2Ptr, com1, 1);   //green LED off     
  GPIO2_SetFieldValue(MyGPIO2Ptr, com2, 1);   //red LED off
  GPIO2_SetFieldValue(MyGPIO2Ptr, com3, 1);   // EMG LED off
  GPIO2_SetFieldValue(MyGPIO2Ptr, com4, 1);   //RLY   off 
  
  

/*
  GPIO2_ClearFieldBits(MyGPIO2Ptr, com1, 1);        //green LED on    
  GPIO2_ClearFieldBits(MyGPIO2Ptr, com2, 1);   //red LED on
  GPIO2_ClearFieldBits(MyGPIO2Ptr, com3, 1);   // EMG LED on
  GPIO2_ClearFieldBits(MyGPIO2Ptr, com4, 1);    //RLY   on
  GPIO2_SetFieldValue(MyGPIO2Ptr, com1, 1);   //green LED off     
  GPIO2_SetFieldValue(MyGPIO2Ptr, com2, 1);   //red LED off
  GPIO2_SetFieldValue(MyGPIO2Ptr, com3, 1);   // EMG LED off
  GPIO2_SetFieldValue(MyGPIO2Ptr, com4, 1);   //RLY   off 
*/
  //Timer_Delay_ms(200);
 
 GPIO_STATUS();
 I2C_switch(1);

// CRC1_GetBlockCRC(DeviceData, (LDD_TData*) TXD_Buf, 13*sizeof(uint8_t), &CRC_Result);
 
//get_Block_Volts(0);  //fetch voltages
//get_Block_Volts(1);  //fetch voltages
//get_Block_temp(0);
//get_Block_temp(1);
 
Timer_Delay_ms(300); 
//I2C_switch(0);
//Timer_Delay_ms(100); 
#if 0
for(BMUID=0;BMUID<BMU_NUMBERS;BMUID++)  //BMUID
{    Intial_parameter(BMUID) ; 
     Timer_Delay_ms(2);   //must 20ms 
    //  Intial_parameter(BMUID) ;
     char i=0;
     for(i=0;i<8;i++){
     if(Block_Volts_Data[BMUID][i]==0)
	 	Init_fault_Flag=1;	
     	}
}
#endif
 //===========================================start==============
   
  for(;;) {         

Bits1_Deinit(ddBitsIO);		//release buff
	 CI2C1_Deinit(MyI2CPtr);  //=========================
	   Timer_Delay_ms(20); 			
I2C_switch(1);

#if 1
		#if 1
		if( fetch_Ready_Flag==0)    // 1 : on  ; 0:off
		{
	
			Timer_Delay_ms(10);
			 ddBitsIO = Bits1_Init(NULL);
  			MyI2CPtr = CI2C1_Init(NULL); 
			
		alert_flag=0;  // start alarm count
	       Alarm_Flag=0;  // safetyAlarm
		Alert_LED_Flag=0;  // lock start button
		SUMBMU_fault=0;
		BMU_fault_Flag=0;
		Count_4=0;
		Count_5=0;
		Count_6=0;
			 Timer_Delay_ms(200);	
				 


		}
			#endif
//==============================================================================

 
 if(fetch_Ready_Flag==1) {	 // 1 : on  ; 0:off
 
 	
		BMUID=0;
		int i =0;
				
				  int j=0;
				  for(j=0;j<6 ;j++)    
					{
					//  Bit1_SetVal(bit1Ptr);
					Bit2_SetVal(bit2Ptr);
					Timer_Delay_ms(66); 
					//Bit1_ClrVal(bit1Ptr);
					Bit2_ClrVal(bit2Ptr);
					Timer_Delay_ms(66);
					}
		
	I2C_switch(0);
	

Timer_Delay_ms(10); 

// clear flag

/*
for(BMUID=0;BMUID<BMU_NUMBERS;BMUID++)  //BMUID
      		 { 
			 I2C_fault_Flag=0;
			Timer_Delay_ms(2);
			Intial_parameter(BMUID);  //fetch voltages
			Timer_Delay_ms(2);
			Clr_Buff(BMUID);
      		 }

*/
					
			SUM_Safety_Flag=0;
				
					for(BMUID=0;BMUID<BMU_NUMBERS;BMUID++) 
						{			SafetyAlarm(BMUID);
									Timer_Delay_ms(2); 
																							
								if (I2C_fault_Flag==1)	
									 { 			I2C_fault_Flag=0;
												 BMU_fault_Count[BMUID]++; 
												if(  BMU_fault_Count[BMUID]>200)  BMU_fault_Count[BMUID]=100;												
												
												 }			 
								else  BMU_fault_Count[BMUID]=0;		//clear			
		 //====================================BMU safety alarm count============================================= 
		if((Alarm[BMUID][15] ==1)|| (Alarm[BMUID][14]==1) ||(Alarm[BMUID][6]==1) ||(Alarm[0][13]==1)||(Alarm[0][12]==1) )
			{
				SafetyAlarm(BMUID);
				if((Alarm[BMUID][15] ==1)|| (Alarm[BMUID][14]==1) ||(Alarm[BMUID][6]==1) ||(Alarm[0][13]==1)||(Alarm[0][12]==1) )
				{	SUM_Safety_Flag++; 	 }
			 	
			}

		if(Alarm[BMUID][7]==1)  {
			SafetyAlarm(BMUID);
			if(Alarm[BMUID][7]==1)
			{  UV_flag=1;	}

			
			}
		 				
					}//end for
//======================================================
				I2C_switch(1);
				
//========================Safety alarm flag======================================

		if ( SUM_Safety_Flag > 0 )  
			{	
				Alarm_Flag=1;   //safetyAlarm ON
				SUM_Safety_Flag=0;  //clr

					}
			else if ( SUM_Safety_Flag==0)
				 Alarm_Flag=0;


		//=============================================================
#if 0

	for(BMUID=0;BMUID<BMU_NUMBERS;BMUID++) {
  
   Get_parameter(BMUID) ; 
       Timer_Delay_ms(2);   //must 20ms 
     Find_Block_max_min_Value( BMUID);
	 						}
#endif
			


	//=====================I2C fault====================================
	SUMBMU_fault=0;  //init
	
		for(BMUID=0;BMUID<BMU_NUMBERS;BMUID++)
		{
   			SUMBMU_fault+=BMU_fault_Count[BMUID]; 
		 if (  (BMU_fault_Count[BMUID]  >16) ||(SUMBMU_fault >27)  ) 
						BMU_fault_Flag=1;	
			}
		
		  if( SUMBMU_fault < 16 ) 	
		  	BMU_fault_Flag=0;  //clear
//================================I2C fault end==========================================
	


 
		//=========================================
			if(BMUID==BMU_NUMBERS)   {
			Bit1_ClrVal(bit1Ptr);
			Bit2_ClrVal(bit2Ptr);
			Bit1_SetVal(bit1Ptr);
			//Bit2_SetVal(bit2Ptr);
			Timer_Delay_ms(16);
			Bit1_ClrVal(bit1Ptr);
			//Bit2_ClrVal(bit2Ptr);
			}
			
			//================================================
 	 	 	 	 	 	 
	

 
#if 0	
//===========================alarm===========================================================
/*        OV=3.65  ,  3.4   ;UV=2.7,   2.9  ;		T >60 ; 		OCC > 50A  ;		OCD > 300A    */ 
								    		
				    		if  ( (Pack_minVolt>2000)&&(Pack_minVolt<2700) )	
								UV_flag=1;
						if(  (Pack_maxVolt > 3650) &&  (Pack_maxVolt < 6500)   )
							 OV_flag=1;
						if(Pack_Current <-30000) 
							OCD_flag=1;	//unit 10mA
					       if(Pack_Current >5000) 	
						   	OCC_flag=1;
						if( Pack_maxTemp > 550)  
							OT_flag=1;
	
						  //  BMUID=0;
						        for( BMUID = 0; i < BMU_NUMBERS; BMUID++){  

									if( (Block_maxVolts_Data[BMUID][0]  > 3650 ) &&  (Block_maxVolts_Data[BMUID][0]  < 6500 )  ) //====
										{    	OV_flag=1; 		}									
									if( (Block_minVolts_Data[BMUID][0]  < 2700 )&& (Block_minVolts_Data[BMUID][0]  > 2000 ) ) 
										{ 	UV_flag=1;					}							   
									if ( (Block_maxTemp_Data[BMUID][0] > 550) && (Block_maxTemp_Data[BMUID][0]<1200) ) 
										{	OT_flag=1;				 }
						        }  
				
		//===============recover condition===============================
			if ( SUMBMU_fault < 10) 
					BMU_fault_Flag=0;
		
		if(Pack_maxVolt < 3400) //===test
		{	OV_flag=0;  
		 }
		if(Pack_minVolt > 2900) 
    				{	UV_flag=0;  
				 }
		if(Pack_Current >-200) 
		{OCD_flag=0;    }
	       if(Pack_Current <300) 
	       {OCC_flag=0;	  }
		if(Pack_maxTemp < 500) 
		{ OT_flag=0;	  }
		   	
#endif




#if 1
				//===================RS485=============================
if(RS485_S_F==1) {
						RS485_S_F=0;
						Count_4=0;
					I2C_switch(0);	//lock PC
					Timer_Delay_ms(2);
					RSOC(0)	; 
					Timer_Delay_ms(2);
					CURRENT(0) ;
					Timer_Delay_ms(2);
					ASOC(0); //get_Block_temp(0);
					BMUID=0;
						for(BMUID=0;BMUID<BMU_NUMBERS;BMUID++)  //BMUID
				      		 { 
							 //I2C_fault_Flag=0;
							Timer_Delay_ms(2);
							get_Block_Volts(BMUID);  //fetch voltages
								Find_max_min_Value(BMUID);
							Timer_Delay_ms(2);
							TEMPERATURE(BMUID);			
							
						}	
I2C_switch(1);
						
			Pack_maxTemp=Module_Temp[0];  // init	
			for(BMUID=0;BMUID<BMU_NUMBERS;BMUID++)  //BMUID
			 {													
							   	 //��X�̤j�� 
					  if(   Pack_maxTemp < Module_Temp[BMUID]    ){
							                      Pack_maxTemp= Module_Temp[BMUID];
							 }    
				}


						P_Buff_Volt=0; char i=0, w=0;
					for(BMUID=0;BMUID<BMU_NUMBERS;BMUID++) {
						
						for(i=0;i<1;i++) {
					P_Buff_Volt=P_Buff_Volt+( Block_Volts_Data[BMUID][i]*8 / 10);
							if(Block_Volts_Data[BMUID][i] ==0) w=1;
							}
						}
						if(w==1)  Pack_Volt=Pack_Volt;
					 else Pack_Volt=P_Buff_Volt + 60;

					 	 		      	Bit1_ClrVal(bit1Ptr);
									Bit2_ClrVal(bit1Ptr);
									Timer_Delay_ms(5);
									Bit1_SetVal(bit1Ptr);
									Bit2_SetVal(bit2Ptr);
									Timer_Delay_ms(5);
									 Bit1_ClrVal(bit1Ptr);
									Bit2_ClrVal(bit1Ptr);
												
					RS485_TX();

					

							
		//====================================balance========================================
				#if 1
				BMUID=0;
				char map=0;
					      //check each BMU
					       
						  I2C_switch(0);
						  if( ( BMU_fault_Flag==0) && (Init_fault_Flag==0)  &&  (Alarm_Flag==0)&&( SUMBMU_fault==0)&&(fetch_Ready_Flag==1)) 
						  	{
					      	 for( BMUID = 0; BMUID < BMU_NUMBERS ; BMUID++)          
					      	 {
					      		 map=0;
								 i=0;
					      		for( i = 0; i < 1 ; i++)
					      		{
					      			if((Block_Volts_Data[BMUID][i] > 3450)&& (Pack_minVolt>1000) )	//balance start point
					      			{
										if(    Block_Volts_Data[BMUID][i]  > (Pack_minVolt +80)      ) //delta_V
										{
											map |= (0x01 << i) ;	// or ++
											
										}
								}
					      		
					      		}
					      		balance(BMUID,map);		//balance on
					      		Timer_Delay_ms(2);   //rest time
					      	} 
							 }

			    #endif
				  I2C_switch(1);
		//=============================end balance===================================
								
					}
					
//==============================================end rs485===========================================		
						 #endif

					 
}


#endif
}	//end big for-----------------------------------

//==========================================================


  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
