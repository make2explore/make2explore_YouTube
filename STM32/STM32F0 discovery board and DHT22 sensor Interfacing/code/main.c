// ------------------- www.make2explore.com-----------------------------------------------------------
// Project           - Tutorial - STM32F0 discovery board and DHT22 sensor Interfacing
// Created By        - admin@make2explore.com
// Version - 1.0
// Software Platform - C, Keil uVision5
// Hardware Platform - STM32F040 Discovery Board             
// Sensors/Modules   - DHT22 Temp+Humidity Sensor, FTDI module -usb to UART
// ---------------------------------------------------------------------------------------------------

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx.h"
#include <dht22.c>
#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t msTicks;
float RH, TEMP;
/* Private function prototypes -----------------------------------------------*/
int fputc(int ch, FILE * f);
void SysTick_Handler(void);
void Delay(uint32_t dlyTicks);
void USART1_IRQHandler(void);
void EXTI0_1_IRQHandler(void);
void BTN_Init(void);
void USART1_Init(void);
void TIM2_Init(void);

/* Private functions ---------------------------------------------------------*/

/*
  retargeting output of printf() to serial line
*/
/* Write the 'ch' character into the 'f' file */
int fputc(int ch, FILE * f) {
  /* Transmit the character using USART1 */
  USART_SendData(USART1, (uint8_t) ch);

  /* Wait until transmit finishes */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}

  return ch;
}

/*
  SysTick handler
 */
void SysTick_Handler(void) {
  msTicks++;
}

/*
  Function to provide delay (in mSec)
 */
void Delay (uint32_t delayTicks) {                                              
  uint32_t currentTicks;

  currentTicks = msTicks;
  while ((msTicks - currentTicks) < delayTicks);
}

/*
  USART1 interrupt handler
 */
void USART1_IRQHandler(void) {
  uint8_t data = 0x00;

  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    data = USART_ReceiveData(USART1);
   
    printf("Received character: %c\r\n", data);
  }
}

/*
  Button interrupt handler
 */
void EXTI0_1_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
    printf("EXTI_Line0 interrupt\r\n");
    //debouncing
    while(GPIOA->IDR & GPIO_IDR_0){}
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

/*
  button gpio interrupt clock configuration
 */
void BTN_Init(void) {
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*
  USART1 Tx PA.9, Rx PA.10 gpio clock interrupt configuration
 */
void USART1_Init(void) {
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // configuring clock sources for USART1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
   
  /* Configure USART1 pins:  Rx and Tx ----------------------------*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  /* Enable USART1 IRQ */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
   
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  
  // enabling USART1
  USART_Cmd(USART1,ENABLE);

  // enable incoming data interrupt
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

/*
  Function that initializes timer 2
 */
void TIM2_Init(void) {
  uint16_t PrescalerValue = 0;
  
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_OCInitTypeDef         TIM_OCInitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535 - 1;      //in uSecs
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void) {
  SystemCoreClockUpdate();                          /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) {     /* SysTick 1 mSec interrupts  */
    while (1);                                      /* Capture error              */
  }

  BTN_Init();
  USART1_Init();
  TIM2_Init();
  DHT22_Init();

  printf("STM32F04-Discovery Board\r\n");
  printf("\tSystem core clock: %u MHz\r\n", (SystemCoreClock / 1000000));
  printf("\tDHT22 sensor test\r\n\r\n");
  Delay(1000);

  /*
    ENDLESS LOOP BEGIN
   */
  while (1) {
    DHT22_Read();
		RH = DHT22getHumidity();
		TEMP = DHT22getTemperature();
    printf("Temperature:\t%.1f C\r\nHumidity:\t%.1f %%\r\n\r\n", DHT22getTemperature(), DHT22getHumidity());
    Delay(2500);
  }

  return(0);
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
// -------------------------------------------- www.make2explore.com-------------------------------------------------//