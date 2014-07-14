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
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t PrescalerValue = 0;
uint8_t Buffer[6];
__IO uint32_t TimingDelay = 0;
__IO int8_t XOffset;
__IO int8_t YOffset;
__IO int8_t ZOffset;

__IO uint8_t SingleClickDetect = 0x00;
extern uint8_t ClickReg;

/* Private function prototypes -----------------------------------------------*/
static void TIM_Config(void);
void GPIO_Config(void);

/* Private functions ---------------------------------------------------------*/

uint8_t getBit(int x,uint8_t index);


void ImpostaBit(int asse, int pin, int val){
        printf("Asse:%d\n",asse);
        printf("Pin:%d\n",pin);
        printf("Val:%d\n",val);
	/*switch(asse){
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
	}*/

}

/**
* @brief   Main program
* @param  None
* @retval None
*/
int main(void)
{
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

	/* TIM configuration -------------------------------------------------------*/
	//TIM_Config(); 
	LIS302DL_Read(Buffer, LIS302DL_OUT_X_ADDR, 6);

	XOffset = Buffer[0];
	YOffset = Buffer[2];
	ZOffset = Buffer[4];
	GPIO_Config();

	/* Inizializzo i vettori con i pin liberi per poterli utilizzare come uscite
e ingressi. */

	int freeA[]={1,2,3,8,15};
	int freeB[]={0,1,2,4,5,7,8,11,13,14,15}; //utilizzato per l'asse x
	int freeC[]={1,2,4,5,6,8,9,11,13,14,15}; //utilizzato per l'asse y
	int freeD[]={0,1,2,3,6,7,8,9,10,11}; //utilizzato per l'asse z
	int freeE[]={3,4,5,6,7,8,9,10,11,12,13,14,15};

	while(1)
	{
		int i,j;
		uint8_t resPin=0;
		
		LIS302DL_Read(Buffer, LIS302DL_OUT_X_ADDR, 6);
		Buffer[0] -= XOffset;
		Buffer[2] -= YOffset;
		Buffer[4] -= ZOffset;
                
                GPIO_SetBits(GPIOA,GPIO_Pin_2);
                /*
		for(i=0;i<=4;i=i+2){
			printf("Asse %d:%d\n",i,(int8_t)Buffer[i]);
			for(j=7;j>=0;j--){
				//int* vett;
				if(i==0) ImpostaBit(i, freeB[j], getBit(Buffer[i],j));
				else if(i==2) ImpostaBit(i, freeC[j], getBit(Buffer[i],j));
				else if(i==4) ImpostaBit(i, freeD[j], getBit(Buffer[i],j));
				printf("%d",getBit(Buffer[i],j));
				
			}
			printf("\n");
		}
                */
		/*
	for(i=4;i<12;i++){
	resPin=GPIO_ReadInputDataBit(GPIOE,(uint16_t)(1 << i));
	printf("Valore del pin %d:%d\n",i,resPin);
}*/
		Delay(200);  
	}
}

uint8_t getBit(int x,uint8_t index){
	return ((x >> index)  & 0x01);
}

void GPIO_Config(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);

	GPIO_InitTypeDef GPIO_InitStructureA,GPIO_InitStructureB,GPIO_InitStructureC,GPIO_InitStructureD,GPIO_InitStructureE;
	GPIO_InitStructureA.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_8 | GPIO_Pin_3 | GPIO_Pin_2 |GPIO_Pin_1;
	GPIO_InitStructureA.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructureA.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureA.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureA.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructureA);

	GPIO_InitStructureB.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 |GPIO_Pin_11 |GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_4 |GPIO_Pin_2 |GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructureB.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructureB.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureB.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructureB);

	GPIO_InitStructureC.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_11 |GPIO_Pin_9 |GPIO_Pin_8 | GPIO_Pin_6 |GPIO_Pin_5 |GPIO_Pin_4 | GPIO_Pin_2 |GPIO_Pin_1;
	GPIO_InitStructureC.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructureC.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureC.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructureC);

	GPIO_InitStructureD.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10|GPIO_Pin_9 | GPIO_Pin_8 |GPIO_Pin_7 | GPIO_Pin_6|GPIO_Pin_3 | GPIO_Pin_2 |GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructureD.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructureD.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureD.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureD.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructureD);


	GPIO_InitStructureE.GPIO_Pin =GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructureE.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructureE.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructureE);


}



/**
* @brief  Configures the TIM Peripheral.
* @param  None
* @retval None
*/
static void TIM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* --------------------------- System Clocks Configuration -----------------*/
	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* GPIOD clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/*-------------------------- GPIO Configuration ----------------------------*/
	/* GPIOD Configuration: Pins 12, 13, 14 and 15 in output push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Connect TIM4 pins to AF2 */  
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4); 

	/* -----------------------------------------------------------------------
TIM4 Configuration: Output Compare Timing Mode:

In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
since APB1 prescaler is different from 1 (APB1 Prescaler = 4, see system_stm32f4xx.c file).
TIM4CLK = 2 * PCLK1  
PCLK1 = HCLK / 4 
=> TIM4CLK = 2*(HCLK / 4) = HCLK/2 = SystemCoreClock/2

To get TIM4 counter clock at 2 KHz, the prescaler is computed as follows:
Prescaler = (TIM4CLK / TIM1 counter clock) - 1
Prescaler = (168 MHz/(2 * 2 KHz)) - 1 = 41999

To get TIM4 output clock at 1 Hz, the period (ARR)) is computed as follows:
ARR = (TIM4 counter clock / TIM4 output clock) - 1
= 1999

TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 50%
TIM4 Channel3 duty cycle = (TIM4_CCR3/ TIM4_ARR)* 100 = 50%
TIM4 Channel4 duty cycle = (TIM4_CCR4/ TIM4_ARR)* 100 = 50%

==> TIM4_CCRx = TIM4_ARR/2 = 1000  (where x = 1, 2, 3 and 4).

Note: 
SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
function to update SystemCoreClock variable value. Otherwise, any configuration
based on this variable will be incorrect.    
----------------------------------------------------------------------- */ 

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 2000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIM_ARR;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* Enable TIM4 Preload register on ARR */
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	/* TIM PWM1 Mode configuration: Channel */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIM_CCR;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* Output Compare PWM1 Mode configuration: Channel1 */
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_CCxCmd(TIM4, TIM_Channel_1, DISABLE);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* Output Compare PWM1 Mode configuration: Channel2 */
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_CCxCmd(TIM4, TIM_Channel_2, DISABLE);

	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* Output Compare PWM1 Mode configuration: Channel3 */
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_CCxCmd(TIM4, TIM_Channel_3, DISABLE);

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* Output Compare PWM1 Mode configuration: Channel4 */
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_CCxCmd(TIM4, TIM_Channel_4, DISABLE);

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);
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
