#include "stm32f108.h"
#include "string.h"
#include "stdlib.h"
#include "os.h"
#define STACK_SIZE 100
#define CMD_MAX_LEN 16
uint32_t idle_stack[STACK_SIZE];
uint32_t task1_stack[STACK_SIZE];
uint32_t task2_stack[STACK_SIZE];
char rx_buffer[CMD_MAX_LEN];
volatile int rx_index = 0;
volatile int rx_complete = 0;
TCB_t *waiting_task = 0;
TCB_t tcbidle, tcb1, tcb2;
extern TCB_t volatile *current_task;
extern TCB_t volatile *next_task;
typedef enum
{
    STOP = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} CarDirection;
typedef struct
{
    CarDirection dir;
    uint16_t speed;
    int32_t time;
} Car;
volatile Car car_target = {STOP, 0};
void Idle_task(void)
{
    while (1)
    {
        __asm volatile("WFI \n");
    }
}
void GPIO_Init()
{
    // === GPIO SETTING === //
    GPIOA->CRL &= ~((0xF << 8) | (0xF << 4));                               // Clear PA1 and PA2
    GPIOA->CRH &= ~((0xF << 8) | (0xF << 4));                               // Clear PA9 and PA10
    GPIOA->CRL |= (0x0B << 8) | (0x0B << 4);                                // Set PA1 and PA2 as AF
    GPIOA->CRH |= (0x0B << 4) | (0x04 << 8);                                // Config PA9 as AF and PA10 as INPUT FLOATING
    GPIOB->CRL &= ~((0xF << 16) | (0xF << 20) | (0xF << 24) | (0xF << 28)); // Clear PB4, 5, 6, 7
    GPIOB->CRH &= ~(0xF << 1);                                              // Clear PB8
    GPIOB->CRL |= (0x1 << 16) | (1 << 20) | (0x1 << 24) | (0x1 << 28);      // Set PB4, 5, 6, 7 as OUTPUT
    GPIOB->CRH |= (0x1 << 1);                                               // Set PB8 as OUTPUT
}
void TIMER_Init()
{
    // === TIMER SETTING === //
    TIM2->PSC = 7;                     // Set PreScaler = 7
    TIM2->ARR = 999;                   // Auto Reset = 999
    TIM2->CCMR1 &= ~(0x7 << 12);       // Clear Output compare 2 mode
    TIM2->CCMR1 |= (0x6 << 12);        // Set 110 -> PWM Mode 1
    TIM2->CCMR2 &= ~(0x7 << 4);        // Clear Output compare 3 mode
    TIM2->CCMR2 |= (0x6 << 4);         // Set 110 -> PWM Mode 1
    TIM2->CCER |= (1 << 4) | (1 << 8); // Enable Capture/Compare 2 and 3
    TIM2->CCR2 = 0;
    TIM2->CCR3 = 0;
    TIM2->CR1 |= (1 << 0); // Enable Counter
}
void USART_Init()
{
    // === UART SETTING === //
    USART1->BRR = (8000000 + (115200 / 2)) / 115200;      // Baudrate = 115200
    USART1->CR1 |= (1 << 2) | (1 << 3) | (1 << 13);       // Config TE, RE and UE
    USART1->CR1 |= (1 << 5);                              // RXNE interrupt enable
    *(volatile uint32_t *)0xE000E104 |= (1 << (37 - 32)); // NVIC_ISER1
    // NVIC_EnableIRQ(USART1_IRQn);
}
void USART1_IRQHandler(void)
{
    uint32_t sr = USART1->SR;
    if (sr & ((1 << 3) | (1 << 2) | (1 << 1)))
    {
        char dummy = USART1->DR;
        (void)dummy;
    }
    if (sr & (1 << 5))
    {
        char c = (char)USART1->DR;
        if (c == '\r' || rx_index > CMD_MAX_LEN - 1)
        {
            rx_buffer[rx_index] = '\0';
            rx_complete = 1;
            rx_index = 0;
            if (waiting_task != 0)
            {
                waiting_task->sleep_time = 0;
                waiting_task = 0;
                *(volatile uint32_t *)0xE000ED04 |= (1 << 28);
            }
        }
        else
        {
            if (c != '\n')
            {
                rx_buffer[rx_index++] = c;
            }
        }
    }
}
void OS_USART_Received_String(char *out_buffer)
{
    __asm volatile("CPSID I \n");
    rx_complete = 0;
    waiting_task = current_task;
    __asm volatile("CPSIE I \n");
    while (1)
    {
        __asm volatile("CPSID I \n");
        if (rx_complete == 1)
        {
            __asm volatile("CPSIE I \n");
            break;
        }
        __asm volatile("CPSIE I \n");

        OS_Delay(10);
    }
    while (rx_complete == 0)
    {
        OS_Delay(0xFFFFFFFF);
    }
    int i = 0;
    for (i = 0; rx_buffer[i] != '\0'; i++)
    {
        out_buffer[i] = rx_buffer[i];
    }
    out_buffer[i] = '\0';
}
void USART_Command_Task(void *arg)
{
    char cmd_buffer[CMD_MAX_LEN];
    while (1)
    {
        OS_USART_Received_String(cmd_buffer);
        char action = cmd_buffer[0];
        int val_speed = atoi(&cmd_buffer[1]);
        int val_time = 0;
        for (int i = 1; i < CMD_MAX_LEN; i++)
        {
            if (cmd_buffer[i] == ',')
            {
                val_time = atoi(&cmd_buffer[i + 1]);
                break;
            }
        }
        __asm volatile("CPSID I \n");
        if (action == 'F')
            car_target.dir = FORWARD;
        else if (action == 'B')
            car_target.dir = BACKWARD;
        else if (action == 'L')
            car_target.dir = LEFT;
        else if (action == 'R')
            car_target.dir = RIGHT;
        else
            car_target.dir = STOP;

        car_target.speed = val_speed;
        car_target.time = val_time;
        __asm volatile("CPSIE I \n");
    }
}
void Move_Forward(uint32_t speed)
{
    uint32_t pulse = (speed * 1000) / 100;
    TIM2->CCR2 = pulse;
    TIM2->CCR3 = pulse;
    GPIOB->BSRR = (1 << 4);
    GPIOB->BRR = (1 << 5);
    GPIOB->BSRR = (1 << 6);
    GPIOB->BRR = (1 << 7);
}
void Move_Backward(uint32_t speed)
{
    uint32_t pulse = (speed * 1000) / 100;
    TIM2->CCR2 = pulse;
    TIM2->CCR3 = pulse;
    GPIOB->BSRR = (1 << 5);
    GPIOB->BRR = (1 << 4);
    GPIOB->BSRR = (1 << 7);
    GPIOB->BRR = (1 << 6);
}
void Move_Left(uint32_t speed)
{
    uint32_t pulse = (speed * 1000) / 100;
    TIM2->CCR2 = pulse;
    TIM2->CCR3 = pulse;
    GPIOB->BSRR = (1 << 5);
    GPIOB->BRR = (1 << 4);
    GPIOB->BSRR = (1 << 6);
    GPIOB->BRR = (1 << 7);
}
void Move_Right(uint32_t speed)
{
    uint32_t pulse = (speed * 1000) / 100;
    TIM2->CCR2 = pulse;
    TIM2->CCR3 = pulse;
    GPIOB->BSRR = (1 << 4);
    GPIOB->BRR = (1 << 5);
    GPIOB->BSRR = (1 << 7);
    GPIOB->BRR = (1 << 6);
}
void Stop()
{
    GPIOB->BSRR = (1 << 4);
    GPIOB->BSRR = (1 << 5);
    GPIOB->BSRR = (1 << 7);
    GPIOB->BSRR = (1 << 6);
}
void Car_Control_Task(void *arg)
{
    while (1)
    {
        if (car_target.time > 0)
        {
            switch (car_target.dir)
            {
            case FORWARD:
                GPIOC->ODR ^= (1 << 13);
                Move_Forward(car_target.speed);
                break;
            case BACKWARD:
                GPIOC->ODR ^= (1 << 13);
                Move_Backward(car_target.speed);
                break;
            case LEFT:
                GPIOC->ODR ^= (1 << 13);
                Move_Left(car_target.speed);
                break;
            case RIGHT:
                GPIOC->ODR ^= (1 << 13);
                Move_Right(car_target.speed);
                break;
            default:
                Stop();
                break;
            }
            __asm volatile("CPSID I \n");
            car_target.time -= 20;
            __asm volatile("CPSIE I \n");
        }
        else
        {
            Stop();
        }

        OS_Delay(20);
    }
}
int main(void)
{
    // === CONFIG === //
    RCC->APB2ENR |= (1 << 2) | (1 << 3) | (1 << 0) | (1 << 4) | (1 << 14);
    RCC->APB1ENR |= (1 << 0);
    AFIO->MAPR |= (0x2 << 24);
    GPIO_Init();
    TIMER_Init();
    USART_Init();
    // === TESTING === //
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x1 << 20);
    GPIOC->BRR = (1 << 13);
    OS_TaskCreate(&tcbidle, Idle_task, NULL, idle_stack, STACK_SIZE);
    OS_AddThread(&tcbidle);
    OS_TaskCreate(&tcb1, USART_Command_Task, NULL, task1_stack, STACK_SIZE);
    OS_TaskCreate(&tcb2, Car_Control_Task, NULL, task2_stack, STACK_SIZE);
    OS_AddThread(&tcb1);
    OS_AddThread(&tcb2);
    OS_Start();
    return 0;
}