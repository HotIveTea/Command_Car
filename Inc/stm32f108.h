#ifndef STM32F108_H
#define STM32F108_H
#include <stdint.h>
#define PERIPH_BASE 0x40000000UL
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000UL)
#define APB1PERIPH_BASE (PERIPH_BASE)
#define AHBPERIPH_BASE (PERIPH_BASE + 0x20000UL)
#define RCC_PERIPH_BASE (AHBPERIPH_BASE + 0x1000UL)
#define AFIO_PERIPH_BASE (APB2PERIPH_BASE + 0x0000UL)
#define GPIOC_PERIPH_BASE (APB2PERIPH_BASE + 0x1000UL)
#define GPIOB_PERIPH_BASE (APB2PERIPH_BASE + 0x0C00UL)
#define GPIOA_PERIPH_BASE (APB2PERIPH_BASE + 0x0800UL)
#define USART1_PERIPH_BASE (APB2PERIPH_BASE + 0x3800UL)
#define USART2_PERIPH_BASE (APB1PERIPH_BASE + 0x4400UL)
#define TIMER1_PERIPH_BASE (APB2PERIPH_BASE + 0x2C00UL)
#define TIMER2_PERIPH_BASE (APB1PERIPH_BASE + 0x0000UL)
typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t AHBRSTR;
    volatile uint32_t CFGR2;
} RCC_TypeDef;
typedef struct
{
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR1;
    volatile uint32_t EXTICR2;
    volatile uint32_t EXTICR3;
    volatile uint32_t EXTICR4;
    volatile uint32_t MAPR2;
} AFIO_TydeDef;
typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    uint32_t re0;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    uint32_t re1;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIMER_TypeDef;
#define RCC ((RCC_TypeDef *)RCC_PERIPH_BASE)
#define AFIO ((AFIO_TydeDef *)AFIO_PERIPH_BASE)
#define GPIOA ((GPIO_TypeDef *)GPIOA_PERIPH_BASE)
#define GPIOB ((GPIO_TypeDef *)GPIOB_PERIPH_BASE)
#define GPIOC ((GPIO_TypeDef *)GPIOC_PERIPH_BASE)
#define USART1 ((USART_TypeDef *)USART1_PERIPH_BASE)
#define USART2 ((USART_TypeDef *)USART2_PERIPH_BASE)
#define TIM1 ((TIMER_TypeDef *)TIMER1_PERIPH_BASE)
#define TIM2 ((TIMER_TypeDef *)TIMER2_PERIPH_BASE)
#endif