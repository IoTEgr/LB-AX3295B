/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         UART DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_uart.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN UART0 HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"

#define  R_UART_CTRL0         UART0CON
#define  R_UART_BAUDRATE0     UART0BAUD
#define  R_UART_PEND0         UART0PND
#define  R_UART_DATA0         UART0DAT

static void (*uart0Callback)(u8 data);
/*******************************************************************************
* Function Name  : ax32xx_uart0IRQHandler
* Description    : uart 0 IRQ handler
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0IRQHandler(void)
{
    R_UART_PEND0 |= BIT(4);
	if(uart0Callback)
		uart0Callback(REG32(R_UART_DATA0)&0xff);
}
/*******************************************************************************
* Function Name  : ax32xx_uart0IOCfg
* Description    : uart0 rx tx config
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0IOCfg(u8 en)
{
	if(en)
	{
        //ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_TX_POS);
        //ax32xx_gpioSFRSet(GPIO_MAP_UARTRX0,UART0_RX_POS);
        
        if(UART0_TX_POS == UART0_POS_PB0)
        {
            ax32xx_gpioDigitalSet(GPIO_PB,GPIO_PIN0,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet (GPIO_PB,GPIO_PIN0,GPIO_OUTPUT);
            ax32xx_gpioPullSet(GPIO_PB,GPIO_PIN0,GPIO_PULL_UP);
            ax32xx_gpioDataSet(GPIO_PB,GPIO_PIN0,GPIO_HIGH);
			 ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_TX_POS);   // tx-pa7
        }
		else if(UART0_TX_POS == UART0_POS_PA7)
        {
			ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN7,GPIO_OUTPUT);
            ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN7,GPIO_PULL_UP);
            ax32xx_gpioDataSet(GPIO_PA,GPIO_PIN7,GPIO_HIGH);
            ax32xx_gpioMapSet(GPIO_PA,GPIO_PIN7,GPIO_FUNC_SFR);		
            ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_TX_POS);   // tx-pa7
        }
		else if(UART0_TX_POS == UART0_POS_PA9)
        {
			ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN9,GPIO_OUTPUT);
            ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN9,GPIO_PULL_UP);
            ax32xx_gpioDataSet(GPIO_PA,GPIO_PIN9,GPIO_HIGH);
            ax32xx_gpioMapSet(GPIO_PA,GPIO_PIN9,GPIO_FUNC_SFR);		
            ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_TX_POS);   // tx-pa9
        }
		else if(UART0_TX_POS == UART0_POS_PE1)
        {
			ax32xx_gpioDirSet(GPIO_PE,GPIO_PIN1,GPIO_OUTPUT);
            ax32xx_gpioPullSet(GPIO_PE,GPIO_PIN1,GPIO_PULL_UP);
            ax32xx_gpioDataSet(GPIO_PE,GPIO_PIN1,GPIO_HIGH);
            ax32xx_gpioMapSet(GPIO_PE,GPIO_PIN1,GPIO_FUNC_SFR);		
            ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_TX_POS);   // tx-pE1
        }
    //FIXME:must marco == marco
	#if UART0_RX_POS == UART0_POS_NONE
	    ax32xx_gpioSFRSet(GPIO_MAP_UARTRX0,UART0_POS_NONE);   // rx-none
	#else
	
	#endif
	}
	else
	{
		ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_POS_NONE);
		ax32xx_gpioSFRSet(GPIO_MAP_UARTRX0,UART0_POS_NONE);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_uart0Init
* Description    : uart0 initial 
* Input          : u32 baudrate : uart0 baudrate
                      void (*isr)(u8 data) : rx callback
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0Init(u32 baudrate,void (*isr)(u8 data))
{
	
    uart0Callback = isr;
	
	R_UART_CTRL0 = 0;
	R_UART_BAUDRATE0 = APB_CLK/baudrate-1;                           //baud rate 
	R_UART_PEND0 |= 3<<4;    
	R_UART_CTRL0 |= (1<<4);         

	R_UART_CTRL0 |= 0; //RX	

	ax32xx_intEnable(IRQ_UART0,0); // enable irq
}
/*******************************************************************************
* Function Name  : ax32xx_uart0SendByte
* Description    : uart0 send data
* Input          : u8 data : send data
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0SendByte(u8 data)
{
	R_UART_DATA0 = data;
	while((R_UART_PEND0 & 0x2)==0);
	R_UART_PEND0 |= 1;
}





