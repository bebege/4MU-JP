/* ###################################################################
**     Filename    : Events.c
**     Project     : KEAZ128
**     Processor   : SKEAZ128MLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-02-23, 12:00, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMI - void Cpu_OnNMI(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

extern LDD_TError Error;
extern LDD_TDeviceData *MyGPIO1Ptr, *MyGPIO2Ptr,*MyTU1Ptr,*MyI2CPtr, *bit1Ptr, *bit2Ptr;

extern unsigned int Delay_ms;
extern unsigned char  BMUID ;
extern volatile unsigned char Count_1,Count_2,Count_3,Count_4,Count_6;
extern volatile unsigned short Count_0, Count_5; 

extern volatile bool  I2C_flag,alert_flag,Alert_LED_Flag,fetch_Ready_Flag,I2C_fault_Flag;
extern volatile bool UV_flag, OV_flag,SOC_low_flag,OCC_flag,OCD_flag,OT_flag,UT_flag;
extern volatile bool recover_flag,Alarm_Flag;
extern unsigned short  BMU_fault_Count[16];
extern volatile bool BMU_fault_Flag;
extern volatile bool Init_fault_Flag;

extern volatile bool IIC_TransmittedFlg,IIC_ReceivedFlg;
extern volatile bool UART_TransmittedFlg,UART_ReceivedFlg;
extern volatile bool RS232_TransmittedFlg,RS232_ReceivedFlg;
extern volatile bool I2C_status,RS485_S_F;
extern char Alarm[16][16];
extern unsigned char  BMU_NUMBERS;        //===================

#define  busy 1
#define idle 0

extern void GPIO_STATUS();  
extern void Timer_Delay_ms();
extern short SafetyAlarm() ;

extern void I2C_switch();

/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [SKEAZ128LK4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  CI2C1_OnMasterBlockSent  (module Events)
**
**     Component   :  CI2C1 [I2C_LDD]
*/
/*!
**     @brief
**         This event is called when I2C in master mode finishes the
**         transmission of the data successfully. This event is not
**         available for the SLAVE mode and if MasterSendBlock is
**         disabled. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void CI2C1_OnMasterBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
  IIC_TransmittedFlg = TRUE; /* Set DataTransmittedFlg flag */
  	I2C_flag=0;
	Count_2=0;

}

/*
** ===================================================================
**     Event       :  CI2C1_OnMasterBlockReceived (module Events)
**
**     Component   :  CI2C1 [I2C_LDD]
*/
/*!
**     @brief
**         This event is called when I2C is in master mode and finishes
**         the reception of the data successfully. This event is not
**         available for the SLAVE mode and if MasterReceiveBlock is
**         disabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void CI2C1_OnMasterBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
  IIC_ReceivedFlg = TRUE; /* Set DataReceivedFlg flag */
  	I2C_flag=0;
	Count_2=0;

   }

