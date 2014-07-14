/**
******************************************************************************
* @file    LIS302DL/main.c 
* @author  MCD Application Team
* @version V1.0.0
* @date    19-September-2011
* @brief   Main program body
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
******************************************************************************  
*/ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
* @{
*/

/** @addtogroup LIS302DL_Example
* @{
*/ 

/* Private typedef -----------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
EXTI_InitTypeDef   EXTI_InitStructure;
/* Private define ------------------------------------------------------------*/

#define START 0x01
#define STOP 0x02
#define AZIONE1 0x04
#define AZIONE2 0x08
#define AZIONE3 0x10
#define AZIONE4 0x20

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t PrescalerValue = 0;
uint8_t Buffer[6];
int8_t comando=0;

/* Inizializzo i vettori con i pin liberi per poterli utilizzare come uscite
e ingressi. */
int freeA[]={1,2,3,8,15};
int freeB[]={0,1,2,4,5,7,8,11,13,14,15}; //utilizzato per l'asse x
int freeC[]={1,2,4,5,6,8,9,11,13,14,15}; //utilizzato per l'asse y
int freeD[]={0,1,2,3,6,7,8,9,10,11}; //utilizzato per l'asse z
int freeE[]={4,5,6,7,8,9,10,11,12,13,14,15};
int changed=0;

__IO uint32_t TimingDelay = 0;
__IO int8_t XOffset;
__IO int8_t YOffset;
__IO int8_t ZOffset;

__IO uint8_t SingleClickDetect = 0x00;
extern uint8_t ClickReg;

/* Private function prototypes -----------------------------------------------*/
static void TIM_Config(void);
void GPIO_Config(void);
void EXTILine0_Config(void);
void scriviValoreInUscita(void);
void configuraAccelerometro(void);
void Timer(uint32_t t);
/* Private functions ---------------------------------------------------------*/

uint8_t getBit(int x,uint8_t index);


void ImpostaBit(int asse, int pin, int val){
	switch(asse){
	case 0:{
			if(val==1){
				GPIO_SetBits(GPIOB,(uint16_t)(1 << pin));
			}
			else{ 
				GPIO_ResetBits(GPIOB,(uint16_t)(1 << pin));
			}
			break;
		}
	case 2:{
			if(val==1){
				GPIO_SetBits(GPIOC,(uint16_t)(1 << pin));
			}
			else{ 
				GPIO_ResetBits(GPIOC,(uint16_t)(1 << pin));
			}
			break;
		}
	case 4:{
			if(val==1){
				GPIO_SetBits(GPIOD,(uint16_t)(1 << pin));
			}
			else{ 
				GPIO_ResetBits(GPIOD,(uint16_t)(1 << pin));
			}
			break;
		}
	}

}

/**
* @brief   Main program
* @param  None
* @retval None
*/
int main(void)
{
	
        
        STM_EVAL_LEDInit(LED6);
        STM_EVAL_LEDInit(LED5);
        STM_EVAL_LEDInit(LED4);
        STM_EVAL_LEDInit(LED3);
      
        configuraAccelerometro();
	

	XOffset = Buffer[0];
	YOffset = Buffer[2];
	ZOffset = Buffer[4];
	// GPIO_Config();
        //EXTILine0_Config();
	main_vcp();
        
        char buffer[30];
        buffer[0]='\0';
        for(int k =0; k<30;k++){
        buffer[k]='\0';
        }
	while(1)
	{
          /*scriviValoreInUscita();
          VCP_get_string(buffer);
          Delay(1000);
          printf("%s\n",buffer);
          buffer[0]='\0';
          continue;
          */
          comando='\0';
          buffer[0]='\0';
          VCP_get_string(buffer);
          if(buffer[0]!=0) comando=buffer[0];
          printf("Comando ricevuto (buffer): %d\n",buffer[0]);
          switch(comando){
            case START:{
              scriviValoreInUscita();
              break;}
            case STOP:{
              break;}
              case AZIONE1:{
                STM_EVAL_LEDOn(LED3);
              break;}
              case AZIONE2:{
                STM_EVAL_LEDOff(LED3);
              break;}
              case AZIONE3:{
                STM_EVAL_LEDOn(LED5);
              break;}
              case AZIONE4:{
                STM_EVAL_LEDOff(LED5);
              break;}
          }
                                  
		Delay(10);  
	}
}

void scriviValoreInUscita(void){
  int i,j,k;
  char bufferStr [40];//24(valori assi)+3(x,y,z)+3(:)+3(\n)+1(\0)=34
  int8_t numeroIn=0;
  int8_t resPin=0;

  LIS302DL_Read(Buffer, LIS302DL_OUT_X_ADDR, 6);
  Buffer[0] -= XOffset;
  Buffer[2] -= YOffset;
  Buffer[4] -= ZOffset;
  
  k=0;
  for(i=0;i<=4;i=i+2){
          printf("Asse %d:%d - ",i,(int8_t)Buffer[i]);
          
          bufferStr[k]=(i+48); k++;
          bufferStr[k]=':';k++;
          for(j=7;j>=0;j--){
              
                  printf("%d",getBit(Buffer[i],j));
                  bufferStr[k]=(getBit(Buffer[i],j)+48);k++;
          }
          if(i!=4){bufferStr[k]=(',');k++;}
          printf("\n");
  }
  bufferStr[k]=';';
  k++;
  bufferStr[k]='\0';
  VCP_send_str(bufferStr);
}

uint8_t getBit(int x,uint8_t index){
	return ((x >> index)  & 0x01);
}


void EXTI0_IRQHandler(void)
{
  
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    int i;
    int8_t numeroIn=0;
    int8_t resPin=0;
    /* Toggle LED4 */
    STM_EVAL_LEDOn(LED4);
    
    /* LETTURA DEI PIN DI GPIOE */
    for(i=7;i>=0;i--){
            resPin=GPIO_ReadInputDataBit(GPIOE,(1 << freeE[i]));
            numeroIn=numeroIn | resPin << i;
    }
    printf("Il valore letto da GPIOE e' %d\n",numeroIn);
    if(numeroIn!=comando) changed=1;
    comando=numeroIn;
    /* INVIO ACK */
    GPIO_SetBits(GPIOB,GPIO_Pin_0);
     STM_EVAL_LEDOff(LED4);
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}


void EXTILine0_Config(void)
{
  
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void GPIO_Config(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
        
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
        
	GPIO_InitTypeDef GPIO_InitStructureA,GPIO_InitStructureB,GPIO_InitStructureC,GPIO_InitStructureD,GPIO_InitStructureE;
	GPIO_InitStructureA.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_8 | GPIO_Pin_3 | GPIO_Pin_2 |GPIO_Pin_1;
	GPIO_InitStructureA.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureA.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureA.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructureA);

	GPIO_InitStructureB.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 |GPIO_Pin_11 |GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_4 |GPIO_Pin_2 |GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructureB.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureB.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureB.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructureB);

	GPIO_InitStructureC.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_11 |GPIO_Pin_9 |GPIO_Pin_8 | GPIO_Pin_6 |GPIO_Pin_5 |GPIO_Pin_4 | GPIO_Pin_2 |GPIO_Pin_1;
	GPIO_InitStructureC.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureC.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureC.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructureC);

	GPIO_InitStructureD.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10|GPIO_Pin_9 | GPIO_Pin_8 |GPIO_Pin_7 | GPIO_Pin_6|GPIO_Pin_3 | GPIO_Pin_2 |GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructureD.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureD.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureD.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureD.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructureD);


	GPIO_InitStructureE.GPIO_Pin =GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructureE.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureE.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructureE);


}

