
#include "stm32f407xx.h"


static void GPIO_LedConfig();
static void GPIO_ButtonInterruptConfig();
static void SPI_Config();
static void SPI_GPIO_Config();

SPI_HandleTypeDef_t SPI_HandleStructure = {0};


void EXTI0_IRQHandler()
{
	char msgToSend[] = "Hello World!\n";

	if(EXTI ->PR & 0x1)
	{
		EXTI ->PR |= (0x1U << 0U); //clearing

		SPI_TransmitData_IT(&SPI_HandleStructure, (uint8_t*)msgToSend, strlen(msgToSend) );

	}
}

void SPI1_IRQHandler()
{
	SPI_InterruptHandler(&SPI_HandleStructure);

}

int main(void)
{
	GPIO_LedConfig();

	GPIO_ButtonInterruptConfig();

	SPI_GPIO_Config();

	SPI_Config();


	for(;;)
	{

	}
}

static void GPIO_LedConfig()
{
	GPIO_InitTypeDef_t GPIO_InitStruct = {0};

	RCC_GPIOD_CLK_ENABLE();				// Clock is active
	RCC_GPIOA_CLK_ENABLE();				// Clock is active

	GPIO_InitStruct.pinNumber = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPdr = GPIO_PUPD_NOPULL;

	GPIO_Init(GPIOD, &GPIO_InitStruct);	// Config

	memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));

	GPIO_InitStruct.pinNumber = GPIO_PIN_O;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.PuPdr = GPIO_PUPD_PULLDOWN;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

}

static void GPIO_ButtonInterruptConfig()
{
	EXTI_InitTypeDef_t EXTI_InitStruct = { 0 };

	RCC_GPIOA_CLK_ENABLE();				// Clock is active

	RCC_SYSCFG_CLK_ENABLE();

	EXTI_LineConfig(EXTI_PortSource_GPIOA, EXTI_LineSource_0);

	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_LineNumber = EXTI_LineSource_0;
	EXTI_InitStruct.EXTI_Mode = EXTI_MODE_Interrupt;
	EXTI_InitStruct.TriggerSelection = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_EnableInterrupt(EXTI0_IRQNumber);

}
static void SPI_Config()
{
	//SPI_HandleTypeDef_t SPI_HandleStructure = { 0};

	RCC_SPI1_CLK_ENABLE();

	SPI_HandleStructure.Instance = SPI1;
	SPI_HandleStructure.Init.BaudRate = SPI_BAUDRATE_DIV8; //2MHz
	SPI_HandleStructure.Init.BusConfig = SPI_BUS_FullDuplex;
	SPI_HandleStructure.Init.CPHA = SPI_CPHA_FIRST;
	SPI_HandleStructure.Init.CPOL = SPI_CPOL_LOW;
	SPI_HandleStructure.Init.DFF_Format = SPI_DFF_8BITS;
	SPI_HandleStructure.Init.FrameFormat = SPI_FRAMEFORMAT_MSB;
	SPI_HandleStructure.Init.Mode = SPI_MODE_MASTER;
	SPI_HandleStructure.Init.SSM_Cmd = SPI_SSM_ENABLE;

	SPI_Init(&SPI_HandleStructure);

	NVIC_EnableInterrupt(SPI1_IRQNumber);

	SPI_PeriphCmd(&SPI_HandleStructure, ENABLE);

}

static void SPI_GPIO_Config()
{
	GPIO_InitTypeDef_t GPIO_InitStruct = {0};

	GPIO_InitStruct.pinNumber = GPIO_PIN_5 | GPIO_PIN_7; //PA5 = SCK, PA7 = MOSI
	GPIO_InitStruct.Mode = GPIO_MODE_AF;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPdr = GPIO_PUPD_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_VERY;
	GPIO_InitStruct.Alternate = GPIO_AF5;

	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