/*
** ===================================================================
**     Event       :  TU1_OnCounterRestart (module Events)
**
**     Component   :  TU1 [TimerUnit_LDD]
*/
/*!
**     @brief
**         Called if counter overflow/underflow or counter is
**         reinitialized by modulo or compare register matching.
**         OnCounterRestart event and Timer unit must be enabled. See
**         [SetEventMask] and [GetEventMask] methods. This event is
**         available only if a [Interrupt] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void TU1_OnCounterRestart(LDD_TUserData *UserDataPtr)
{ //1s


// when execute, reset count==================================================
	
	        Count_3++;	     
		if(Count_3>3) { 
			Count_3=0;	
			GPIO_STATUS();   //fetch ready
					}
//================統一管理==========================
		
//=====================recover count ================================

if( (Alarm_Flag==0)   )
		{	
			Count_5++;     // recover count on
					}		
			
	if(Count_5>700) {
		// clear state-------------
		Count_5=0;  
		alert_flag=0;  
		Alert_LED_Flag=0; //button lock
		}
//===================alarm count======================================
	if(   (Alarm_Flag ==1)  )
					{ 			
								alert_flag=1; 
								Count_5=0; // recover reset
									}	
//==========================alarm on=================		
						 if(alert_flag==1)	{  	Count_1++;	 }
	 if(Count_1 > 3)
				{    
						Count_1=0;   //clr
						Alert_LED_Flag=1;  //block start button
					
						if((GPIO1_GetFieldValue(MyGPIO1Ptr,input1) ==1)&& (BMU_fault_Flag==0)&& (UV_flag==0) ) 
							{  // //start button on (N.O.)
						GPIO2_ClearFieldBits(MyGPIO2Ptr, com2, 1);   //red LED On  
						Timer_Delay_ms(200);
						 GPIO2_SetFieldValue(MyGPIO2Ptr, com2, 1);   //red LED off
						 
						GPIO2_SetFieldValue(MyGPIO2Ptr, com1, 1);   //green LED off
						GPIO2_SetFieldValue(MyGPIO2Ptr, com4, 1);   //RLY   off ------------------------
						
							}	
				}


											

			
//---------------------------------------------------------------
	 if(( UV_flag==1 )||(BMU_fault_Flag==1))	Count_6++;
	 	if( Count_6 >5)
		{			
			Count_6=0;
					if(UV_flag==1 ) {
					UV_flag=0; //clr flag
					GPIO2_SetFieldValue(MyGPIO2Ptr, com1, 1);   //green LED off
					GPIO2_ClearFieldBits(MyGPIO2Ptr, com2, 1);   //red LED On  
					Timer_Delay_ms(200);
					GPIO2_SetFieldValue(MyGPIO2Ptr, com2, 1);   //red LED off
					//GPIO2_ClearFieldBits(MyGPIO2Ptr, com1, 1);        //green LED on  
						}


								if(BMU_fault_Flag==1) 
									{	
										//BMU_fault_Flag=0;
										GPIO2_SetFieldValue(MyGPIO2Ptr, com4, 1);   //RLY   off ---------------												
										
										GPIO2_ClearFieldBits(MyGPIO2Ptr, com2, 1);   //red LED On  
										GPIO2_ClearFieldBits(MyGPIO2Ptr, com1, 1);        //green LED on
										Timer_Delay_ms(200);
										GPIO2_SetFieldValue(MyGPIO2Ptr, com1, 1);   //green LED off
										GPIO2_SetFieldValue(MyGPIO2Ptr, com2, 1);   //red LED off											
										}
					
					
			}
//===========================================================================	
}
				
/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt_LDD]
*/
/*!
**     @brief
**         Called if periodic event occur. Component and OnInterrupt
**         event must be enabled. See [SetEventMask] and [GetEventMask]
**         methods. This event is available only if a [Interrupt
**         service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void TI1_OnInterrupt(LDD_TUserData *UserDataPtr)
{

  /* Write your code here ... */

			if(Delay_ms) 
			Delay_ms--;
}

/*
** ===================================================================
**     Event       :  AS1_OnBlockReceived (module Events)
**
**     Component   :  AS1 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void AS1_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
  UART_ReceivedFlg=TRUE;
  
}

/*
** ===================================================================
**     Event       :  AS1_OnBlockSent (module Events)
**
**     Component   :  AS1 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void AS1_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
  UART_TransmittedFlg=TRUE;
}


/*
** ===================================================================
**     Event       :  TU3_OnCounterRestart (module Events)
**
**     Component   :  TU3 [TimerUnit_LDD]
*/
/*!
**     @brief
**         Called if counter overflow/underflow or counter is
**         reinitialized by modulo or compare register matching.
**         OnCounterRestart event and Timer unit must be enabled. See
**         [SetEventMask] and [GetEventMask] methods. This event is
**         available only if a [Interrupt] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void TU3_OnCounterRestart(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	// 1s
	Count_4++;  
	if(Count_4 > 36) 
		{
			RS485_S_F=1;
			Count_4=0;
		}
	

							if(I2C_flag==1)  { 	Count_2++;	}	
										
												if(Count_2>1)   //  
											 		 {     
											 		 	Count_2=0;
														I2C_flag=0;
														 I2C_fault_Flag=1; 
														 I2C_switch(0);													 	
											 		   	 IIC_TransmittedFlg=1;  
											  			 IIC_ReceivedFlg=1;	
											 		//      	Bit1_ClrVal(bit1Ptr);
													//	 Bit1_SetVal(bit1Ptr);
													//	 Timer_Delay_ms(1);
													//	 Bit2_ClrVal(bit1Ptr);
														 Bit2_SetVal(bit2Ptr);
														 Timer_Delay_ms(1);
														 Bit2_ClrVal(bit2Ptr);		
													
											 		 }
}

/*
** ===================================================================
**     Event       :  RS232_OnBlockReceived (module Events)
**
**     Component   :  RS232 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void RS232_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
  RS232_ReceivedFlg=TRUE;
}

/*
** ===================================================================
**     Event       :  RS232_OnBlockSent (module Events)
**
**     Component   :  RS232 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void RS232_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
  RS232_TransmittedFlg=TRUE;
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
