#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#include <stm32f1xx.h>
#pragma GCC diagnostic pop

#include <system_stm32f1xx.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/* 
 * Part number: STM32F030F4C6
 */

/* static volatile uint32_t sysTickCount = 0;

void myDelay(uint32_t nTime)
{
    sysTickCount = nTime;
    while(sysTickCount != 0);
}

void SysTick_Handler()
{
    if (sysTickCount != 0) {
        sysTickCount--;
    }
}

int uart_putc(USART_TypeDef* USARTx, char c)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
  USARTx->DR =  (c & 0xff);
  return 0;
}

int uart_getc (USART_TypeDef* USARTx)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
  return  USARTx->DR & 0xff;
}*/

int main(void) {
    //////CLOCKS///////
    /* External crystal: 8MHz */
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR&RCC_CR_HSERDY));

    /* Sysclk = HCLK = 48MHz */
    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_PLLMULL_Msk & ~RCC_CFGR_SW_Msk & ~RCC_CFGR_PPRE1_Msk & ~RCC_CFGR_PPRE2_Msk & ~RCC_CFGR_HPRE_Msk))
        | (10<<RCC_CFGR_PLLMULL_Pos) | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC | (4<<RCC_CFGR_PPRE1_Pos) | (4<<RCC_CFGR_PPRE2_Pos);

    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR&RCC_CR_PLLRDY));

    /* Switch to PLL */
    RCC->CFGR |= (2<<RCC_CFGR_SW_Pos);
    //RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_PPRE1_Msk & ~RCC_CFGR_PPRE2_Msk))
    //    | (4<<RCC_CFGR_PPRE1_Pos) | (4<<RCC_CFGR_PPRE2_Pos);
    SystemCoreClockUpdate();
    //////END CLOCKS//////
    
    // Enable A, B, C GPIO Banks on the bus 
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;

    // GPIO Configuration
    GPIOC->CRH = 0x44144444; // sets C13 as output
    
    while (1) {
        GPIOC->ODR ^= 0x0001 << 13;
        for (int sleep = 0; sleep < 72000000; sleep++){};
    }
}
/*
int main2(void) {
    


    // print trace info - goes to Eclipse console
    //trace_puts("Hello ARM World!");
    //trace_printf("System clock: %u Hz\n", SystemCoreClock);

    // LED is on PA5

    // enable clock for peripheral GPIOA and GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

    // initialize GPIO structure
    GPIO_InitTypeDef gpioInitStruct;
    GPIO_StructInit(&gpioInitStruct);
    gpioInitStruct.GPIO_Pin = GPIO_Pin_5;
    gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpioInitStruct);

    // set PC13 (User button 1) as input
    gpioInitStruct.GPIO_Pin = GPIO_Pin_13;
    gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &gpioInitStruct);


    // serial comms
    // use PA2 (TX) and PA3 (RX)
    // USART2
    // These are connected to the ST-Link circuit on the Nucleo board
    // so the serial output will be via USB, on your computer

    // enable clock for USART2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // setup Tx/RX pins:

    // init TX
    gpioInitStruct.GPIO_Pin = GPIO_Pin_2;
    gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpioInitStruct);
    // init RX
    gpioInitStruct.GPIO_Pin = GPIO_Pin_3;
    gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpioInitStruct);

    // setup USART2:

    // initialize struct
    USART_InitTypeDef usartInitStruct;
    USART_StructInit(&usartInitStruct);
    // set parameters
    usartInitStruct.USART_BaudRate = 9600;
    usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &usartInitStruct);
    USART_Cmd(USART2, ENABLE);

    // set up a timer
    SysTick_Config(SystemCoreClock/1000);

    // Infinite loop
    volatile uint8_t val = 0;
    uart_putc(USART2, '.');
    uart_putc(USART2, '\n');
    while (1)
    {
        // read PC13 input
        uint8_t input = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
        if (!input) {
            trace_printf("button pressed!\n");
            val = 1;
        }

        // if flag set go into serial input mode
        if (val) {
            // show prompt
            uart_putc(USART2, '>');
            // get input
            char c = uart_getc(USART2);
            uart_putc(USART2, '\n');
            uart_putc(USART2, '.');
            uart_putc(USART2, '\n');
            // set LED
            int led = (c == '0') ? 0 : 1;
            GPIO_WriteBit(GPIOA, GPIO_Pin_5, led);
            // reset input mode flag
            val = 0;
        }

        // wait
        myDelay(500);
    }
}*/

void NMI_Handler(void) {
}

void HardFault_Handler(void) __attribute__((naked));
void HardFault_Handler() {
    asm volatile ("bkpt");
}

void SVC_Handler(void) {
}


void PendSV_Handler(void) {
}

void SysTick_Handler(void) {
}

void _init(void) {
}

void MemManage_Handler(void) __attribute__((naked));
void MemManage_Handler() {
    asm volatile ("bkpt");
}

void BusFault_Handler(void) __attribute__((naked));
void BusFault_Handler() {
    asm volatile ("bkpt");
}
