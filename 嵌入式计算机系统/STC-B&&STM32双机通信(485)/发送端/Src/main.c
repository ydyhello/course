
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2023 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
struct 
{ unsigned char head;
	unsigned char function;
	short AD_Res[3];
} TXD_M1={0xaa,0x55,1,2,3};

unsigned char display[8]={1,2,3,4,5,6,7,8};
char Q=0;
char Flag_100uS;    //100uS
char Flag_1mS;      //1mS
char Flag_20mS;     //10mS
char Flag_500mS;     //10mS
char Flag_1S;       //1S
char Flag_1Min;     //1Min
char Flag_1Hour;    //1Hour
unsigned char count_100uS;    //100uS
unsigned char count_1mS;      //1mS
unsigned char count_20mS;     //20mS
unsigned char count_500mS;     //20mS
unsigned char count_1S;       //1S
unsigned char count_1Min;     //1Min
char kkk; 
uint8_t  message[2];
uint8_t ans;
uint8_t tmp;
uint8_t wei;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void HAL_SYSTICK_Callback(void);
void Func_100uS(void);
void Func_1mS(void);
void Func_500mS(void);
void Func_20mS(void);
void Func_1S(void);
void Func_1Min(void);
void Func_1Hour(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	ans=message[0]+message[1];
	HAL_UART_Transmit_DMA(&huart1,&ans,1);
	HAL_UART_Receive_DMA(&huart1,message,2);
}
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
int Counter;
unsigned int Tick;
unsigned int Pins [8] = {GPIO_PIN_15,GPIO_PIN_14,GPIO_PIN_13,GPIO_PIN_12,GPIO_PIN_11,GPIO_PIN_10,GPIO_PIN_9,GPIO_PIN_8};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/10000);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&TXD_M1.AD_Res, 3);
	HAL_UART_Receive_DMA(&huart1,message,2);
	
	//led
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,1UL);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	if (Flag_100uS!=0) { Flag_100uS = 0;  Func_100uS();} 
			if (Flag_1mS!=0)   { Flag_1mS = 0;    Func_1mS();} 
			if (Flag_20mS!=0)  { Flag_20mS = 0;   Func_20mS(); } 
			if (Flag_500mS!=0)  { Flag_500mS = 0;   Func_500mS(); }
			if (Flag_1S!=0)    { Flag_1S = 0;     Func_1S();} 
			if (Flag_1Min!=0)  { Flag_1Min = 0;   Func_1Min();} 
			if (Flag_1Hour!=0) { Flag_1Hour = 0;  Func_1Hour();}
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_SYSTICK_Callback(void)
{ Flag_100uS=1;
	if (++count_100uS>=10) 
	 { count_100uS=0;  Flag_1mS=1;
		 if (++count_1mS>=20) 
			 { count_1mS=0;		 Flag_20mS=1;
				 if (++count_20mS>=25) 
					 { count_20mS=0;			 Flag_500mS=1;
							if( ++count_500mS>=2)
							{
								count_500mS=0;			 Flag_1S=1;
							 if(++count_1S>=60) 
								{ count_1S=0;						Flag_1Min=1;
									if(++count_1Min>=60) 
										{ count_1Min=0;							Flag_1Hour=1;
										}
								}
							}								
					 }
			 }
	 }
}

void Func_100uS(void)
{ 
}

void LedValue(char value)
{
	GPIOE->ODR &= ~(0xff<<8);
	GPIOE->ODR |= value<<8;
}

void sel(char value)
{
	GPIOB->ODR &= ~(0x07);
	GPIOB->ODR |= (value & 0x07);
}

void SetLed(char w, char value)
{char segTable[] = {
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,   
0x77,0x7c,0x39,0x5e,0x79,0x71,0x3d,0x76,0x0f,0x0e,
0x75,0x38,0x37,0x54,0x5c,0x73,0x67,0x31,0x49,0x78,
0x3e,0x1c,0x7e,0x64,0x6e,0x59,0x00
};
  sel(w);
	LedValue(segTable[value]);
}	

void Func_1mS(void)
{ 
	static char i;
	display[7]=Q%16;

	//if (++i>7) i=0;
}

void Func_20mS(void)
{ 

	SetLed(7, display[7]);
	HAL_ADC_Start ( &hadc1 );
	/*
	  if(Q%16==1){
			unsigned int Time = HAL_GetTick();
		if(Time % 500UL == 0UL && Tick != Time){
			Tick =  Time;
			HAL_GPIO_WritePin(GPIOE,0xFF00,0UL);
			HAL_GPIO_WritePin(GPIOE,Pins[Counter],1UL);
			if(Counter < 8UL){
				Counter++;
			}else{
				Counter = 0;
			}
		}
	}*/
	/*
		if(Q%16==2)
		{
			
		}
	*/
	
}
void Func_500mS(void)
{   

	if(TXD_M1.AD_Res[0]/100==11)//shang
	{	
			Q--;
	}
	if(TXD_M1.AD_Res[0]/100==28)//xia
	{	
			Q++;
	}
	if(TXD_M1.AD_Res[0]/100==5)//zuo

	if(TXD_M1.AD_Res[0]/100==22||TXD_M1.AD_Res[0]/100==23)//you


	 if(TXD_M1.AD_Res[0]/100==17){

	}

}
void Func_1S(void)
{   
	RS485DIR_TX;
	HAL_UART_Transmit(&huart2,(uint8_t*)&Q,sizeof(Q),1000);
	//RS485DIR_RX;
	//HAL_UART_Receive(&huart2,(uint8_t*)&Q,sizeof(Q),10000);
}

void Func_1Min(void)
{  
	
}

void Func_1Hour(void)
{
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
