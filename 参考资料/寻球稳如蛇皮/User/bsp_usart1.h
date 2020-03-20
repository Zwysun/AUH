#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stm32f10x_usart.h>

void USART1_Config(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
void USART2_Config(void);
void USART3_Config(void);
void UART4_Config(u32 pclk2,u32 bound);

#endif /* __USART1_H */
