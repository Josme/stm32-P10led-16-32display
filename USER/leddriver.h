#include "stm32f10x.h"


/*********************************************************************/
#define CK1_high	    (GPIOC->BSRR = GPIO_Pin_8)		
#define CK1_low			(GPIOC->BRR = GPIO_Pin_8)		  	
			
#define HS1_high		(GPIOA->BSRR = GPIO_Pin_8)						
#define HS1_low	        (GPIOA->BRR = GPIO_Pin_8)			
			 				
#define R1_high			(GPIOC->BSRR = GPIO_Pin_6)						
#define R1_low			(GPIOC->BRR = GPIO_Pin_6)			
	
#define OE1_high		(GPIOB->BSRR = GPIO_Pin_8)						
#define OE1_low			(GPIOB->BRR = GPIO_Pin_8)
	
#define A1_high			(GPIOB->BSRR = GPIO_Pin_6)						
#define A1_low			(GPIOB->BRR = GPIO_Pin_6)
	
#define B1_high			(GPIOA->BSRR = GPIO_Pin_10)							
#define B1_low			(GPIOA->BRR = GPIO_Pin_10)

/*************************************************************/
#define OE(a)	if (a)	\
					GPIOB->BSRR = GPIO_Pin_8;\
					else		\
					GPIOB->BRR = GPIO_Pin_8;
#define PIAN_A(a)	if (a)	\
					GPIOB->BSRR = GPIO_Pin_6;\
					else		\
					GPIOB->BRR = GPIO_Pin_6;
#define PIAN_B(a)	if (a)	\
					GPIOA->BSRR = GPIO_Pin_10;\
					else		\
					GPIOA->BRR = GPIO_Pin_10;
#define CLK_S(a)	if (a)	\
					GPIOA->BSRR = GPIO_Pin_8;\
					else		\
					GPIOA->BRR = GPIO_Pin_8;                   
#define CLK_L(a)	if (a)	\
					GPIOC->BSRR = GPIO_Pin_8;\
					else		\
					GPIOC->BRR = GPIO_Pin_8;
#define DATA_R(a)	if (a)	\
					GPIOC->BSRR = GPIO_Pin_6;\
					else		\
					GPIOC->BRR = GPIO_Pin_6;
                    
                    
void GPIO_Config_P1(void);
void SerialPut(unsigned char dat,uint8_t Px);
void Scan(unsigned char temp,uint8_t Px);