void configuraAccelerometro(void){
  uint8_t ctrl = 0;
LIS302DL_InitTypeDef  LIS302DL_InitStruct;
	LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;  

	/* SysTick end of count event each 10ms */
	SysTick_Config(SystemCoreClock/ 100);

	/* Set configuration of LIS302DL*/
	LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
	LIS302DL_InitStruct.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE;
	LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
	LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
	LIS302DL_Init(&LIS302DL_InitStruct);

	/* Set configuration of Internal High Pass Filter of LIS302DL*/
	LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_LATCHED;
	LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE;
	LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE;
	LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);

	/* Required delay for the MEMS Accelerometre: Turn-on time = 3/Output data Rate 
= 3/100 = 30ms */
	Delay(30);

	/* Configure Interrupt control register: enable Click interrupt1 */
	ctrl = 0x07;
	LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG3_ADDR, 1);

	/* Enable Interrupt generation on click/double click on Z axis */
	ctrl = 0x70;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);

	/* Configure Click Threshold on X/Y axis (10 x 0.5g) */
	ctrl = 0xAA;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSY_X_REG_ADDR, 1);

	/* Configure Click Threshold on Z axis (10 x 0.5g) */
	ctrl = 0x0A;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSZ_REG_ADDR, 1);

	/* Configure Time Limit */
	ctrl = 0x03;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_TIMELIMIT_REG_ADDR, 1);

	/* Configure Latency */
	ctrl = 0x7F;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_LATENCY_REG_ADDR, 1);

	/* Configure Click Window */
	ctrl = 0x7F;
	LIS302DL_Write(&ctrl, LIS302DL_CLICK_WINDOW_REG_ADDR, 1);

	LIS302DL_Read(Buffer, LIS302DL_OUT_X_ADDR, 6);
}


/**
* @brief  Inserts a delay time.
* @param  nTime: specifies the delay time length, in milliseconds.
* @retval None
*/
void Delay(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

/**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval None
*/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

/**
* @brief  MEMS accelerometre management of the timeout situation.
* @param  None.
* @retval None.
*/
uint32_t LIS302DL_TIMEOUT_UserCallback(void)
{
	/* MEMS Accelerometer Timeout error occured */
	while (1)
	{   
	}
}

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
* @}
*/ 

/**
* @}
*/ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
